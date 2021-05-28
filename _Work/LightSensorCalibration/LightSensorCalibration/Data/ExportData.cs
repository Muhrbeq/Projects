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
using System.Windows;

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
                            DataRow += "Temperature" + Delimiter;
                            DataRow += "Humidity" + Delimiter;
                            
                            DataRow += Delimiter;
                            DataRow += Delimiter;

                            DataRow += "Reference LightSensor" + Delimiter;
                            //DataRow += _ref.SensorName + Delimiter + _ref.SerialNumber + Delimiter;
                            DataRow += "Calibration LightSensor" + Delimiter;
                            //DataRow += _cal.SensorName + Delimiter + _cal.SerialNumber + Delimiter;

                            swr.WriteLine(DataRow);

                            DataRow = Delimiter + Delimiter + Delimiter + Delimiter + Delimiter + Delimiter + "Name" + Delimiter;
                            DataRow += _ref.SensorName + Delimiter;
                            DataRow += _cal.SensorName + Delimiter;
                            swr.WriteLine(DataRow);

                            DataRow = Delimiter + Delimiter + Delimiter + Delimiter + Delimiter + Delimiter + "Serial number" + Delimiter;
                            DataRow += _ref.SerialNumber + Delimiter;
                            DataRow += _cal.SerialNumber + Delimiter;
                            swr.WriteLine(DataRow);

                            foreach (SensorData sd in _sd)
                            {
                                DataRow = sd.SetLight.ToString("F1", nfi) + Delimiter;
                                DataRow += sd.ReferenceLight.ToString("F1", nfi) + Delimiter;
                                DataRow += sd.CalibrationLight.ToString("F1", nfi) + Delimiter;
                                DataRow += sd.Temperature.ToString("F2", nfi) + Delimiter;
                                DataRow += sd.Humidity.ToString("F2", nfi) + Delimiter;

                                swr.WriteLine(DataRow);
                            }

                            sw.Stop();

                            MessageBox.Show("Export completed, elapsed time: " + sw.ElapsedMilliseconds + "ms");

                            return true;

                        }
                    }
                    throw new Exception("No file selected");
                }
                throw new Exception("Empty collection");

            }
            catch (InvalidOperationException ex)
            {
                MessageBox.Show(ex.Message);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
            }

            return false;
        }
    }
}
