﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
using USART;

namespace QA_LightBoxController
{
    public class ILT2400 : INotifyPropertyChanged
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

        public GUI Gui;
        public void SetReference(ref GUI _Gui)
        {
            Gui = _Gui;
        }

        private bool _isConnected;
        public bool isConnected
        {
            get { return _isConnected; }
            set { _isConnected = value; OnPropertyChanged("isConnected"); }
        }

        private Comport_Interface _ILT_ComPort = new Comport_Interface();
        public Comport_Interface ILT_ComPort
        {
            get { return _ILT_ComPort; }
            set { _ILT_ComPort = value; OnPropertyChanged("ILT_ComPort"); }
        }

        private DispatcherTimer _ILT_Timer = new DispatcherTimer();
        public DispatcherTimer ILT_Timer
        {
            get { return _ILT_Timer; }
            set { _ILT_Timer = value; OnPropertyChanged("Time_Timer"); }
        }

        private bool _isSetup;
        public bool isSetup
        {
            get { return _isSetup; }
            set { _isSetup = value; OnPropertyChanged("isSetup"); }
        }

        private string ILT_ReceiveBuffer = "";
        const char endChar = '\r';

        public string ILT_Name = "QALAB";

        /* Tuning or running parameters */
        private int xCounter_Tuning = 0;
        public bool isTuning = false;
        public bool isRunning = false;

        public ILT2400()
        {

        }

        public bool Connect(string ComPort)
        {
            if(ComPort == String.Empty)
            {
                ComPort = "COM5";
            }
            if(SetSampleFreq(100, ComPort))
            {
                if(ConnectToCOM(ComPort))
                {
                    return true;
                }
            }
            return false;
        }

        public bool ConnectToCOM(string ComPort)
        {
            isConnected = false;

            SerialPort _Serial = new SerialPort();

            if(ComPort == String.Empty)
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

                _Serial.ReadTimeout = 2000;

                string ILT_Response = "";

                ILT_Response = _Serial.ReadLine();

                _Serial.Close();

                if(ILT_Response == ILT_Name + "\r")
                {
                    ILT_ComPort.PortName = ComPort;
                    ILT_ComPort.BaudRate = "115200";

                    ILT_ComPort.COM_UseRxTimer = false;

                    ILT_ComPort.RX_Handle += new Comport_Interface.RXHandler(ILT_Com_AddToRxBuffer);
                    ILT_ComPort.RX_HandlerEvent_Enable = true;

                    ILT_ComPort.OpenPort();

                    if(ILT_ComPort.PortConnected)
                    {
                        isConnected = true;
                        Gui.LightSensorInfo += "Light Sensor connected to " + ComPort;
                    }
                    else
                    {
                        isConnected = false;
                        ILT_ComPort.ClosePorts();
                        Gui.LightSensorInfo += "Couldn't open port";
                    }
                }
                else
                {
                    Gui.LightSensorInfo += "Wrong name of ILT!";
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                Gui.LightSensorInfo += "Could not connect to ILT sensor" + Environment.NewLine;
                return isConnected;
            }

            return isConnected;
        }

        public bool SetName(string newName, string comPort)
        {
            SerialPort _Serial = new SerialPort();

            _Serial.PortName = comPort;
            _Serial.BaudRate = 115200;
            _Serial.Parity = Parity.None;
            _Serial.DataBits = 8;
            _Serial.StopBits = StopBits.One;
            _Serial.Handshake = Handshake.None;

            if(comPort == null)
            {
                Gui.LightSensorInfo += "Please specify comport";
                return false;
            }

            try
            {
                _Serial.Open();

                string setFriendlyName = "setfriendlyname " + newName + "\r";

                _Serial.Write(setFriendlyName);

                _Serial.ReadTimeout = 2000;

                string ILT_Response = "";

                ILT_Response = _Serial.ReadLine();

                _Serial.Close();

                if (ILT_Response == "0\r")
                {
                    ILT_Name = newName.Substring(0, newName.Length - 1);

                    Properties.Settings.Default.ILT_Name = ILT_Name;

                    return true;
                }
                throw new Exception("Error changing ILT name");
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
        }

        public bool SetSampleFreq(int sFreq, string comPort)
        {
            SerialPort _Serial = new SerialPort();

            _Serial.PortName = comPort;
            _Serial.BaudRate = 115200;
            _Serial.Parity = Parity.None;
            _Serial.DataBits = 8;
            _Serial.StopBits = StopBits.One;
            _Serial.Handshake = Handshake.None;

            if (comPort == null)
            {
                Gui.LightSensorInfo += "Please specify comport";
                return false;
            }

            try
            {
                _Serial.Open();

                string setSampleTime = "setsampletime " + sFreq + "\r";

                _Serial.Write(setSampleTime);

                _Serial.ReadTimeout = 2000;

                string ILT_Response = "";

                ILT_Response = _Serial.ReadLine();

                _Serial.Close();

                if (ILT_Response == "0\r")
                {
                    Gui.LightSensorInfo += "Samplefrequeny: " + sFreq + Environment.NewLine;
                    return true;
                    
                }
                throw new Exception("Error changing ILT name");
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
        }

        public string GetName(string comPort)
        {
            SerialPort _Serial = new SerialPort();

            _Serial.PortName = comPort;
            _Serial.BaudRate = 115200;
            _Serial.Parity = Parity.None;
            _Serial.DataBits = 8;
            _Serial.StopBits = StopBits.One;
            _Serial.Handshake = Handshake.None;

            try
            {
                _Serial.Open();

                _Serial.Write("getfriendlyname\r");

                _Serial.ReadTimeout = 2000;

                string ILT_Response = "";

                ILT_Response = _Serial.ReadLine();

                _Serial.Close();

                return ILT_Response.Substring(0, ILT_Response.Length - 1);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return String.Empty;
            }
        }

        public string GetSampleFreq(string comPort)
        {
            SerialPort _Serial = new SerialPort();

            _Serial.PortName = comPort;
            _Serial.BaudRate = 115200;
            _Serial.Parity = Parity.None;
            _Serial.DataBits = 8;
            _Serial.StopBits = StopBits.One;
            _Serial.Handshake = Handshake.None;

            try
            {
                _Serial.Open();

                _Serial.Write("getsampletime\r");

                _Serial.ReadTimeout = 2000;

                string ILT_Response = "";

                ILT_Response = _Serial.ReadLine();

                _Serial.Close();

                return ILT_Response.Substring(0, ILT_Response.Length - 1);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
                return String.Empty;
            }
        }

        public void ILT_Com_AddToRxBuffer(string Data)
        {
            ILT_ReceiveBuffer += Data;
            ILT_Decode();
        }

        public void ILT_Decode()
        {
            int endPos = 0;
            string CheckData = "";

            try
            {
                /* Remove \n */
                endPos = ILT_ReceiveBuffer.LastIndexOf(endChar, ILT_ReceiveBuffer.Length - 1);

                if (endPos == -1)
                {
                    return;
                }

                /* Remove \r */
                CheckData = ILT_ReceiveBuffer.Substring(0, endPos);

                ILT_ReceiveBuffer = "";

                string[] parts = CheckData.Split('e');

                double lowerLux;
                int exponentLux;

                /* Parse data */
                double.TryParse(parts[0], System.Globalization.NumberStyles.Any, CultureInfo.InvariantCulture, out lowerLux);

                int.TryParse(parts[1], out exponentLux);

                double ILT_Value = lowerLux * Math.Pow(10, exponentLux);



                Gui.CurrentIrradiance = ILT_Value;
                Gui.Measure_Lux = (int)ILT_Value;

                /* Check if tuning */
                if (isTuning)
                {
                    /* Add PID data to tune graph */
                    Gui.Pid_TunerGraph.AddDataToGraph_NoUpdate(Gui.Pid_TunerGraph.CurveGetID("Nominal Lux"), xCounter_Tuning, ILT_Value);

                    Gui.PID.ProcessVariable = ILT_Value;
                    double CurrentToPSU = Gui.PID.Control(100);

                    Gui.PSU_LightBox.PSU_BK_Set_Current(CurrentToPSU);

                    //Gui.Pid_TunerGraph.AddDataToGraph_NoUpdate(Gui.Pid_TunerGraph.CurveGetID("PID Output"), xCounter_Tuning, CurrentToPSU);

                    xCounter_Tuning++;
                }
                else
                {
                    /* Make PID calculations */
                    Gui.PID.ProcessVariable = ILT_Value;
                    double CurrentToPSU = Gui.PID.Control(100);

                    /* Set PSU to pid output */
                    Gui.PSU_LightBox.PSU_BK_Set_Current(CurrentToPSU);

                    Gui.Graph_Lux.AddDataToGraph_NoUpdate(Gui.Graph_Lux.CurveGetID("Measured Lux"), Gui.Graph_Lux_Counter, ILT_Value);
                    Gui.Graph_Lux.AddDataToGraph_NoUpdate(Gui.Graph_Lux.CurveGetID("Nominal Lux"), Gui.Graph_Lux_Counter, Gui.PID.SetPoint);

                    Gui.Graph_Lux_Counter++;

                    //Gui.Graph_Small.AddDataToGraph_NoUpdate(Gui.Graph_Small.CurveGetID("ILT Lux"), Gui.Graph_Small_X_Counter, ILT_Value);
                    //Gui.Graph_ILT.AddDataToGraph_NoUpdate(Gui.Graph_ILT.CurveGetID("ILT Lux"), Gui.Graph_Small_X_Counter, ILT_Value);
                    //Gui.Graph_Small_X_Counter++;
                }
            }
            catch
            {
                ILT_ReceiveBuffer = "";
            }

        }

        public bool ILT_Setup()
        {
            return isSetup;
        }

        public void GetIrradianceValue()
        {
            ILT_ComPort.SendData("gi\r");
        }

        public void InitILT_Timer()
        {
            ILT_Timer.Tick += new EventHandler(ILT_Timer_Tick);
            ILT_Timer.Interval = new TimeSpan(0, 0, 0, 0, 1000);
        }

        public void StartILT_Timer(int sampleTime)
        {
            ILT_Timer.Interval = new TimeSpan(0, 0, 0, 0, sampleTime);
            ILT_Timer.Start();
        }

        public void StopILT_Timer()
        {
            ILT_Timer.Stop();
        }

        private void ILT_Timer_Tick(object sender, EventArgs e)
        {
            /* get value each tick from ILT */
            GetIrradianceValue();
        }

    }
}