using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    public class CitizenController : Controller
    {
        // GET: /<controller>/
        public ViewResult Contact()
        {
            ViewBag.Title = "Citizen contact";
            return View();
        }

        public ViewResult Faq()
        {
            ViewBag.Title = "Citizen Faq";
            return View();
        }

        public ViewResult Services()
        {
            ViewBag.Title = "Citizen Services";
            return View();
        }

        public ViewResult Thanks()
        {
            ViewBag.Title = "Citizen Thanks";
            return View();
        }

        public ViewResult Validate()
        {
            ViewBag.Title = "Citizen Validate";
            return View();
        }
    }
}
