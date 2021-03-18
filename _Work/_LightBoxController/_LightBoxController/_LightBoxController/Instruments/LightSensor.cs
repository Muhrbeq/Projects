using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _LightBoxController.Instruments
{
    public class LightSensor
    {
        public string ID
        {
            get { return _ID; };
        }
        private string _ID;

        public double CurrentLightIntensity
        {
            get { return _CurrentLightIntensity; };
        }
        private double _CurrentLightIntensity;
    }
}
