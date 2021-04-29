using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.IO.Ports;
using System.Windows;

namespace LightSensorCalibration.Instruments
{
    public class LightSensor
    {
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

        

        private SensorState _sState;
        public SensorState sState
        {
            get { return _sState; }
        }

        private SerialPort _Serial = new SerialPort("COM1", 115200, Parity.None, 8, StopBits.One);

        public LightSensor()
        {
            //Setup serialport
            _Serial.ReadTimeout = 500;
            _Serial.WriteTimeout = 500;
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

        private string GetModelName(string Comport)
        {
            string reply = String.Empty;

            //Change comport
            _Serial.PortName = Comport;
            try
            {
                //Flush existing
                _Serial.ReadExisting();

                // Open serial Port
                _Serial.Open();

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
                _Serial.ReadExisting();

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
                    //Cehck if name of light sensor is correct, otherwise its the calibration
                    if (GetFriendlyName() == "QAREF")
                    {
                        //Close serialport
                        _Serial.Close();
                        // If correct, its the reference and return
                        return SensorState.SENSOR_REFERENCE;
                    }
                    // Close serialport
                    _Serial.Close();

                    //Return calibration sensor
                    return SensorState.SENSOR_CALIBRATION;
                }
                //Close port for next iteration
                _Serial.Close();
                
                //Debug
                MessageBox.Show(port);
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
