using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace LayoutExercise.Controllers
{
    public class HomeController : Controller
    {
        // GET: /<controller>/
        public ViewResult Index()
        {
            ViewBag.Title = "Knottens Bokförlag";
            return View();
        }

        public ViewResult Kontakt()
        {
            ViewBag.Title = "Kontakt: Knottens Bokförlag";
            return View();
        }

        public ViewResult Press()
        {
            ViewBag.Title = "Press: Knottens Bokförlag";
            return View();
        }
    }
}
