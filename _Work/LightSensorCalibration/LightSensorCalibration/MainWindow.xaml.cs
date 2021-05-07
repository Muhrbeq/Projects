using System;
using System.Collections.Generic;
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
using LightSensorCalibration.Instruments;

namespace LightSensorCalibration
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        GlobalVariables g = new GlobalVariables();

        LightSensor ReferenceSensor = new LightSensor();
        LightSensor CalibrationSensor = new LightSensor();
        PowerSupplyUnit PSU = new PowerSupplyUnit(24.0);
        Microcontroller MCU = new Microcontroller();
        PID_Controller ReferencePID = new PID_Controller(7E-05, 0.0001, 9E-07, 0, 8);

        private int _SetLightLevel;
        public int SetLightLevel
        {
            get { return _SetLightLevel; }
            set { _SetLightLevel = value; }
        }

        public MainWindow()
        {
            InitializeComponent();

            this.DataContext = g;

            //Try to connect to Reference Light sensor
            switch (ReferenceSensor.ConnectSensor())
            {
                case ReturnLightSensorState.LIGHTSENSORRETURN_NOLIGHTSENSORFOUND:
                    txb_InfoBox.Text += "Reference - No light sensor found" + Environment.NewLine;
                    break;
                case ReturnLightSensorState.LIGHTSENSORRETURN_SETSAMPLEFREQUENCYERROR:
                    txb_InfoBox.Text += "Reference - Sample frequency couldn't be changed" + Environment.NewLine;
                    break;

                case ReturnLightSensorState.LIGHTSENSORRETURN_PASS:
                    txb_InfoBox.Text += "Reference - Connected, Serial: " + ReferenceSensor.SerialNumber + Environment.NewLine;
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
                    icon_btnConnectCalibrationSensor.Foreground = Brushes.Green;
                    break;
            }

            switch (PSU.Connect())
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

            //Try to connect to Power supply
        }

        #region Timers
        private DispatcherTimer _Reference_Timer = new DispatcherTimer();
        public DispatcherTimer Reference_Timer
        {
            get { return _Reference_Timer; }
            set { _Reference_Timer = value;}
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

            if(g.LockPowerSupplyCurrent || Math.Floor(g.CurrentIrradiance) == SetLightLevel)
            {
                g.LockPowerSupplyCurrent = true;
            }
            else
            {
                ReferencePID.ProcessVariable = g.CurrentIrradiance;
                double CurrentToPSU = ReferencePID.Control(100);
                PSU.SetOutputCurrent(CurrentToPSU);
            }
        }

        #endregion

        #region Buttons
        private void btn_CloseApplication_Click(object sender, RoutedEventArgs e)
        {
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
            txb_CalibrationLux.Text = "10000";
            txb_ReferenceLux.Text = "15000";
        }

        private void btn_SetLightLevel_Click(object sender, RoutedEventArgs e)
        {
            icon_BtnSetLightLevel.Foreground = icon_BtnSetLightLevel.Foreground == Brushes.Yellow ? Brushes.Black : Brushes.Yellow;
        }

        private void btn_LockLightLevel_Click(object sender, RoutedEventArgs e)
        {
            switch (icon_BtnLockLightLevel.Kind)
            {
                case MahApps.Metro.IconPacks.PackIconMaterialKind.Lock:
                    {
                        icon_BtnLockLightLevel.Kind = MahApps.Metro.IconPacks.PackIconMaterialKind.LockOpen;
                        icon_BtnLockLightLevel.Foreground = Brushes.Green;
                        break;
                    }
                default:
                    icon_BtnLockLightLevel.Kind = MahApps.Metro.IconPacks.PackIconMaterialKind.Lock;
                    icon_BtnLockLightLevel.Foreground = Brushes.Red;
                    break;
            }
        }

        private void btn_ExtraInfo_Click(object sender, RoutedEventArgs e)
        {
            ExtraInfoWindow extraInfoW = new ExtraInfoWindow(ref ReferenceSensor, ref CalibrationSensor, ref MCU, ref PSU);

            extraInfoW.ShowDialog();
        }

        private void btn_SaveLightLevel_Click(object sender, RoutedEventArgs e)
        {

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
    }
}
