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
    public class HomeController : Controller
    {
        // GET: /<controller>/
        public ViewResult Index()
        {
            ViewBag.Title = "Homepage";

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

        public ViewResult Login()
        {
            ViewBag.Title = "Login";
            return View();
        }
    }
}
