using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MySchool.Models
{
    public interface ISchoolRepository
    {
        IQueryable<Student> Students { get; }
        IQueryable<Teacher> Teachers{ get; }
        Task<Teacher> GetTeacherDetail(int id);
        Student GetStudentDetail(int id);
        IQueryable<Course> Courses { get; }
    }
}
