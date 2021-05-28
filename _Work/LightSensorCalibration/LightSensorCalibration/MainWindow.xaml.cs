using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using LightSensorCalibration.Data;
using LightSensorCalibration.Instruments;

namespace LightSensorCalibration
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window , INotifyPropertyChanged

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

        GlobalVariables g = new GlobalVariables();

        LightSensor ReferenceSensor = new LightSensor(SensorState.SENSOR_REFERENCE);
        LightSensor CalibrationSensor = new LightSensor(SensorState.SENSOR_CALIBRATION);
        
        Microcontroller MCU = new Microcontroller();

        ObservableCollection<SensorData> sData = new ObservableCollection<SensorData>();


        public MainWindow()
        {
            InitializeComponent();

            this.DataContext = g;

            ReferenceSensor.SetReference(ref g);
            CalibrationSensor.SetReference(ref g);
            MCU.SetReference(ref g);

            InitCalibrationLightSensorTimer();
            InitReferenceLightSensorTimer();
            InitMCUTimer();

            g.ReferencePID = new PID_Controller(7E-05, 0.0001, 9E-07, 0, 8);
            g.PSU = new PowerSupplyUnit(24.0);
            g.PSU.SetupStandardPSU();

            //Try to connect to Reference Light sensor
            
            switch (ReferenceSensor.ConnectSensor())
            {
                case ReturnLightSensorState.LIGHTSENSORRETURN_NOLIGHTSENSORFOUND:
                    txb_InfoBox.Text += "Reference - No light sensor found" + Environment.NewLine;
                    break;
                case ReturnLightSensorState.LIGHTSENSORRETURN_WRONGREFERENCENAME:
                    txb_InfoBox.Text += "Reference - Wrong name on reference device - Should be: QAREF" + Environment.NewLine;
                    break;

                case ReturnLightSensorState.LIGHTSENSORRETURN_PASS:
                    txb_InfoBox.Text += "Reference - Connected, Serial: " + ReferenceSensor.SerialNumber + Environment.NewLine;
                    ReferenceSensor.Connect();
                    icon_btnConnectReferenceSensor.Foreground = Brushes.Green;
                    break;
            }

            //Try to connect to Calibration Light sensor
            switch (CalibrationSensor.ConnectSensor())
            {
                case ReturnLightSensorState.LIGHTSENSORRETURN_NOLIGHTSENSORFOUND:
                    txb_InfoBox.Text += "Calibration - No light sensor found" + Environment.NewLine;
                    break;
                case ReturnLightSensorState.LIGHTSENSORRETURN_SETSAMPLEFREQUENCYERROR:
                    txb_InfoBox.Text += "Calibration - Sample frequency couldn't be changed" + Environment.NewLine;
                    break;

                case ReturnLightSensorState.LIGHTSENSORRETURN_PASS:
                    txb_InfoBox.Text += "Calibration - Connected, Serial: " + CalibrationSensor.SerialNumber + Environment.NewLine;
                    CalibrationSensor.Connect();
                    icon_btnConnectCalibrationSensor.Foreground = Brushes.Green;
                    break;
            }

            switch (g.PSU.Connect())
            {
                case PSU_ReturnCodes.PSU_COULDNTFINDDEVICE:
                    txb_InfoBox.Text += "PSU - No device found" + Environment.NewLine;
                    break;
                case PSU_ReturnCodes.PSU_SETVOLTAGEFAIL:
                    txb_InfoBox.Text += "PSU - Voltage set failed" + Environment.NewLine;
                    break;
                case PSU_ReturnCodes.PSU_SETCURRENTFAIL:
                    txb_InfoBox.Text += "PSU - Current set failed" + Environment.NewLine;
                    break;
                case PSU_ReturnCodes.PSU_PASSED:
                    icon_btnConnectPSU.Foreground = Brushes.Green;
                    txb_InfoBox.Text += "PSU - Connected" + Environment.NewLine;
                    break;
            }

            //switch(MCU.ConnectMCU())
            //{
            //    case MCU_ReturnCodes.MCU_COULDNTCONNECT:
            //        txb_InfoBox.Text += "MCU - Couldn't connect" + Environment.NewLine;
            //        break;
            //    case MCU_ReturnCodes.MCU_COULDNTFINDDEVICE:
            //        txb_InfoBox.Text += "MCU - No devices found" + Environment.NewLine;
            //        break;
            //    case MCU_ReturnCodes.MCU_PASSED:
            //        iConnectMicrocontroller.Foreground = Brushes.Green;
            //        txb_InfoBox.Text += "MCU - Connected" + Environment.NewLine;
            //        break;
            //}

            //Try to connect to Power supply
        }

        #region Timers
        private DispatcherTimer _Reference_Timer = new DispatcherTimer();
        public DispatcherTimer Reference_Timer
        {
            get { return _Reference_Timer; }
            set { _Reference_Timer = value; OnPropertyChanged("Reference_Timer"); }
        }

        private void InitReferenceLightSensorTimer()
        {
            Reference_Timer.Tick += new EventHandler(ReferenceLightSensor_Timer_Tick);
            Reference_Timer.Interval = new TimeSpan(0, 0, 0, 0, 1000);
        }

        private void StartReferenceLightSensorTimer(int UpdateRate_ms)
        {
            Reference_Timer.Interval = new TimeSpan(0, 0, 0, 0, UpdateRate_ms);
            Reference_Timer.Start();
        }

        private void StopReferenceLightSensorTimer()
        {
            Reference_Timer.Stop();
        }

        private void ReferenceLightSensor_Timer_Tick(object sender, EventArgs e)
        {
            ReferenceSensor.GetIrradianceValue();

            g.ReferenceLux = ReferenceSensor.CurrentIrradiance;

            if(g.LockPowerSupplyCurrent || Math.Floor(ReferenceSensor.CurrentIrradiance) == g.SetLightLevel)
            {
                g.LockPowerSupplyCurrent = true;
            }
            else
            {
                g.ReferencePID.ProcessVariable = ReferenceSensor.CurrentIrradiance;

                g.PSU.SetOutputCurrent(g.ReferencePID.Control(100));
            }

            if(Math.Floor(ReferenceSensor.CurrentIrradiance) < g.SetLightLevel * 0.95 || Math.Floor(ReferenceSensor.CurrentIrradiance) > g.SetLightLevel * 1.05)
            {
                g.LockPowerSupplyCurrent = false;
            }
        }

        private DispatcherTimer _Calibration_Timer = new DispatcherTimer();
        public DispatcherTimer Calibration_Timer
        {
            get { return _Calibration_Timer; }
            set { _Calibration_Timer = value; OnPropertyChanged("Calibration_Timer"); }
        }

        private void InitCalibrationLightSensorTimer()
        {
            Calibration_Timer.Tick += new EventHandler(CalibrationLightSensor_Timer_Tick);
            Calibration_Timer.Interval = new TimeSpan(0, 0, 0, 0, 1000);
        }

        private void StartCalibrationLightSensorTimer(int UpdateRate_ms)
        {
            Calibration_Timer.Interval = new TimeSpan(0, 0, 0, 0, UpdateRate_ms);
            Calibration_Timer.Start();
        }

        private void StopCalibrationLightSensorTimer()
        {
            Calibration_Timer.Stop();
        }

        private void CalibrationLightSensor_Timer_Tick(object sender, EventArgs e)
        {
            CalibrationSensor.GetIrradianceValue();

            g.CalibrationLux = CalibrationSensor.CurrentIrradiance;
        }


        private DispatcherTimer _MCU_Timer = new DispatcherTimer();
        public DispatcherTimer MCU_Timer
        {
            get { return _MCU_Timer; }
            set { _MCU_Timer = value; OnPropertyChanged("MCU_Timer"); }
        }

        private void InitMCUTimer()
        {
            MCU_Timer.Tick += new EventHandler(MCU_Timer_Tick);
            MCU_Timer.Interval = new TimeSpan(0, 0, 0, 0, 1000);
        }

        private void StartMCUTimer(int UpdateRate_ms)
        {
            MCU_Timer.Interval = new TimeSpan(0, 0, 0, 0, UpdateRate_ms);
            MCU_Timer.Start();
        }

        private void StopMCUTimer()
        {
            MCU_Timer.Stop();
        }

        private void MCU_Timer_Tick(object sender, EventArgs e)
        {
            MCU.RequestData();
        }

        #endregion

        #region Buttons
        private void btn_CloseApplication_Click(object sender, RoutedEventArgs e)
        {
            g.PSU.SetEnable(false);
            this.Close();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.Opacity = 0.5;
                DragMove();
            }
            if (e.LeftButton == MouseButtonState.Released)
            {
                this.Opacity = 1.0;
            }
        }

        

        

        private void btn_ExportData_Click(object sender, RoutedEventArgs e)
        {
            ExportData.ExportToCSV(ReferenceSensor, CalibrationSensor, g.PSU, sData);
        }

        private void btn_SetLightLevel_Click(object sender, RoutedEventArgs e)
        {
            icon_BtnSetLightLevel.Foreground = g.SetLightLevel == 0 ? Brushes.Black : Brushes.Yellow;

            if(g.SetLightLevel != 0)
            {
                g.PSU.SetEnable(true);
                StartReferenceLightSensorTimer(100);
                StartCalibrationLightSensorTimer(100);
                //StartMCUTimer(1000);
            }
            else
            {
                StopReferenceLightSensorTimer();
                StopCalibrationLightSensorTimer();
                //StopMCUTimer();
            }

        }

        private void btn_ExtraInfo_Click(object sender, RoutedEventArgs e)
        {
            ExtraInfoWindow extraInfoW = new ExtraInfoWindow(ref ReferenceSensor, ref CalibrationSensor, ref MCU);

            extraInfoW.ShowDialog();
        }

        private void btn_SaveLightLevel_Click(object sender, RoutedEventArgs e)
        {
            SensorData sD = new SensorData();

            sD.CalibrationLight = g.CalibrationLux;
            sD.ReferenceLight = g.ReferenceLux;
            sD.SetLight = g.SetLightLevel;
            sD.Humidity = g.SensorHumidity;
            sD.Temperature = g.SensorTemperature;

            sData.Add(sD);
        }

        private void btn_ConnectReferenceLightSensor_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {

        }

        private void btn_ConnectCalibrationLightSensor_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {

        }

        private void btn_ConnectPowerSupply_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {

        }
        #endregion

        private void tbx_SetLightLevel_TextChanged(object sender, TextChangedEventArgs e)
        {
            g.LockPowerSupplyCurrent = false;
        }

        private void btn_PSU_Voltage_Click(object sender, RoutedEventArgs e)
        {
            if(btn_PSU_Voltage.Content.ToString() == "32V PSU")
            {
                btn_PSU_Voltage.Content = "24V PSU";
                g.PSU.SetMaximumVoltage(24);
            }
            else
            {
                btn_PSU_Voltage.Content = "32V PSU";
                g.PSU.SetMaximumVoltage(32);
            }
        }

        private void btn_UpdateLightLevel_Click(object sender, RoutedEventArgs e)
        {
            g.ReferencePID.SetPoint = g.SetLightLevel;
        }
    }
}
