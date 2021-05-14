using LightSensorCalibration.Instruments;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LightSensorCalibration.Data
{
    public static class ExportData
    {
        public static bool ExportToCSV(LightSensor _ref, LightSensor _cal, PowerSupplyUnit PSU, ObservableCollection<SensorData> _sd)
        {
            string Delimiter = ";";
            string fileName = "";
            bool FileSelected = false;
            string DataRow = "";

            try
            {
                /* Culture specific information for formatting numbers */
                NumberFormatInfo nfi = new NumberFormatInfo();
                nfi.NumberDecimalSeparator = ",";

                Microsoft.Win32.SaveFileDialog dlg = new Microsoft.Win32.SaveFileDialog();
                dlg.FileName = "_File"; // Default file name
                dlg.DefaultExt = ".csv"; // Default file extension
                dlg.Filter = "Text documents (.csv)|*.csv"; // Filter files by extension

                // Show save file dialog box
                Nullable<bool> result = dlg.ShowDialog();

                // Process save file dialog box results
                if (result == true)
                {
                    // Save document
                    fileName = dlg.FileName;
                    FileSelected = true;
                }
                else
                {
                    throw new InvalidOperationException("Need to press OK to save file in file explorer");
                }

                if(_sd.Count > 0)
                {
                    if(FileSelected)
                    {
                        Stopwatch sw = new Stopwatch();
                        sw.Start();

                        using (StreamWriter swr = new StreamWriter(File.Open(fileName, FileMode.Create), Encoding.Default, 1000000))
                        {
                            /* Fill the top row with titles of colomns */
                            DataRow = "Set Lux" + Delimiter;
                            DataRow += "Reference Lux" + Delimiter;
                            DataRow += "Calibration Lux" + Delimiter;
                            
                            DataRow += Delimiter;

                            DataRow += "Reference LightSensor Name" + Delimiter;
                            DataRow += _ref.
                            

                            swr.WriteLine(DataRow);


                        }
                    }
                }

            }
            catch (InvalidOperationException ex)
            {

            }
            finally
            { 
            }

            return false;
        }
    }
}
