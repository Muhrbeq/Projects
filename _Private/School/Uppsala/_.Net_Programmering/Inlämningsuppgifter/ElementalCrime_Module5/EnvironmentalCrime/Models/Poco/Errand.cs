using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;


namespace EnvironmentalCrime.Models
{
    public class Errand
    {
        public int ErrandId { get; set; }
        public string RefNumber { get; set; }
        [Display(Name = "Var har brottet skett någonstans ?")]
        [Required(ErrorMessage = "Du har inte fyllt i var brottet skett")]
        public string Place { get; set; }
        [Display(Name = "Vilken typ av brott?")]
        [Required(ErrorMessage = "Fyll i typ av brott")]
        public string TypeOfCrime { get; set; }
        [Display(Name = "När skedde brottet?")]
        [Required(ErrorMessage = "Fyll i när brottet hände")]
        [DataType(DataType.Date)]
        [DisplayFormat(DataFormatString = "{0:yyyy-MM-dd}")]
        public DateTime DateOfObservation { get; set; }
        [Display(Name = "Beskriv din observation (ex. namn på misstänkt person):")]
        public string Observation { get; set; }
        public string InvestigatorInfo { get; set; }
        public string InvestigatorAction { get; set; }
        [Display(Name = "Ditt namn (för- och efternamn):")]
        [Required(ErrorMessage = "Fyll i ditt namn")]
        public string InformerName { get; set; }
        [Display(Name = "Ditt telefonnummer:")]
        [Required(ErrorMessage = "Fyll i telefonnummer")]
        [RegularExpressionAttribute(pattern: @"^[0]{1}[0-9]{1,3}-[0-9]{5,9}$", ErrorMessage = "Formatet för mobilnummber ska vara xxxx-xxxxxxxxx")]
        public string InformerPhone { get; set; }
        public string StatusId { get; set; }
        public string DepartmentId { get; set; }
        public string EmployeeId { get; set; }
        public ICollection<Sample> Samples { get; set; }
        public ICollection<Picture> Pictures { get; set; }
    }
}
