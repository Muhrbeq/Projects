﻿using EnvironmentalCrime.Models;
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

        public async Task<IViewComponentResult> InvokeAsync(string errand)
        {
            var errandDetails = await repository.GetErrandDetails(errand);

            return View(errandDetails);
        }
    }
}
