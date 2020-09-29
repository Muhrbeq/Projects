using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace NestedLayouts.Controllers
{
  public class HomeController : Controller
  {
    public ViewResult Index()
    {
      return View();
    }

    public ViewResult Automation()
    {
      return View();
    }

    public ViewResult Electronics()
    {
      return View();
    }
  }
}
