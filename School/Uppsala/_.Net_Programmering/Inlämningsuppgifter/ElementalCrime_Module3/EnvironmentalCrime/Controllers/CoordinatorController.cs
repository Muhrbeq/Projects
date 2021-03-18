using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Infrastructure;
using Microsoft.AspNetCore.Mvc;
using EnvironmentalCrime.Models;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    public class CoordinatorController : Controller
    {
        private IEnvironmentalCrime repository;

        public CoordinatorController(IEnvironmentalCrime repo)
        {
            repository = repo;
        }

        // GET: /<controller>/
        public ViewResult CrimeCoordinator(int id)
        {
            ViewBag.Title = "Coordinator CrimeCoordinator";

            //Save id to ViewBag to access it in view
            ViewBag.ID = id;
            return View(repository.Departments);
        }

        public ViewResult ReportCrime()
        {
            ViewBag.Title = "Coordinator ReportCrime";

            // Get the saved session
            var myErrand = HttpContext.Session.GetJson<Errand>("NewErrand");

            //if cannot find, return basic view
            if (myErrand == null)
            {
                return View();
            }
            //else return view filled with info
            else
            {
                return View(myErrand);
            }
        }

        public ViewResult StartCoordinator()
        {
            ViewBag.Title = "Coordinator StartCoordinator";
            return View(repository);
        }

        public ViewResult Thanks()
        {
            ViewBag.Title = "Coordinator Thanks";

            //Save ref number to viewbag to access it by getting the session for newErrand
            ViewBag.RefNumber = repository.SaveErrand(HttpContext.Session.GetJson<Errand>("NewErrand"));

            //Remove session "newErrand"
            HttpContext.Session.Remove("NewErrand");

            return View();
        }

        [HttpPost]
        public ViewResult Validate(Errand errand)
        {
            ViewBag.Title = "Coordinator Validate";

            //Create session for newErrand
            HttpContext.Session.SetJson("NewErrand", errand);
            
            return View(errand);
        }
    }
}
