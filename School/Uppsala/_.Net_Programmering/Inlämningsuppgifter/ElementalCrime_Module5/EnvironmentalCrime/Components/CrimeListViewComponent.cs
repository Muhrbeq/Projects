using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Components
{
    public class CrimeListViewComponent : ViewComponent
    {
        private IEnvironmentalCrime repository;

        public CrimeListViewComponent(IEnvironmentalCrime repo)
        {
            repository = repo;
        }

        public async Task<IViewComponentResult> InvokeAsync(int errandID)
        {
            var errandDetails = await repository.GetErrandDetails(errandID);

            ViewBag.ListOfPictures = repository.Pictures;
            ViewBag.ListOfSamples = repository.Samples;

            return View(errandDetails);
        }
    }
}
