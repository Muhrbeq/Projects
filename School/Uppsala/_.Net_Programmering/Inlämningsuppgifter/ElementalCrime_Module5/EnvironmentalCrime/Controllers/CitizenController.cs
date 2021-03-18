using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Mvc;
using EnvironmentalCrime.Infrastructure;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    public class CitizenController : Controller
    {
        private IEnvironmentalCrime repository;

        public CitizenController(IEnvironmentalCrime repo)
        {
            repository = repo;
        }

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

        public ViewResult Thanks(Errand errand)
        {
            ViewBag.Title = "Citizen Thanks";

            //Save ref number to viewbag to access it by getting the session for newErrand
            ViewBag.RefNumber = repository.SaveErrand(HttpContext.Session.GetJson<Errand>("NewErrand"));

            //Remove session "newErrand"
            HttpContext.Session.Remove("NewErrand");

            return View();
        }

        [HttpPost]
        public ViewResult Validate(Errand errand)
        {
            ViewBag.Title = "Citizen Validate";

            //Create session for newErrand
            HttpContext.Session.SetJson("NewErrand", errand);

            return View(errand);
        }
    }
}
