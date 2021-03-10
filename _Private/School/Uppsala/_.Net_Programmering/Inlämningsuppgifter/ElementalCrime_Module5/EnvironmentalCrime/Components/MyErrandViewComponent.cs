using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Components
{
    public class MyErrandViewComponent : ViewComponent
    {
        private IEnvironmentalCrime repository;

        public MyErrandViewComponent(IEnvironmentalCrime repo)
        {
            repository = repo;
        }

        public async Task<IViewComponentResult> InvokeAsync(string role)
        {
            ViewBag.ListOfMyErrands = await repository.GetFilteredErrands();

            ViewBag.Role = role;

            return View(repository);
        }
    }
}
