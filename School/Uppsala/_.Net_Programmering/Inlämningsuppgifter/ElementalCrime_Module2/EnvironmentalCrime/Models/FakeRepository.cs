using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Models
{
    public class FakeRepository : IEnvironmentalCrime
    {
        public IQueryable<Department> Departments => new List<Department>
        {
            new Department{ DepartmentID = "D00", DepartmentName = "Småstads Kommun" },
            new Department{ DepartmentID = "D01", DepartmentName = "IT-avdelningen" },
            new Department{ DepartmentID = "D02", DepartmentName = "Lek och Skoj" },
            new Department{ DepartmentID = "D03", DepartmentName = "Miljöskydd" }
        }.AsQueryable<Department>();

        public IQueryable<Employee> Employees => new List<Employee>
        {
            new Employee{ EmployeeID = "E302", EmployeeName = "Martin Kvist", RoleTitle = "Investigator", DepartmentID = "D01" },
            new Employee{ EmployeeID = "E301", EmployeeName = "Lena Larsson", RoleTitle = "Investigator", DepartmentID = "D01" },
            new Employee{ EmployeeID = "E401", EmployeeName = "Oskar Ivarsson", RoleTitle = "Investigator", DepartmentID = "D02" },
            new Employee{ EmployeeID = "E501", EmployeeName = "Susanne Fred", RoleTitle = "Investigator", DepartmentID = "D03" }
        }.AsQueryable<Employee>();

        public IQueryable<Errand> Errands => new List<Errand>
        {
            new Errand{ ErrandID = "2020-45-0001", Place = "Skogslunden vid Jensens gård", TypeOfCrime="Sopor", DateOfObservation = new DateTime(2020,04,24), Observation ="Anmälaren var på promeand i skogslunden när hon upptäckte soporna", InvestigatorInfo = "Undersökning har gjorts och bland soporna hittades bl.a ett brev till Gösta Olsson", InvestigatorAction = "Brev har skickats till Gösta Olsson om soporna och anmälan har gjorts till polisen 2018-05-01", InformerName = "Ada Bengtsson", InformerPhone = "0432-5545522", StatusID="Klar", DepartmentID="Renhållning och avfall", EmployeeID ="Susanne Fred"},
            new Errand{ ErrandID = "2020-45-0002", Place = "Småstadsjön", TypeOfCrime="Oljeutsläpp", DateOfObservation = new DateTime(2020,04,29), Observation ="Jag såg en oljefläck på vattnet när jag var där för att fiska", InvestigatorInfo = "Undersökning har gjorts på plats, ingen fläck har hittas", InvestigatorAction = "", InformerName = "Bengt Svensson", InformerPhone = "0432-5152255", StatusID="Ingen åtgärd", DepartmentID="Natur och Skogsvård", EmployeeID ="Oskar Ivarsson"},
            new Errand{ ErrandID = "2020-45-0003", Place = "Ödehuset", TypeOfCrime="Skrot", DateOfObservation = new DateTime(2020,05,02), Observation ="Anmälaren körde förbi ödehuset och upptäcker ett antal bilar och annat skrot", InvestigatorInfo = "Undersökning har gjorts och bilder har tagits", InvestigatorAction = "", InformerName = "Olle Pettersson", InformerPhone = "0432-5255522", StatusID="Påbörjad", DepartmentID="Miljö och Hälsoskydd", EmployeeID ="Lena Larsson"},
            new Errand{ ErrandID = "2020-45-0004", Place = "Restaurang Krögaren", TypeOfCrime="Buller", DateOfObservation = new DateTime(2020,06,04), Observation ="Restaurangen hade för högt ljud på så man inte kunde sova", InvestigatorInfo = "Bullermätning har gjorts. Man håller sig inom riktvärden", InvestigatorAction = "Meddelat restaurangen att tänka på ljudet i fortsättning", InformerName = "Roland Jönsson", InformerPhone = "0432-5322255", StatusID="Klar", DepartmentID="Miljö och Hälsokydd", EmployeeID ="Martin Kvist"},
            new Errand{ ErrandID = "2020-45-0005", Place = "Torget", TypeOfCrime="Klotter", DateOfObservation = new DateTime(2020,07,10), Observation ="Samtliga skräpkorgar och bänkar är nedklottrade", InvestigatorInfo = "", InvestigatorAction = "", InformerName = "Peter Svensson", InformerPhone = "0432-5322555", StatusID="Inrapporterad", DepartmentID="Ej tillsatt", EmployeeID ="Ej tillsatt"}
        }.AsQueryable<Errand>();

        public IQueryable<ErrandStatus> ErrandStatuses => new List<ErrandStatus>
        {
            new ErrandStatus{ StatusID = "S_A", StatusName = "Rapporterad" },
            new ErrandStatus{ StatusID = "S_B", StatusName = "Ingen åtgärd" },
            new ErrandStatus{ StatusID = "S_C", StatusName = "Startad" },
            new ErrandStatus{ StatusID = "S_D", StatusName = "Färdig" }
        }.AsQueryable<ErrandStatus>();

        public Task<Errand> GetErrandDetails(string id)
        {
            return Task.Run(() =>
            {
                var errandDetail = Errands.Where(ed => ed.ErrandID == id).First();
                return errandDetail;
            });
        }
    }
}
