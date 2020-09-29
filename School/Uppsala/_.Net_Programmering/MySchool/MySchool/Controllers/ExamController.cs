using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace MySchool.Controllers
{
    public class ExamController : Controller
    {
        // GET: /<controller>/
        public ViewResult ExamOne()
        {
            ViewBag.Title = "Exam1 - MySchool";
            return View();
        }
        public ViewResult ExamTwo()
        {
            ViewBag.Title = "Exam2 - MySchool";
            return View();
        }
        public ViewResult ExamThree()
        {
            ViewBag.Title = "Exam3 - MySchool";
            return View();
        }
    }
}
