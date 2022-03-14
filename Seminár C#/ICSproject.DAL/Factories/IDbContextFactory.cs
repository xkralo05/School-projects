using Microsoft.EntityFrameworkCore;

namespace ICSproject.DAL.Factories
{
    public interface IDbContextFactory<out TDbContext> where TDbContext : DbContext
    {
        TDbContext Create();
    }
}