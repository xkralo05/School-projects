using Microsoft.EntityFrameworkCore;
using ICSproject.DAL;

namespace Tests
{
    public class DbContextInMemoryFactory
    {
        private readonly string _databaseName;
        public DbContextInMemoryFactory(string databaseName)
        {
            this._databaseName = databaseName;
        }
        public ProjectDbContext Create()
        {
            var contextOptionsBuilder = new DbContextOptionsBuilder<ProjectDbContext>().EnableSensitiveDataLogging(true);
            contextOptionsBuilder.UseInMemoryDatabase(_databaseName);
            return new ProjectDbContext(contextOptionsBuilder.Options);
        }
    }
}
