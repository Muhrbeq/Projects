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
using LightSensorCalibration.Instruments;

namespace LightSensorCalibration
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        LightSensor ReferenceSensor = new LightSensor();
        LightSensor CalibrationSensor = new LightSensor();
        PowerSupplyUnit PSU = new PowerSupplyUnit();
        Microcontroller MCU = new Microcontroller();

        public MainWindow()
        {
            InitializeComponent();
        }

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

        }

        private void btn_LockLightLevel_Click(object sender, RoutedEventArgs e)
        {

        }

        private void btn_ConnectReferenceLightSensor_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {

        }

        private void btn_ConnectCalibrationLightSensor_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {

        }

        private void btn_ConnectPowerSupply_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            txb_InfoBox.Text += "Helloooooo" + Environment.NewLine;
            scr_InfoScroll.ScrollToEnd();
        }

        private void btn_ConnectMicrocontroller_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            iConnectMicrocontroller.Foreground = Brushes.Green;
        }

        private void btn_ExtraInfo_Click(object sender, RoutedEventArgs e)
        {
            ExtraInfoWindow eW = new ExtraInfoWindow(ReferenceSensor, CalibrationSensor, MCU, PSU);

            eW.ShowDialog();
        }

        private void btn_SaveLightLevel_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
