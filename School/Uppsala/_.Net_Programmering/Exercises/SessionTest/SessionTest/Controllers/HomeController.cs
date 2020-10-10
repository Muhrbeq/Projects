using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using SessionTest.Infrastructure;
using SessionTest.Models;

namespace SessionTest.Controllers
{
  public class HomeController : Controller
  {
    public ViewResult Index()
    {
      var myCustomer = HttpContext.Session.GetJson<Customer>("NewCustomer");
      if(myCustomer == null)
      {
        return View();
      }
      else
      {
        return View(myCustomer);
      }
      
    }

    [HttpPost]
    public ViewResult ValidateCustomer (Customer cust)
    {
      HttpContext.Session.SetJson("NewCustomer", cust);
      return View(cust);
    }

    public ViewResult SavedCustomer()
    {
      //Hämta ut från sessionen för att spara i databasen
      HttpContext.Session.Remove("NewCustomer");
      return View();
    }
  }
}
