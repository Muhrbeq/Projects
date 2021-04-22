using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;

namespace LightSensorCalibration.Instruments
{
    public class Microcontroller
    {
        public string ID;
        public bool IsConnected;

        public void RequestData()
        {

        }

        public bool Connect()
        {
            return true;
        }

        public DispatcherTimer MicrocontrollerTimer = new DispatcherTimer();
    }
}
