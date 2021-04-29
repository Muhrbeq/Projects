using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Converter_Class;
using System.Threading;
using System.Collections.Concurrent;

namespace USART
{
    public class ComCommands
    {

        #region Delegate Events
        /////////////////////////////////////////////////
        // Tx buffer Send Event holder
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate bool TXHandler(string message, bool AddStartEndChar);
        // Define an Event based on the above Delegate
        public static event TXHandler TX_Handle;


        /////////////////////////////////////////////////
        // Rx Commad Queue holds data
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void RxCommadQueueHandler();
        // Define an Event based on the above Delegate
        public static event RxCommadQueueHandler Rx_CommadQueue_Handler;

        /////////////////////////////////////////////////
        // Error event
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void ErrorHandler(string message);
        // Define an Event based on the above Delegate
        public static event ErrorHandler Error_Handle;

        /////////////////////////////////////////////////
        // ACK Error event
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void ACKErrorHandler(TxData Package);
        // Define an Event based on the above Delegate
        public static event ACKErrorHandler ACK_Error_Handle;

        #endregion


        // Timer calling Tx_ACK_Check_Timer_Event
       // private static System.Threading.Timer Tx_Ack_Timer;

        // Char Encoding 
        private static readonly Encoding encoding = new ASCIIEncoding();

        // Tx Send Data Queue (RAW Data)
        private static ConcurrentQueue<TxData> Tx_DataQueue = new ConcurrentQueue<TxData>();

        // Rx Incomming Data Queue (RAW incomming Data)
        private static ConcurrentQueue<RxData> Rx_IncommingDataQueue = new ConcurrentQueue<RxData>();

        // OK RX Command Data Queue ( Data that has been CRC checked and decrypted )
        public static ConcurrentQueue<RxCommandData> Rx_CommandQueue = new ConcurrentQueue<RxCommandData>();

        // Tx 
        public struct TxData
        {
            public string Data;                         // Data in Package
            public int TimeDelay_After;                 // Time to wait after this package has been sent, utill next package should be send, (Zero wait time -> TimeDelay_After = 0)
            public bool AddStartAndEndChar;             // If Start and End Char should be added, this is always added in lowest com layer
            public bool SendACK;                        // If package has a ACK request char in data
            public int PackageID;                       // if no id is set -> PackageID = -1
        }

        // Rx 
        public struct RxData
        {
            public string Data;
        }

        // Error values is for Exceptions, If CRC is used the data is the data that has been sent.
        // 
        public struct RxCommandData
        {
            public bool PackID_Exist;
            public uint PackID;
            public int PackID_Error;                // 0 = OK
            public bool Command_Exist;
            public string Command;
            public int Command_Error;               // 0 = OK
            public bool Address_Exist;
            public uint Address;
            public int Address_Error;               // 0 = OK
            public bool Data_Exist;
            public string Data;
            public int Data_Error;                  // 0 = OK
            public bool DataArray_Exist;
            public string[] DataArray;
            public int DataArray_Error;             // 0 = OK
        }

        // Rx Data
        public static long BytesRecived = 0;
        public static long BytesRecived_CMD = 0;
        public static int PackagesRecived_Total = 0;
        public static int PackagesRecived_OK = 0;

        // Tx 
        private static TxData Tx_LastSentPackageData;             // Holds last sent package data, if we need to resend 
        //private static int Tx_PackID_Counter = 0;
        //private static int Tx_LastSent_PackID = 0;

        // Tx Send Status
        //private static bool Tx_Faild = false;                       //--- Not sure if this should be used

        // Tx package Ack
        //private static bool ACK_For_Tx_Package = false;                 
        private static int Tx_Resend_counter = 0;               // Hold current resend count 
        private static int Tx_Resend_MaxTimes = 4;              // Max resend times before "Error_ACK_Faild" is triggerd
        private static int ACK_TimeoutValue = 1000;             // One second ACK Timeout is defualt
        private static bool ACK_Arrived = false;                // Holds status if ACK has been recived
        // Global status
        private static bool Error_ACK_Faild = false;            // If this is true, the send from queue is stoped/halted

        // Command delimiters
        private static string StartChar = "$";
        private static string PackIDChar = "!";
        private static string CommandChar = "%";
        private static string AddressChar = "@";
        private static string DataChar = "*";
        private static string DataDelimiterChar = ":";
        private static string EndChar = "#";
        public static string EndChar_Defualt = "#";
        public static string EndChar_HUB = "]";
        private static string TxACK_Respons_char = "~";
        private static string RxACK_Request_char = "?";
        // Data Size
        private static int DataSize_Max = 4096;                 // Acceptible incomming data lenght , if bigger -> trigger error

        // CRC Check
        private static bool CRC_Check_Enable = true;            // If CRC check should be used on incomming data

        // Tx Queue status
        private static bool Tx_Queue_Active = false;            // Tells if Queue send Thread is active

        #region Get / Set

        // Set this when ACK is recived (incomming data)
        public static void ACK_Incomming_status_Set(bool value)
        {
            ACK_Arrived = value;
        }

        #endregion

        // Extract addresses from a 32bit data 
        public static AddressData ExtractAddressData(uint BigAddress)
        {
            AddressData Addresses = new AddressData();
            //////////////////////////////////
            // Extract addresses
            //////////////////////////////////
            // ModuleID (FF 00 00 00)
            Addresses.ModuleID = (int)((BigAddress & 0xFF000000) >> 24);
            // Session ID (00 FF 00 00)
            Addresses.SessionID = (int)((BigAddress & 0x00FF0000) >> 16);
            // Cycle ID (00 00 FF 00)
            Addresses.CycleID = (int)((BigAddress & 0x0000FF00) >> 8);
            // Period ID (00 00 00 FF)
            Addresses.PeriodID = (int)(BigAddress & 0x000000FF);

            return Addresses;

        }

        // Extract 4*8bit from a 32bit data 
        public static void Extract4x8bitFrom32bitData(uint BigData, ref int MSBdata, ref int Data1, ref int Data2, ref int LSBdata)
        {
            //////////////////////////////////
            // Extract Data
            //////////////////////////////////
            // ModuleID (FF 00 00 00)
            MSBdata = (int)((BigData & 0xFF000000) >> 24);
            // Session ID (00 FF 00 00)
            Data1 = (int)((BigData & 0x00FF0000) >> 16);
            // Cycle ID (00 00 FF 00)
            Data2 = (int)((BigData & 0x0000FF00) >> 8);
            // Period ID (00 00 00 FF)
            LSBdata = (int)(BigData & 0x000000FF);

        }

        // Extract 2*16bit from a 32bit data 
        public static void Extract2x16bitFrom32bitData(uint BigData, ref int MSBdata, ref int LSBdata)
        {
            //////////////////////////////////
            // Extract Data
            //////////////////////////////////
            // ModuleID (FF FF 00 00)
            MSBdata = (int)((BigData & 0xFFFF0000) >> 16);
            // Period ID (00 00 FF FF)
            LSBdata = (int)(BigData & 0x0000FFFF);

        }

        public static uint AssambleAddressData(int ModuleID, int SessionID, int CycleID, int PeriodID)
        {
            uint AddressData = 0;
            //////////////////////////////////
            // Assamble addresses
            //////////////////////////////////
            // ModuleID (FF 00 00 00)
            AddressData = (uint)((ModuleID & 0xFF) << 24);
            // Session ID (00 FF 00 00)
            AddressData |= (uint)((SessionID & 0xFF) << 16);
            // Cycle ID (00 00 FF 00)
            AddressData |= (uint)((CycleID & 0xFF) << 8);
            // Period ID (00 00 00 FF)
            AddressData |= (uint)(PeriodID & 0xFF);

            return AddressData;
        }

        #region Events

        // RX Handler Event process
        protected static void TxProcess(string Data, bool AddStartEndChar)
        {
            if (TX_Handle != null)
            {
                TX_Handle(Data, AddStartEndChar);
            }
        }

        // Error Handler Event process
        protected static void ErrorProcess(string Data)
        {
            if (Error_Handle != null)
            {
                Error_Handle(Data);
            }
        }

        // RX Handler Event process
        protected static void RxCommandQueue_Trigg()
        {
            if (Rx_CommadQueue_Handler != null)
            {
                Rx_CommadQueue_Handler();
            }
        }

        // Send ACK Error Handler Event process
        protected static void ACKErrorProcess(TxData Package)
        {
            if (ACK_Error_Handle != null)
            {
                ACK_Error_Handle(Package);
            }
        }


        #endregion

        #region Send Package generate
        /// <summary>
        /// Create Send Package
        /// </summary>
        /// <param name="PackID"></param>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="Data"></param>
        /// <returns></returns>

        // With PackID
        private static string DataPackage(bool RequestACK, uint PackID, string Command, uint Address, string[] Data)
        {
            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar[0] + Data;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar[0] + Data;
            }


            // Add List Data
            for (int i = 1; i < Data.Length; i++)
            {
                Sdata += DataDelimiterChar + Data[i];
            }
            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, uint PackID, string Command, uint Address, string Data)
        {

            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar + Data;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar + Data;
            }


            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, uint PackID, string Command, uint Address)
        {

            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address);
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address);
            }


            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, uint PackID, string Command, string Data)
        {
            // PackageID , Command , Address , Data

            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + DataChar + Data;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command + DataChar + Data;
            }

            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, uint PackID, string Command)
        {
            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = PackIDChar + ConverterClass.uInt16_2Hex(PackID) + CommandChar + Command;
            }

            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        //Without Pack ID
        private static string DataPackage(bool RequestACK, string Command, uint Address, string[] Data)
        {
            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar[0] + Data;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata =  CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar[0] + Data;
            }


            // Add List Data
            for (int i = 1; i < Data.Length; i++)
            {
                Sdata += DataDelimiterChar + Data[i];
            }
            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, string Command, uint Address, string Data)
        {

            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar + Data;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata =  CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address) + DataChar + Data;
            }


            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, string Command, uint Address)
        {

            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address);
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = CommandChar + Command + AddressChar + ConverterClass.uInt32_2Hex(Address);
            }


            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, string Command, string Data)
        {
            // PackageID , Command , Address , Data

            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + CommandChar + Command + DataChar + Data;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = CommandChar + Command + DataChar + Data;
            }

            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        private static string DataPackage(bool RequestACK, string Command)
        {
            string Sdata;
            if (RequestACK == true)
            {
                // AckRequest, PackageID , Command , Address , Data
                Sdata = RxACK_Request_char + CommandChar + Command;
            }
            else
            {
                // PackageID , Command , Address , Data
                Sdata = CommandChar + Command;
            }

            // CRC32
            uint CRCvalue = CRC32.CRCcompute(Sdata);
            // Start , CRC , Data , EndChar
            // Start/End char is added in Comport class
            return ConverterClass.uInt16_2Hex(CRCvalue) + Sdata;

        }

        #endregion

        #region Tx Data Send

        // With Package ID
        /// <summary>
        /// Data Array (Multi data)
        /// Package ID
        /// PackID, Command, Address, Data[]
        /// </summary>
        /// <param name="PackID"></param>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="Data"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(uint PackID, string Command, uint Address, string[] Data, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, PackID, Command, Address, Data), Timedelay_After, StartEndChar, RequestACK, (int)PackID);
        }
        /// <summary>
        /// Singel Data
        /// Package ID
        /// PackID, Command, Address, Data
        /// </summary>
        /// <param name="PackID"></param>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="Data"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(uint PackID, string Command, uint Address, string Data, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, PackID, Command, Address, Data), Timedelay_After, StartEndChar, RequestACK, (int)PackID);
        }
        /// <summary>
        /// No Data
        /// Package ID
        /// PackID, Command, Address
        /// </summary>
        /// <param name="PackID"></param>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(uint PackID, string Command, uint Address, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, PackID, Command, Address), Timedelay_After, StartEndChar, RequestACK, (int)PackID);
        }
        /// <summary>
        /// No Adress
        /// Package ID
        /// PackID, Command, Data
        /// </summary>
        /// <param name="PackID"></param>
        /// <param name="Command"></param>
        /// <param name="Data"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(uint PackID, string Command, string Data, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, PackID, Command, Data), Timedelay_After, StartEndChar, RequestACK, (int)PackID);
        }
        /// <summary>
        /// Only Command
        /// Package ID
        /// PackID, Command
        /// </summary>
        /// <param name="PackID"></param>
        /// <param name="Command"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(uint PackID, string Command, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, PackID, Command), Timedelay_After, StartEndChar, RequestACK, (int)PackID);
        }

        // Without Package ID
        /// <summary>
        /// Data Array (Multi data)
        /// Command, Address, Data[]
        /// </summary>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="Data"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(string Command, uint Address, string[] Data, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, Command, Address, Data), Timedelay_After, StartEndChar, RequestACK);
        }
        /// <summary>
        /// Singel Data
        /// Command, Address, Data
        /// </summary>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="Data"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(string Command, uint Address, string Data, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, Command, Address, Data), Timedelay_After, StartEndChar, RequestACK);
        }
        /// <summary>
        /// No Data
        /// Command, Address
        /// </summary>
        /// <param name="Command"></param>
        /// <param name="Address"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(string Command, uint Address, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, Command, Address), Timedelay_After, StartEndChar, RequestACK);
        }
        /// <summary>
        /// No Adress
        /// Command, Data
        /// </summary>
        /// <param name="Command"></param>
        /// <param name="Data"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(string Command, string Data, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK,  Command, Data), Timedelay_After, StartEndChar, RequestACK);
        }
        /// <summary>
        /// Only Command
        /// Command
        /// </summary>
        /// <param name="Command"></param>
        /// <param name="RequestACK"></param>
        /// <param name="Timedelay_After"></param>
        /// <param name="StartEndChar"></param>
        public static void Tx_Queue_Add(string Command, bool RequestACK = false, int Timedelay_After = 10, bool StartEndChar = true)
        {
            Tx_Queue_Add_Data(DataPackage(RequestACK, Command), Timedelay_After, StartEndChar, RequestACK);
        }


        #endregion

        #region TX Queue

        public static void Queue_Clear()
        {
            Tx_DataQueue = new ConcurrentQueue<TxData>();
            Rx_IncommingDataQueue = new ConcurrentQueue<RxData>();
            Rx_CommandQueue = new ConcurrentQueue<RxCommandData>();

        }

        private static void Tx_Queue_Add_Data(string Data,  int Timedelay_After, bool StartEndChar, bool ACK = false, int PackID = -1)
        {
            ////////////

            TxData Sdata = new TxData();
            Sdata.Data = Data;
            Sdata.TimeDelay_After = Timedelay_After;
            Sdata.AddStartAndEndChar = StartEndChar;
            Sdata.SendACK = ACK;
            Sdata.PackageID = PackID;

            // Add
            Tx_DataQueue.Enqueue(Sdata);

        }


        private static void Tx_Queue_Add_AndSend_Data(string Data, int Timedelay_After, bool StartEndChar, bool ACK = false, int PackID = -1)
        {
            ////////////

            TxData Sdata = new TxData();
            Sdata.Data = Data;
            Sdata.TimeDelay_After = Timedelay_After;
            Sdata.AddStartAndEndChar = StartEndChar;
            Sdata.SendACK = ACK;
            Sdata.PackageID = PackID;

            // Add
            Tx_DataQueue.Enqueue(Sdata);

            Tx_Queue_StartSend();

        }


        public static void Tx_Queue_StartSend()
        {
            // Send Thread
            Thread Tx_Queue_Send_Thread = new Thread(() =>
            {
                lock (Tx_DataQueue)
                {

                    // Mark thread as active
                    Tx_Queue_Active = true;
                    //
                    TxData _txData;

                    while (Tx_DataQueue.TryDequeue(out _txData))
                    {

                        // Save Package Data if we need to resend data if ACK fail
                        Tx_LastSentPackageData.Data = _txData.Data;
                        Tx_LastSentPackageData.TimeDelay_After = _txData.TimeDelay_After;
                        Tx_LastSentPackageData.AddStartAndEndChar = _txData.AddStartAndEndChar;
                        Tx_LastSentPackageData.SendACK = _txData.SendACK;
                        Tx_LastSentPackageData.PackageID = _txData.PackageID;                          // if no id is set -> defualt is -1

                        //Send Data
                        TxProcess(_txData.Data, _txData.AddStartAndEndChar);

                        // Check if we should wait for ACK
                        if (_txData.SendACK == true)
                        {

                            // Reset ACK acknowlage
                            ACK_Arrived = false;
                            // Number of resend
                            for (int iResend = 0; iResend < Tx_Resend_MaxTimes; iResend++)
                            {
                                Tx_Resend_counter = iResend;                                    // make Resend count global
                                // Wait for ACK to arrive
                                for (int i = 0; i < ACK_TimeoutValue; i++)
                                {
                                    // sleep for 1ms
                                    Thread.Sleep(1);
                                    // Check if ACK 
                                    if (ACK_Arrived == true)
                                    {
                                        // Set values so we exit both loops 
                                        break;
                                    }
                                }

                                // Check if we should send data once again if no ACK has arrived
                                if (ACK_Arrived == false)
                                {
                                    //Send Data again
                                    TxProcess(Tx_LastSentPackageData.Data, Tx_LastSentPackageData.SendACK);
                                }
                                else
                                {
                                    // ACK = true
                                    // Exit for loop
                                    break;
                                }
                            }

                            // Check if we did get an ACK, or if the Queue shuold hold data send
                            if (ACK_Arrived == false)
                            {
                                // Set ACK error , 
                                Error_ACK_Faild = true;
                                // Trigger ACK Error Event, send the package that faild
                                ACKErrorProcess(Tx_LastSentPackageData);
                                // Mark Thread as done
                                Tx_Queue_Active = false;
                                //Exit this while loop
                                break;
                            }


                        }


                        // Check if we should wait untill we should send next
                        if (_txData.TimeDelay_After > 0)
                        {
                            // wait for x ms
                            Thread.Sleep(_txData.TimeDelay_After);
                        }

                    }
                }
                // Mark thread as done
                Tx_Queue_Active = false;
            });


            // Trigg DeQueue event to force send rutin to begin 
            if (Tx_Queue_Active == false)
            {
                // Activate Queue send 
                // Tx_Queue_Send_Thread.IsBackground = true;
                Tx_Queue_Send_Thread.Start();
            }
        }

        #endregion

        #region Tx ACK Timer
        ///// <summary>
        ///// Start timer calling Tx_ACK_Timer_Event
        ///// </summary>
        //static void Tx_ACK_Timer_Start()
        //{
        //    // Reset ACK status 
        //    ACK_For_Tx_Package = false;
        //    Tx_Resend_counter = 0;
        //    // Start timer calling Tx_ACK_Timer_Event
        //    // Timer is set to 1 sec as defualt
        //    System.Threading.Timer Tx_Ack_Timer = new System.Threading.Timer(new System.Threading.TimerCallback(Tx_ACK_Timer_Event), null, ACK_TimeoutValue, ACK_TimeoutValue);
        //}

        ///// <summary>
        ///// Dispose Current running ACK timer
        ///// </summary>
        //static void Tx_ACK_Timer_Stop()
        //{
        //    Tx_Ack_Timer.Dispose();
        //}

        ///// <summary>
        ///// Timer event to check if Tx package respons ack was recived, 
        ///// and reSend Tx package if Ack was not recived
        ///// </summary>
        ///// <param name="state"></param>
        //static void Tx_ACK_Timer_Event(object state)
        //{
        //    // Check if Ack was recived
        //    if (ACK_For_Tx_Package == false)
        //    {
        //        // Check if total resend times is less then max resend times
        //        if (Tx_Resend_counter < Tx_Resend_MaxTimes)
        //        {
        //            // resend last package
        //            Tx_Resend_counter++;
        //        }
        //        else
        //        {
        //            // Resend fail, raise Tx ACK fail Event Handler

        //            // Stop Tx_ACK_Check_Timer
        //            Tx_Ack_Timer.Dispose();
        //        }
        //    }
        //    else
        //    {
        //        // some thing is wrong, ACK is registred but send loop has not registerd that the ACK is recived 
        //    }

        //}

        #endregion 

        #region Rx Data Decrypt

        /// <summary>
        /// Use to input new data into the communication class
        /// 1st Input Stage
        /// </summary>
        /// <param name="Data"></param>
        public static void RxData_Process(string Data)
        {
            BytesRecived += Data.Length;
            PackagesRecived_Total++;

            // Check that there is Data 
            if (Data.Length > 0)
            {
                // Check if CRC should be done
                if (CRC_Check_Enable == true)
                {
                    // Do CRC Check
                    if (RxData_CRC_Check(Data) == true)
                    {
                        // Package CRC OK
                        RxData_Decrypt(Data);
                    }
                    else
                    {
                        // Package CRC Fail, Save Package to be able to analyse Failed Packages
                        ErrorProcess("Rx Package CRC Fail!!. muppo");
                    }
                }
                else
                {
                    // Go direct to decrypt data
                    //RxData_Decrypt(Data);
                }


            }

        }

        private static bool RxData_CRC_Check(string Data)
        {
            
            
            
            //"$E3452E2E!9C%DATA@58*AllInfo#"
            // Get end CRC index
            int CRCendIndex = ConverterClass.GetFirstNonHexIndex(Data);

            //E3 45 2E 2E
            // Check if CRC data exist
            if (Data.Length < 2)
            {
                // 32 CRC do not exist, return Error
                ErrorProcess("Rx Package CRC lenght Fail!! -> " + Data.Length.ToString() + " ->" + Data);
                return false;
            }
            // Check if Any other data exist 
            if (Data.Length < CRCendIndex)
            {
                // No other data is in the package , only CRC value
                ErrorProcess("Rx Package lenght Fail!!, No Other data in package then CRC -> " + Data.Length.ToString() + " ->" + Data);
                return false;
            }

            // Check if CRC end char exist
            if (CRCendIndex < 0)
            {
                // No other data is in the package , only CRC value
                ErrorProcess("Rx Package CRC end Char Fail!!, No end char ->" + Data);
                return false;
            }


            // Get CRC string (32bit = 8 char in Hex, ex: 00 54 98 61)
            string CRCstring = Data.Substring(0, CRCendIndex);
            // check if string is valid Hex string
            if (ConverterClass.CheckHEXstring(CRCstring) == false)
            {
                // CRC header includes characters that is not HEX
                ErrorProcess("Rx CRC header includes characters that is not HEX");
                return false;
            }
            // Convert CRC Header to uint
            uint CRCheaderVaule = ConverterClass.Hex2_uInt32(CRCstring);
            // Calculate CRC value from Body, start after CRC Checksum value
            string CRCBodyString = Data.Substring(CRCendIndex, Data.Length - CRCendIndex);
            uint CRCbodyValue = CRC32.CRCcompute(CRCBodyString);
            // check if header CRC resualt is the same as body CRC resualt
            
            if (CRCheaderVaule != CRCbodyValue)
            {
                // Return false, the header and body CRC is not the same
                ErrorProcess("Error: Rx Header and body CRC is not the same !!");
                ErrorProcess("Header: " + CRCstring + " _int:" + CRCheaderVaule.ToString());
                ErrorProcess("body  : " + ConverterClass.uInt32_2Hex(CRCbodyValue) + " _int:" + CRCbodyValue.ToString() + System.Environment.NewLine);
                ErrorProcess("bodyData: " + Data.Substring(CRCendIndex, Data.Length - CRCendIndex));
                return false;
            }
            else
            {
                return true;
            }
        }

        /// <summary>
        /// $CRC?!PackID%Command@Address*Data#                      One Data
        /// $CRC?!PackID%Command@Address:Data1:Data2:Data3:Data4:#  Data Array
        /// $CRC:Data1:Data2:Data3:Data4:#                          Data Array
        /// The command Order must be in 
        /// ?!%@*
        /// OR in Data Array 
        /// ?!%@::
        /// </summary>
        /// <param name="Data"></param>
        public static void RxData_Decrypt(string Data)
        {
            
            int index = -1;
            int LastCapturedIndex = 0;


            // Temp String
            string TempData = "";
            int TempDataSize = 0;

            // Sorting Array
            int[] DelimiterPos = new int[8];
            int DelimiterPosCounter = 0;

            // Get end CRC index
            int CRCendIndex = ConverterClass.GetFirstNonHexIndex(Data);

            BytesRecived_CMD += Data.Length;
            PackagesRecived_OK++;

            // If CRC Check is enabled, remove CRC Data
            // we know it exist becuse of the CRC check was OK
            if (CRC_Check_Enable == true)
            {
                Data = Data.Substring(CRCendIndex, Data.Length - CRCendIndex);
            }

            
            // Check if ACK request has been made
            bool RxACK_Request = false;
            if(Data.IndexOf(RxACK_Request_char,0) != -1)
            {
                // Send ACK to Tranciver
                RxACK_Request = true;
                Tx_Queue_Add_AndSend_Data(TxACK_Respons_char, 1, false, false);
            }

            //////////////////////////////////////////////////
            // Find Delimiter pos
            #region Find Delimiters
            // Check if PackID exist
            // -1 = false, if found = store index
            int PackID_Found = Data.IndexOf(PackIDChar, 0);
            int PackID_Found_ID = -1;
            if (PackID_Found != -1)                                 
            {
                // If PackID found, save pos in Array
                PackID_Found_ID = DelimiterPosCounter;              // Save Array Pos for Pack ID
                DelimiterPos[DelimiterPosCounter] = PackID_Found;   // Save Pos in Array
                DelimiterPosCounter++;                              // Inc Pos Counter
            }
            

            // Check if command exist
            // -1 = false, if found = store index
            int Command_Found = Data.IndexOf(CommandChar, 0);
            int Command_Found_ID = -1;
            if (Command_Found != -1)
            {
                Command_Found_ID = DelimiterPosCounter;
                DelimiterPos[DelimiterPosCounter] = Command_Found;
                DelimiterPosCounter++;
            }

            // Check if Address exist
            // -1 = false, if found = store index
            int Address_Found = Data.IndexOf(AddressChar, 0);
            int Address_Found_ID = -1;
            if (Address_Found != -1)
            {
                Address_Found_ID = DelimiterPosCounter;
                DelimiterPos[DelimiterPosCounter] = Address_Found;
                DelimiterPosCounter++;
            }

            // Check if Data exist
            // -1 = false, if found = store index
            int Data_Found = Data.IndexOf(DataChar, 0);
            int Data_Found_ID = -1;
            if (Data_Found != -1)
            {
                Data_Found_ID = DelimiterPosCounter;
                DelimiterPos[DelimiterPosCounter] = Data_Found;
                DelimiterPosCounter++;
            }

            // Check if Data Array exist
            // -1 = false, if found = store index
            int DataArray_Found = Data.IndexOf(DataDelimiterChar, 0);
            int DataArray_Found_ID = -1;
            if (DataArray_Found != -1)
            {
                DataArray_Found_ID = DelimiterPosCounter;
                DelimiterPos[DelimiterPosCounter] = DataArray_Found;
                DelimiterPosCounter++;
            }

            // Last entry in Array should hold the total lenght of Data string
            DelimiterPos[DelimiterPosCounter] = Data.Length;

            #endregion

            //////////////////////////////////////////////////
            // Start To extract data
            #region Extract data
            //////////////
            // Get PackID
            uint PackID_Data = 0;
            bool PackID_Exist = false;
            int PackID_Error = 0;
            if (PackID_Found != -1)
            {
                TempDataSize = DelimiterPos[PackID_Found_ID + 1] - DelimiterPos[PackID_Found_ID] - 1;
                // Check if there is any data in PackID
                // Max 16bit HEX = 4char long
                if ((TempDataSize > 0) && (TempDataSize < 5))
                {
                    // Extract  
                    TempData = Data.Substring(DelimiterPos[PackID_Found_ID] + 1, TempDataSize);
                    // check if string is valid Hex string
                    if (ConverterClass.CheckHEXstring(TempData) == true)
                    {
                        // Convert Hex to uint
                        PackID_Data = ConverterClass.Hex2_uInt16(TempData);
                        // Data holds PackID
                        PackID_Exist = true;
                    }
                    else
                    {
                        // PackID Data Error
                        ErrorProcess("Error: PackID Data is not in HEX !!");
                        PackID_Error = 1;
                    }
                }
                else
                {
                    // PackID Size Error
                    ErrorProcess("PackID Data Size Error !! ->" + TempDataSize.ToString());
                    PackID_Error = 2;
                }
            }

            //////////////
            // Get Command
            string Command_Data = "";
            bool Command_Exist = false;
            int Command_Error = 0;
            if (Command_Found != -1)
            {
                TempDataSize = DelimiterPos[Command_Found_ID + 1] - DelimiterPos[Command_Found_ID] - 1;
                // Check if there is any data in PackID
                // No direct Max Size, defualt right now is 4char long
                if ((TempDataSize > 0) && (TempDataSize < 5))
                {
                    // Extract  
                    Command_Data = Data.Substring(DelimiterPos[Command_Found_ID] + 1, TempDataSize);
                    // Data holds PackID
                    Command_Exist = true;       
                }
                else
                {
                    // Command Size Error
                    ErrorProcess("Command Data Size Error !! ->" + TempDataSize.ToString());
                    Command_Error = 2;
                }
            }

            //////////////
            // Get PackID
            uint Address_Data = 0;
            bool Address_Exist = false;
            int Address_Error = 0;
            if (Address_Found != -1)
            {
                TempDataSize = DelimiterPos[Address_Found_ID + 1] - DelimiterPos[Address_Found_ID] - 1;
                // Check if there is any data in Address
                // Max 32bit HEX = 8char long
                if ((TempDataSize > 0) && (TempDataSize < 9))
                {
                    // Extract  
                    TempData = Data.Substring(DelimiterPos[Address_Found_ID] + 1, TempDataSize);
                    // check if string is valid Hex string
                    if (ConverterClass.CheckHEXstring(TempData) == true)
                    {
                        // Convert Hex to uint
                        Address_Data = ConverterClass.Hex2_uInt32(TempData);
                        Address_Exist = true;
                    }
                    else
                    {
                        // Address Data Error
                        ErrorProcess("Error: Address Data is not in HEX !!");
                        Address_Error = 1;
                    }
                }
                else
                {
                    // Address Size Error
                    ErrorProcess("Address Data Size Error!! ->" + TempDataSize.ToString());
                    Address_Error = 2;
                }
            }

            //////////////
            // Get Data (Single mode)
            string Data_Data = "";
            bool Data_Exist = false;
            int Data_Error = 0;
            if (Data_Found != -1)
            {
                TempDataSize = DelimiterPos[Data_Found_ID + 1] - DelimiterPos[Data_Found_ID] - 1;
                // Check if there is any data in PackID
                // No direct Max Size, defualt right now is 8char long -> 32bit
                if ((TempDataSize > 0) && (TempDataSize < DataSize_Max +1))
                {
                    // Extract  
                    Data_Data = Data.Substring(DelimiterPos[Data_Found_ID] + 1, TempDataSize);
                    // Data holds PackID
                    Data_Exist = true;
                }
                else
                {
                    // Command Size Error
                    ErrorProcess("Data (Single) Size Error!! ->" + TempDataSize.ToString());
                    Data_Error = 2;
                }
            }

            //////////////
            // Get Data (Array mode)
            List<string> DataArray_Data = new List<string>();
            bool DataArray_Exist = false;
            int DataArray_Error = 0;
            index = -1;
            if (DataArray_Found != -1)
            {
                // Get size of Data Array string
                TempDataSize = Data.Length - DelimiterPos[DataArray_Found_ID];
                // Extract  
                TempData = Data.Substring(DelimiterPos[DataArray_Found_ID], TempDataSize);

                do
                {
                    // find first Delimiter Char 
                    index = Data.IndexOf(DataDelimiterChar, index + 1);
                    if (index != -1)
                    {
                        LastCapturedIndex = index;
                        index = Data.IndexOf(DataDelimiterChar, index + 1);
                        if (index != -1)
                        {
                            // Get size of Data Array Part string
                            TempDataSize = index - LastCapturedIndex;
                            // Check if there is any data in DataArray
                            // No direct Max Size, defualt right now is 8char long -> 32bit "1 to 9" = 8 chars
                            if ((TempDataSize > 0) && (TempDataSize < 10))
                            {
                                // Extract  
                                DataArray_Data.Add(Data.Substring(LastCapturedIndex + 1, TempDataSize-1));
                                // Data holds PackID
                                DataArray_Exist = true;
                                // Dec index so the last delimiter acts like the start delimiter
                                index--;
                            }
                            else
                            {
                                // DataArray Size Error
                                ErrorProcess("Data (Array) Size Error!! ->" + TempDataSize.ToString());
                                ErrorProcess("->" + Data.Substring(LastCapturedIndex + 1, TempDataSize - 1));
                                DataArray_Error = 2;
                            }

                        }
                    
                    }

                    
                } while (index != -1);

            }
            #endregion

            //////////////////////////////////////////////////
            // Save to Queue
            #region Save Data into Queue

            RxCommandData QueueData = new RxCommandData();
            
            // PackID
            QueueData.PackID_Exist = PackID_Exist;
            QueueData.PackID = PackID_Data;
            QueueData.PackID_Error = PackID_Error;
            // Address
            QueueData.Address_Exist = Address_Exist;
            QueueData.Address = Address_Data;
            QueueData.Address_Error = Address_Error;
            // Command
            QueueData.Command_Exist = Command_Exist;
            QueueData.Command = Command_Data;
            QueueData.Command_Error = Command_Error;
            //Data
            QueueData.Data_Exist = Data_Exist;
            QueueData.Data = Data_Data;
            QueueData.Data_Error = Data_Error;
            //ArrayData
            QueueData.DataArray_Exist = DataArray_Exist;
            QueueData.DataArray = DataArray_Data.ToArray();
            QueueData.DataArray_Error = DataArray_Error;

            // Save in Queue
            Rx_CommandQueue.Enqueue(QueueData);

            // Trigg data in Rx Queue event
         //   if (QueueData.PackID == 9999)
         //   {
                RxCommandQueue_Trigg();
         //   }
            #endregion

            TempData = "";


      



        }


        #endregion

        #region Rx Data Queue
	
        // Just for show
        //private static RxCommandData DeQueue_Rx_IncommingDataQueue()
        //{

        //    RxCommandData _Rx_CommandQueue;
        //    while (Rx_CommandQueue.TryDequeue(out _Rx_CommandQueue))
        //    {
        //        return _Rx_CommandQueue;
        //    }
        //}


        #endregion

    }


    public class AddressData
    {
        public int ModuleID;
        public int SessionID;
        public int CycleID;
        public int PeriodID;
    }
}
