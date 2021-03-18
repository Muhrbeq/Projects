using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    public class InvestigatorController : Controller
    {
        private IEnvironmentalCrime repository;

        private IWebHostEnvironment environment;

        public InvestigatorController(IEnvironmentalCrime repo, IWebHostEnvironment env)
        {
            repository = repo;
            environment = env;
        }

        // GET: /<controller>/
        public ViewResult CrimeInvestigator(int id)
        {
            //Save id to ViewBag to access it in view
            ViewBag.ID = id;
            TempData["InvestigatorID"] = id;
            ViewBag.ListOfErrandStatus = repository.ErrandStatuses;
            ViewBag.Title = "Investigator CrimeInvestigator";

            return View();
        }

        public ViewResult StartInvestigator()
        {
            ViewBag.Title = "Investigator CrimeInvestigator";

            /* Assign role that goes into ViewComponent*/
            ViewBag.Role = "Investigator";

            return View(repository);
        }

        /*  */
        public async Task<IActionResult> UpdateStatusInvestigator(Errand errand, IFormFile loadSample, IFormFile loadImage)
        {
            /* Get investigator ID and errand detail */
            int someID = int.Parse(TempData["InvestigatorID"].ToString());
            var errandDetail = repository.Errands.Where(ed => ed.ErrandId == someID).First();

            if(errand.InvestigatorAction != null)
            {
                errandDetail.InvestigatorAction += errand.InvestigatorAction;
            }
            if(errand.InvestigatorInfo != null)
            {
                errandDetail.InvestigatorInfo += errand.InvestigatorInfo;
            }

            if(errand.StatusId != "Välj")
            {
                errandDetail.StatusId = errand.StatusId;
            }

            /* Get temp file name */
            var tempSample = Path.GetTempFileName();
            string uniqueFileName = Guid.NewGuid().ToString() + "_";

            if (loadSample != null)
            {
                if (loadSample.Length > 0)
                {
                    using (var stream = new FileStream(tempSample, FileMode.Create))
                    {
                        await loadSample.CopyToAsync(stream);
                    }

                    string uniqueSample = uniqueFileName + loadSample.FileName;

                    //Create path
                    var path = Path.Combine(environment.WebRootPath, "Samples", uniqueSample);

                    //Move temp to correct place
                    System.IO.File.Move(tempSample, path);

                    Sample sample = new Sample();
                    sample.ErrandId = errandDetail.ErrandId;
                    sample.SampleName = uniqueSample;
                    errandDetail.Samples.Add(sample);
                }
            }

            var tempImage = Path.GetTempFileName();
            if (loadImage != null)
            {
                if (loadImage.Length > 0)
                {
                    using (var stream = new FileStream(tempImage, FileMode.Create))
                    {
                        await loadImage.CopyToAsync(stream);
                    }

                    string uniqueImage = uniqueFileName + loadImage.FileName;

                    //Skapa ny sökväg
                    var path = Path.Combine(environment.WebRootPath, "Pictures", uniqueImage);

                    //Flytta den temp filen rätt
                    System.IO.File.Move(tempImage, path);

                    Picture pic = new Picture();
                    pic.ErrandId = errandDetail.ErrandId;
                    pic.PictureName = uniqueImage;
                    errandDetail.Pictures.Add(pic);
                }
            }
            
            /* Save errand */
            repository.SaveErrand(errandDetail);

            return RedirectToAction("CrimeInvestigator", new { id = someID });
        }
    }
}
