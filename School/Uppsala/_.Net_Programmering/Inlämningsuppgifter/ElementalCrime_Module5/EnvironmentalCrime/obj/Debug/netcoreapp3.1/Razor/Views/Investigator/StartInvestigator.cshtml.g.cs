#pragma checksum "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d9263"
// <auto-generated/>
#pragma warning disable 1591
[assembly: global::Microsoft.AspNetCore.Razor.Hosting.RazorCompiledItemAttribute(typeof(AspNetCore.Views_Investigator_StartInvestigator), @"mvc.1.0.view", @"/Views/Investigator/StartInvestigator.cshtml")]
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
#line 1 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\_ViewImports.cshtml"
using EnvironmentalCrime.Models;

#line default
#line hidden
#nullable disable
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d9263", @"/Views/Investigator/StartInvestigator.cshtml")]
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"3fb52e8910a4a8d30f38cb5f01b9856db08bfd19", @"/Views/_ViewImports.cshtml")]
    public class Views_Investigator_StartInvestigator : global::Microsoft.AspNetCore.Mvc.Razor.RazorPage<IEnvironmentalCrime>
    {
        private static readonly global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute __tagHelperAttribute_0 = new global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute("selected", new global::Microsoft.AspNetCore.Html.HtmlString("selected"), global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
        private static readonly global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute __tagHelperAttribute_1 = new global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute("asp-controller", "Investigator", global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
        private static readonly global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute __tagHelperAttribute_2 = new global::Microsoft.AspNetCore.Razor.TagHelpers.TagHelperAttribute("asp-action", "CrimeInvestigator", global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
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
        private global::Microsoft.AspNetCore.Mvc.TagHelpers.AnchorTagHelper __Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper;
        private global::Microsoft.AspNetCore.Mvc.Razor.TagHelpers.UrlResolutionTagHelper __Microsoft_AspNetCore_Mvc_Razor_TagHelpers_UrlResolutionTagHelper;
        #pragma warning disable 1998
        public async override global::System.Threading.Tasks.Task ExecuteAsync()
        {
#nullable restore
#line 1 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
  
    Layout = "_InvestigatorLayout";

#line default
#line hidden
#nullable disable
            WriteLiteral("\r\n");
            WriteLiteral(@"
<h2>Ärenden</h2>
<p class=""info"">Du är inloggad som handläggare</p>

<table id=""managerForm"">
    <tr>
        <td class=""label"">Välj status:</td>
        <td>&nbsp;</td>
        <td class=""label"">Ärendenummer:</td>
        <td>&nbsp;</td>
    </tr>
    <tr>
        <td>
            <select name=""status"">
                ");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("option", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d92635144", async() => {
                WriteLiteral("Välj alla");
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
#line 21 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                 foreach (ErrandStatus errandStatus in Model.ErrandStatuses)
                {

#line default
#line hidden
#nullable disable
            WriteLiteral("                    ");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("option", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d92636608", async() => {
#nullable restore
#line 23 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                                                      Write(errandStatus.StatusName);

#line default
#line hidden
#nullable disable
            }
            );
            __Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper = CreateTagHelper<global::Microsoft.AspNetCore.Mvc.TagHelpers.OptionTagHelper>();
            __tagHelperExecutionContext.Add(__Microsoft_AspNetCore_Mvc_TagHelpers_OptionTagHelper);
            BeginWriteTagHelperAttribute();
#nullable restore
#line 23 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                       WriteLiteral(errandStatus.StatusId);

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
#line 24 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                }

#line default
#line hidden
#nullable disable
            WriteLiteral(@"            </select>
        </td>
        <td><input class=""button"" type=""submit"" value=""Hämta lista"" /></td>
        <td><input name=""casenumber"" /></td>
        <td><input class=""button"" type=""submit"" value=""Sök"" /></td>
    </tr>
</table>

<!--Nedan ser man en lista på ärenden-->
<table>
    <tr>
        <th>Ärende anmält</th>
        <th>Ärendenummer</th>
        <th>Miljöbrott</th>
        <th>Status</th>
        <th>Avdelning</th>
        <th>Handläggare</th>
        <th>Uppladdade filer</th>
    </tr>
");
#nullable restore
#line 44 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
     foreach (MyErrand errand in ViewBag.ListOfMyErrands)
    {

#line default
#line hidden
#nullable disable
            WriteLiteral("        <tr>\r\n            <td>");
#nullable restore
#line 47 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
           Write(errand.DateOfObservation.ToShortDateString());

#line default
#line hidden
#nullable disable
            WriteLiteral("</td>\r\n            <td>");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("a", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d926310325", async() => {
#nullable restore
#line 48 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                                                                                                           Write(errand.RefNumber);

#line default
#line hidden
#nullable disable
            }
            );
            __Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper = CreateTagHelper<global::Microsoft.AspNetCore.Mvc.TagHelpers.AnchorTagHelper>();
            __tagHelperExecutionContext.Add(__Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper);
            __Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper.Controller = (string)__tagHelperAttribute_1.Value;
            __tagHelperExecutionContext.AddTagHelperAttribute(__tagHelperAttribute_1);
            __Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper.Action = (string)__tagHelperAttribute_2.Value;
            __tagHelperExecutionContext.AddTagHelperAttribute(__tagHelperAttribute_2);
            if (__Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper.RouteValues == null)
            {
                throw new InvalidOperationException(InvalidTagHelperIndexerAssignment("asp-route-id", "Microsoft.AspNetCore.Mvc.TagHelpers.AnchorTagHelper", "RouteValues"));
            }
            BeginWriteTagHelperAttribute();
#nullable restore
#line 48 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                                                                                  WriteLiteral(errand.ErrandId);

#line default
#line hidden
#nullable disable
            __tagHelperStringValueBuffer = EndWriteTagHelperAttribute();
            __Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper.RouteValues["id"] = __tagHelperStringValueBuffer;
            __tagHelperExecutionContext.AddTagHelperAttribute("asp-route-id", __Microsoft_AspNetCore_Mvc_TagHelpers_AnchorTagHelper.RouteValues["id"], global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
            await __tagHelperRunner.RunAsync(__tagHelperExecutionContext);
            if (!__tagHelperExecutionContext.Output.IsContentModified)
            {
                await __tagHelperExecutionContext.SetOutputContentAsync();
            }
            Write(__tagHelperExecutionContext.Output);
            __tagHelperExecutionContext = __tagHelperScopeManager.End();
            WriteLiteral("</td>\r\n            <td>");
#nullable restore
#line 49 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
           Write(errand.TypeOfCrime);

#line default
#line hidden
#nullable disable
            WriteLiteral("</td>\r\n            <td>");
#nullable restore
#line 50 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
           Write(errand.StatusName);

#line default
#line hidden
#nullable disable
            WriteLiteral("</td>\r\n            <td>");
#nullable restore
#line 51 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
           Write(errand.DepartmentName);

#line default
#line hidden
#nullable disable
            WriteLiteral("</td>\r\n            <td>");
#nullable restore
#line 52 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
           Write(errand.EmployeeName);

#line default
#line hidden
#nullable disable
            WriteLiteral("</td>\r\n            <td>\r\n");
#nullable restore
#line 54 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                 foreach (Picture picture in Model.Pictures)
                {
                    

#line default
#line hidden
#nullable disable
#nullable restore
#line 56 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                     if (picture.ErrandId == errand.ErrandId)
                    {

#line default
#line hidden
#nullable disable
            WriteLiteral("                        <p>");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("a", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d926315494", async() => {
                WriteLiteral("picture_");
#nullable restore
#line 58 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                                                                        Write(picture.PictureId);

#line default
#line hidden
#nullable disable
            }
            );
            __Microsoft_AspNetCore_Mvc_Razor_TagHelpers_UrlResolutionTagHelper = CreateTagHelper<global::Microsoft.AspNetCore.Mvc.Razor.TagHelpers.UrlResolutionTagHelper>();
            __tagHelperExecutionContext.Add(__Microsoft_AspNetCore_Mvc_Razor_TagHelpers_UrlResolutionTagHelper);
            BeginAddHtmlAttributeValues(__tagHelperExecutionContext, "href", 2, global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
            AddHtmlAttributeValue("", 1885, "~/Pictures/", 1885, 11, true);
#nullable restore
#line 58 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
AddHtmlAttributeValue("", 1896, picture.PictureName, 1896, 20, false);

#line default
#line hidden
#nullable disable
            EndAddHtmlAttributeValues(__tagHelperExecutionContext);
            await __tagHelperRunner.RunAsync(__tagHelperExecutionContext);
            if (!__tagHelperExecutionContext.Output.IsContentModified)
            {
                await __tagHelperExecutionContext.SetOutputContentAsync();
            }
            Write(__tagHelperExecutionContext.Output);
            __tagHelperExecutionContext = __tagHelperScopeManager.End();
            WriteLiteral("</p>\r\n");
#nullable restore
#line 59 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                    }

#line default
#line hidden
#nullable disable
#nullable restore
#line 59 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                     
                }

#line default
#line hidden
#nullable disable
#nullable restore
#line 61 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                 foreach (Sample sample in Model.Samples)
                {
                    

#line default
#line hidden
#nullable disable
#nullable restore
#line 63 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                     if (sample.ErrandId == errand.ErrandId)
                    {

#line default
#line hidden
#nullable disable
            WriteLiteral("                        <p>");
            __tagHelperExecutionContext = __tagHelperScopeManager.Begin("a", global::Microsoft.AspNetCore.Razor.TagHelpers.TagMode.StartTagAndEndTag, "2fa47a71dc44b6a0ca68849d3d0ac5b2ba7d926318916", async() => {
                WriteLiteral("sample_");
#nullable restore
#line 65 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                                                                    Write(sample.SampleId);

#line default
#line hidden
#nullable disable
            }
            );
            __Microsoft_AspNetCore_Mvc_Razor_TagHelpers_UrlResolutionTagHelper = CreateTagHelper<global::Microsoft.AspNetCore.Mvc.Razor.TagHelpers.UrlResolutionTagHelper>();
            __tagHelperExecutionContext.Add(__Microsoft_AspNetCore_Mvc_Razor_TagHelpers_UrlResolutionTagHelper);
            BeginAddHtmlAttributeValues(__tagHelperExecutionContext, "href", 2, global::Microsoft.AspNetCore.Razor.TagHelpers.HtmlAttributeValueStyle.DoubleQuotes);
            AddHtmlAttributeValue("", 2195, "~/Samples/", 2195, 10, true);
#nullable restore
#line 65 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
AddHtmlAttributeValue("", 2205, sample.SampleName, 2205, 18, false);

#line default
#line hidden
#nullable disable
            EndAddHtmlAttributeValues(__tagHelperExecutionContext);
            await __tagHelperRunner.RunAsync(__tagHelperExecutionContext);
            if (!__tagHelperExecutionContext.Output.IsContentModified)
            {
                await __tagHelperExecutionContext.SetOutputContentAsync();
            }
            Write(__tagHelperExecutionContext.Output);
            __tagHelperExecutionContext = __tagHelperScopeManager.End();
            WriteLiteral("</p>\r\n");
#nullable restore
#line 66 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                    }

#line default
#line hidden
#nullable disable
#nullable restore
#line 66 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
                     
                }

#line default
#line hidden
#nullable disable
            WriteLiteral("            </td>\r\n        </tr>\r\n");
#nullable restore
#line 70 "C:\Users\rasmu\OneDrive\Documents\GitHub\Projects\School\Uppsala\_.Net_Programmering\Inlämningsuppgifter\ElementalCrime_Module5\EnvironmentalCrime\Views\Investigator\StartInvestigator.cshtml"
    }

#line default
#line hidden
#nullable disable
            WriteLiteral("</table>");
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
        public global::Microsoft.AspNetCore.Mvc.Rendering.IHtmlHelper<IEnvironmentalCrime> Html { get; private set; }
    }
}
#pragma warning restore 1591
