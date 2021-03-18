using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    public class InvestigatorController : Controller
    {
        private IEnvironmentalCrime repository;

        public InvestigatorController(IEnvironmentalCrime repo)
        {
            repository = repo;
        }

        // GET: /<controller>/
        public ViewResult CrimeInvestigator(int id)
        {
            //Save id to ViewBag to access it in view
            ViewBag.ID = id;
            ViewBag.Title = "Investigator CrimeInvestigator";
            return View(repository.Departments);
        }

        public ViewResult StartInvestigator()
        {
            ViewBag.Title = "Investigator CrimeInvestigator";
            return View(repository);
        }
    }
}
