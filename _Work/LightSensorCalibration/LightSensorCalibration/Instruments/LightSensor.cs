using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.IO.Ports;
using System.Windows;
using USART;
using System.Globalization;
using System.ComponentModel;

namespace LightSensorCalibration.Instruments
{
    public class LightSensor : INotifyPropertyChanged
    {

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        public void OnPropertyChanged(string propName)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(
                    this, new PropertyChangedEventArgs(propName));
            }
        }

        #endregion

        private GlobalVariables g;
        public void SetReference(ref GlobalVariables _g)
        {
            g = _g;
        }

        private bool _isConnected;
        public bool isConnected
        {
            set 
            { 
                _isConnected = value; 
            }
            get
            {
                return _isConnected;
            }
        }

        private DispatcherTimer _LightSensorTimer = new DispatcherTimer();
        public DispatcherTimer LightSensorTimer
        {
            set
            {
                _LightSensorTimer = value;
            }
            get
            {
                return _LightSensorTimer;
            }
        }

        private Comport_Interface _LightSensor_ComPort = new Comport_Interface();
        public Comport_Interface LightSensor_ComPort
        {
            get { return _LightSensor_ComPort; }
            set { _LightSensor_ComPort = value; }
        }

        private bool _isSetup;
        public bool isSetup
        {
            set
            {
                _isSetup = value;
            }
            get
            {
                return _isSetup;
            }
        }

        private string _SensorName;
        public string SensorName
        {
            set
            {
                _SensorName = value;
            }
            get
            {
                return _SensorName;
            }
        }

        private string _SerialNumber;
        public string SerialNumber
        {
            get
            {
                return _SerialNumber;
            }
        }

        private double _CurrentIrradiance;
        public double CurrentIrradiance
        {
            get
            {
                return _CurrentIrradiance;
            }
        }

        private string LightSensor_ReceiveBuffer = "";
        const char endChar = '\r';

        private SensorState _sState;
        public SensorState sState
        {
            get { return _sState; }
        }

        private SerialPort _Serial = new SerialPort("COM1", 115200, Parity.None, 8, StopBits.One);

        public LightSensor()
        {
            //Setup serialport
            _Serial.ReadTimeout = 2000;
            _Serial.WriteTimeout = 2000;
        }

        public ReturnLightSensorState ConnectSensor()
        {
            switch (CheckForSensors())
            {
                case SensorState.SENSOR_CALIBRATION:
                    {
                        _sState = SensorState.SENSOR_CALIBRATION;
                        break;
                        
                    }
                case SensorState.SENSOR_REFERENCE:
                    {
                        _sState = SensorState.SENSOR_REFERENCE;
                        break;
                    }
                default:
                    return ReturnLightSensorState.LIGHTSENSORRETURN_NOLIGHTSENSORFOUND;
            }
            if (SetSampleFrequency(100))
            {
                _isConnected = true;
                return ReturnLightSensorState.LIGHTSENSORRETURN_PASS;
            }
            return ReturnLightSensorState.LIGHTSENSORRETURN_SETSAMPLEFREQUENCYERROR;
        }

        public bool ConnectToCOM(string ComPort)
        {
            isConnected = false;

            SerialPort _Serial = new SerialPort();

            if (ComPort == String.Empty)
            {
                ComPort = "COM1";
            }

            _Serial.PortName = ComPort;
            _Serial.BaudRate = 115200;
            _Serial.Parity = Parity.None;
            _Serial.DataBits = 8;
            _Serial.StopBits = StopBits.One;
            _Serial.Handshake = Handshake.None;

            try
            {
                _Serial.Open();

                _Serial.Write("getfriendlyname\r");

                _Serial.ReadTimeout = 500;

                string ILT_Response = "";

                ILT_Response = _Serial.ReadLine();

                _Serial.Close();

                if (MessageBox.Show("Do you want to connect " + _SensorName + " to " + ILT_Response + " on port " + ComPort + "?", "Choose to connect", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                {
                    _LightSensor_ComPort.PortName = ComPort;
                    _LightSensor_ComPort.BaudRate = "115200";

                    _LightSensor_ComPort.COM_UseRxTimer = false;

                    _LightSensor_ComPort.RX_Handle += new Comport_Interface.RXHandler(Com_AddToRxBuffer);
                    _LightSensor_ComPort.RX_HandlerEvent_Enable = true;

                    _LightSensor_ComPort.OpenPort();

                    if (_LightSensor_ComPort.PortConnected)
                    {
                        isConnected = true;
                    }
                    else
                    {
                        isConnected = false;
                        LightSensor_ComPort.ClosePorts();
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return isConnected;
            }

            return isConnected;
        }

        public void Com_AddToRxBuffer(string Data)
        {
            LightSensor_ReceiveBuffer += Data;
            Decode();
        }

        public void Decode()
        {
            int endPos = 0;
            string CheckData = "";

            try
            {
                /* Remove \n */
                endPos = LightSensor_ReceiveBuffer.LastIndexOf(endChar, LightSensor_ReceiveBuffer.Length - 1);

                if (endPos == -1)
                {
                    return;
                }

                /* Remove \r */
                CheckData = LightSensor_ReceiveBuffer.Substring(0, endPos);

                LightSensor_ReceiveBuffer = "";

                string[] parts = CheckData.Split('e');

                double lowerLux;
                int exponentLux;

                /* Parse data */
                double.TryParse(parts[0], System.Globalization.NumberStyles.Any, CultureInfo.InvariantCulture, out lowerLux);

                int.TryParse(parts[1], out exponentLux);

                _CurrentIrradiance = lowerLux * Math.Pow(10, exponentLux);

                g.ReferencePID.ProcessVariable = _CurrentIrradiance;

                g.PSU.SetOutputCurrent(g.ReferencePID.Control(100));
            }
            catch
            {
                LightSensor_ReceiveBuffer = "";
            }

        }

        public void GetIrradianceValue()
        {
            LightSensor_ComPort.SendData("gi\r");
        }

        private string GetModelName(string Comport)
        {
            string reply = String.Empty;

            //Change comport
            _Serial.PortName = Comport;
            try
            {
                // Open serial Port
                _Serial.Open();

                //Flush existing
                _Serial.ReadExisting();

                //Write to get model name
                _Serial.Write("getmodelname\r");

                // save to reply variable
                reply = _Serial.ReadLine();

                //return evertything except "\r"
                return reply.Substring(0, reply.Length - 1); 
            }
            catch
            {
                //Catch for no crash
            }

            return String.Empty; 
        }

        private string GetSerialNumber(string Comport)
        {
            string reply = String.Empty;

            try
            {
                //Flush existing
                _Serial.ReadExisting();

                //Write to get model name
                _Serial.Write("getserialnumber\r");

                // save to reply variable
                reply = _Serial.ReadLine();

                //return evertything except "\r"
                return reply.Substring(0, reply.Length - 1);
            }
            catch
            {
                //Catch for no crash
            }

            return String.Empty;
        }

        private string GetFriendlyName()
        {
            string reply = String.Empty;

            try
            {
                //Flush existing
                _Serial.ReadExisting();

                //Write to get friendlyName
                _Serial.Write("getfriendlyname\r");

                //save to return var
                reply = _Serial.ReadLine();

                //return evertything except "\r"
                return reply.Substring(0, reply.Length - 1);
            }
            catch
            {
                //Catch for no crash
            }
            return String.Empty;
        }

        /// <summary>
        /// Sets friendly name of light sensor
        /// </summary>
        /// <param name="fName"></param>
        /// <returns></returns>
        public bool SetFriendyName(string fName)
        {
            try
            {
                //Flush existing
                _Serial.ReadExisting();
            }
            catch
            {

            }


            return true;
        }

        /// <summary>
        /// Sets sample frequency of light sensor
        /// </summary>
        /// <param name="sFreq"></param>
        /// <returns></returns>
        public bool SetSampleFrequency(int sFreq)
        {
            string reply = String.Empty;

            try
            {
                //Flush existing
                //_Serial.ReadExisting();

                //Concatinating string 
                string setSampleTime = "setsampletime " + sFreq + "\r";

                //Write to get sampletime
                _Serial.Write(setSampleTime);

                //Read the reply
                reply = _Serial.ReadLine();

                //Check if reply is "correct"
                return (reply.Substring(0, reply.Length - 1) == "0") ? true : false;
            }
            catch
            {

            }
            return false ;
        }

        /// <summary>
        /// Checks comports for light sensors
        /// </summary>
        /// <returns></returns>
        public SensorState CheckForSensors()
        {
            // Get serial ports open
            string[] ports = SerialPort.GetPortNames();

            //Loop through com ports
            foreach (string port in ports)
            {
                //Check for model name
                if(GetModelName(port) == "ILT2400")
                {
                    _SerialNumber = GetSerialNumber(port);

                    //Cehck if name of light sensor is correct, otherwise its the calibration
                    if (GetFriendlyName() == "QAREF")
                    {
                        // If correct, its the reference and return
                        return SensorState.SENSOR_REFERENCE;
                    }
                    // Close serialport
                    //_Serial.Close();

                    //Return calibration sensor
                    return SensorState.SENSOR_CALIBRATION;
                }
                //Close port for next iteration
                _Serial.Close();
                
                //Debug
                //MessageBox.Show(port);
            }
            //Return that no sensor available
            return SensorState.SENSOR_ERROR;
        }



    }

    /// <summary>
    /// State of sensor connected
    /// </summary>
    public enum SensorState
    {
        SENSOR_ERROR = 0,
        SENSOR_CALIBRATION = 1,
        SENSOR_REFERENCE = 2,
    }

    /// <summary>
    /// Return states for light sensor class
    /// </summary>
    public enum ReturnLightSensorState
    {
        LIGHTSENSORRETURN_PASS,
        LIGHTSENSORRETURN_NOLIGHTSENSORFOUND,
        LIGHTSENSORRETURN_SETSAMPLEFREQUENCYERROR,
    }
}
