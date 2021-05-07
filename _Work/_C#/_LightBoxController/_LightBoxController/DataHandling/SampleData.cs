using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _LightBoxController.DataHandling
{
    public class SampleData
    {
        public ObservableCollection<CellData> cellData;
        public ObservableCollection<SensorData> sensorData;
        public ObservableCollection<InstrumentData> instrumentData;
    }

    public class CellData
    {
        double CellVoltage = 0.0d;
        double CellCurrent = 0.0d;
        double BoostVoltage = 0.0d;
        double BoostCurrent = 0.0d;

        double PowerIn = 0.0d;
        double PowerOut = 0.0d;

        double BoostEfficiency = 0.0d;
        double BoostLoss = 0.0d;
    }

    public class SensorData
    {
        /* Temperature sensor */
        double CellTemperature = 0.0d;
        double AmbientTemperature = 0.0d;
        double PCBTemperature = 0.0d;

        /* Humidity */
        double AmbientHumidity = 0.0d;
        double PCBHumidity = 0.0d;

        /* Derivative */
        double dAmbientTemperature = 0.0d;
        double dPCBTemperaure = 0.0d;

        double dAmbientHumidity = 0.0d;
        double dPCBHumidity = 0.0d;
    }

    public class InstrumentData
    {
        string InstrumentName = String.Empty;
        string InstrumentPort = String.Empty;
        string InstrumentVariable_1 = String.Empty;
        string InstrumentVariable_2 = String.Empty;
        string InstrumentVariable_3 = String.Empty;
    }



}
