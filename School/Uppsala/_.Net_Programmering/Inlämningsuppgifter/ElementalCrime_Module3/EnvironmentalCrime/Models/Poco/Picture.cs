using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Models
{
    public class Picture
    {
        int PictureId { get; set; }
        string PictureName { get; set; }
        int ErrandId { get; set; }
    }
}
