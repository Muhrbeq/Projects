using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;
using USART;

namespace PropertyRef
{
    public class FontSizeItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();
            sizes.Add(5.0, "Five");
            sizes.Add(5.5);
            sizes.Add(6.0, "Six");
            sizes.Add(6.5);
            sizes.Add(7.0, "Seven");
            sizes.Add(7.5);
            sizes.Add(8.0, "Eight");
            sizes.Add(8.5);
            sizes.Add(9.0, "Nine");
            sizes.Add(9.5);
            sizes.Add(10.0);
            sizes.Add(12.0, "Twelve");
            sizes.Add(14.0);
            sizes.Add(16.0);
            sizes.Add(18.0);
            sizes.Add(20.0);
            return sizes;
        }
    }

    public class BoudRateItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();
            sizes.Add(2400);
            sizes.Add(9600);
            sizes.Add(19200);
            sizes.Add(38400);
            sizes.Add(57600);
            sizes.Add(115200);
            sizes.Add(230400);
            sizes.Add(460800);
            sizes.Add(921600);
            return sizes;
        }
    }

    public class ParityItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();

            foreach (string str in Enum.GetNames(typeof(Parity)))
            {
                sizes.Add(str);
            }

            return sizes;
        }
    }



    public class StopBitsItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();

            foreach (string str in Enum.GetNames(typeof(StopBits)))
            {
                sizes.Add(str);
            }

            return sizes;
        }
    }


    public class DataBitsItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();
            sizes.Add(5);
            sizes.Add(6);
            sizes.Add(7);
            sizes.Add(8);
            return sizes;
        }
    }




    public class HandshakeItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();

            foreach (string str in Enum.GetNames(typeof(Handshake)))
            {
                sizes.Add(str);
            }

            return sizes;
        }
    }

    public class TypeItemsSource : IItemsSource
    {
        public ItemCollection GetValues()
        {
            ItemCollection sizes = new ItemCollection();
            sizes.Add(0,"Comport");
            sizes.Add(1,"FTDI");
            return sizes;
        }
    }

}
