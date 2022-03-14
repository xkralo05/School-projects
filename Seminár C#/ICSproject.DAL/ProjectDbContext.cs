using ICSproject.DAL.Entities;
using Microsoft.EntityFrameworkCore;
using ICSproject.DAL.Seeds;

namespace ICSproject.DAL
{
    public class ProjectDbContext : DbContext
    {
        public ProjectDbContext(DbContextOptions options) : base(options)
        {
        }

        public DbSet<TimeSlotEntity> TimeSlots { get; set; }
        public DbSet<InterpretEntity> Interprets { get; set; }
        public DbSet<StageEntity> Stages { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {

        }
    }
}
