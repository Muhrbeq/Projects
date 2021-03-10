using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace MySchool.Models
{
    public class Student
    {
        public int StudentID { set; get; }
        public string Code { set; get; }
        [Display(Name = "Fullständigt namn")]
        [Required(ErrorMessage = "Du måste fylla i ditt namn")]
        public string Name { set; get; }
        public string EnrollmentNo { set; get; }
    }
}
