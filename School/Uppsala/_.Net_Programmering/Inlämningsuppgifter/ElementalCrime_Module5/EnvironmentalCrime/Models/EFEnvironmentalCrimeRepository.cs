using Microsoft.AspNetCore.Http;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Models
{
    public class EFEnvironmentalCrimeRepository : IEnvironmentalCrime
    {
        private ApplicationDbContext context;
        private IHttpContextAccessor contextAcc;

        public EFEnvironmentalCrimeRepository(ApplicationDbContext ctx, IHttpContextAccessor cont)
        {
            context = ctx;
            contextAcc = cont;
        }

        public IQueryable<Department> Departments => context.Departments;

        public IQueryable<Employee> Employees => context.Employees;

        public IQueryable<Errand> Errands => context.Errands.Include(e => e.Samples).Include(e => e.Pictures);

        public IQueryable<ErrandStatus> ErrandStatuses => context.ErrandStatuses;
        public IQueryable<Picture> Pictures => context.Pictures;
        public IQueryable<Sample> Samples => context.Samples;
        public IQueryable<Sequence> Sequences => context.Sequences;


        public Task<Errand> GetErrandDetails(int id)
        {
            return Task.Run(() =>
            {
                var errandDetail = Errands.Where(ed => ed.ErrandId == id).First();
                return errandDetail;
            });
        }

        public IQueryable<MyErrand> GetFilteredErrands()
        {
            var userName = contextAcc.HttpContext.User.Identity.Name;

            var employeeDetail = GetEmployeeDetail(userName);

            //var tempName = userName;
            if(employeeDetail.RoleTitle.Equals("Coordinator"))
            {
                var errandList = from err in Errands
                        join stat in ErrandStatuses on err.StatusId equals stat.StatusId
                        join dep in Departments on err.DepartmentId equals dep.DepartmentId
                        into departmentErrand
                        from deptE in departmentErrand.DefaultIfEmpty()
                        join em in Employees on err.EmployeeId equals em.EmployeeId
                        into employeeErrand
                        from empE in employeeErrand.DefaultIfEmpty()
                        orderby err.RefNumber descending
                        select new MyErrand
                        {
                            DateOfObservation = err.DateOfObservation,
                            ErrandId = err.ErrandId,
                            RefNumber = err.RefNumber,
                            TypeOfCrime = err.TypeOfCrime,
                            StatusName = stat.StatusName,
                            DepartmentName = (err.DepartmentId == null ? "ej tillsatt" : deptE.DepartmentName),
                            EmployeeName = (err.EmployeeId == null ? "ej tillsatt" : empE.EmployeeName)
                        };
                return errandList;
            }
            else if(employeeDetail.RoleTitle.Equals("Investigator"))
            {
                var errandList = from err in Errands
                        join stat in ErrandStatuses on err.StatusId equals stat.StatusId
                        join dep in Departments on employeeDetail.DepartmentId equals dep.DepartmentId
                        into departmentErrand
                        from deptE in departmentErrand.DefaultIfEmpty()
                        join em in Employees on err.EmployeeId equals em.EmployeeId
                        into employeeErrand
                        from empE in employeeErrand.DefaultIfEmpty()
                        orderby err.RefNumber descending
                        select new MyErrand
                        {
                            DateOfObservation = err.DateOfObservation,
                            ErrandId = err.ErrandId,
                            RefNumber = err.RefNumber,
                            TypeOfCrime = err.TypeOfCrime,
                            StatusName = stat.StatusName,
                            DepartmentName = (err.DepartmentId == null ? "ej tillsatt" : deptE.DepartmentName),
                            EmployeeName = (err.EmployeeId == null ? "ej tillsatt" : empE.EmployeeName)
                        };
                return errandList;
            }
            else if(employeeDetail.RoleTitle.Equals("Manager"))
            {
                var errandList = from err in Errands
                    join stat in ErrandStatuses on err.StatusId equals stat.StatusId
                    join dep in Departments on err.DepartmentId equals dep.DepartmentId
                    into departmentErrand
                    from deptE in departmentErrand.DefaultIfEmpty()
                    join em in Employees on err.EmployeeId equals em.EmployeeId
                    into employeeErrand
                    from empE in employeeErrand.DefaultIfEmpty()
                    orderby err.RefNumber descending
                    where err.DepartmentId == employeeDetail.DepartmentId
                    select new MyErrand
                    {
                        DateOfObservation = err.DateOfObservation,
                        ErrandId = err.ErrandId,
                        RefNumber = err.RefNumber,
                        TypeOfCrime = err.TypeOfCrime,
                        StatusName = stat.StatusName,
                        DepartmentName = (err.DepartmentId == null ? "ej tillsatt" : deptE.DepartmentName),
                        EmployeeName = (err.EmployeeId == null ? "ej tillsatt" : empE.EmployeeName)
                    };
                return errandList;
            }

            return null;
            
        }

        public IQueryable<Employee> GetManagerEmployees()
        {
            var userName = contextAcc.HttpContext.User.Identity.Name;

            var employeeDetail = GetEmployeeDetail(userName);

            var errandList = from emp in Employees
                             where emp.DepartmentId == employeeDetail.DepartmentId && emp.EmployeeName != employeeDetail.EmployeeName
                             select new Employee
                             {
                                 EmployeeName = emp.EmployeeName,
                                 EmployeeId = emp.EmployeeId,
                                 DepartmentId = emp.DepartmentId,
                                 RoleTitle = emp.RoleTitle
                             };
            return errandList;
        }

        public Employee GetEmployeeDetail(string empID)
        {
            var employeeDetail = Employees.Where(em => em.EmployeeId.Equals(empID)).First();
            return employeeDetail;
        }

        public int GetSequenceDetails()
        {
            //get sequence id
            var sequenceDetail = Sequences.Where(sd => sd.Id == 1).First().CurrentValue;
            return sequenceDetail;
        }

        public string SaveErrand(Errand errand)
        {
            string errandRefNumber = "";
            if(errand.ErrandId == 0)
            {
                // Get ref id
                errand.RefNumber = "2020-45-" + GetSequenceDetails();

                //Hardcode statusID
                errand.StatusId = "S_A";

                //Add to database
                context.Errands.Add(errand);

                //Save return value
                errandRefNumber = errand.RefNumber;

                //Increment CurrentValue
                Sequences.Where(sd => sd.Id == 1).First().CurrentValue++;
            }
            else
            {
                Errand dbEntry = context.Errands.FirstOrDefault(e => e.ErrandId == errand.ErrandId);
                if(dbEntry != null)
                {
                    dbEntry.DateOfObservation = errand.DateOfObservation;
                    dbEntry.DepartmentId = errand.DepartmentId;
                    dbEntry.EmployeeId = errand.EmployeeId;
                    dbEntry.InformerName = errand.InformerName;
                    dbEntry.InformerPhone = errand.InformerPhone;
                    dbEntry.InvestigatorAction = errand.InvestigatorAction;
                    dbEntry.InvestigatorInfo = errand.InvestigatorInfo;
                    dbEntry.Observation = errand.Observation;
                    dbEntry.Place = errand.Place;
                    dbEntry.RefNumber = errand.RefNumber;
                    dbEntry.StatusId = errand.StatusId;
                    dbEntry.TypeOfCrime = errand.TypeOfCrime;

                    
                }
            }

            if(errand.Pictures != null)
            {
                if (errand.Pictures.Count > 0)
                {
                    foreach (Picture picture in errand.Pictures)
                    {
                        if (!context.Pictures.Contains(picture))
                        {
                            context.Pictures.Add(picture);
                        }
                    }
                }
            }

            
            if(errand.Samples != null)
            {
                if (errand.Samples.Count > 0)
                {
                    foreach (Sample sample in errand.Samples)
                    {
                        if(!context.Samples.Contains(sample))
                        {
                            context.Samples.Add(sample);
                        }
                    }
                }
            }
            
            

            //Save Database
            context.SaveChanges();

            return errandRefNumber;
        }

        public Errand DeleteErrand(int id)
        {
            Errand dbEntry = context.Errands.FirstOrDefault(e => e.ErrandId == id);
            if(dbEntry != null)
            {
                context.Errands.Remove(dbEntry);
                context.SaveChanges();
            }
            return dbEntry;
        }
    }
}
