#pragma checksum "C:\Github\Ramu_Projects-master\MySchool\MySchool\Views\Shared\Components\ShowOneTeacher\Default.cshtml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "b3db7f648163ceb1e34a193c443e9af446ac96d7"
// <auto-generated/>
#pragma warning disable 1591
[assembly: global::Microsoft.AspNetCore.Razor.Hosting.RazorCompiledItemAttribute(typeof(AspNetCore.Views_Shared_Components_ShowOneTeacher_Default), @"mvc.1.0.view", @"/Views/Shared/Components/ShowOneTeacher/Default.cshtml")]
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
#line 1 "C:\Github\Ramu_Projects-master\MySchool\MySchool\Views\_ViewImports.cshtml"
using MySchool.Models;

#line default
#line hidden
#nullable disable
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"b3db7f648163ceb1e34a193c443e9af446ac96d7", @"/Views/Shared/Components/ShowOneTeacher/Default.cshtml")]
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"d12f99ca127a3a478ebcc9ccf8cf952bf2ff4987", @"/Views/_ViewImports.cshtml")]
    public class Views_Shared_Components_ShowOneTeacher_Default : global::Microsoft.AspNetCore.Mvc.Razor.RazorPage<Teacher>
    {
        #pragma warning disable 1998
        public async override global::System.Threading.Tasks.Task ExecuteAsync()
        {
            WriteLiteral("\r\n<h2>Du sökte efter:</h2>\r\n\r\n\r\n<p>");
#nullable restore
#line 6 "C:\Github\Ramu_Projects-master\MySchool\MySchool\Views\Shared\Components\ShowOneTeacher\Default.cshtml"
Write(Model.Name);

#line default
#line hidden
#nullable disable
            WriteLiteral("</p>\r\n<p>");
#nullable restore
#line 7 "C:\Github\Ramu_Projects-master\MySchool\MySchool\Views\Shared\Components\ShowOneTeacher\Default.cshtml"
Write(Model.Code);

#line default
#line hidden
#nullable disable
            WriteLiteral("</p>\r\n<p>");
#nullable restore
#line 8 "C:\Github\Ramu_Projects-master\MySchool\MySchool\Views\Shared\Components\ShowOneTeacher\Default.cshtml"
Write(Model.TeacherID);

#line default
#line hidden
#nullable disable
            WriteLiteral("</p>\r\n");
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
        public global::Microsoft.AspNetCore.Mvc.Rendering.IHtmlHelper<Teacher> Html { get; private set; }
    }
}
#pragma warning restore 1591
