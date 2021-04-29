using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph; 

namespace Graph_Component
{
    public partial class Graph_Comp : UserControl
    {

        GraphPane myPane;




        // Internal timing by PC clock
        // Starting time in milliseconds
        bool InternalTimerRef = false;
        int tickStart = 0;
        int tickTotalPause = 0;
        int tickPause = 0;

        // Chart Window dimension
        double Chart_Window_Lenght = 20;
        double Chart_MinorStep = 0.1;
        double Chart_MajorStep = 2;

        int[] Axis_ID = new int[10];

        // Signal and line items list
        string[] signal_ID = new string[100];
        // Line item 
        int[] Line_ID = new int[100];
        int Line_ID_Count = 0;

        /////////////////////////////////////////////////
        // 
        // Declare a delegate that takes a single string parameter
        // and has no return type.
        public delegate void SetMarkerHandler();
        // Define an Event based on the above Delegate
        public event SetMarkerHandler SetMarker_Event_Handle;

        // SetMarker Handler Event process
        protected void SetMarkerProcess()
        {
            if (SetMarker_Event_Handle != null)
            {
                SetMarker_Event_Handle();
            }
        }



        #region Get/Set

        #region Graph Text
        /// Holds title of the plot
        private string _Title = "Plot";
        public string Title
        {
            get { return _Title; }
            set { _Title = value;}
        }

        /// Holds title of the X axis
        private string _X_axis_Text = "X";
        public string X_axis_Text
        {
            get { return _X_axis_Text; }
            set { _X_axis_Text = value; }
        }

        /// Holds title of the Y axis
        private string _Y_axis_Text = "Y";
        public string Y_axis_Text
        {
            get { return _Y_axis_Text; }
            set { _Y_axis_Text = value; }
        }
        #endregion


        #region InternalTimerRef_ON
        public bool Get_InternalTimerRef_ON()
        {
            return InternalTimerRef;
        }
        public void Set_InternalTimerRef_ON(bool value)
        {
            InternalTimerRef = value;
        }
        #endregion

        #region InternalTimerRef_ON
        public int Get_TickStart()
        {
            return tickStart;
        }
        public void Set_TickStart(int value)
        {
            tickStart = value;
        }
        #endregion

        #region TickTotalPause
        public int Get_TickTotalPause()
        {
            return tickTotalPause;
        }
        public void Set_TickTotalPause(int value)
        {
            tickTotalPause = value;
        }
        #endregion

        #region TickPause
        public int Get_TickPause()
        {
            return tickPause;
        }
        public void Set_TickPause(int value)
        {
            tickPause = value;
        }
        #endregion

        #region Chart_Window_Lenght
        public double Get_Chart_Window_Lenght()
        {
            return Chart_Window_Lenght;
        }
        public void Set_Chart_Window_Lenght(double value)
        {
            Chart_Window_Lenght = value;
        }
        #endregion

        #region Chart_MinorStep
        public double Get_Chart_MinorStep()
        {
            return Chart_MinorStep;
        }
        public void Set_Chart_MinorStep(double value)
        {
            Chart_MinorStep = value;
        }
        #endregion

        #region Chart_MajorStep
        public double Get_Chart_MajorStep()
        {
            return Chart_MajorStep;
        }
        public void Set_Chart_MajorStep(double value)
        {
            Chart_MajorStep = value;
        }
        #endregion

        #endregion



        public Graph_Comp()
        {
            InitializeComponent();

            myPane = zedGraphControl1.GraphPane;
        }

        #region Test
        public void Load_Test_Pattern()
        {
            // Lets generate sine and cosine wave
            double[] x = new double[100];
            double[] y = new double[100];
            double[] z = new double[100];

            for (int i = 0; i < x.Length; i++)
            {
                x[i] = i;
                y[i] = Math.Sin(0.3 * x[i]);
                z[i] = Math.Cos(0.3 * x[i]);
            }

            // This is to remove all plots
            zedGraphControl1.GraphPane.CurveList.Clear();

            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;

            // PointPairList holds the data for plotting, X and Y arrays
            PointPairList spl1 = new PointPairList(x, y);
            PointPairList spl2 = new PointPairList(x, z);

            // Add cruves to myPane object
            LineItem myCurve1 = myPane.AddCurve("Sine Wave", spl1, Color.Blue, SymbolType.None);
            LineItem myCurve2 = myPane.AddCurve("Cosine Wave", spl2, Color.Red, SymbolType.None);

            myCurve1.Line.Width = 5.0F;
            myCurve2.Line.Width = 3.0F;
            myPane.Title.Text = "test Plot";

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

        }
        #endregion


        #region Clear graph
        public void Clear_All()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            if (myPane != null)
            {
                // This is to remove all plots
                myPane.CurveList.Clear();
                myPane.YAxisList.Clear();
                myPane.GraphObjList.Clear();

                myPane.AddYAxis("Y");

                // I add all three functions just to be sure it refeshes the plot.  
                myPane.AxisChange();
                zedGraphControl1.Invalidate();
                zedGraphControl1.Refresh();
            }

        }

        #endregion


        #region Load Static Data


        public void LoadStatic_Graph_Data2(double[] X, double[] Y, double xRange, string Text, Color SignalColor, bool Smooth, float LineThicknes, SymbolType symbol)
        {

            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            PointPairList spl1 = new PointPairList(X, Y);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);
            // Change line thicknes
            curve.Line.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            //curve.YAxisIndex

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;
            myPane.YAxis.Title.Text = _Y_axis_Text;

            


            myPane.XAxis.Scale.MaxAuto = true;
            myPane.XAxis.Scale.MinAuto = true;
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = xRange;

            myPane.XAxis.Scale.MaxGrace = 0.0;
            myPane.XAxis.Scale.MinGrace = 0.0;



            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();


        }

        public void LoadStatic_Graph_Data(double[] X, double[] Y, string Text, Color SignalColor, bool Smooth, float LineThicknes, SymbolType symbol)
        {

            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            PointPairList spl1 = new PointPairList(X, Y);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);
            // Change line thicknes
            curve.Line.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            //curve.YAxisIndex

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;
            myPane.YAxis.Title.Text = _Y_axis_Text;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

        }

        public void LoadStatic_Graph_Data(double[] X, double[] Y, string Text, string SignalColor, bool Smooth, float LineThicknes, string symbol)
        {

            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            PointPairList spl1 = new PointPairList(X, Y);

            LineItem curve = myPane.AddCurve(Text, spl1, ColorTranslator.FromHtml(SignalColor), SYMBOLS[ParseSymbolName(symbol)]);

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;
            myPane.YAxis.Title.Text = _Y_axis_Text;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();


            //curve.Line.StepType = StepType.ForwardSegment;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();


        }


        public void LoadStatic_Graph_Data_NoUpdate(double[] X, double[] Y, double xRange, string Text, Color SignalColor, bool Smooth, float LineThicknes, SymbolType symbol)
        {

            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            PointPairList spl1 = new PointPairList(X, Y);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);
            // Change line thicknes
            curve.Line.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            //curve.YAxisIndex

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;
            myPane.YAxis.Title.Text = _Y_axis_Text;







        }

        public void LoadStatic_Graph_Show(double xRange, double yRange)
        {
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;

            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;

            myPane.XAxis.Scale.MaxAuto = true;
            myPane.XAxis.Scale.MinAuto = true;
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = xRange;

            myPane.XAxis.Scale.MaxGrace = 0.0;
            myPane.XAxis.Scale.MinGrace = 0.0;
        }

        #endregion 


        #region Add Dynamic Signal

        public void ADD_Dynamic_Graph_BarItem_WithoutUpdate(int Size, string Text, Color SignalColor, float LineThicknes, int DisplayOrder, SymbolType symbol)
        {
            //
            //--            list.Clear();
            //
            GraphPane myPane = zedGraphControl1.GraphPane;


            // Save the beginning time for reference
            // tickStart = Environment.TickCount;


            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            BarItem curve = myPane.AddBar(Text, spl1, SignalColor);
            // Change line thicknes
           // curve.Bar.Border.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            curve.Tag = DisplayOrder;

        }

        public void ADD_Dynamic_Graph_WithoutUpdate(int Size, string Text, Color SignalColor, bool Smooth, float LineThicknes, SymbolType symbol)
        {
            //
            //--            list.Clear();
            //
            GraphPane myPane = zedGraphControl1.GraphPane;


            // Save the beginning time for reference
            // tickStart = Environment.TickCount;


            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);
            // Change line thicknes
            curve.Line.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }
        }

        public void ADD_Dynamic_Graph_WithoutUpdate(int Size, string Text, Color SignalColor, bool Smooth, float LineThicknes, SymbolType symbol , int YAxisID)
        {

            GraphPane myPane = zedGraphControl1.GraphPane;


            // Save the beginning time for reference
            // tickStart = Environment.TickCount;
            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);
            // Change line thicknes
            curve.Line.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            curve.YAxisIndex = Axis_ID[YAxisID];

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }
        }

        public void ADD_Dynamic_BarItem_Graph(int Size, string Text, Color SignalColor, int DisplayOrder, SymbolType symbol)
        {
            //
            //--            list.Clear();
            //
            GraphPane myPane = zedGraphControl1.GraphPane;


            // Save the beginning time for reference
            // tickStart = Environment.TickCount;


            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            BarItem curve = myPane.AddBar(Text, spl1, SignalColor);
            // Change line thicknes
        //    curve.Bar.Border.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            curve.Tag = DisplayOrder;

            // Set ID in list
            CurveSetID(Text);

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();
        }

        public void ADD_Dynamic_BarItem_Graph(int Size, string Text, Color SignalColor, int DisplayOrder, SymbolType symbol, int YAxisID, string YAxisText)
        {

            GraphPane myPane = zedGraphControl1.GraphPane;
            // int y1 = myPane.AddYAxis(YAxisText);
            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;

            var y1 = myPane.AddYAxis(YAxisText);

            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);
            // Change line thicknes
          //  curve.Line.Width = LineThicknes;
            // If line should be visible 
            //myCurve1.Line.IsVisible = false;

            curve.Tag = DisplayOrder;


            myPane.YAxisList[y1].Scale.FontSpec.FontColor = SignalColor;
            myPane.YAxisList[y1].Title.FontSpec.FontColor = SignalColor;
            myPane.YAxisList[y1].Scale.FontSpec.Size = 8.0f;
            myPane.YAxisList[y1].Title.FontSpec.Size = 8.0f;
            curve.YAxisIndex = y1;

            //if(YAxisID == 0)
            //{
            //    curve.IsY2Axis = true;
            //}

            // Set ID in list
            CurveSetID(Text);

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

        }

        public void ADD_Dynamic_Graph(int Size, string Text, Color SignalColor, bool Smooth, float LineThicknes, int DisplayOrder, SymbolType symbol)
        {
            //
            //--            list.Clear();
            //
            GraphPane myPane = zedGraphControl1.GraphPane;


            // Save the beginning time for reference
            // tickStart = Environment.TickCount;


            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);

            if (LineThicknes < 0.0f)
            {
                // If line should be visible 
                curve.Line.IsVisible = false;
            }
            else
            {
                // Change line thicknes
                curve.Line.Width = LineThicknes;
                curve.Line.IsVisible = true;
            }

            curve.Tag = DisplayOrder;

  

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

           

            // Set ID in list
            CurveSetID(Text);

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();
        }

        public void ADD_Dynamic_Graph(int Size, string Text, string SignalColor, bool Smooth, float LineThicknes, string symbol)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;
            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, ColorTranslator.FromHtml(SignalColor), SYMBOLS[ParseSymbolName(symbol)]);

            if (LineThicknes < 0.0f)
            {
                // If line should be visible 
                curve.Line.IsVisible = false;
            }
            else
            {
                // Change line thicknes
                curve.Line.Width = LineThicknes;
                curve.Line.IsVisible = true;
            }

            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

            // Set ID in list
            CurveSetID(Text);

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();


        }

        public void ADD_Dynamic_Graph(int Size, string Text, Color SignalColor, bool Smooth, float LineThicknes, SymbolType symbol, int YAxisID, string YAxisText)
        {

            GraphPane myPane = zedGraphControl1.GraphPane;
           // int y1 = myPane.AddYAxis(YAxisText);
            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;

            var y1 = myPane.AddYAxis(YAxisText);

            // PointPairList holds the data for plotting, X and Y arrays
            //----PointPairList spl1 = new PointPairList(X, Y);
            RollingPointPairList spl1 = new RollingPointPairList(Size);
            // Add cruves to myPane object
            LineItem curve = myPane.AddCurve(Text, spl1, SignalColor, symbol);

            if (LineThicknes < 0.0f)
            {
                // If line should be visible 
                curve.Line.IsVisible = false;
            }
            else
            {
                // Change line thicknes
                curve.Line.Width = LineThicknes;
                curve.Line.IsVisible = true;
            }

            // Axis_ID[YAxisID] = 0;
            // curve.YAxisIndex = Axis_ID[YAxisID];


            myPane.YAxisList[y1].Scale.FontSpec.FontColor = SignalColor;
            myPane.YAxisList[y1].Title.FontSpec.FontColor = SignalColor;
            myPane.YAxisList[y1].Scale.FontSpec.Size = 8.0f;
            myPane.YAxisList[y1].Title.FontSpec.Size = 8.0f;
            curve.YAxisIndex = y1;

            //if(YAxisID == 0)
            //{
            //    curve.IsY2Axis = true;
            //}


            if (Smooth == true)
            {
                curve.Line.IsSmooth = true;
            }

            // Set ID in list
            CurveSetID(Text);

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

        }

        public void Set_Common_Y_To_Zero()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            // int y1 = myPane.AddYAxis(YAxisText);
            ////////////////////////////////////////////////////////////////////////
            // GraphPane object holds one or more Curve objects (or plots)
            myPane = zedGraphControl1.GraphPane;

            foreach (YAxis item in myPane.YAxisList)
            {
                item.Scale.Min = 0.0d;
            }




        }


            public void ADD_Dynamic_Graph_GridLines()
        {

            GraphPane myPane = zedGraphControl1.GraphPane;

            CurveSetID(Text);

            // Align the Y2 axis labels so they are flush to the axis
            myPane.Y2Axis.Scale.Align = AlignP.Inside;

            //Grid
            //myPane.XAxis.MajorGrid.IsVisible = true;
            //myPane.XAxis.MajorGrid.DashOn = 10;
            //myPane.XAxis.MajorGrid.DashOff = 5;
            myPane.YAxis.MajorGrid.IsVisible = true;
            myPane.YAxis.MajorGrid.DashOn = 10;
            myPane.YAxis.MajorGrid.DashOff = 5;

            //myPane.Y2Axis.MajorGrid.IsVisible = true;
            //myPane.Y2Axis.MajorGrid.DashOn = 10;
            //myPane.Y2Axis.MajorGrid.DashOff = 1;

            myPane.XAxis.MajorGrid.IsVisible = true;
            myPane.XAxis.MinorGrid.IsVisible = true;

            // Make the Y axis scale red
//            myPane.YAxis.Scale.FontSpec.FontColor = Color.Red;
//            myPane.YAxis.Title.FontSpec.FontColor = Color.Red;
            // turn off the opposite tics so the Y tics don't show up on the Y2 axis
            myPane.YAxis.MajorTic.IsOpposite = false;
            myPane.YAxis.MinorTic.IsOpposite = false;
            // Don't display the Y zero line
            myPane.YAxis.MajorGrid.IsZeroLine = false;
            // Align the Y axis labels so they are flush to the axis
           myPane.YAxis.Scale.Align = AlignP.Inside;



            // Enable the Y2 axis display
            myPane.Y2Axis.IsVisible = true;
            // Make the Y2 axis scale blue
//            myPane.Y2Axis.Scale.FontSpec.FontColor = Color.Blue;
//            myPane.Y2Axis.Title.FontSpec.FontColor = Color.Blue;
            // turn off the opposite tics so the Y2 tics don't show up on the Y axis
            myPane.Y2Axis.MajorTic.IsOpposite = false;
            myPane.Y2Axis.MinorTic.IsOpposite = false;
            // Display the Y2 axis grid lines
            myPane.Y2Axis.MajorGrid.IsVisible = true;
            myPane.Y2Axis.MinorGrid.IsVisible = true;
            // Align the Y2 axis labels so they are flush to the axis
            myPane.Y2Axis.Scale.Align = AlignP.Inside;

            myPane.Y2Axis.IsAxisSegmentVisible = true;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

        }


        #endregion


        #region Add Data
        /// <summary>
        /// Add data to dynamic list
        /// </summary>XDate xValue
        /// <param name="xValue"></param>
        /// <param name="yValues"></param>
        public void AddDataToGraph(double xValue, double[] yValues)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            //GraphPane myPane = zg1.GraphPane;
            // Make sure that the curvelist has at least one curve
            if (myPane.CurveList.Count <= 0)
                return;
            else if (myPane.CurveList.Count != yValues.Length)
                return;

            for (int i = 0; i < yValues.Length; i++)
            {
                ((IPointListEdit)myPane.CurveList[i].Points).Add(xValue, yValues[i]);
            }


            // update Screen
            UpdateGraph_ByValue(xValue);
        }

        public void AddDataToGraph(int channel, double xValue, double yValue)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            //GraphPane myPane = zg1.GraphPane;
            // Make sure that the curvelist has at least one curve
            if (myPane.CurveList.Count <= 0)
                return;
            else if (myPane.CurveList.Count < channel)
                return;

            ((IPointListEdit)myPane.CurveList[channel].Points).Add(xValue, yValue);

            // Make sure the Y axis is rescaled to accommodate actual data
         //   zedGraphControl1.AxisChange();

            //Update_Steady_Lines();

            // update Screen
            UpdateGraph_ByValue(xValue);

        }

        public void AddDataToGraph(string ID, double xValue, double yValue)
        {

            //graphMapFeature.GraphPane.CurveList["M2"].Clear();

            GraphPane myPane = zedGraphControl1.GraphPane;
            //GraphPane myPane = zg1.GraphPane;
            // Make sure that the curvelist has at least one curve
            if (myPane.CurveList.Count <= 0)
                return;

            ((IPointListEdit)myPane.CurveList[ID].Points).Add(xValue, yValue);

            // Make sure the Y axis is rescaled to accommodate actual data
            //   zedGraphControl1.AxisChange();

           // Update_Steady_Lines();

            // update Screen
            UpdateGraph_ByValue(xValue);
        }

        public void AddDataToGraph_NoUpdate(int channel, double xValue, double yValue)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            //GraphPane myPane = zg1.GraphPane;
            // Make sure that the curvelist has at least one curve
            if (myPane.CurveList.Count <= 0)
                return;
            else if (myPane.CurveList.Count < channel)
                return;

            ((IPointListEdit)myPane.CurveList[channel].Points).Add(xValue, yValue);

        }


        #endregion


        #region Graph Screen Update


        public void UpdateView2()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            /// Set name on title
            //myPane.Title.Text = _Title;
            //myPane.XAxis.Title.Text = _X_axis_Text;
            //myPane.YAxis.Title.Text = _Y_axis_Text;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl1.AxisChange();
            // Force a redraw
            zedGraphControl1.Invalidate();
            //
            zedGraphControl1.Refresh();
        }

        public void UpdateView_ByValue()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;
            myPane.YAxis.Title.Text = _Y_axis_Text;

            // Set the XAxis to date type
            myPane.XAxis.Type = AxisType.Linear;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl1.AxisChange();
            // Force a redraw
            zedGraphControl1.Invalidate();
            //
            zedGraphControl1.Refresh();
        }


        public void UpdateView_XDate()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;
            /// Set name on title
            myPane.Title.Text = _Title;
            myPane.XAxis.Title.Text = _X_axis_Text;
            myPane.YAxis.Title.Text = _Y_axis_Text;

            //myPane.BarSettings.MinBarGap = 0;
            myPane.BarSettings.MinClusterGap = 0.0f;


            // Set the XAxis to date type
            myPane.XAxis.Type = AxisType.Date;

            // I add all three functions just to be sure it refeshes the plot.  
            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl1.AxisChange();
            // Force a redraw
            zedGraphControl1.Invalidate();
            //
            zedGraphControl1.Refresh();
        }


        public void SetStandardView()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            // Just manually control the X axis range so it scrolls continuously
            // instead of discrete step-sized jumps
            //            myPane.XAxis.Scale.Min = 0.0;
            //myPane.YAxis.Scale.MinAuto = true;
            myPane.XAxis.Scale.Max = Chart_Window_Lenght;
            myPane.XAxis.Scale.MinorStep = Chart_MinorStep;
            myPane.XAxis.Scale.MajorStep = Chart_MajorStep;

        }

        public void UpdateGraph_ByTime()
        {
            double time = (Environment.TickCount - tickStart) / 1000.0;
            //--            list.Add(time, Y);


            // Keep the X scale at a rolling 30 second interval, with one    xScale.MajorStep
            // major step between the max X value and the end of the axis
            Scale xScale = zedGraphControl1.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                xScale.Max = time + xScale.MajorStep;
                xScale.Min = xScale.Max - Chart_Window_Lenght;                     // -30
            }

            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl1.AxisChange();

            // Force a redraw
            zedGraphControl1.Invalidate();

            //
            zedGraphControl1.Refresh();


            ///??
            //myPane.XAxis.Scale.MinAuto = true;
            //myPane.XAxis.Scale.MaxAuto = true;
            //myPane.YAxis.Scale.MinAuto = true;
            //myPane.YAxis.Scale.MaxAuto = true;
            //myPane.XAxis.Scale.MagAuto = true;
            //myPane.YAxis.Scale.MagAuto = true;

        }

        public void UpdateGraph_ByValue(double time)
        {

            // Keep the X scale at a rolling 30 second interval, with one    xScale.MajorStep
            // major step between the max X value and the end of the axis
            Scale xScale = zedGraphControl1.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                xScale.Max = time + xScale.MajorStep;
                xScale.Min = xScale.Max - Chart_Window_Lenght;                     // -30
            }

            // Make sure the Y axis is rescaled to accommodate actual data
            zedGraphControl1.AxisChange();

            // Force a redraw
            zedGraphControl1.Invalidate();

            //
            zedGraphControl1.Refresh();


            ///??
            //myPane.XAxis.Scale.MinAuto = true;
            //myPane.XAxis.Scale.MaxAuto = true;
            //myPane.YAxis.Scale.MinAuto = true;
            //myPane.YAxis.Scale.MaxAuto = true;
            //myPane.XAxis.Scale.MagAuto = true;
            //myPane.YAxis.Scale.MagAuto = true;

        }

        public void UpdateGraph_Update_DisplayOrder()
        {
            myPane.CurveList.Sort(new CurveItemTagComparer());
        }

        #endregion


        #region Read/Set curve ID

        public void CurveSetID(string Text)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            for (int i = 0; i < myPane.CurveList.Count; i++)
            {
                if (myPane.CurveList[i].Label.Text == Text)
                {
                    signal_ID[i] = Text;
                }
            }
        }

        public int CurveGetID(string Text)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            for (int i = 0; i < myPane.CurveList.Count; i++)
            {
                if (myPane.CurveList[i].Label.Text == Text)
                {
                    return i;
                }
            }

            return -1;          // False
        }

        public string CurveGetName(int CurveID)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            return myPane.CurveList[CurveID].Label.Text;
        }

        #endregion


        #region Read/Set Line ID

        public void LineSetID(string Text)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            for (int i = 0; i < myPane.CurveList.Count; i++)
            {
                if (myPane.CurveList[i].Label.Text == Text)
                {
                    signal_ID[i] = Text;
                    //
                    Line_ID_Count++;                // ID zero = none
                    Line_ID[Line_ID_Count] = i;
                }
            }
        }

        public int LineGetID(string Text)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            for (int i = 0; i < myPane.CurveList.Count; i++)
            {
                if (myPane.CurveList[i].Label.Text == Text)
                {
                    return i;
                }
            }

            return -1;          // False
        }

        public string LineGetName(int CurveID)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            return myPane.CurveList[CurveID].Label.Text;
        }

        #endregion




        
        #region Lines

        public void Add_Line(string Name,double X1,double Y1, double X2, double Y2, Color SignalColor, float LineThicknes, SymbolType symbol,bool Steady)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            LineItem line = new LineItem(Name, new[] { X1, X2 },
                            new[] { Y1, Y2 },
                                    SignalColor,
                                    symbol);

         //   line.Line.Style = System.Drawing.Drawing2D.DashStyle.Dot;
            line.Line.Width = LineThicknes;

            myPane.CurveList.Add(line);


            if (Steady == true)
            {
                LineSetID(Name);
            }
            else
            {
                CurveSetID(Name);
            }
        }

        public void Add_Line(double X1, double Y1, double X2, double Y2, Color SignalColor, float LineThicknes, SymbolType symbol)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            LineItem line = new LineItem(Name, new[] { X1, X2 },
                            new[] { Y1, Y2 },
                                    SignalColor,
                                    symbol);

            //   line.Line.Style = System.Drawing.Drawing2D.DashStyle.Dot;
            line.Line.Width = LineThicknes;

            myPane.CurveList.Add(line);

        }

        public void Add_Line( double Y1, double Y2, Color SignalColor, float LineThicknes, SymbolType symbol)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            LineItem line = new LineItem(Name, new[] { myPane.XAxis.Scale.Min, myPane.XAxis.Scale.Max },
                            new[] { Y1, Y2 },
                                    SignalColor,
                                    symbol);

            line.Line.Style = System.Drawing.Drawing2D.DashStyle.Dot;
            line.Line.Width = LineThicknes;

            myPane.CurveList.Add(line);

        }

        public void Update_Steady_Lines_Pos(int ID, double Xvalue)
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            try
            {
                ((IPointListEdit)myPane.CurveList[ID].Points).Clear();
                ((IPointListEdit)myPane.CurveList[ID].Points).Add(Xvalue, myPane.YAxis.Scale.Max);
                ((IPointListEdit)myPane.CurveList[ID].Points).Add(Xvalue, myPane.YAxis.Scale.Min);
            }
            catch (Exception)
            {
                // Need to make a new line
                Add_Line(Marker_Data[ID].Name, Xvalue, myPane.YAxis.Scale.Max, Xvalue, myPane.YAxis.Scale.Min, Marker_Data[ID]._color, 1, SymbolType.None, true);
                // Get ID
                Marker_Data[ID].LineID = LineGetID(Marker_Data[ID].Name);               
            }

        }

        public void Update_Steady_Lines()
        {
            GraphPane myPane = zedGraphControl1.GraphPane;

            for (int i = 1; i < Line_ID_Count; i++)
            {
                //Line_ID[i];
                ((IPointListEdit)myPane.CurveList[Line_ID[i]].Points).Clear();
                ((IPointListEdit)myPane.CurveList[Line_ID[i]].Points).Add(myPane.XAxis.Scale.Min, 2);
                ((IPointListEdit)myPane.CurveList[Line_ID[i]].Points).Add(myPane.XAxis.Scale.Max, 2);
            }

        }

        #endregion


        #region Edit current line

        public void Change_Signal_Width(int ID_number, float width)
        {
            ((LineItem)zedGraphControl1.GraphPane.CurveList[ID_number]).Line.Width = width;
        }
        public void Change_Signal_Width(string ID, float width)
        {
            ((LineItem)zedGraphControl1.GraphPane.CurveList[ID]).Line.Width = width;
        }
        #endregion 


        #region SymbolType functions

        /// <summary>
        /// The <see cref="SymbolType"/>s <see cref="ColorSymbolRotator"/> 
        /// rotates through.
        /// </summary>
        public static readonly SymbolType[] SYMBOLS = new SymbolType[]
        {
          SymbolType.None,
          SymbolType.Square,
          SymbolType.Diamond,
          SymbolType.Triangle,
          SymbolType.Circle,
          SymbolType.XCross,
          SymbolType.Plus,
          SymbolType.Star,
          SymbolType.TriangleDown,
          SymbolType.HDash,
          SymbolType.VDash
        };

        public int ParseSymbolName(string SymbolName)
        {

            try
            {
                switch (SymbolName)
                {
                    case ("None"):
                        {
                            return 0;
                        }
                    case ("Square"):
                        {
                            return 1;
                        }
                    case ("Diamond"):
                        {
                            return 2;
                        }
                    case ("Triangle"):
                        {
                            return 3;
                        }
                    case ("Circle"):
                        {
                            return 4;
                        }
                    case ("XCross"):
                        {
                            return 5;
                        }
                    case ("Plus"):
                        {
                            return 6;
                        }
                    case ("Star"):
                        {
                            return 7;
                        }
                    case ("TriangleDown"):
                        {
                            return 8;
                        }
                    case ("HDash"):
                        {
                            return 9;
                        }
                    case ("VDash"):
                        {
                            return 10;
                        }
                    default:
                        {
                            return 0;
                        }

                }
            }
            catch
            {
                return 0;
            }

        }

        public string GetSymbolName(int ID)
        {
            if (ID == 0)
            {
                return "None";
            }
            else if (ID == 1)
            {
                return "Square";
            }
            else if (ID == 2)
            {
                return "Diamond";
            }
            else if (ID == 3)
            {
                return "Triangle";
            }
            else if (ID == 4)
            {
                return "Circle";
            }
            else if (ID == 5)
            {
                return "XCross";
            }
            else if (ID == 6)
            {
                return "Plus";
            }
            else if (ID == 7)
            {
                return "Star";
            }
            else if (ID == 8)
            {
                return "TriangleDown";
            }
            else if (ID == 9)
            {
                return "HDash";
            }
            else if (ID == 10)
            {
                return "VDash";
            }
            else
            {
                return "None";
            }

        }

        #endregion



        public void SetMarkerLine_Init()
        {
            Marker_Data[0].Name = "Start";
            Marker_Data[0]._color = Color.Green;
            Marker_Data[0].LineID = 99;

            Marker_Data[1].Name = "End";
            Marker_Data[1]._color = Color.Blue;
            Marker_Data[1].LineID = 99;

            Marker_Data[2].Name = "Point1";
            Marker_Data[2]._color = Color.Pink;
            Marker_Data[2].LineID = 99;

            Marker_Data[3].Name = "Point2";
            Marker_Data[3]._color = Color.Purple;
            Marker_Data[3].LineID = 99;
        }

        public void MarkerLinesEnabled(bool IsEnabled)
        {
            Maker_Data_Enable = IsEnabled;
        }

        public void SetMarkerLine(int ID)
        {
            Maker_Data_ID = ID;
            Maker_Data_Enable = true;
        }

        public double GetMarkerLinePos(int ID)
        {
            return Marker_Data[ID].Xpos;
        }

        public DateTime GetMarkerLinePos_DateTime(int ID)
        {
            XDate the_date = new XDate(Marker_Data[ID].Xpos);

            return the_date.DateTime;
        }

        public void SetMarkerLinePos(int ID, double Xpos)
        {
            if (myPane != null)
            {
                Marker_Data[ID].Xpos = Xpos;

                // Check if this line has been set, returns -1 if name is not found
                if (LineGetID(Marker_Data[ID].Name) != -1)
                {
                    // Get ID, be sure we have the right id
                    Marker_Data[ID].LineID = LineGetID(Marker_Data[ID].Name);
                    // only need to update line
                    Update_Steady_Lines_Pos(Marker_Data[ID].LineID, Xpos);
                }
                else
                {
                    // Need to make a new line
                    Add_Line(Marker_Data[ID].Name, Xpos, myPane.YAxis.Scale.Max, Xpos, myPane.YAxis.Scale.Min, Marker_Data[ID]._color, 1, SymbolType.None, true);
                    // Get ID
                    Marker_Data[ID].LineID = LineGetID(Marker_Data[ID].Name);
                }

                zedGraphControl1.Refresh();
            }
        }

        public void SetMarkerLinePos2(int ID, double Xpos)
        {
            if (myPane != null)
            {

                Marker_Data[ID].Xpos = Xpos;

                // Clear the previous values if any
                //myPane.Legend.IsVisible = false;

                // Check if this line has been set, returns -1 if name is not found
                if (LineGetID(Marker_Data[ID].Name) != -1)
                {
                    // Get ID, be sure we have the right id
                    Marker_Data[ID].LineID = LineGetID(Marker_Data[ID].Name);
                    // only need to update line
                    Update_Steady_Lines_Pos(Marker_Data[ID].LineID, Xpos);
                }
                else
                {
                    // Need to make a new line
                    Add_Line(Marker_Data[ID].Name, Xpos, myPane.YAxis.Scale.Max, Xpos, myPane.YAxis.Scale.Min, Marker_Data[ID]._color, 1, SymbolType.None, true);
                    // Get ID
                    Marker_Data[ID].LineID = LineGetID(Marker_Data[ID].Name);
                }

                zedGraphControl1.Refresh();
            }
        }
    





        MarkerData[] Marker_Data = new MarkerData[5];
        int Maker_Data_ID = 0;
        bool Maker_Data_Enable = false;

        private void zedGraphControl1_MouseClick(object sender, MouseEventArgs e)
        {

            if ((myPane != null) && (Maker_Data_Enable == true))
            {
                // x & y variables to store the axis values
                double xVal;
                double yVal;

                // Use the current mouse locations to get the corresponding 
                // X & Y CO-Ordinates
                myPane.ReverseTransform(e.Location, out xVal, out yVal);

                Marker_Data[Maker_Data_ID].Xpos = xVal;

                // Clear the previous values if any
                //myPane.Legend.IsVisible = false;

                // Check if this line has been set, returns -1 if name is not found
                if (LineGetID(Marker_Data[Maker_Data_ID].Name) != -1)
                {
                    // Get ID, be sure we have the right id
                    Marker_Data[Maker_Data_ID].LineID = LineGetID(Marker_Data[Maker_Data_ID].Name);
                    // only need to update line
                    Update_Steady_Lines_Pos(Marker_Data[Maker_Data_ID].LineID, xVal);
                }
                else
                {
                    // Need to make a new line
                    Add_Line(Marker_Data[Maker_Data_ID].Name, xVal, myPane.YAxis.Scale.Max, xVal, myPane.YAxis.Scale.Min, Marker_Data[Maker_Data_ID]._color, 2, SymbolType.None, true);
                    // Get ID
                    Marker_Data[Maker_Data_ID].LineID = LineGetID(Marker_Data[Maker_Data_ID].Name);
                }

                zedGraphControl1.Refresh();
                // report back
                SetMarkerProcess();
            }

            Maker_Data_Enable = false;
        }



        private void Graph_Comp_Resize(object sender, EventArgs e)
        {
            double Hight = this.Size.Height;
            double With = this.Size.Width;

            zedGraphControl1.Height = (int)Hight - 10;
            zedGraphControl1.Width = (int)With - 10;


            // this.Size = new System.Drawing.Size(775, 434);
        }



    }

    public class CurveItemTagComparer : IComparer<CurveItem>
    {
        public int Compare(CurveItem x, CurveItem y)
        {
            return ((int)x.Tag).CompareTo((int)y.Tag);
        }
    }

    // Marker Class
    public struct MarkerData
    {
        public string Name;
        public int LineID;
        public double Xpos;
        public double Ypos;
        public Color _color;
    }
}
