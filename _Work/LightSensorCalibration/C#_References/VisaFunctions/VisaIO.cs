using Ivi.Visa.Interop;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VisaFunctions
{
    public static class VisaIO
    {
        /// <summary>
        /// Check if visa com drivers are present 
        /// </summary>
        /// <param name="ErrorString"></param>
        /// <returns></returns>
        public static bool CheckIfDriversExist(ref string ErrorString)
        {
            bool DoExist = false;

            try
            {
                ResourceManagerClass rm = new ResourceManagerClass();
                FormattedIO488Class ioobj = new FormattedIO488Class();
                DoExist = true;
                ErrorString = "";

                // Try to close the connection
                try {
                    if (ioobj.IO != null)
                    {
                        ioobj.IO.Close();
                    }
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(ioobj);
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(rm);
                }
                catch { }

            }
            catch (Exception e)
            {
                ErrorString = "VISA-COM 5.9 Type Library Do not exist !! -> Ivi.Visa.Interop.dll" + Environment.NewLine + e.ToString();
                throw;
            }


            return DoExist;
        }


        /// <summary>
        /// Search for all instruments that are connected by USB or ethernet
        /// </summary>
        /// <param name="ErrorRespons"></param>
        /// <returns></returns>
        public static ObservableCollection<InstrumentInfo> SearchForDevices(ref string ErrorRespons)
        {
            // holds the found SN 
            ObservableCollection<string> SN_List = new ObservableCollection<string>();
            // holds all devices found
            ObservableCollection<InstrumentInfo> Device_List = new ObservableCollection<InstrumentInfo>();

            // check if the drivers exist before trying to connect
            if (CheckIfDriversExist(ref ErrorRespons) == false)
            {
                // No drivers, report and exit
                return Device_List;
            }


            // start by checking if we can find any devices
            if (ListConnected_SN_IsOK(ref SN_List, ref ErrorRespons) == true)
            {
                // Yes the search went OK
                // Check if we have any devices in the list
                if(SN_List.Count > 0)
                {
                    // Yes there is devices in the list
                    foreach (string SN in SN_List)
                    {
                        // then try to connect to each of the found devices
                        // so we can retrieve more information
                        InstrumentInfo DeviceInfo = new InstrumentInfo();
                        // save the serial number
                        DeviceInfo.ConnectionID = SN;

                        // Check if SN has any valid data
                        if (SN.Length > 0)
                        {
                            // try to connect to the device
                            if (InstrumentIO(SN, ref DeviceInfo) == true)
                            {
                                // device responded and we have info
                                DeviceInfo.DidRespond = true;
                            }
                            else
                            {
                                // device did not respond
                                DeviceInfo.DidRespond = false;
                            }

                            // add the device in the list
                            Device_List.Add(DeviceInfo);
                        }
                    }
                }
            }

            return Device_List;
        }

        private static bool ListConnected_SN_IsOK(ref ObservableCollection<string> SN_List, ref string ErrorRespons)
        {
            string[] devices = new string[0];
            bool IsOK = false;
            ResourceManagerClass rm = new ResourceManagerClass();

            try
            {
                // Find all devices serial number
                devices = rm.FindRsrc("?*");
                // clear the ref array
                SN_List.Clear();
                // Add all the found devices
                foreach (string SN_item in devices)
                {
                    SN_List.Add(SN_item);
                }
                // indicate that we are fine
                IsOK = true;
            }
            catch (Exception e)
            {
                ErrorRespons += "FindRsrc Error = " + e.ToString() + Environment.NewLine;
            }
            finally
            {
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(rm);
                }
                catch { }
            }

            return IsOK;
        }


        private static bool InstrumentIO(string ID, ref InstrumentInfo returnData)
        {

            ResourceManagerClass rm = new ResourceManagerClass();
            FormattedIO488Class ioobj = new FormattedIO488Class();
            bool resualt = false;


            try
            {
                object[] idnItems;
                // ioobj.IO = (Ivi.Visa.Interop.IMessage)rm.Open("USB0::0x2A8D::0x0101::MY54502617::0::INSTR", Ivi.Visa.Interop.AccessMode.NO_LOCK, 0, "");

                ioobj.IO = (Ivi.Visa.Interop.IMessage)rm.Open(ID);

                // Save info
                returnData.Device_Manufacture = ioobj.IO.SoftwareManufacturerName;
                returnData.Connection_Port_ID = ioobj.IO.HardwareInterfaceNumber;
                returnData.Connection_Port_Type = ioobj.IO.HardwareInterfaceName;

                // Request info about the device
                ioobj.WriteString("*IDN?", true);

                // read the response 
                idnItems = (object[])ioobj.ReadList(Ivi.Visa.Interop.IEEEASCIIType.ASCIIType_Any, ",");
                // convert to array
                returnData.InfoArray = Array.ConvertAll<object, string>(idnItems, ConvertObjectToString);

                foreach (object idnItem in idnItems)
                {
                    returnData.Info += idnItem.ToString() + " ; ";
                }

                // get model ID
                returnData.Device_Model = returnData.InfoArray[1];

                // check if the short Device information item is the connect string
                foreach (string DI_item in returnData.InfoArray)
                {
                    // start by checking if the string contains any data
                    if(DI_item.Length > 0)
                    {
                        // check if this is the short ID string
                        if(ID.Contains(DI_item) == true)
                        {
                            // save data and break
                            returnData.ConnectionID_Short = DI_item;
                            break;
                        }
                    }
                }

                


                // indicate that we have data
                resualt = true;
            }
            catch (Exception e)
            {
                returnData.ErrorInfo = e.ToString();
            }
            finally
            {
                // Try to close the connection
                try
                {
                    if (ioobj.IO != null)
                    {
                        ioobj.IO.Close();
                    }
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(ioobj);
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(rm);
                }
                catch { }
            }

            return resualt;
        }

        public static bool Send_Command(string ID,string Command, ref string ErrorInfo)
        {

            ResourceManagerClass rm = new ResourceManagerClass();
            FormattedIO488Class ioobj = new FormattedIO488Class();
            bool resualt = false;


            try
            {
                ioobj.IO = (Ivi.Visa.Interop.IMessage)rm.Open(ID);

                // Request info about the device
                ioobj.WriteString(Command, true);

                // indicate that we have data
                resualt = true;
            }
            catch (Exception e)
            {
                ErrorInfo = e.ToString();
            }
            finally
            {
                // Try to close the connection
                try {
                    if (ioobj.IO != null)
                    {
                        ioobj.IO.Close();
                    }
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(ioobj);
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(rm);
                }
                catch { }
            }

            return resualt;
        }

        public static string Read_Command(string ID, string Command, ref string ErrorInfo)
        {

            ResourceManagerClass rm = new ResourceManagerClass();
            FormattedIO488Class ioobj = new FormattedIO488Class();
            string resualt = "";


            try
            {
                ioobj.IO = (Ivi.Visa.Interop.IMessage)rm.Open(ID);

                // Request info about the device
                ioobj.WriteString(Command, true);
                

                // indicate that we have data
                resualt = ioobj.ReadString();
            }
            catch (Exception e)
            {
                ErrorInfo = e.ToString();
            }
            finally
            {
                // Try to close the connection
                try {
                    if (ioobj.IO != null)
                    {
                        ioobj.IO.Close();
                    }
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(ioobj);
                }
                catch { }
                try
                {
                    System.Runtime.InteropServices.Marshal.ReleaseComObject(rm);
                }
                catch { }
            }

            return resualt;
        }




        private static string ConvertObjectToString(object obj)
        {
            return obj?.ToString() ?? string.Empty;
        }




        // old function
        /*
        private static ObservableCollection<InstrumentInfo> SearchForDevices()
        {
            ObservableCollection<InstrumentInfo> instruments = new ObservableCollection<InstrumentInfo>();
            InstrumentInfo info = new InstrumentInfo();
            string TestString = "";

            string[] IdStrings = new string[10];

            IdStrings[0] = "USBInstrument";

            for (int s = 0; s < 1; s++)
            {
                for (int i = 0; i < 3; i++)
                {


                    TestString = IdStrings[s] + i.ToString();
                    if (InstrumentIO(TestString, ref info) == true)
                    {

                    }
                }
            }

            return instruments;
        }
        */
    }

    /// <summary>
    /// Holds the information about the instrument
    /// </summary>
    public class InstrumentInfo
    {
        /// <summary>
        /// Like Keysight 
        /// </summary>
        public string Device_Manufacture = "";

        /// <summary>
        /// Model number of the device (34465a)
        /// </summary>
        public string Device_Model = "";

        /// <summary>
        /// Long Connection string to the instrument
        /// </summary>
        public string ConnectionID = "";

        /// <summary>
        /// Short Connection string to the instrument
        /// </summary>
        public string ConnectionID_Short = "";

        /// <summary>
        /// Port ID, like if USB is used then 0 = USB0
        /// </summary>
        public int Connection_Port_ID = 0;

        /// <summary>
        /// Ethernet or USB
        /// </summary>
        public string Connection_Port_Type = "";

        /// <summary>
        /// If the instrument was found and did respond
        /// </summary>
        public bool DidRespond = false;

        /// <summary>
        ///  Extra info
        /// </summary>
        public string Info = "";

        /// <summary>
        /// Extra info in array type
        /// </summary>
        public string[] InfoArray;

        /// <summary>
        /// If there was an error
        /// </summary>
        public string ErrorInfo = "";


    }

}
