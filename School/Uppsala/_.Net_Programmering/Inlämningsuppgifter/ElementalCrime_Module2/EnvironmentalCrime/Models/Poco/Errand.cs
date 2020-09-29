using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Models
{
    public class Errand
    {
        public string ErrandID { get; set; }
        [Display(Name = "Var har brottet skett någonstans ?")]
        public string Place { get; set; }
        [Display(Name = "Vilken typ av brott?")]
        public string TypeOfCrime { get; set; }
        [Display(Name = "När skedde brottet?")]
        public DateTime DateOfObservation { get; set; }
        public string Observation { get; set; }
        public string InvestigatorInfo { get; set; }
        public string InvestigatorAction { get; set; }
        [Display(Name = "Ditt namn (för- och efternamn):")]
        public string InformerName { get; set; }
        [Display(Name = "Din telefon:")]
        public string InformerPhone { get; set; }
        public string StatusID { get; set; }
        public string DepartmentID { get; set; }
        public string EmployeeID { get; set; }

    }
}
