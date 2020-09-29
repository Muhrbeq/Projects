using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace MemberForm.Models
{
    public class Member
    {
        [Display(Name = "Fullständigt namn")]
        [Required(ErrorMessage = "Du måste fylla i ditt namn")]
        public string Name { get; set; }
        [Required(ErrorMessage = "Du måste fylla i din mail")]
        public string Email { get; set; }
        [Display(Name = "Mobilnummer")]
        [RegularExpression(pattern:@"^[0]{1}[0-9]{1,3}-[0-9]{5,9}$", ErrorMessage = "Formatet för mobilnummber ska vara xxxx-xxxxxxxxx")]
        public string Phone { get; set; }
        [Display(Name = "Berätta om dig själv")]
        public string Story { get; set; }
        public bool News { get; set; }
    }
}
