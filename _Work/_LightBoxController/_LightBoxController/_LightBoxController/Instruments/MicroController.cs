using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _LightBoxController.Instruments
{
    public class MicroController
    {
        /* Can be whatever Microcontroller, Needs to be connected via UART */
        public string Role;
        
    }

    public enum MicroControllerRole
    {
        DEFAULT,
        TEMPERATURE,
        HUMIDITY,
        CELLMEASUREMENT,
        ERROR,
    }

    public enum MicroControllerStatus
    {
        IDLE,
        STOPPED,
        SAMPLING,
        ERROR,

    }
}
