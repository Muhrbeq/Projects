using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    public class ManagerController : Controller
    {
        private IEnvironmentalCrime repository;

        public ManagerController(IEnvironmentalCrime repo)
        {
            repository = repo;
        }

        // GET: /<controller>/
        public ViewResult CrimeManager(string id)
        {
            ViewBag.Title = "Manager CrimeManager";
            ViewBag.ID = id;
            return View(repository.Employees);
        }

        public ViewResult StartManager()
        {
            ViewBag.Title = "Manager CrimeManager";
            
            return View(repository);
        }
    }
}
