using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
using USART;

namespace LightSensorCalibration.Instruments
{
    public class Microcontroller
    {
        public string ID;
        public bool isConnected;

        private Comport_Interface _MCU_ComPort = new Comport_Interface();
        public Comport_Interface MCU_ComPort
        {
            get { return _MCU_ComPort; }
            set { _MCU_ComPort = value; }
        }

        private string MCU_ReceiveBuffer = "";
        const char endChar = '\r';

        private GlobalVariables g;
        public void SetReference(ref GlobalVariables _g)
        {
            g = _g;
        }

        private SerialPort _Serial = new SerialPort("COM1", 115200, Parity.None, 8, StopBits.One);


        /// <summary>
        /// Should return some state
        /// </summary>
        public string[] CheckForDevices()
        {
            return SerialPort.GetPortNames();
        }

        public Microcontroller()
        {
            _Serial.ReadTimeout = 2000;
            _Serial.WriteTimeout = 2000;
        }
        
        public MCU_ReturnCodes ConnectMCU()
        {
            string[] ports = CheckForDevices();

            foreach (string port in ports)
            {
                try
                {
                    //Change comport
                    _Serial.PortName = port;

                    // Open serial Port
                    _Serial.Open();

                    if (GetModelName(port))
                    {
                        if (Connect())
                        {
                            isConnected = true;
                            _Serial.Close();
                            return MCU_ReturnCodes.MCU_PASSED;
                        }
                        _Serial.Close();
                        return MCU_ReturnCodes.MCU_COULDNTCONNECT;
                    }
                    _Serial.Close();
                }
                catch
                {

                }
                
                
                
            }
            return MCU_ReturnCodes.MCU_COULDNTFINDDEVICE;
        }

        public bool Connect()
        {
            _MCU_ComPort.PortName = _Serial.PortName;
            _MCU_ComPort.BaudRate = "115200";

            _MCU_ComPort.COM_UseRxTimer = false;

            _MCU_ComPort.RX_Handle += new Comport_Interface.RXHandler(Com_AddToRxBuffer);
            _MCU_ComPort.RX_HandlerEvent_Enable = true;

            _MCU_ComPort.OpenPort();

            if (_MCU_ComPort.PortConnected)
            {
                isConnected = true;
                return true;
            }
            else
            {
                isConnected = false;
                _MCU_ComPort.ClosePorts();
                return false;
            }
        }

        public bool ConnectToPort(string ComPort)
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

                _Serial.Write("getname\r");

                _Serial.ReadTimeout = 500;

                string MCU_Response = "";

                MCU_Response = _Serial.ReadLine();

                _Serial.Close();

                if (MessageBox.Show("Do you want to connect " + ID + " to " + MCU_Response + " on port " + ComPort + "?", "Choose to connect", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                {
                    _MCU_ComPort.PortName = ComPort;
                    _MCU_ComPort.BaudRate = "115200";

                    _MCU_ComPort.COM_UseRxTimer = false;

                    _MCU_ComPort.RX_Handle += new Comport_Interface.RXHandler(Com_AddToRxBuffer);
                    _MCU_ComPort.RX_HandlerEvent_Enable = true;

                    _MCU_ComPort.OpenPort();

                    if (_MCU_ComPort.PortConnected)
                    {
                        isConnected = true;
                    }
                    else
                    {
                        isConnected = false;
                        _MCU_ComPort.ClosePorts();
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

        public bool GetModelName(string Comport)
        {
            string reply = String.Empty;

            
            try
            {
                

                //Flush existing
                _Serial.ReadExisting();

                //Write to get model name
                _Serial.Write("getname\r");

                // save to reply variable
                reply = _Serial.ReadLine();

                ID = reply.Substring(0, reply.Length - 1);

                //return evertything except "\r"
                return (reply.Substring(0, reply.Length - 1) == "STM32");
            }
            catch
            {
                //Catch for no crash
            }

            return false;
        }

        public void RequestData()
        {
            try
            {
                MCU_ComPort.SendData("rd\r");
            }
            catch
            {

            }
            
        }

        public void Com_AddToRxBuffer(string Data)
        {
            MCU_ReceiveBuffer += Data;
            Decode();
        }

        public void Decode()
        {
            int endPos = 0;
            string CheckData = "";

            try
            {
                /* Remove \n */
                endPos = MCU_ReceiveBuffer.LastIndexOf(endChar, MCU_ReceiveBuffer.Length - 1);

                if (endPos == -1)
                {
                    return;
                }

                /* Remove \r */
                CheckData = MCU_ReceiveBuffer.Substring(0, endPos);

                MCU_ReceiveBuffer = "";

                string[] parts = CheckData.Split(':');

                double mcuHumidity = 0.0d ;
                double mcuTemperature = 0.0d;

                double.TryParse(parts[0], System.Globalization.NumberStyles.Any, CultureInfo.InvariantCulture, out mcuHumidity);
                double.TryParse(parts[1], System.Globalization.NumberStyles.Any, CultureInfo.InvariantCulture, out mcuTemperature);

                g.SensorHumidity = mcuHumidity;
                g.SensorTemperature = mcuTemperature;
            }
            catch
            {
                MCU_ReceiveBuffer = "";
            }

        }

    }

    public enum MCU_ReturnCodes
    {
        MCU_PASSED,
        MCU_COULDNTFINDDEVICE,
        MCU_COULDNTCONNECT,

    }
}
