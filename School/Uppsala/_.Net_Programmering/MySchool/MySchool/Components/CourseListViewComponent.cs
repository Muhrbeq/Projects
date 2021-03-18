using Microsoft.AspNetCore.Mvc;
using MySchool.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MySchool.Components
{
    public class CourseListViewComponent : ViewComponent
    {
        private ISchoolRepository repository;

        public CourseListViewComponent(ISchoolRepository repo)
        {
            repository = repo;
        }

        public IViewComponentResult Invoke()
        {
            return View("CourseList", repository.Courses);
        }
    }
}
