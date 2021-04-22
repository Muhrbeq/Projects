using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LightSensorCalibration.Instruments
{
    public class PowerSupplyUnit
    {
        private string _ID;
        public string ID
        {
            get { return _ID; }
            set { _ID = value; }
        }

        private bool _IsConnected;
        public bool IsConnected
        {
            get
            {
                return _IsConnected;
            }
            set { _IsConnected = value; }
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

        public bool Connect()
        {
            return true;
        }

        public void SetOutputCurrent(double outputCurrent)
        {
            OutputCurrent = outputCurrent;
        }

        public void SetOutputVoltage(double outputVoltage)
        {
            OutputVoltage = outputVoltage;
        }
    }
}
