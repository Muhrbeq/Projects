using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using MySchool.Models;

namespace MySchool.Controllers
{
    public class HomeController : Controller
    {
        private ISchoolRepository repository;

        public HomeController(ISchoolRepository repo)
        {
            repository = repo;
        }

        public ViewResult Index()
        {
            ViewBag.Title = "Home - MySchool";
            return View();
        }

        [HttpPost]
        public ViewResult Index(Student student)
        {
            ViewBag.Name = $"Välkommen {student.Name}";
            return View("NewStudent");
        }

        public ViewResult Courses()
        {
            ViewBag.Title = "Courses - MySchool";
            return View(repository.Courses);
        }

        public ViewResult Exam()
        {
            ViewBag.Title = "Exam - MySchool";
            return View();
        }

        public ViewResult Roster()
        {
            ViewBag.Title = "Roster - MySchool";
            return View(repository);
        }

        public ViewResult ShowTeacherInfo(int id)
        {
            ViewBag.ID = id;
            return View();
        }

        public ViewResult ShowStudentInfo(int id)
        {
            var studentDetail = repository.GetStudentDetail(id);
            return View(studentDetail);
        }

        public ViewResult NewStudent(string fullname)
        {
            ViewBag.Name = fullname;
            return View();
        }
    }
}
