using System;
using System.Collections.Generic;
using System.IO.Ports;
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
        private LightSensor _refLight;
        private LightSensor _calLight;
        private PowerSupplyUnit _PSU;
        private Microcontroller _MCU;
        private int ERRORLISTINDEX = -1;

        public ExtraInfoWindow(ref LightSensor ReferenceLightSensor, ref LightSensor CalibrationLightSensor, ref Microcontroller MCU, ref PowerSupplyUnit PSU)
        {
            InitializeComponent();

            _refLight = ReferenceLightSensor;
            _calLight = CalibrationLightSensor;
            _PSU = PSU;
            _MCU = MCU;

            GetComPorts();
            GetInstrumentInfo();

            //lstBox_ComPorts.Items.Add("Hello");
            //lstBox_Instruments.Items.Add("There");
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

        private void GetComPorts()
        {
            string[] ports = SerialPort.GetPortNames();

            lstBox_ComPorts.ItemsSource = ports;
        }

        private void GetInstrumentInfo()
        {
            if(!_refLight.isConnected)
            {
                _refLight.SensorName = _refLight.SensorName == null ? "Reference" : _refLight.SensorName;
                lstBox_Instruments.Items.Add(_refLight.SensorName);
            }
                
            if(!_calLight.isConnected)
            {
                _calLight.SensorName = _calLight.SensorName == null ? "Calibration" : _calLight.SensorName;
                lstBox_Instruments.Items.Add(_calLight.SensorName);
            }
                
            if(!_MCU.IsConnected)
            {
                _MCU.ID = _MCU.ID == null ? "MCU" : _MCU.ID;
                lstBox_Instruments.Items.Add(_MCU.ID);
            }
            
        }

        private void btn_ConnectInstrumentToPort_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if(lstBox_ComPorts.SelectedIndex != ERRORLISTINDEX)
                {
                    if(lstBox_Instruments.SelectedIndex != ERRORLISTINDEX)
                    {
                        //txbl_ConnectedInstruments.Text += lstBox_Instruments.SelectedItem.ToString() + " is connected to " + lstBox_ComPorts.SelectedItem.ToString() + Environment.NewLine;
                        if(TryConnect(lstBox_Instruments.SelectedItem.ToString(), lstBox_ComPorts.SelectedItem.ToString()))
                        {
                            txbl_ConnectedInstruments.Text += lstBox_Instruments.SelectedItem.ToString() + " is connected to " + lstBox_ComPorts.SelectedItem.ToString() + Environment.NewLine;
                            return;
                        }
                        
                    }
                }
                throw new InvalidOperationException("Select com port and instrument");
            }
            catch (InvalidOperationException ex)
            {
                MessageBox.Show(ex.Message);
            }
            //refLight.Connect("2");
            
        }

        private bool TryConnect(string ID, string comport)
        {
            if (ID == _refLight.SensorName)
                return _refLight.ConnectToCOM(comport);
            else if (ID == _calLight.SensorName)
                return _calLight.ConnectToCOM(comport);
            else if (ID == _MCU.ID)
                return _MCU.ConnectToPort(comport);
            return false;

        }
    }
}
