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
using System.Windows.Shapes;
using LightSensorCalibration.Instruments;

namespace LightSensorCalibration
{
    /// <summary>
    /// Interaction logic for ExtraInfoWindow.xaml
    /// </summary>
    public partial class ExtraInfoWindow : Window
    {
        LightSensor refLight;

        public ExtraInfoWindow(ref LightSensor ReferenceLightSensor, ref LightSensor CalibrationLightSensor, ref Microcontroller MCU, ref PowerSupplyUnit PSU)
        {
            InitializeComponent();

            refLight = ReferenceLightSensor;

            lstBox_ComPorts.Items.Add("Hello");
            lstBox_Instruments.Items.Add("There");
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

        private void btn_Close_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btn_ConnectInstrumentToPort_Click(object sender, RoutedEventArgs e)
        {
            //refLight.Connect("2");
            txbl_ConnectedInstruments.Text += "LightSensor Connected to Com 5" + Environment.NewLine ;
        }
    }
}
