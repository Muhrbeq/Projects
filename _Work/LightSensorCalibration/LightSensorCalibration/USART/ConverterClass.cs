using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Converter_Class
{
    public class ConverterClass
    {

        #region Check

        public static bool CheckHEXstring(string Data)
        {
            foreach (char CharItem in Data)
            {
                bool is_hex_char = (CharItem >= '0' && CharItem <= '9') ||
                                   (CharItem >= 'a' && CharItem <= 'f') ||
                                   (CharItem >= 'A' && CharItem <= 'F');

                if (!is_hex_char)
                {
                    return false;
                }
            }

            return true;
        }

        public static int GetFirstNonHexIndex(string Data)
        {
            int index = 0;
            foreach (char CharItem in Data)
            {
                bool is_hex_char = (CharItem >= '0' && CharItem <= '9') ||
                                   (CharItem >= 'a' && CharItem <= 'f') ||
                                   (CharItem >= 'A' && CharItem <= 'F');

                if (!is_hex_char)
                {
                    return index;
                }
                index++;
            }

            return -1; // If only hex in string,
        }


        #endregion

        #region Hex to Value

        public static bool Hex2_Boolean(string Data)
        {
            if (Data == "1")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public static byte Hex2_uChar8(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToByte(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static sbyte Hex2_Char8(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToSByte(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static uint Hex2_uInt16(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToUInt16(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static int Hex2_Int16(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToInt16(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static uint Hex2_uInt32(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToUInt32(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static int Hex2_Int32(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToInt32(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static ulong Hex2_uInt64(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToUInt64(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static long Hex2_Int64(string Data)
        {
            if (CheckHEXstring(Data) == true)
            {
                return Convert.ToInt64(Data, 16);
            }
            else
            {
                return 0;
            }
        }

        public static float Hex2_Float(string hexString, bool IsLittleEndian = true)
        {
            uint num = uint.Parse(hexString, System.Globalization.NumberStyles.AllowHexSpecifier);

            byte[] floatVals = BitConverter.GetBytes(num);
            if (IsLittleEndian == true)
            {
                floatVals = floatVals.Reverse().ToArray();
            }

            return BitConverter.ToSingle(floatVals, 0);

        }

        #endregion

        #region value to HEX String

        public static string Bool_2Hex(bool Data)
        {
            if (Data == true)
            {
                return "1";
            }
            else
            {
                return "0";
            }
        }

        public static string uChar8_2Hex(byte Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X2");
            }
        }

        public static string Char8_2Hex(sbyte Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X2");
            }
        }

        public static string uInt16_2Hex(uint Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X4");
            }
        }

        public static string Int16_2Hex(int Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X4");
            }
        }

        public static string uInt32_2Hex(uint Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X8");
            }
        }

        public static string Int32_2Hex(int Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X8");
            }
        }

        public static string uInt64_2Hex(ulong Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X16");
            }
        }

        public static string Int64_2Hex(long Data, bool FixedSize = false)
        {
            if (FixedSize == false)
            {
                return Data.ToString("X");
            }
            else
            {
                return Data.ToString("X16");
            }
        }

        public static string Float_2Hex(float Data, bool IsLittleEndian = true)
        {
            byte[] convertToBytes = new byte[32];
            string hex = "";
            convertToBytes = BitConverter.GetBytes(Data);

            if (IsLittleEndian == true)
            {
                convertToBytes = convertToBytes.Reverse().ToArray();
            }

            for (int i = (convertToBytes.Count(x => x != 0)) - 1; i >= 0; i--)
            {
                hex = hex + convertToBytes[i].ToString("X2");
            }

            return hex;
        }

        #endregion

    }
}
