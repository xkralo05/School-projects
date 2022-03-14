using Microsoft.EntityFrameworkCore;

namespace ICSproject.DAL.Factories
{
    public class DbContextInMemoryFactory: IDbContextFactory<ProjectDbContext>, IDbFactoryCommon
    {
        private readonly string _databaseName;

        public DbContextInMemoryFactory(string databaseName)
        {
            _databaseName = databaseName;
        }
        public ProjectDbContext Create()
        {
            var contextOptionsBuilder = new DbContextOptionsBuilder<ProjectDbContext>();
            
            contextOptionsBuilder.UseInMemoryDatabase(_databaseName);
            return new ProjectDbContext(contextOptionsBuilder.Options);
        }
    }
}