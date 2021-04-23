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

namespace QA_LightBoxController
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
        /// Try to connect from startup of program with previous com port
        /// </summary>
        /// <returns></returns>
        public bool ConnectFromStart()
        {
            if (ConnectSTM32(Properties.Settings.Default.STM32_Comport, Properties.Settings.Default.STM32_Baudrate))
            {
                //MessageBox.Show("STM32 Found at startup!");
                Gui.STM_Info += "STM found at startup!" + Environment.NewLine;
                return true;
            }
            Gui.STM_Info += "STM NOT found at startup!" + Environment.NewLine;
            return false;
        }

        /// <summary>
        /// Connect STM to port and baud
        /// </summary>
        /// <param name="comport"></param>
        /// <param name="baudrate"></param>
        /// <returns></returns>
        public bool ConnectSTM32(string comport, string baudrate)
        {
            isConnected = false;

            SerialPort _sP = new SerialPort();

            try
            {
                STM32_Com.PortName = comport;
                STM32_Com.BaudRate = baudrate;

                STM32_Com.COM_UseRxTimer = false;

                Properties.Settings.Default.STM32_Comport = comport;

                _sP.PortName = comport;
                _sP.BaudRate = 115200;
                _sP.Parity = Parity.None;
                _sP.DataBits = 8;
                _sP.StopBits = StopBits.One;
                _sP.Handshake = Handshake.None;

                _sP.Open();

                _sP.Write("$%31#\r");

                _sP.ReadTimeout = 1000;

                string STM_Name = "";

                STM_Name = _sP.ReadLine();

                _sP.Close();

                // Register RxEvent Handler function
                STM32_Com.RX_Handle += new Comport_Interface.RXHandler(STM32_Com_AddToRxBuffer);
                STM32_Com.RX_HandlerEvent_Enable = true;

                STM32_Com.OpenPort();

                if (STM32_Com.PortConnected)
                {
                    if (STM_Name == "$*107#")
                    {
                        isThermoCouple = false;
                        isConnected = true;
                    }
                    else if (STM_Name == "$*108#")
                    {
                        isConnected = true;
                        isThermoCouple = true;
                    }
                    else
                    {
                        throw new Exception("Not correct name on stm");
                    }
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
            string TempData = "";

            endPos = STM32_RxBuffer.LastIndexOf('#', STM32_RxBuffer.Length - 1);

            if (endPos == -1)
            {
                return;
            }

            CheckData = STM32_RxBuffer.Substring(0, endPos);

            STM32_RxBuffer = "";

            if (CheckData.Length > 0)
            {
                string[] parts = CheckData.Split('*');

                if (parts[0].StartsWith("$"))
                {
                    string[] SensorData = parts[1].Split(':');

                    if (SensorData.Length > 2)
                    {
                        try
                        {
                            /* Parse all data */
                            //CellData cD = new CellData;

                            //cD.MeasurementLevel = Gui.MeasurementIndex;

                            Gui.TemperatureInsideBox = int.Parse(SensorData[0], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);
                            Gui.TemperatureInsideBox /= 1000;

                            Gui.HumidityInsideBox = int.Parse(SensorData[1], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);

                            //Gui.ThermocoupleTemperature = int.Parse(SensorData[2], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);
                            //Gui.ThermocoupleTemperature /= 1000;

                            Gui.AmbientTemperature = int.Parse(SensorData[2], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);
                            Gui.AmbientTemperature /= 1000;

                            Gui.AmbientHumidity = int.Parse(SensorData[3], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);

                            //cD.Temperature = int.Parse(SensorData[0], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);
                            //cD.Temperature /= 1000;
                            //cD.Humidity = int.Parse(SensorData[1], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);
                            //cD.Humidity /= 1000;

                            //if (isThermoCouple)
                            //{
                            //    cD.ThermoCoupleTemperature = int.Parse(SensorData[2], NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture);
                            //}
                            //else
                            //{
                            //    cD.ThermoCoupleTemperature = 0;
                            //}

                            //cD.IrradianceActual = Gui.CurrentIrradiance;
                            //cD.IrradianceGoal = Gui.PID.SetPoint;

                            /* Add to graph, whether sampling or not */
                            Gui.Graph_Temperature.AddDataToGraph_NoUpdate(Gui.Graph_Temperature.CurveGetID("Temperature Inside"), Gui.Graph_Temperature_Counter, Gui.TemperatureInsideBox);
                            Gui.Graph_Temperature.AddDataToGraph_NoUpdate(Gui.Graph_Temperature.CurveGetID("Ambient Temperature"), Gui.Graph_Temperature_Counter, Gui.AmbientTemperature);
                            Gui.Graph_Humidity.AddDataToGraph_NoUpdate(Gui.Graph_Humidity.CurveGetID("Humidity Inside"), Gui.Graph_Humidity_Counter, Gui.HumidityInsideBox);
                            Gui.Graph_Humidity.AddDataToGraph_NoUpdate(Gui.Graph_Humidity.CurveGetID("Ambient Humidity"), Gui.Graph_Humidity_Counter, Gui.AmbientHumidity);
                            Gui.Graph_Thermocouple.AddDataToGraph_NoUpdate(Gui.Graph_Thermocouple.CurveGetID("Thermocouple Temperature"), Gui.Graph_Thermocouple_Counter, 0);
                            //Gui.Graph_Power_InOut.AddDataToGraph_NoUpdate(Gui.Graph_Power_InOut.CurveGetID("Power Out/In"), BatteryCounter, PowerOutIn);

                            //if (Gui.isTakingSample)
                            //{
                            //    /* If taking sample, add it to the data central */
                            //    Gui.DataCentral.DataCentralCellData.Add(cD);
                            //}

                            Gui.Graph_Thermocouple_Counter++;
                            Gui.Graph_Temperature_Counter++;
                            Gui.Graph_Humidity_Counter++;
                        }
                        catch
                        {
                            throw new Exception();
                        }
                    }
                }
            }
        }
    }
}
