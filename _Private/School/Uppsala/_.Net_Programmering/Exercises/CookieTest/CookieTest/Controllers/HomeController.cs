using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace CookieTest.Controllers
{
  public class HomeController : Controller
  {
    public ViewResult Index()
    {
      return View();
    }

    //Skapa en cookie där setting är antingen fontname, fontsize eller color och settingValue det värde som den settingen har. 
    public ViewResult WriteCookies(string setting, string settingValue, bool isPersistent)
    {
      if (isPersistent)
      {
        CookieOptions options = new CookieOptions();
        options.Expires = DateTime.Now.AddDays(1);
        Response.Cookies.Append(setting, WebUtility.UrlEncode(settingValue), options);
      }
      else
      {
        Response.Cookies.Append(setting, WebUtility.UrlEncode(settingValue));
      }
      ViewBag.Message = "Cookie written successfully!";
      return View("Index");
    }

    //Läsa data från cookien alt. lägga in defaultvärden
    public ViewResult ReadCookies()
    {
      ViewBag.FontName = WebUtility.UrlDecode(Request.Cookies["fontName"]);
      ViewBag.FontSize = WebUtility.UrlDecode(Request.Cookies["fontSize"]);
      ViewBag.Color = WebUtility.UrlDecode(Request.Cookies["color"]);

      if (string.IsNullOrEmpty(ViewBag.FontName))
      {
        ViewBag.FontName = "Arial";
      }
      if (string.IsNullOrEmpty(ViewBag.FontSize))
      {
        ViewBag.FontSize = "22px";
      }
      if (string.IsNullOrEmpty(ViewBag.Color))
      {
        ViewBag.Color = "Blue";
      }
      return View();
    }
  }
}
