using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LightSensorCalibration
{
    public class GlobalVariables
    {
        public double CurrentIrradiance = 0.0d;
        public bool LockPowerSupplyCurrent = false;

        public double CalibrationLux;
        public double ReferenceLux;
    }
}
