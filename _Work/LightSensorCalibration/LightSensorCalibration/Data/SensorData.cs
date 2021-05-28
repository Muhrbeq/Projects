using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LightSensorCalibration.Data
{
    public class SensorData
    {
        private double _PlateTemperature;
        public double PlateTemperature
        {
            get{ return _PlateTemperature;}
            set{ _PlateTemperature = value; }
        }
        private double _Lux;
        public double Lux
        {
            get{ return _Lux;}
            set{ _Lux = value; }
        }
        private double _BoxHumidity;
        public double BoxHumidity
        {
            get{ return _BoxHumidity;}
            set{ _BoxHumidity = value; }
        }
        private double _OutsideTemperature;
        public double OutsideTemperature
        {
            get{ return _OutsideTemperature;}
            set{ _OutsideTemperature = value; }
        }
        private double _OutsideHumidity;
        public double OutsideHumidity
        {
            get{return _OutsideHumidity;}
            set { _OutsideHumidity = value; }
        }

        private double _ReferenceLight;
        public double ReferenceLight
        {
            get { return _ReferenceLight; }
            set { _ReferenceLight = value; }
        }

        private double _CalibrationLight;
        public double CalibrationLight
        {
            get { return _CalibrationLight; }
            set { _CalibrationLight = value; }
        }

        private double _SetLight;
        public double SetLight
        {
            get { return _SetLight; }
            set { _SetLight = value; }
        }

        private double _Temperature;
        public double Temperature
        {
            get { return _Temperature; }
            set { _Temperature = value; }
        }

        private double _Humidity;
        public double Humidity
        {
            get { return _Humidity; }
            set { _Humidity = value; }
        }

    }

    public class SensorDataCollection
    {
        public ObservableCollection<SensorData> sData = new ObservableCollection<SensorData>();
        
    }
}
