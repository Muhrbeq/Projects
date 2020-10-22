using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using EnvironmentalCrime.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace EnvironmentalCrime.Controllers
{
    [Authorize]
    public class AccountController : Controller
    {
        private UserManager<IdentityUser> userManager;
        private SignInManager<IdentityUser> signInManager;

        public AccountController(UserManager<IdentityUser> userMgr, SignInManager<IdentityUser> signInMgr)
        {
            userManager = userMgr;
            signInManager = signInMgr;
        }

        [AllowAnonymous]
        // GET: /<controller>/
        public ViewResult Login(string returnURL)
        {
            return View(new LoginModel
            {
                ReturnURL = returnURL
            });
        }

        [HttpPost]
        [AllowAnonymous]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Login(LoginModel loginModel)
        {
            IdentityUser user = await userManager.FindByNameAsync(loginModel.UserName);

            if (ModelState.IsValid)
            {
                if(user != null)
                {
                    //Ensure logout
                    await signInManager.SignOutAsync();

                    if((await signInManager.PasswordSignInAsync(user, loginModel.Password, false, false)).Succeeded)
                    {
                        if (await userManager.IsInRoleAsync(user, "Coordinator"))
                        {
                            return Redirect("/Coordinator/StartCoordinator");
                        }
                        if(await userManager.IsInRoleAsync(user, "Investigator"))
                        {
                            return Redirect("/Investigator/StartInvestigator");
                        }
                        if(await userManager.IsInRoleAsync(user, "Manager"))
                        {
                            return Redirect("/Manager/StartManager");
                        }
                    }
                }
            }

            ModelState.AddModelError("", "Felaktigt användarnamn eller lösenord!");

            return View(loginModel);
        }

        public async Task<IActionResult> Logout(string returnURL = "/")
        {
            await signInManager.SignOutAsync();
            return Redirect(returnURL);
        }

        [AllowAnonymous]
        public ViewResult AccessDenied()
        {
            return View();
        }
    }
}
