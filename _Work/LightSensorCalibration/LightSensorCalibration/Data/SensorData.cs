using System;
using System.Collections.Generic;
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

    }

    public class SensorDataCollection
    {
        public ObservableCollection<SensorData> sData = new ObservableCollection<SensorData>();
        
    }
}
