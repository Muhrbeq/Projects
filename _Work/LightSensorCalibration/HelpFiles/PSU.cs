using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VisaFunctions;

namespace QA_LightBoxController
{
    public class PSU : INotifyPropertyChanged
    {
        public GUI Gui;

        public void SetReference(ref GUI _Gui)
        {
            Gui = _Gui;
        }



        /* ID of PSU */
        public string PSU_BK_ID_String = "";

        public bool PSU_BK_Connected = false;

        public string PrintInfoString = "";

        /* PSU not connected yet */
        //public bool PSU_BK_Connected = false;

        #region PSU

        public PSU()
        {

        }

        public bool PSU_BK_IsFound()
        {
            string Errorresualt = "";
            ObservableCollection<InstrumentInfo> instruments = VisaIO.SearchForDevices(ref Errorresualt);

            foreach (InstrumentInfo In_item in instruments)
            {
                if (In_item.Device_Model == "9205")
                {
                    if (In_item.DidRespond == true)
                    {
                        // Use this ID
                        PSU_BK_ID_String = In_item.ConnectionID;

                        Gui.PSU_Info += "PSU_BK found with ID " + PSU_BK_ID_String + Environment.NewLine;
                        //mW.PrintInfo(PrintInfoString);
                        return true;
                    }
                }
            }
            Gui.PSU_Info += "No PSU found. " + PSU_BK_ID_String + Environment.NewLine;
            return false;
        }

        public bool PSU_BK_Reset()
        {
            string ErrorInfo = "";

            if (VisaIO.Send_Command(PSU_BK_ID_String, "*RST", ref ErrorInfo) == true)
            {
                return true;
            }
            Gui.PSU_Info += "PSU_BK Err " + ErrorInfo + Environment.NewLine;
            return false;
        }


        public bool PSU_BK_Set_Enable(bool enable)
        {
            string ErrorInfo = "";
            string CMD = "OUTP 1";

            if (enable == false)
            {
                CMD = "OUTP 0";
            }

            if (VisaIO.Send_Command(PSU_BK_ID_String, CMD, ref ErrorInfo) == true)
            {
                Gui.PSU_Info += "Sent command to enable PSU" + Environment.NewLine;
                return true;
            }
            //Gui.Info += "PSU_BK Enable Err " + ErrorInfo + Environment.NewLine;
            return false;
        }


        public bool PSU_BK_Set_Volt(double voltage)
        {
            string ErrorInfo = "";
            string CMD = "VOLT " + voltage.ToString("N5", System.Globalization.CultureInfo.InvariantCulture);

            if (VisaIO.Send_Command(PSU_BK_ID_String, CMD, ref ErrorInfo) == true)
            {
                return true;
            }
            //Gui.Info += "PSU_BK Voltage Err " + ErrorInfo + Environment.NewLine;
            return false;
        }

        public bool PSU_BK_Set_Current(double Current)
        {
            string ErrorInfo = "";
            string CMD = "CURR " + Current.ToString("N5", System.Globalization.CultureInfo.InvariantCulture);

            if (VisaIO.Send_Command(PSU_BK_ID_String, CMD, ref ErrorInfo) == true)
            {
                return true;    
            }
            //Gui.Info += "PSU_BK Current Err " + ErrorInfo + Environment.NewLine;
            return false;
        }

        #endregion

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
    }
}
