using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExegerHeadphoneLightSimulator
{
    public class GUI : INotifyPropertyChanged
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


        private double _OPT1_Light;
        public double OPT1_Light
        {
            get { return _OPT1_Light; }
            set { _OPT1_Light = value; OnPropertyChanged("OPT1_Light"); }
        }
        private double _OPT1_Percent;
        public double OPT1_Percent
        {
            get { return _OPT1_Percent; }
            set { _OPT1_Percent = value; OnPropertyChanged("OPT1_Percent"); }
        }

        private double _OPT2_Light;
        public double OPT2_Light
        {
            get { return _OPT2_Light; }
            set { _OPT2_Light = value; OnPropertyChanged("OPT2_Light"); }
        }
        private double _OPT2_Percent;
        public double OPT2_Percent
        {
            get { return _OPT2_Percent; }
            set { _OPT2_Percent = value; OnPropertyChanged("OPT2_Percent"); }
        }

        private double _OPT3_Light;
        public double OPT3_Light
        {
            get { return _OPT3_Light; }
            set { _OPT3_Light = value; OnPropertyChanged("OPT3_Light"); }
        }
        private double _OPT3_Percent;
        public double OPT3_Percent
        {
            get { return _OPT3_Percent; }
            set { _OPT3_Percent = value; OnPropertyChanged("OPT3_Percent"); }
        }

        private double _OPT4_Light;
        public double OPT4_Light
        {
            get { return _OPT4_Light; }
            set { _OPT4_Light = value; OnPropertyChanged("OPT4_Light"); }
        }
        private double _OPT4_Percent;
        public double OPT4_Percent
        {
            get { return _OPT4_Percent; }
            set { _OPT4_Percent = value; OnPropertyChanged("OPT4_Percent"); }
        }

        private double _OPT5_Light;
        public double OPT5_Light
        {
            get { return _OPT5_Light; }
            set { _OPT5_Light = value; OnPropertyChanged("OPT5_Light"); }
        }
        private double _OPT5_Percent;
        public double OPT5_Percent
        {
            get { return _OPT5_Percent; }
            set { _OPT5_Percent = value; OnPropertyChanged("OPT5_Percent"); }
        }

        private double _OPT6_Light;
        public double OPT6_Light
        {
            get { return _OPT6_Light; }
            set { _OPT6_Light = value; OnPropertyChanged("OPT6_Light"); }
        }
        private double _OPT6_Percent;
        public double OPT6_Percent
        {
            get { return _OPT6_Percent; }
            set { _OPT6_Percent = value; OnPropertyChanged("OPT6_Percent"); }
        }

        private double _OPT7_Light;
        public double OPT7_Light
        {
            get { return _OPT7_Light; }
            set { _OPT7_Light = value; OnPropertyChanged("OPT7_Light"); }
        }
        private double _OPT7_Percent;
        public double OPT7_Percent
        {
            get { return _OPT7_Percent; }
            set { _OPT7_Percent = value; OnPropertyChanged("OPT7_Percent"); }
        }

        private double _OPT8_Light;
        public double OPT8_Light
        {
            get { return _OPT8_Light; }
            set { _OPT8_Light = value; OnPropertyChanged("OPT8_Light"); }
        }
        private double _OPT8_Percent;
        public double OPT8_Percent
        {
            get { return _OPT8_Percent; }
            set { _OPT8_Percent = value; OnPropertyChanged("OPT8_Percent"); }
        }

        private double _OPT9_Light;
        public double OPT9_Light
        {
            get { return _OPT9_Light; }
            set { _OPT9_Light = value; OnPropertyChanged("OPT9_Light"); }
        }
        private double _OPT9_Percent;
        public double OPT9_Percent
        {
            get { return _OPT9_Percent; }
            set { _OPT9_Percent = value; OnPropertyChanged("OPT9_Percent"); }
        }

        private double _OPT10_Light;
        public double OPT10_Light
        {
            get { return _OPT10_Light; }
            set { _OPT10_Light = value; OnPropertyChanged("OPT10_Light"); }
        }
        private double _OPT10_Percent;
        public double OPT10_Percent
        {
            get { return _OPT10_Percent; }
            set { _OPT10_Percent = value; OnPropertyChanged("OPT10_Percent"); }
        }

        private double _ILT_Light;
        public double ILT_Light
        {
            get { return _ILT_Light; }
            set { _ILT_Light = value; OnPropertyChanged("ILT_Light"); }
        }
    }
}
