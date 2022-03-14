using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;

namespace ICSproject.DAL.Factories
{
    /// <summary>
    /// Allows to use `dotnet ef migrations add xxx`
    /// https://docs.microsoft.com/en-us/dotnet/api/microsoft.entityframeworkcore.design.idesigntimedbcontextfactory-1
    /// https://codingblast.com/entityframework-core-idesigntimedbcontextfactory/
    /// </summary>
    public class DesignTimeDbContextFactory : IDesignTimeDbContextFactory<ProjectDbContext>, IDbFactoryCommon
    {
        public ProjectDbContext CreateDbContext(string[] args)
        {
            var builder = new DbContextOptionsBuilder<ProjectDbContext>(); 
            builder.UseSqlServer(
                @"Data Source=(LocalDB)\MSSQLLocalDB;
                Initial Catalog = ICSproject;
                MultipleActiveResultSets = True;
                Integrated Security = True; ");

            return new ProjectDbContext(builder.Options);
        }
        public ProjectDbContext Create()
        {
            var builder = new DbContextOptionsBuilder<ProjectDbContext>();
            builder.UseSqlServer(
                @"Data Source=(LocalDB)\MSSQLLocalDB;
                Initial Catalog = ICSproject;
                MultipleActiveResultSets = True;
                Integrated Security = True; ");

            return new ProjectDbContext(builder.Options);
        }
    }
}
