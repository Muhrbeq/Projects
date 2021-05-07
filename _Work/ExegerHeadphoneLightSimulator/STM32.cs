using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using USART;

namespace ExegerHeadphoneLightSimulator
{
    public class STM32 : INotifyPropertyChanged
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

        private Comport_Interface _STM32_Com = new Comport_Interface();
        public Comport_Interface STM32_Com
        {
            get { return _STM32_Com; }
            set { _STM32_Com = value; OnPropertyChanged("STM32_Com"); }
        }

        private bool _isConnected = false;
        public bool isConnected
        {
            get { return _isConnected; }
            set { _isConnected = value; OnPropertyChanged("isConnected"); }
        }

        private bool isThermoCouple = false;

        private string STM32_RxBuffer = "";
        const char LF = '\r'; //13;
        const char NL = '\n'; //10;

        private int BatteryCounter = 1;

        public STM32()
        {

        }


        /// <summary>
        /// Connect STM to port and baud
        /// </summary>
        /// <param name="comport"></param>
        /// <param name="baudrate"></param>
        /// <returns></returns>
        public bool ConnectSTM32(string comport)
        {
            isConnected = false;

            SerialPort _sP = new SerialPort();

            try
            {
                STM32_Com.PortName = comport;
                STM32_Com.BaudRate = "115200";

                STM32_Com.COM_UseRxTimer = false;

                // Register RxEvent Handler function
                STM32_Com.RX_Handle += new Comport_Interface.RXHandler(STM32_Com_AddToRxBuffer);
                STM32_Com.RX_HandlerEvent_Enable = true;

                STM32_Com.OpenPort();

                if (STM32_Com.PortConnected)
                {
                    return isConnected;
                }
            }
            catch (Exception e)
            {
                STM32_Com.ClosePorts();
                MessageBox.Show(e.Message);
            }
            return isConnected;
        }

        /// <summary>
        /// Send command to STM wanting to get measurement
        /// </summary>
        public void GetMeasurement()
        {
            STM32_Com.SendData("$%30#\r");
        }

        private void STM32_Com_AddToRxBuffer(string Data)
        {
            STM32_RxBuffer += Data;
            STM32_Decode();
        }

        /// <summary>
        /// Decode what comes back from STM
        /// </summary>
        private void STM32_Decode()
        {
            int endPos = 0;
            string CheckData = "";

            endPos = STM32_RxBuffer.LastIndexOf('\n', STM32_RxBuffer.Length - 1);

            if (endPos == -1)
            {
                return;
            }

            CheckData = STM32_RxBuffer.Substring(0, endPos);

            STM32_RxBuffer = "";

            if (CheckData.Length > 0)
            {
                string[] parts = CheckData.Split(':');

                try
                {
                    if(parts[0] == "OPT1")
                    {
                        Gui.OPT1_Light = Double.Parse(parts[1]);
                        Gui.OPT1_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT2")
                    {
                        Gui.OPT2_Light = Double.Parse(parts[1]);
                        Gui.OPT2_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT3")
                    {
                        Gui.OPT3_Light = Double.Parse(parts[1]);
                        Gui.OPT3_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT4")
                    {
                        Gui.OPT4_Light = Double.Parse(parts[1]);
                        Gui.OPT4_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT5")
                    {
                        Gui.OPT5_Light = Double.Parse(parts[1]);
                        Gui.OPT5_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT6")
                    {
                        Gui.OPT6_Light = Double.Parse(parts[1]);
                        Gui.OPT6_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT7")
                    {
                        Gui.OPT7_Light = Double.Parse(parts[1]);
                        Gui.OPT7_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT8")
                    {
                        Gui.OPT8_Light = Double.Parse(parts[1]);
                        Gui.OPT8_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT9")
                    {
                        Gui.OPT9_Light = Double.Parse(parts[1]);
                        Gui.OPT9_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                    else if (parts[0] == "OPT10")
                    {
                        Gui.OPT10_Light = Double.Parse(parts[1]);
                        Gui.OPT10_Percent = Gui.ILT_Light / Double.Parse(parts[1]);
                    }
                }
                catch
                {
                    throw new Exception();
                }
            }
        }
    }
}
