using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace MySchool.Models
{
    public class FakeSchoolRepository : ISchoolRepository
    {
        public IQueryable<Student> Students => new List<Student>
        {
            new Student{ StudentID = 1, Code = "L0001", Name = "Amelia G", EnrollmentNo = "201804150001" },
            new Student{ StudentID = 2, Code = "L0002", Name = "Charlie J", EnrollmentNo = "201804150002" },
            new Student{ StudentID = 3, Code = "L0003", Name = "Bertil P", EnrollmentNo = "201804150003" },
        }.AsQueryable<Student>();

        public IQueryable<Teacher> Teachers => new List<Teacher>
        {
            new Teacher{ TeacherID = 1, Code = "TT", Name = "Tomas T" },
            new Teacher{TeacherID = 2, Code = "JC", Name = "Jonas C" },
            new Teacher{ TeacherID = 3, Code = "RM", Name = "Rakel M" }
        }.AsQueryable<Teacher>();

        public Task<Teacher> GetTeacherDetail(int id)
        {
            return Task.Run(() =>
            {
                var teacherDetail = Teachers.Where(tD => tD.TeacherID == id).First();
                return teacherDetail;
            });
        }

        public Student GetStudentDetail(int id)
        {
            var studentDetail = Students.Where(st => st.StudentID == id).First();
            return studentDetail;
        }

        public IQueryable<Course> Courses => new List<Course>
        {
            new Course {CourseID = 1, CourseCode = "M101", Name = "Math 101" },
            new Course {CourseID = 2, CourseCode = "E101", Name = "English 101" },
            new Course {CourseID = 3, CourseCode = "P101", Name = "Poetry 101" },
            new Course {CourseID = 4, CourseCode = "Pr101", Name = "Programming 101" }
        }.AsQueryable<Course>();
    }
}
