using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LightSensorCalibration.Instruments
{
    public class PID_Controller : INotifyPropertyChanged
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

        private double _P_Gain;
        public double P_Gain
        {
            get { return _P_Gain; }
            set { _P_Gain = value; OnPropertyChanged("P_Gain"); }
        }

        private double _I_Gain;
        public double I_Gain
        {
            get { return _I_Gain; }
            set { _I_Gain = value; OnPropertyChanged("I_Gain"); }
        }

        private double _D_Gain;
        public double D_Gain
        {
            get { return _D_Gain; }
            set { _D_Gain = value; OnPropertyChanged("D_Gain"); }
        }

        private double _PID_MinValue;
        public double PID_MinValue
        {
            get { return _PID_MinValue; }
            set { _PID_MinValue = value; OnPropertyChanged("PID_MinValue"); }
        }

        private double _PID_MaxValue;
        public double PID_MaxValue
        {
            get { return _PID_MaxValue; }
            set { _PID_MaxValue = value; OnPropertyChanged("PID_MaxValue"); }
        }

        private double _SetPoint;
        public double SetPoint
        {
            get { return _SetPoint; }
            set { _SetPoint = value; OnPropertyChanged("SetPoint"); }
        }

        private double _ProcessVariable = 0;
        public double ProcessVariable
        {
            get { return _ProcessVariable; }
            set
            {
                _ProcessVariableLast = _ProcessVariable;
                _ProcessVariable = value;
                OnPropertyChanged("ProcessVariable");
            }
        }

        private double _ProcessVariableLast;
        public double ProcessVariableLast
        {
            get { return _ProcessVariableLast; }
            private set { _ProcessVariableLast = value; OnPropertyChanged("ProcessVariable"); }

        }

        private double _IntegralTerm;
        public double IntegralTerm
        {
            get { return _IntegralTerm; }
            set { _IntegralTerm = value; OnPropertyChanged("IntegralTerm"); }
        }


        public PID_Controller(double P, double I, double D, double minValue, double maxValue)
        {
            P_Gain = P;
            I_Gain = I;
            D_Gain = D;
            PID_MinValue = minValue;
            PID_MaxValue = maxValue;
        }

        public double Control(int milliSeconds)
        {
            double error = SetPoint - ProcessVariable;

            double secondsSinceLast = milliSeconds / 1000.0;

            double ProportionalTerm = P_Gain * error;

            IntegralTerm += I_Gain * error * secondsSinceLast;

            IntegralTerm = OutputBound(IntegralTerm);

            double dInput = ProcessVariable - ProcessVariableLast;
            double DerivativeTerm = D_Gain * dInput / secondsSinceLast;

            double outputControl = ProportionalTerm + IntegralTerm + DerivativeTerm;

            return OutputBound(outputControl);
        }

        private double OutputBound(double clamp)
        {
            if (clamp >= PID_MaxValue)
            {
                return PID_MaxValue;
            }
            if (clamp <= PID_MinValue)
            {
                return PID_MinValue;
            }
            return clamp;
        }
    }
}
