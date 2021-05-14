using LightSensorCalibration.Instruments;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LightSensorCalibration
{
    public class GlobalVariables : INotifyPropertyChanged
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

        private int _SetLightLevel;
        public int SetLightLevel
        {
            get { return _SetLightLevel; }
            set { _SetLightLevel = value; OnPropertyChanged("SetLightLevel"); }
        }

        private double _CalibrationLux;
        public double CalibrationLux
        {
            get { return _CalibrationLux; }
            set { _CalibrationLux = value; OnPropertyChanged("CalibrationLux"); }
        }

        private double _ReferenceLux;
        public double ReferenceLux
        {
            get { return _ReferenceLux; }
            set { _ReferenceLux = value; OnPropertyChanged("ReferenceLux"); }
        }

        private double _SensorTemperature;
        public double SensorTemperature
        {
            get { return _SensorTemperature; }
            set { _SensorTemperature = value; OnPropertyChanged("SensorTemperature"); }
        }

        private double _SensorHumidity;
        public double SensorHumidity
        {
            get { return _SensorHumidity; }
            set { _SensorHumidity = value; OnPropertyChanged("SensorHumidity"); }
        }

        public PID_Controller ReferencePID = new PID_Controller(7E-05, 0.0001, 9E-07, 0, 8);
        public PowerSupplyUnit PSU = new PowerSupplyUnit(24.0);

        public double CurrentIrradiance = 0.0d;
        public bool LockPowerSupplyCurrent = false;

        
    }
}
