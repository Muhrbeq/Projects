using System;
using System.Collections.Generic;
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

namespace ExegerHeadphoneLightSimulator
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
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

        private ILT2400 i2400 = new ILT2400();
        private STM32 mcu = new STM32();
        private GUI Gui = new GUI();

        public MainWindow()
        {
            InitializeComponent();

            this.DataContext = Gui;

            //Specify COM port (Hardcoded)
            if(i2400.ConnectToCOM("COM4"))
            {
                //Specify COM port (Hardcoded)
                if(mcu.ConnectSTM32("COM5"))
                {
                    InitILT_Timer();
                    StartILT_Timer(100);
                }
            }

            

            
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

        private void btn_CloseApplication_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private DispatcherTimer _ILT_Timer = new DispatcherTimer();
        public DispatcherTimer ILT_Timer
        {
            get { return _ILT_Timer; }
            set { _ILT_Timer = value; OnPropertyChanged("ILT_Timer"); }
        }

        public void InitILT_Timer()
        {
            ILT_Timer.Tick += new EventHandler(ILT_Timer_Tick);
            ILT_Timer.Interval = new TimeSpan(0, 0, 0, 0, 100);
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
            i2400.GetIrradianceValue();
            if(Gui.OPT1_Percent < 0.3)
            {
                txb_opt1.Foreground = Brushes.Red;
            }
            else if(Gui.OPT1_Percent < 0.7)
            {
                txb_opt1.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt1.Foreground = Brushes.Green;
            }

            if (Gui.OPT2_Percent < 0.3)
            {
                txb_opt2.Foreground = Brushes.Red;
            }
            else if (Gui.OPT2_Percent < 0.7)
            {
                txb_opt2.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt2.Foreground = Brushes.Green;
            }

            if (Gui.OPT3_Percent < 0.3)
            {
                txb_opt3.Foreground = Brushes.Red;
            }
            else if (Gui.OPT3_Percent < 0.7)
            {
                txb_opt3.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt3.Foreground = Brushes.Green;
            }

            if (Gui.OPT4_Percent < 0.3)
            {
                txb_opt4.Foreground = Brushes.Red;
            }
            else if (Gui.OPT4_Percent < 0.7)
            {
                txb_opt4.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt4.Foreground = Brushes.Green;
            }

            if (Gui.OPT5_Percent < 0.3)
            {
                txb_opt5.Foreground = Brushes.Red;
            }
            else if (Gui.OPT5_Percent < 0.7)
            {
                txb_opt5.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt5.Foreground = Brushes.Green;
            }

            if (Gui.OPT6_Percent < 0.3)
            {
                txb_opt6.Foreground = Brushes.Red;
            }
            else if (Gui.OPT6_Percent < 0.7)
            {
                txb_opt6.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt6.Foreground = Brushes.Green;
            }

            if (Gui.OPT7_Percent < 0.3)
            {
                txb_opt7.Foreground = Brushes.Red;
            }
            else if (Gui.OPT7_Percent < 0.7)
            {
                txb_opt7.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt7.Foreground = Brushes.Green;
            }

            if (Gui.OPT8_Percent < 0.3)
            {
                txb_opt8.Foreground = Brushes.Red;
            }
            else if (Gui.OPT8_Percent < 0.7)
            {
                txb_opt8.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt8.Foreground = Brushes.Green;
            }

            if (Gui.OPT9_Percent < 0.3)
            {
                txb_opt9.Foreground = Brushes.Red;
            }
            else if (Gui.OPT9_Percent < 0.7)
            {
                txb_opt9.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt9.Foreground = Brushes.Green;
            }

            if (Gui.OPT10_Percent < 0.3)
            {
                txb_opt10.Foreground = Brushes.Red;
            }
            else if (Gui.OPT10_Percent < 0.7)
            {
                txb_opt10.Foreground = Brushes.Yellow;
            }
            else
            {
                txb_opt10.Foreground = Brushes.Green;
            }
        }
    }
}
