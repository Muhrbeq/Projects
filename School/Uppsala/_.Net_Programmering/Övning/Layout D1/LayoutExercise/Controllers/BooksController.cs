using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace LayoutExercise.Controllers
{
    public class BooksController : Controller
    {
        // GET: /<controller>/
        public ViewResult Bocker()
        {
            ViewBag.Title = "Böcker: Knottens Bokförlag";
            return View();
        }

        public ViewResult Amfibie()
        {
            ViewBag.Title = "Amfibie: Knottens Bokförlag";
            return View();
        }

        public ViewResult Krokodil()
        {
            ViewBag.Title = "Krokodil: Knottens Bokförlag";
            return View();
        }

        public ViewResult Odlan()
        {
            ViewBag.Title = "Odlan: Knottens Bokförlag";
            return View();
        }

        public ViewResult Titta()
        {
            ViewBag.Title = "Titta: Knottens Bokförlag";
            return View();
        }
    }
}
