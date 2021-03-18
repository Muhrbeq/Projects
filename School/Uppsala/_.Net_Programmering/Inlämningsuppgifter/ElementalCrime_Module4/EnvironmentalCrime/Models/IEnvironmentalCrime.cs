using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EnvironmentalCrime.Models
{
    public interface IEnvironmentalCrime
    {
        IQueryable<Department> Departments { get; }
        IQueryable<Employee> Employees { get; }
        IQueryable<Errand> Errands { get; }
        IQueryable<ErrandStatus> ErrandStatuses { get; }
        IQueryable<Picture> Pictures { get; }
        IQueryable<Sample> Samples { get; }
        IQueryable<Sequence> Sequences { get; }
        Task<Errand> GetErrandDetails(int id);
        int GetSequenceDetails();
        string SaveErrand(Errand errand);
        Errand DeleteErrand(int id);
    }
}
