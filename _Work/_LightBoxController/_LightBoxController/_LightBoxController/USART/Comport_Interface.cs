using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
//using System.Drawing;
using FTD2XX_NET;
using System.IO.Ports;
using System.Threading;
using System.Windows.Threading;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;
using System.ComponentModel;
using PropertyRef;
using System.Windows.Documents;
using System.Windows;
using System.Windows.Media;


namespace USART 
{

    [DisplayName("ComPort")]
    public class Comport_Interface : INotifyPropertyChanged
    {
        // Capture Timer
        DispatcherTimer RxTimer;
        // Capture Timer
        DispatcherTimer TxTimer;


        public struct RichData
        {
            internal string Text;
            internal Color color;
        }


        // Comport
        private SerialPort _ComPort = new SerialPort();
        [Browsable(false)]
        public SerialPort ComPort
        {
            get { return _ComPort; }
            set { _ComPort = value; OnPropertyChanged("ComPort"); }
        }


        // FTDI
        FTDI.FT_STATUS ftStatus = FTDI.FT_STATUS.FT_OK;
        FTDI myFtdiDevice = new FTDI();

        string RxBuffer = "";                                           // Recived Rx Buffer 1 buffer before paring data

        public string[] FoundPorts;


        // Command delimiters
        // private static string StartChar = "$";
        // private static string PackIDChar = "!";
        //  private static string CommandChar = "%";
        // private static string AddressChar = "@";
        // private static string DataChar = "*";
        // private static string DataDelimiterChar = ":";

        public char COM_EndChar = '#';
        public bool COM_UseInternalRXBuffer = true;
        public bool COM_UseRxTimer = false;

        public string EndChar = "#";
        private string TxACK_Respons_char = "~";
        //private static string RxACK_Request_char = "?";


        private bool _Info_HandlerEvent_Enable = false;                 // If Event shuold be triggerd for info about connections status
        private bool _RX_HandlerEvent_Enable = false;                   // if Event should be triggerd (fired) for every incomming data string
        private bool _TX_HandlerEvent_Enable = false;                   // if Event should be triggerd (fired) for every outgoing data string
        private bool _RX_CommandEvent_Enable = false;                   // if Event should be triggerd (fired) first when incomming string ends with _CommandEndBit char
        



        //////////////////////////////////////////////////////////////////////////////////////////////////////////








        /////////////////////////////////////////////////
        // Info Event Holder
        // Declare a delegate that takes a single string parameter
        // and has no return type. 0 = status
        public delegate void InfoHandler(string message);  
        // Define an Event based on the above Delegate
        public event InfoHandler Info_Handle;


        /////////////////////////////////////////////////
        // Rx buffer Event holder
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void RXHandler(string message);    
        // Define an Event based on the above Delegate
        public event RXHandler RX_Handle;

        /////////////////////////////////////////////////
        // Tx buffer Event holder
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void TXHandler(string message);
        // Define an Event based on the above Delegate
        public event TXHandler TX_Handle;

        ////////////////////////////////////////////////
        // Rx Command in Queue
        public delegate void RXCommandHandler(string message);   
        // Define an Event based on the above Delegate
        public event RXCommandHandler RX_Command_Handle;

        ////////////////////////////////////////////////
        // Rx (incoming) ACK Event
        public delegate void RXACKHandler(bool Status);
        // Define an Event based on the above Delegate
        public event RXACKHandler RX_ACK_Command_Handle;

        ////////////////////////////////////////////////
        // Rx Command in Queue
        public delegate void RXJunkHandler(string message);
        // Define an Event based on the above Delegate
        public event RXJunkHandler RX_Junk_Handle;


        /////////////////////////////////////
        // How to register events to functions (functions is should call on event is (RxEventHandler and RxCommandEventHandler)
        //// Register RxEvent Handler function
        //ComportData.RX_Handle += new Comport_Interface.RXHandler(RxEventHandler);
        //// Register Rx Command Event Handler function
        //ComportData.RX_Command_Handle += new Comport_Interface.RXCommandHandler(RxCommandEventHandler);



        #region ComPort Properties Get/Set

        /// <summary>
        /// property to hold our display window
        /// value
        /// </summary>
        //[Browsable(false)]
        //public bool DisplayWindowEnable
        //{
        //    get { return _displayWindowEnable; }
        //    set { _displayWindowEnable = value; }
        //}


        [Browsable(false)]
        public bool Info_HandlerEvent_Enable
        {
            get { return _Info_HandlerEvent_Enable; }
            set { _Info_HandlerEvent_Enable = value; }
        }


        [Browsable(false)]
        public bool RX_HandlerEvent_Enable
        {
            get { return _RX_HandlerEvent_Enable; }
            set { _RX_HandlerEvent_Enable = value; }
        }

        [Browsable(false)]
        public bool RX_CommandEvent_Enable
        {
            get { return _RX_CommandEvent_Enable; }
            set { _RX_CommandEvent_Enable = value; }
        }

        [Browsable(false)]
        public bool TX_HandlerEvent_Enable
        {
            get { return _TX_HandlerEvent_Enable; }
            set { _TX_HandlerEvent_Enable = value; }
        }


        /// <summary>
        /// property to hold the connected status
        /// of our manager class
        /// </summary>
        /// 
        private bool _Connected = false;
        [Category("1 ID")]
        [DisplayName("Connected")]
        [Description("--")]
        [PropertyOrder(1)]
        [ReadOnly(true)]
        public bool PortConnected
        {
            get { return _Connected; }
            set { _Connected = value; OnPropertyChanged("PortConnected"); }
        }


        /// <summary>
        /// Type of connection, 1=FTDI , 2=Com
        /// of our manager class
        /// </summary>
        /// 
        private string _ComportInternalID_Name = "New Port";
        [Category("1 ID")]
        [DisplayName("Internal Name")]
        [Description("--")]
        [PropertyOrder(2)]
        public string ComportInternalID_Name
        {
            get { return _ComportInternalID_Name; }
            set { _ComportInternalID_Name = value; OnPropertyChanged("ComportInternalID_Name"); }
        }

        /// <summary>
        /// Type of connection, 1=FTDI , 2=Com
        /// of our manager class
        /// </summary>
        /// 
        private int _ComportInternalID = 0;
        [Category("1 ID")]
        [DisplayName("Internal ID")]
        [Description("--")]
        [PropertyOrder(3)]
        public int ComportInternalID
        {
            get { return _ComportInternalID; }
            set { _ComportInternalID = value; OnPropertyChanged("ComportInternalID"); }
        }

        /// <summary>
        /// Type of connection, 1=FTDI , 2=Com
        /// of our manager class
        /// </summary>
        /// 
        private int _ComportType = 0;
        [Category("1 ID")]
        [DisplayName("Type")]
        [Description("--")]
        //  [ItemsSource(typeof(TypeItemsSource))]
        [ReadOnly(true)]
        //[PropertyOrder(2)]
        [Browsable(false)]
        public int PortType
        {
            get { return _ComportType; }
            set { _ComportType = value; OnPropertyChanged("PortType"); }
        }

        /// <summary>
        /// Type of connection, 1=FTDI , 2=Com
        /// of our manager class
        /// </summary>
        /// 
        private String _ComportTypeName = "ComPort";
        [Category("1 ID")]
        [DisplayName("Port Type")]
        [Description("--")]
        [ReadOnly(true)]
        [PropertyOrder(4)]
        public String ComportTypeName
        {
            get { return _ComportTypeName; }
            set { _ComportTypeName = value; OnPropertyChanged("ComportTypeName"); }
        }


        /// <summary>
        /// property to hold the PortName
        /// of our manager class
        /// </summary>
        /// 
        private string _portName = string.Empty;
        [Category("2 Settings")]
        [DisplayName("PortName")]
        [Description("--")]
        [ReadOnly(true)]
        [PropertyOrder(1)]
        public string PortName
        {
            get { return _portName; }
            set { _portName = value; OnPropertyChanged("PortName"); }
        }

        /// <summary>
        /// Property to hold the BaudRate
        /// of our manager class
        /// </summary>
        private string _baudRate = "115200";
        [Category("2 Settings")]
        [DisplayName("BaudRate")]
        [Description("--")]
        [ItemsSource(typeof(BoudRateItemsSource))]
        [PropertyOrder(2)]
        public virtual string BaudRate
        {
            get { return _baudRate; }
            set { _baudRate = value; OnPropertyChanged("BaudRate"); }
        }

        /// <summary>
        /// property to hold the DataBits
        /// of our manager class
        /// </summary>
        /// 
        private string _dataBits = "8";
        [Category("2 Settings")]
        [DisplayName("DataBits")]
        [Description("--")]
        [ItemsSource(typeof(DataBitsItemsSource))]
        [PropertyOrder(3)]
        public string DataBits
        {
            get { return _dataBits; }
            set { _dataBits = value; OnPropertyChanged("DataBits"); }
        }

        /// <summary>
        /// property to hold the Parity
        /// of our manager class
        /// </summary>
        /// 
        private string _parity = "None";
        [Category("2 Settings")]
        [DisplayName("Parity")]
        [Description("--")]
        [ItemsSource(typeof(ParityItemsSource))]
        [PropertyOrder(4)]
        public string Parity
        {
            get { return _parity; }
            set { _parity = value; OnPropertyChanged("Parity"); }
        }

        /// <summary>
        /// property to hold the StopBits
        /// of our manager class
        /// </summary>
        /// 
        private string _stopBits = "One";
        [Category("2 Settings")]
        [DisplayName("StopBits")]
        [Description("--")]
        [ItemsSource(typeof(StopBitsItemsSource))]
        [PropertyOrder(5)]
        public string StopBits
        {
            get { return _stopBits; }
            set { _stopBits = value; OnPropertyChanged("StopBits"); }
        }



        private string _portHandshake = "None";
        [Category("2 Settings")]
        [DisplayName("Handshake")]
        [Description("--")]
        [ItemsSource(typeof(HandshakeItemsSource))]
        [PropertyOrder(6)]
        public string PortHandshake
        {
            get { return _portHandshake; }
            set { _portHandshake = value; OnPropertyChanged("PortHandshake"); }
        }


        /// <summary>
        /// Set Rx timer (Read) intervall in ms
        /// </summary>
        /// 
        private int _RX_Intervall_ms = 1;                              // Rx buffer intervall check
        [Category("2 Settings")]
        [DisplayName("RX intervall ms")]
        [Description("--")]
        [PropertyOrder(7)]
        public int RXintervall_ms
        {
            get { return _RX_Intervall_ms; }
            set { _RX_Intervall_ms = value; OnPropertyChanged("RXintervall_ms"); }
        }



        /// <summary>
        /// property to hold the connected status
        /// of our manager class
        /// </summary>
        /// 
        private bool _StartEndCommandEnabled = false;
        [Category("3 Send Extra Char Functions")]
        [DisplayName("Enable")]
        [Description("--")]
        [PropertyOrder(1)]
        public bool StartEndCommandEnabled
        {
            get { return _StartEndCommandEnabled; }
            set { _StartEndCommandEnabled = value; OnPropertyChanged("StartEndCommandEnabled"); }
        }


        /// <summary>
        /// Added EndBit
        /// </summary>
        private string _CommandStartBit = "$";
        [Category("3 Send Extra Char Functions")]
        [DisplayName("Start Char")]
        [Description("--")]
        [PropertyOrder(2)]
        public string CommandStartBit
        {
            get { return _CommandStartBit; }
            set { _CommandStartBit = value; OnPropertyChanged("CommandStartBit"); }
        }

        /// <summary>
        /// Added EndBit
        /// </summary>
        private string _CommandEndBit = "#";            // \n = defualt for newline char
        [Category("3 Send Extra Char Functions")]
        [DisplayName("End Char")]
        [Description("--")]
        [PropertyOrder(3)]
        public string CommandEndBit
        {
            get { return _CommandEndBit; }
            set { _CommandEndBit = value; OnPropertyChanged("CommandEndBit"); }
        }

        /// <summary>
        /// property to hold the connected status
        /// of our manager class
        /// </summary>
        /// 
        private bool _NewLineCommandEnabled = false;
        [Category("3 Send Extra Char Functions")]
        [DisplayName("NewLine")]
        [Description("End with newline")]
        [PropertyOrder(4)]
        public bool NewLineCommandEnabled
        {
            get { return _NewLineCommandEnabled; }
            set { _NewLineCommandEnabled = value; OnPropertyChanged("NewLineCommandEnabled"); }
        }



        /// <summary>
        /// 
        /// 
        /// </summary>
        /// 
        private bool _QueueTX = true;
        [Category("4 Queue and Timing")]
        [DisplayName("Enable")]
        [Description("Use Queue and with TX timing on send")]
        [PropertyOrder(1)]
        public bool QueueTX
        {
            get { return _QueueTX; }
            set { _QueueTX = value; OnPropertyChanged("QueueTX"); }
        }

        private int _QueueTX_timing_ms = 100;
        [Category("4 Queue and Timing")]
        [DisplayName("Tx Timing (ms)")]
        [Description("Queue Tx intervall timing")]
        [PropertyOrder(2)]
        public int QueueTX_timing_ms
        {
            get { return _QueueTX_timing_ms; }
            set { _QueueTX_timing_ms = value; OnPropertyChanged("QueueTX_timing_ms"); }
        }





        //private System.Windows.Controls.RichTextBox _rxRichTextBox;
        //private System.Windows.Controls.RichTextBox _txRichTextBox;
        ///// <summary>
        ///// property to hold our display window
        ///// value
        ///// </summary>
        //public System.Windows.Controls.RichTextBox SetRX_RichTextBox
        //{
        //    get { return _rxRichTextBox; }
        //    set { _rxRichTextBox = value; OnPropertyChanged("SetRX_RichTextBox"); }
        //}


        ///// <summary>
        ///// property to hold our display window
        ///// value
        ///// </summary>
        //public System.Windows.Controls.RichTextBox SetTX_RichTextBox
        //{
        //    get { return _txRichTextBox; }
        //    set { _txRichTextBox = value; OnPropertyChanged("SetTX_RichTextBox"); }
        //}




        #endregion




        /// <summary>
        ///  Class constructor
        /// </summary>
        public Comport_Interface()
        {
        }

        #region SendPort

        /// <summary>
        /// Send Data
        /// </summary>
        /// <param name="Data">Data to send</param>
        /// <returns>bool ,true = OK</returns>
        public bool SendData(string Data, bool AddStartEndChar) 
        { 
            bool SendStatus = true;

            //first check if the port is already open
            //if its open then close it
            if ((ComPort.IsOpen == true) || (myFtdiDevice.IsOpen))
            {
                _Connected = true;
            }
            else
            {
                _Connected = false;
            }

            // Check if start and endbit should be added
            if (AddStartEndChar == true)
            {
                Data = _CommandStartBit + Data + _CommandEndBit;    // Add StartBit and end bit

            }


            if ((_Connected == true) && (_ComportType == 1))
            {
                    try
                    {
                            if (Data != null)
                            {

                                // Perform loop back - make sure loop back connector is fitted to the device
                                // Write string data to the device
                                UInt32 numBytesWritten = 0;

                                // Note that the Write method is overloaded, so can write string or byte array data
                                ftStatus = myFtdiDevice.Write(Data, Data.Length, ref numBytesWritten);

                                // Note that the Write method is overloaded, so can write string or byte array data
                                //              ftStatus = myFtdiDevice.Write(textSend.Text.ToString, textSend.Text.Length, ref numBytesWritten);
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    // If data send faild
                                    SendStatus = false;

                                    //DisplayData("Faild to send data!!", Color.Red, FontStyle.Bold, true);
                                    InfoProcess("Faild to send data!!");
                                }
                                else
                                {
                                    // Trigger Tx event, to display/report what has been sent
                                    TxProcess(Data);
                                    //DisplayData(Data, Color.Blue, FontStyle.Regular, true);
                                }
                            }
                    }
                    catch
                    {
                        // If data send faild
                        SendStatus = false;
                    }
            }
            else if ((_Connected == true) && (_ComportType == 2))
            {
                if (ComPort.IsOpen)
                {
                        try
                        {
                            ComPort.Write(Data);
                            // Trigger Tx event, to display/report what has been sent
                            TxProcess(Data);
                            //DisplayData(Data, Color.Blue, FontStyle.Regular,true);
                        }
                        catch
                        {
                            // If data send faild
                            SendStatus = false;
                            //DisplayData("Faild to send data!!", Color.Red, FontStyle.Bold, true);
                            InfoProcess("Faild to send data!!");
                        }
                }
            }
        
            return SendStatus;
        }

        /// <summary>
        /// Send Data, comport settings _StartEndCommandEnabled sets if StartEnd Char is added
        /// </summary>
        /// <param name="Data"></param>
        /// <returns></returns>
        public bool SendData(string Data)
        {
            return SendData(Data, _StartEndCommandEnabled);
        }


        #endregion


        #region ReadPort

        public void ActivateReadPort()
        {
            //Adds the event and the event handler for the method that will
            //process the timer event to the timer
            // Sets the timer interval 
            RxTimer = new DispatcherTimer();
            RxTimer.Tick += new EventHandler(ReadPort);
            RxTimer.Interval = new TimeSpan(0, 0, 0, 0, _RX_Intervall_ms);
            RxTimer.Start();
            
        }


        public void ReadPort(Object myObject, EventArgs myEventArgs)
        {

            UInt32 numBytesAvailable = 0;
            UInt32 numBytesRead = 0;
            string RxData = "";
            int RXc;

            try
            {
                    if (_ComportType == 1)      // FTDI
                    {
                        ftStatus = myFtdiDevice.GetRxBytesAvailable(ref numBytesAvailable);
                        // Note that the Read method is overloaded, so can read string or byte array data
                        if (numBytesAvailable != 0)
                        {
                            ftStatus = myFtdiDevice.Read(out RxData, numBytesAvailable, ref numBytesRead);

                            if (COM_UseInternalRXBuffer == true)
                            {
                                Rx_Buffer(RxData);
                            }
                            
                            RxProcess(RxData);
                        }

                    }
                    else if (_ComportType == 2) // Comport
                    {
                        RXc = ComPort.BytesToRead;
                        if (RXc != 0)
                        {
                            RxData = ComPort.ReadExisting();

                            if (COM_UseInternalRXBuffer == true)
                            {
                                Rx_Buffer(RxData);
                            }

                            RxProcess(RxData);
                        }

                    }
                    else
                    {
                        //DisplayData("PortType Error:" + _ComportType, Colors.Red, true);
                        InfoProcess("PortType Error:" + _ComportType);
                       // InfoProcess(InformationParagraphData("PortType Error:" + _ComportType));
                        RxTimer.Stop();
                    }

            }
            catch (Exception e)
            {
                //DisplayData("Error: " + e.ToString() , Colors.Red, true);
                InfoProcess("Error: " + e.ToString());
                //InfoProcess(InformationParagraphData("Error: " + e.ToString()));

                RxTimer.Stop();
            }

        }

        // Add an extra Event that triggs on ACK respons
        public void Rx_Buffer(string Data)
        {
            Data = RxBuffer + Data;

            int EndIndex = -1;
            int indexStartChar = -1;
            string Cdata = "";
            string DescardedData = "";
            int NextStartChar = 0;



            indexStartChar = Data.IndexOf('$', 0);
            EndIndex = Data.IndexOf(COM_EndChar, 0);


            while ((indexStartChar != -1) && (EndIndex != -1)) 
            {


                indexStartChar = Data.IndexOf('$', 0);
                EndIndex = Data.IndexOf(COM_EndChar, 0);



                if ((indexStartChar != -1) && (EndIndex != -1))
                {
                    /// Check if there is any more start char in the string
                    NextStartChar = Data.IndexOf('$', indexStartChar + 1);

                    while((NextStartChar != -1) && (NextStartChar < EndIndex))
                    {
                        /// One more is found, save this
                        indexStartChar = NextStartChar;
                        /// Check if there is one more
                        NextStartChar = Data.IndexOf('$', NextStartChar + 1);
                    }


                    // Check if there is any data in the substring
                    if (EndIndex > (indexStartChar + 2))
                    {
                        Cdata = Data.Substring(indexStartChar + 1, EndIndex - indexStartChar - 1);
                        RxCommandProcess(Cdata);
                    }

                    /// Check if there is any data that will be discarded
                    if (indexStartChar > 0)
                    {
                        DescardedData = Data.Substring(0, indexStartChar);
                        if (DescardedData.Length > 0)
                        {
                            RxJunkProcess(DescardedData);
                        }
                    }

                    // Remove the extracted string from main buffer
                    if (Data.Length > 0)
                    {
                        Data = Data.Substring(EndIndex + 1, Data.Length - EndIndex - 1);
                    }
                }

            }

            // copy data to buffer
            RxBuffer = Data;                            

        }




        #endregion


        #region Events

        // Comport Rx event handler
        void Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string ReceviedData = "";

            ComPort.DataReceived -= Port_DataReceived;
            do
            {
                ReceviedData += ComPort.ReadExisting();
            } while (ComPort.BytesToRead > 0);

            ComPort.DataReceived += Port_DataReceived;


            if (COM_UseInternalRXBuffer == true)
            {
                Rx_Buffer(ReceviedData);
            }

            // Trigg Rx Event
            RxProcess(ReceviedData);

        }

        
        // Comport Rx Error event handler
        void Port_ErrorRecived(object sender, SerialErrorReceivedEventArgs e)
        {
            InfoProcess(e.ToString());
        }


        // RX Handler Event process
        protected void InfoProcess(string message)
        {
            if ((Info_Handle != null) && (_Info_HandlerEvent_Enable == true))
            {
                Info_Handle(message);
            }
        }

        // RX Handler Event process
        protected void RxProcess(string Data)
        {
            if ((RX_Handle != null) && (_RX_HandlerEvent_Enable == true))
            {
                RX_Handle(Data);
            }
        }

        // RX Command Handler Event process
        protected void RxCommandProcess(string Data)
        {
            if ((RX_Command_Handle != null) && (_RX_CommandEvent_Enable == true))
            {
                RX_Command_Handle(Data);
            }
        }

        // RX Command Handler Event process
        protected void RxJunkProcess(string Data)
        {
            if (RX_Junk_Handle != null) 
            {
                RX_Junk_Handle(Data);
            }
        }

        // TX Handler Event process
        protected void TxProcess(string Data)
        {
            if ((TX_Handle != null) && (_TX_HandlerEvent_Enable == true))
            {
                TX_Handle(Data);
            }
        }

        // RX Command Handler Event process
        protected void RxACKProcess(bool status)
        {
            if (RX_ACK_Command_Handle != null) 
            {
                RX_ACK_Command_Handle(status);
            }
        }

        #endregion


        #region ListPorts
        /// <summary>
        /// Get availible comports (FTDI/COM)
        /// 0=Serial number
        /// 6=Set Com_Type to (FTDI=1 / Com=2)
        /// </summary>
        /// <returns>List, 
        /// 0=Serial number
        /// 1=location identifier
        /// 2=Type
        /// 3=Description
        /// 4=ID
        /// 5=Flag
        /// 6=Set Com_Type to (FTDI=1 / Com =2)
        /// </returns>
        public string[] ListAvaliblePorts()
        {
            int PortsCount = 0;
            string[] AvaliblePorts = new string[100];        // 

            //try
            //{
            UInt32 ftdiDeviceCount = 0;

            // Determine the number of FTDI devices connected to the machine
            ftStatus = myFtdiDevice.GetNumberOfDevices(ref ftdiDeviceCount);


            // Allocate storage for device info list
            FTDI.FT_DEVICE_INFO_NODE[] ftdiDeviceList = new FTDI.FT_DEVICE_INFO_NODE[ftdiDeviceCount];

            // Populate our device list
            ftStatus = myFtdiDevice.GetDeviceList(ftdiDeviceList);

            ///

                if (ftStatus == FTDI.FT_STATUS.FT_OK)
                {
                    for (UInt32 i = 0; i < ftdiDeviceCount; i++)
                    {
                        PortsCount++;
                        AvaliblePorts[i] = ftdiDeviceList[i].SerialNumber.ToString();         // Serial number
                    }
                }
            //}
            //catch
            //{
            //    AvaliblePorts[0, 0] = "Error";
            //}


                string[] AvailableCOMPorts;
                string Port = "";
                string LastCharPort = "";
                bool IsNum = false;
                int Num;

                // Determine the COM ports currently available
                AvailableCOMPorts = System.IO.Ports.SerialPort.GetPortNames();

                for (int j = 0; j < AvailableCOMPorts.Length; j++)
                {
                    // Bluetooth name fix
                    Port = AvailableCOMPorts[j];
                    LastCharPort = Port.Substring(Port.Length - 1).ToLower();   // Get last char
                    IsNum = int.TryParse(LastCharPort, out Num);                //check if LastCharPort is a value

                    if (IsNum)
                    {
                        AvaliblePorts[PortsCount] = AvailableCOMPorts[j];     // Serial number
                    }
                    else
                    {
                        AvaliblePorts[PortsCount] = Port.Substring(0, Port.Length - 1);     // Serial number
                    }


                    PortsCount++;
                }

                // Copy string list to a new with right lenght
                FoundPorts = new string[PortsCount];
                for (int s = 0; s < PortsCount; s++)
                {
                    FoundPorts[s] = AvaliblePorts[s];              // Serial number
   
                }


                return FoundPorts;
        }

        #endregion


        #region OpenPort

        #region FTDI_defines

        private ushort Get_FTDI_Flow(string Name)
        {
            ushort ReturnData = 0x0000;      //FT_FLOW_NONE 

            if(Name == "None")
            {
                ReturnData = 0x0000;            //FT_FLOW_NONE
            }
            else if (Name == "RequestToSend")
            {
                ReturnData = 0x0100;            //FT_FLOW_RTS_CTS
            }
            else if (Name == "RequestToSendXOnXOff")
            {
                ReturnData = 0x0200;            //FT_FLOW_DTR_DSR
            }
            else if (Name == "XOnXOff")
            {
                ReturnData = 0x0400;            //FT_FLOW_XON_XOFF
            }

            return ReturnData;
        }

        private byte Get_FTDI_Paritet(string Name)
        {
            byte ReturnData = 0;      //FT_PARITY_NONE

            if(Name == "None")
            {
                ReturnData = 0;         //FT_PARITY_NONE
            }
            else if (Name == "Odd")
            {
                ReturnData = 1;         //FT_PARITY_ODD
            }
            else if (Name == "Even")
            {
                ReturnData = 2;         //FT_PARITY_EVEN
            }
            else if (Name == "Mark")
            {
                ReturnData = 3;         //FT_PARITY_MARK
            }
            else if (Name == "Space")
            {
                ReturnData = 4;         //FT_PARITY_SPACE
            }
            return ReturnData;
        }

        private byte Get_FTDI_StopBits(string Name)
        {
            byte ReturnData = 0;      //FT_STOP_BITS_1

            if (Name == "One")
            {
                ReturnData = 0;         //FT_STOP_BITS_1
            }
            else if (Name == "Two")
            {
                ReturnData = 2;         //FT_STOP_BITS_2
            }
            return ReturnData;
        }

        private byte Get_FTDI_DataBits(string Name)
        {
            byte ReturnData = 8;      //FT_BITS_8

            if (Name == "8")
            {
                ReturnData = 8;         //FT_BITS_8
            }
            else if (Name == "7")
            {
                ReturnData = 7;         //FT_BITS_7
            }
            return ReturnData;
        }
        #endregion

        public bool OpenPort()
        {

            // Check what kind of port the user whants to connect to
            if (_portName.Substring(0, 3).ToLower() == "com")
            {
                _ComportType = 2;   // Comport
                _ComportTypeName = "Comport";
            }
            else
            {
                _ComportType = 1;   // FTDI
                _ComportTypeName = "FTDI";
            }

            ClosePorts();


            if (_ComportType == 1)
            {
                #region FTDI
                try
                {

                    // Open  device in our list by serial number
                    ftStatus = myFtdiDevice.OpenBySerialNumber(_portName);
                    DisplayData(_portName + " ->" + ftStatus, Colors.Green, true);
                    

                    // Set flow control  (FlowControl,Xon character,Xoff character)
                    //ftStatus = myFtdiDevice.SetFlowControl(FTDI.FT_FLOW_CONTROL.FT_FLOW_NONE, 0x11, 0x13);
                    ftStatus = myFtdiDevice.SetFlowControl(Get_FTDI_Flow(_portHandshake), 0x11, 0x13);
                    DisplayData(_portHandshake + " ->", Colors.Green,  true);

                    // Set data control  (DataBits,StopBits,Parity)
                    ftStatus = myFtdiDevice.SetDataCharacteristics(Get_FTDI_DataBits(_dataBits), Get_FTDI_StopBits(_stopBits), Get_FTDI_Paritet(_parity));
                    DisplayData(_portName + " ->" + ftStatus, Colors.Green, true);

                    // Set Baud rate 
                    ftStatus = myFtdiDevice.SetBaudRate(Convert.ToUInt32(_baudRate));
                    DisplayData(_baudRate + " ->" + ftStatus, Colors.Green, true);

                    // Set read timeout to 5 seconds, write timeout to infinite
                    ftStatus = myFtdiDevice.SetTimeouts(5000, 0);

                    ActivateReadPort();         // activate RX timer
                    _Connected = true;
                    return true;
                }
                catch (Exception ex)
                {
                    //DisplayData(ex.Message, Colors.Red, true);
                    InfoProcess(ex.Message);
                    //InfoProcess(InformationParagraphData(ex.Message));
                    return false;
                }
                #endregion
                
            }
            else
            {
                #region Com
                try
                {
                    //set the properties of our SerialPort Object
                    ComPort.BaudRate = int.Parse(_baudRate);    //BaudRate
                    ComPort.DataBits = int.Parse(_dataBits);    //DataBits
                    ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), _stopBits);    //StopBits
                    ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), _parity);    //Parity
                    ComPort.PortName = _portName;   //PortName
                    ComPort.RtsEnable = true;

                    //now open the port
                    ComPort.Open();
                    //display message
                    DisplayData("Port " + _portName + " opened at " + DateTime.Now, Colors.Green, true);
                    //InfoProcess(InformationParagraphData("Port " + _portName + " opened at " + DateTime.Now));


                    if (COM_UseRxTimer == true)
                    {
                        ActivateReadPort();         // activate RX timer
                    }
                    else
                    {
                        ComPort.DataReceived += Port_DataReceived;
                    }
                    ComPort.ErrorReceived += Port_ErrorRecived;
                    _Connected = true;
                    return true;
                }
                catch (Exception ex)
                {
                    //DisplayData(ex.Message, Colors.Red, true);
                    InfoProcess(ex.Message);
                    //InfoProcess(InformationParagraphData(ex.Message));
                    return false;
                }
                #endregion
                
            }

        }
        #endregion


        #region close Open Ports

        /// <summary>
        ///  Close ports that is open
        /// </summary>
        public void ClosePorts()
        {
            if (RxTimer != null)
            {
                // First disable RXtimer
                RxTimer.Stop();
            }

            //first check if the port is already open
            //if its open then close it
            // Comport
            if (ComPort.IsOpen == true)
            {
                ComPort.Close();
                _Connected = false;
                DisplayData("Comport Closed", Colors.Green, true);
                //InfoProcess(InformationParagraphData("Comport Closed"));
            }
            // FTDI
            if (myFtdiDevice.IsOpen)
            {
                myFtdiDevice.Close();
                _Connected = false;
                DisplayData("FTDI port Closed", Colors.Green, true);
                //InfoProcess(InformationParagraphData("FTDI port Closed"));
            }
        }

        #endregion


        #region Get Settings from Enum
        /// <summary>
        /// point to combox to populate with data
        /// </summary>
        /// <param name="obj"></param>
        public void GetParityValues(object obj)
        {   
            //((ComboBox)obj).Items.Clear();
            //foreach (string str in Enum.GetNames(typeof(Parity)))
            //{
            //    ((ComboBox)obj).Items.Add(str);
            //}
        }


        public void GetStopBitValues(object obj)
        {
            //((ComboBox)obj).Items.Clear();
            //foreach (string str in Enum.GetNames(typeof(StopBits)))
            //{
            //    if (str != "None")
            //    {
            //        ((ComboBox)obj).Items.Add(str);
            //    }
            //}
        }

        public void GetDataBitValues(object obj)
        {
             //((ComboBox)obj).Items.Clear();
             //((ComboBox)obj).Items.Add("5");
             //((ComboBox)obj).Items.Add("6");
             //((ComboBox)obj).Items.Add("7");
             //((ComboBox)obj).Items.Add("8");
        }


        public void GetHandshakeValues(object obj)
        {
            //((ComboBox)obj).Items.Clear();
            //foreach (string str in Enum.GetNames(typeof(Handshake)))
            //{
            //    ((ComboBox)obj).Items.Add(str);
            //}
        }

        public void GetAvailiblePorts(object obj)
        {
            //((ComboBox)obj).Items.Clear();
            //for (int j = 0; j < FoundPorts.Length; j++)
            //{
            //    ((ComboBox)obj).Items.Add(FoundPorts[j]);
            //}
        }


        public void GetBaudRateValues(object obj)
        {
            //((ComboBox)obj).Items.Clear();
            //for (int j = 0; j < FoundPorts.Length; j++)
            //{
            //    ((ComboBox)obj).Items.Add(FoundPorts[j]);
            //}
        }
        #endregion


        #region DisplayData

        /// <summary>
        ///  Only Temp Fix
        /// </summary>
        /// <param name="data"></param>
        /// <param name="_color"></param>
        /// <param name="Style"></param>
        /// <param name="NewLine"></param>
        public void DisplayData(string data, Color _color, bool NewLine)
        {
            InfoProcess(data);
        }


        public RichData InformationParagraphData(string data, Color _color, bool NewLine)
        {
            RichData textData = new RichData();
            textData.Text = data;
            textData.color = _color;
            return textData;
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
