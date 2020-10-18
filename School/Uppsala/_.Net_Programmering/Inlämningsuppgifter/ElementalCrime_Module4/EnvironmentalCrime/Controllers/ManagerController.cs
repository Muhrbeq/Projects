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
        public ViewResult CrimeManager(int id)
        {
            ViewBag.Title = "Manager CrimeManager";
            //Save id to ViewBag to access it in view
            ViewBag.ID = id;
            TempData["Manager_ID"] = id;
            ViewBag.ListOfEmployees = repository.Employees;

            return View();
        }

        public ViewResult StartManager()
        {
            ViewBag.Title = "Manager CrimeManager";
            
            return View(repository);
        }

        public IActionResult UpdateErrandManager(bool noAction, Errand errand)
        {
            int someID = int.Parse(TempData["Manager_ID"].ToString());
            var errandDetail = repository.Errands.Where(ed => ed.ErrandId == someID).First();

            if(noAction)
            {
                errandDetail.StatusId = "S_B";
                if (errand.InvestigatorInfo != null)
                {
                    errandDetail.InvestigatorInfo = errand.InvestigatorInfo;
                }
            }
            else if(errand.EmployeeId != "Välj")
            {
                errandDetail.EmployeeId = errand.EmployeeId;

            }

            repository.SaveErrand(errandDetail);

            return RedirectToAction("CrimeManager", new { id = someID });
        }
    }
}
