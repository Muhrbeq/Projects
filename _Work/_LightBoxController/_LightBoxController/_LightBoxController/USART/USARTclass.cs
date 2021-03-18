using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using USART;

namespace MCstudio.USART
{
    public class USARTclass
    {
        public string Test_RX_Data = "";



        /// <summary>
        ///  Comport instance
        /// </summary>
        public Comport_Interface comport = new Comport_Interface();



        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Tx Data
        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        #region Tx Data decleration
        // Tx Send Data Queue (RAW Data)
        private  ConcurrentQueue<TxData> Tx_DataQueue = new ConcurrentQueue<TxData>();

        // Tx Queue status
        private bool Tx_Queue_Active = false;               // Tells if Queue send Thread is active
        // Tx package Ack
        //private static bool ACK_For_Tx_Package = false;                 
        private int Tx_Resend_counter = 0;               // Hold current resend count 
        private int Tx_Resend_MaxTimes = 4;              // Max resend times before "Error_ACK_Faild" is triggerd
        private int ACK_TimeoutValue = 1000;             // One second ACK Timeout is defualt
        private bool ACK_Arrived = false;                // Holds status if ACK has been recived


        // Global status
        private bool ACK_Faild_Status = false;           // If this is true, the send from queue is stoped/halted

        #endregion

        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// Constructor
        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

       
        // Constructor
        public USARTclass()
        {
            // route incomming ACK event to ACK status set in ComProtocol
            comport.RX_ACK_Command_Handle += new Comport_Interface.RXACKHandler(ACK_Incomming_status_Set);

            // Connect to incomming command data from comport
            comport.RX_Command_Handle += new Comport_Interface.RXCommandHandler(com_Incomming_RX);
            comport.RX_CommandEvent_Enable = true;

            // Connect to incomming data from comport
            comport.RX_Handle += new Comport_Interface.RXHandler(com_Incomming_RX);
            comport.RX_HandlerEvent_Enable = true;

        }


        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///  Events
        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        #region Event registrations

        /////////////////////////////////////////////////
        // Info Event Holder
        // Declare a delegate that takes a single string parameter
        // and has no return type. 0 = status
        public delegate void Info_Handler(string message);
        // Define an Event based on the above Delegate
        public event Info_Handler Info__Handle;

        /////////////////////////////////////////////////
        // Rx buffer Event holder
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void RX_Handler(string message);
        // Define an Event based on the above Delegate
        public event RX_Handler RX__Handle;

        /////////////////////////////////////////////////
        // Tx buffer Event holder
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void TX_Handler(string message);
        // Define an Event based on the above Delegate
        public event TX_Handler TX__Handle;

        ////////////////////////////////////////////////
        // Rx Command in Queue
        public delegate void RX_CommandHandler(string message);
        // Define an Event based on the above Delegate
        public event RX_CommandHandler RX__Command_Handle;

        ////////////////////////////////////////////////
        // Rx (incomming) ACK Event
        public delegate void RX_ACKHandler(bool Status);
        // Define an Event based on the above Delegate
        public event RX_ACKHandler RX__ACK_Command_Handle;


        /// //////////////////////////////////////////////////////////////////////////////

        // Comport Rx Error event handler
        void Port_ErrorRecived(object sender, SerialErrorReceivedEventArgs e)
        {
            InfoProcess(e.ToString());
        }


        // RX Handler Event process
        protected void InfoProcess(string message)
        {
            if (Info__Handle != null) 
            {
                Info__Handle(message);
            }
        }

        // RX Handler Event process
        protected void RxProcess(string Data)
        {
            if (RX__Handle != null) 
            {
                RX__Handle(Data);
            }
        }

        // RX Command Handler Event process
        protected void RxCommandProcess(string Data)
        {
            if (RX__Command_Handle != null)
            {
                RX__Command_Handle(Data);
            }
        }

        // TX Handler Event process
        protected void TxProcess(string Data)
        {
            if (TX__Handle != null) 
            {
                TX__Handle(Data);
            }
        }

        // RX Command Handler Event process
        protected void RxACKProcess(bool status)
        {
            if (RX__ACK_Command_Handle != null)
            {
                RX__ACK_Command_Handle(status);
            }
        }

        /// //////////////////////////////////////////////////////////////////////////////
        /// 

        // Set this when ACK is recived (incomming data)
        public void ACK_Incomming_status_Set(bool value)
        {
            ACK_Arrived = value;
            RxACKProcess(value);
        }

        // Route incomming data from comport 
        public void com_Incomming_RX(string Data)
        {
           // Test_RX_Data = Data;
            RxProcess(Data);
        }

        #endregion

        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// 
        /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        #region Get / Set



        #endregion




        #region TX Queue

        public void Tx_Queue_Clear()
        {
            Tx_DataQueue = new ConcurrentQueue<TxData>();
         //->   Rx_IncommingDataQueue = new ConcurrentQueue<RxData>();
         //->   Rx_CommandQueue = new ConcurrentQueue<RxCommandData>();
        }

        public void Tx_Queue_Add_Data(string Data, int Timedelay_After, bool StartEndChar, bool ACK = false, int PackID = -1)
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

        public void Tx_Queue_Add_AndSend_Data(string Data, int Timedelay_After, bool StartEndChar, bool ACK = false, int PackID = -1)
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

        public void Tx_Queue_StartSend()
        {


            // Send Thread
            Thread Tx_Queue_Send_Thread = new Thread(() =>
            {
                // Tx 
                TxData Tx_LastSentPackageData;                                                          // Holds last sent package data, if we need to resend 

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
                       //-> TxProcess(_txData.Data, _txData.AddStartAndEndChar);
                        comport.SendData(_txData.Data, _txData.AddStartAndEndChar);

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
                                    //-> TxProcess(Tx_LastSentPackageData.Data, Tx_LastSentPackageData.SendACK);
                                    comport.SendData(Tx_LastSentPackageData.Data, Tx_LastSentPackageData.SendACK);
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
                                ACK_Faild_Status = true;
                                // Trigger ACK Error Event, send the package that faild
                                //-> ACKErrorProcess(Tx_LastSentPackageData);
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






    }


    // Tx Package
    public struct TxData
    {
        public string Data;                         // Data in Package
        public int TimeDelay_After;                 // Time to wait after this package has been sent, utill next package should be send, (Zero wait time -> TimeDelay_After = 0)
        public bool AddStartAndEndChar;             // If Start and End Char should be added, this is always added in lowest com layer
        public bool SendACK;                        // If package has a ACK request char in data
        public int PackageID;                       // if no id is set -> PackageID = -1
    }
}
