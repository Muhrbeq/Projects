using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Models
{
    public class LoginModel
    {
        [Required(ErrorMessage = "Vänligen fyll i användarnamn.")]
        [Display(Name = "Användarnamn")]
        public string UserName { get; set; }
        [Required(ErrorMessage = "Vänligen fyll i lösenord.")]
        [Display(Name = "Lösenord:")]
        [UIHint("password")]
        public string Password { get; set; }
        public string ReturnURL { get; set; }

    }
}
