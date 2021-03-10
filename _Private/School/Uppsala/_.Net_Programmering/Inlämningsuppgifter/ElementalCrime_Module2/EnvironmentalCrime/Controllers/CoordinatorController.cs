using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Mvc;

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
        public ViewResult CrimeCoordinator(string id)
        {
            ViewBag.Title = "Coordinator CrimeCoordinator";
            ViewBag.ID = id;
            return View(repository.Departments);
        }

        public ViewResult ReportCrime()
        {
            ViewBag.Title = "Coordinator ReportCrime";
            return View();
        }

        public ViewResult StartCoordinator()
        {
            ViewBag.Title = "Coordinator StartCoordinator";
            return View(repository);
        }

        public ViewResult Thanks()
        {
            ViewBag.Title = "Coordinator Thanks";
            return View();
        }

        [HttpPost]
        public ViewResult Validate(Errand errand)
        {
            ViewBag.Title = "Coordinator Validate";
            return View(errand);
        }
    }
}
