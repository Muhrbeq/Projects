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

        public LightSensor()
        {

        }

        private string GetModelName(string Comport)
        {
            return String.Empty;
        }

        public bool Connect(string Comport)
        {
            string[] ports = SerialPort.GetPortNames();

            foreach (string port in ports)
            {
                MessageBox.Show(port);
            }

            return true;
        }
    }
}
