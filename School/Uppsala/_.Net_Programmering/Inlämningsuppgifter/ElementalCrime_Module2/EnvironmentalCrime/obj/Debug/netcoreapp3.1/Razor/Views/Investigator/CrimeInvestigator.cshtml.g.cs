#pragma checksum "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "08762f48911ee91abedac5a4ecb5e3e64dec26a7"
// <auto-generated/>
#pragma warning disable 1591
[assembly: global::Microsoft.AspNetCore.Razor.Hosting.RazorCompiledItemAttribute(typeof(AspNetCore.Views_Investigator_CrimeInvestigator), @"mvc.1.0.view", @"/Views/Investigator/CrimeInvestigator.cshtml")]
namespace AspNetCore
{
    #line hidden
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading.Tasks;
    using Microsoft.AspNetCore.Mvc;
    using Microsoft.AspNetCore.Mvc.Rendering;
    using Microsoft.AspNetCore.Mvc.ViewFeatures;
#nullable restore
#line 1 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\_ViewImports.cshtml"
using EnvironmentalCrime.Models;

#line default
#line hidden
#nullable disable
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"08762f48911ee91abedac5a4ecb5e3e64dec26a7", @"/Views/Investigator/CrimeInvestigator.cshtml")]
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"3fb52e8910a4a8d30f38cb5f01b9856db08bfd19", @"/Views/_ViewImports.cshtml")]
    public class Views_Investigator_CrimeInvestigator : global::Microsoft.AspNetCore.Mvc.Razor.RazorPage<IEnumerable<Department>>
    {
        private static readonly global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute __tagHelperAttribute_0 = new global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute("selected", new global::Microsoft.AspNetCore.Html.HtmlString("selected"), global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
        #line hidden
        #pragma warning disable 0649
        private global::Microsoft.AspNetCore.Razor.Runtime.TagHelpers.TagHelperExecutionContext __tagHelperExecutionContext;
        #pragma warning restore 0649
        private global::Microsoft.AspNetCore.Razor.Runtime.TagHelpers.TagHelperRunner __tagHelperRunner = new global::Microsoft.AspNetCore.Razor.Runtime.TagHelpers.TagHelperRunner();
        #pragma warning disable 0169
        private string __tagHelperStringValueBuffer;
        #pragma warning restore 0169
        private global::Microsoft.AspNetCore.Razor.Runtime.TagHelpers.TagHelperScopeManager __backed__tagHelperScopeManager = null;
        private global::Microsoft.AspNetCore.Razor.Runtime.TagHelpers.TagHelperScopeManager __tagHelperScopeManager
        {
            get
            {
                if (__backed__tagHelperScopeManager == null)
                {
                    __backed__tagHelperScopeManager = new global::Microsoft.AspNetCore.Razor.Runtime.TagHelpers.TagHelperScopeManager(StartTagHelperWritingScope, EndTagHelperWritingScope);
                }
                return __backed__tagHelperScopeManager;
            }
        }
        private global::Microsoft.AspNetCore.Mvc.TagHelpers.OptionTagHelper __Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper;
        #pragma warning disable 1998
        public async override global::System.Threading.Tasks.Task ExecuteAsync()
        {
#nullable restore
#line 1 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml"
  
    Layout = "_InvestigatorLayout";

#line default
#line hidden
#nullable disable
            WriteLiteral("\r\n");
            WriteLiteral("\r\n<h2>Detaljer för ärendet</h2>\r\n<p class=\"info\">Du är inloggad som handläggare</p>\r\n\r\n");
#nullable restore
#line 10 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml"
Write(await Component.InvokeAsync("CrimeList", ViewBag.ID));

#line default
#line hidden
#nullable disable
            WriteLiteral(@"

<section id=""bottomColumn"">
    <h3>Redigera ärende</h3>
    <p class=""label"">Händelser:</p>
    <textarea name=""events"" rows=""5"" cols=""60""></textarea>

    <p class=""label"">Mer information:</p>
    <textarea name=""information"" rows=""5"" cols=""60""></textarea>

    <p class=""label"">Prover:</p>
    <input type=""file"" name=""loadSample"" />

    <p class=""label"">Ladda upp bilder:</p>
    <input type=""file"" name=""loadImage"" />

    <p class=""label"">Ändring av status:</p>
    <select name=""status"">
        ");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("option", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "08762f48911ee91abedac5a4ecb5e3e64dec26a74907", async() => {
                WriteLiteral("Välj");
            }
            );
            __Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper = CreateTagHelper<global::Microsoft.AspNetCore.Mvc.TagHelpers.OptionTagHelper>();
            __tagHelperExecutionContext.Add(__Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper);
            __tagHelperExecutionContext.AddHtmlAttribute(__tagHelperAttribute_0);
            await __tagHelperRunner.RunAsync(__tagHelperExecutionContext);
            if (!__tagHelperExecutionContext.Output.IsContentModified)
            {
                await __tagHelperExecutionContext.SetOutputContentAsync();
            }
            Write(__tagHelperExecutionContext.Output);
            __tagHelperExecutionContext = __tagHelperScopeManager.End();
            WriteLiteral("\r\n");
#nullable restore
#line 29 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml"
         foreach (Department department in Model)
        {

#line default
#line hidden
#nullable disable
            WriteLiteral("            ");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("option", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "08762f48911ee91abedac5a4ecb5e3e64dec26a76323", async() => {
#nullable restore
#line 31 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml"
                                                Write(department.DepartmentName);

#line default
#line hidden
#nullable disable
            }
            );
            __Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper = CreateTagHelper<global::Microsoft.AspNetCore.Mvc.TagHelpers.OptionTagHelper>();
            __tagHelperExecutionContext.Add(__Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper);
            BeginWriteTagHelperAttribute();
#nullable restore
#line 31 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml"
               WriteLiteral(department.DepartmentID);

#line default
#line hidden
#nullable disable
            __tagHelperStringValueBuffer = EndWriteTagHelperAttribute();
            __Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper.Value = __tagHelperStringValueBuffer;
            __tagHelperExecutionContext.AddTagHelperAttribute("value", __Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper.Value, global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
            await __tagHelperRunner.RunAsync(__tagHelperExecutionContext);
            if (!__tagHelperExecutionContext.Output.IsContentModified)
            {
                await __tagHelperExecutionContext.SetOutputContentAsync();
            }
            Write(__tagHelperExecutionContext.Output);
            __tagHelperExecutionContext = __tagHelperScopeManager.End();
            WriteLiteral("\r\n");
#nullable restore
#line 32 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module2\EnvironmentalCrime\Views\Investigator\CrimeInvestigator.cshtml"
        }

#line default
#line hidden
#nullable disable
            WriteLiteral("    </select>\r\n\r\n    <p>\r\n        <input class=\"button\" type=\"submit\" value=\"Spara ändringarna\" />\r\n    </p>\r\n</section>");
        }
        #pragma warning restore 1998
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.ViewFeatures.IModelExpressionProvider ModelExpressionProvider { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.IUrlHelper Url { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.IViewComponentHelper Component { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.Rendering.IJsonHelper Json { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.Rendering.IHtmlHelper<IEnumerable<Department>> Html { get; private set; }
    }
}
#pragma warning restore 1591
