using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VisaFunctions;

namespace LightSensorCalibration.Instruments
{
    public class PowerSupplyUnit
    {
        private string _ID = "";
        public string ID
        {
            get { return _ID; }
        }

        private bool _IsConnected;
        public bool IsConnected
        {
            get
            {
                return _IsConnected;
            }
        }

        private double _OutputCurrent;
        public double OutputCurrent
        {
            get { return _OutputCurrent; }
            set { _OutputCurrent = value; }
        }

        private double _OutputVoltage;
        public double OutputVoltage
        {
            get { return _OutputVoltage; }
            set
            {
                _OutputVoltage = value;
            }
        }

        private double _MaximumVoltage;
        public double MaximumVoltage
        {
            get { return _MaximumVoltage; }
            set { _MaximumVoltage = value; }
        }

        private double _MinimumVoltage;
        public double MinimumVoltage
        {
            get { return _MinimumVoltage; }
            set { _MinimumVoltage = value; }
        }

        private double _MaximumCurrent;
        public double MaximumCurrent
        {
            get { return _MaximumCurrent; }
            set { _MaximumCurrent = value; }
        }

        private double _MinimumCurrent;
        public double MinimumCurrent
        {
            get { return _MinimumCurrent; }
            set { _MinimumCurrent = value; }
        }

        private double _MaxVoltage = 0.0d;

        public PowerSupplyUnit(double maxVolt)
        {
            _MaxVoltage = maxVolt;
        }

        public void SetMinimumVoltage(double minVoltage)
        {
            MinimumVoltage = minVoltage;
        }

        public void SetMaximumVoltage(double maxVoltage)
        {
            MaximumVoltage = maxVoltage;
        }

        public void SetMinimumCurrent(double minCurrent)
        {
            MinimumCurrent = minCurrent;
        }

        public void SetMaximumCurrent(double maxCurrent)
        {
            MaximumCurrent = maxCurrent;
        }

        private double LimitCurrent(double Current)
        {
            if (Current < MinimumCurrent)
                return MinimumCurrent;
            else if (Current > MaximumCurrent)
                return MaximumCurrent;
            return Current;
        }

        private double LimitVoltage(double Voltage)
        {
            if (Voltage < MinimumVoltage)
                return MinimumVoltage;
            else if (Voltage > MaximumVoltage)
                return MaximumVoltage;
            return Voltage;
        }

        public void SetupStandardPSU()
        {
            SetupStandardPSU(_MaxVoltage, 0.0, 7.0, 0.0);
        }

        public void SetupStandardPSU(double maxVoltage, double minVoltage, double maxCurrent, double minCurrent)
        {
            SetMaximumCurrent(maxCurrent);
            SetMinimumCurrent(minCurrent);
            SetMaximumVoltage(maxVoltage);
            SetMinimumVoltage(minVoltage);
        }

        public PSU_ReturnCodes SetOutputCurrent(double outputCurrent)
        {
            string ErrorInfo = "";
            string CMD = "CURR " + LimitCurrent(outputCurrent).ToString("N5", System.Globalization.CultureInfo.InvariantCulture);

            if (VisaIO.Send_Command(_ID, CMD, ref ErrorInfo) == true)
            {
                return PSU_ReturnCodes.PSU_PASSED;
            }
            return PSU_ReturnCodes.PSU_SETCURRENTFAIL;
        }

        public PSU_ReturnCodes SetOutputVoltage(double outputVoltage)
        {
            string ErrorInfo = "";
            string CMD = "VOLT " + LimitVoltage(outputVoltage).ToString("N5", System.Globalization.CultureInfo.InvariantCulture);

            if (VisaIO.Send_Command(_ID, CMD, ref ErrorInfo) == true)
            {
                return PSU_ReturnCodes.PSU_PASSED;
            }
            return PSU_ReturnCodes.PSU_SETVOLTAGEFAIL;
        }

        /// <summary>
        /// Enable or disable PSU
        /// </summary>
        /// <param name="enable"></param>
        /// <returns></returns>
        public PSU_ReturnCodes SetEnable(bool enable)
        {
            string ErrorInfo = "";
            string CMD = enable ? "OUTP 1" : "OUTP 0";

            // Send command
            if (VisaIO.Send_Command(_ID, CMD, ref ErrorInfo) == true)
            {
                //Everything passed
                return PSU_ReturnCodes.PSU_PASSED;
            }
            return PSU_ReturnCodes.PSU_SETENABLEFAIL;
        }

        public PSU_ReturnCodes Reset()
        {
            string ErrorInfo = "";

            if (VisaIO.Send_Command(_ID, "*RST", ref ErrorInfo) == true)
            {
                return PSU_ReturnCodes.PSU_PASSED;
            }
            return PSU_ReturnCodes.PSU_RESETFAIL;
        }

        private ObservableCollection<InstrumentInfo> SearchForVISADevices()
        {
            string ErrorResult = "";
            return VisaIO.SearchForDevices(ref ErrorResult);
        }

        public PSU_ReturnCodes Connect()
        {
            // Search for Visa devices
            ObservableCollection<InstrumentInfo> instruments = SearchForVISADevices();

            //Loop though devices
            foreach (InstrumentInfo instrument in instruments)
            {
                // Check if device model is the correct
                if(instrument.Device_Model == "9205")
                {
                    //Check if the device responed
                    if(instrument.DidRespond == true)
                    {
                        //Assign ID
                        _ID = instrument.ConnectionID;
                        return PSU_ReturnCodes.PSU_PASSED;
                    }
                }
            }

            return PSU_ReturnCodes.PSU_COULDNTFINDDEVICE;
        }


    }

    public enum PSU_ReturnCodes
    {
        PSU_PASSED,
        PSU_COULDNTFINDDEVICE,
        PSU_SETVOLTAGEFAIL,
        PSU_SETCURRENTFAIL,
        PSU_SETENABLEFAIL,
        PSU_RESETFAIL,
    }
}
