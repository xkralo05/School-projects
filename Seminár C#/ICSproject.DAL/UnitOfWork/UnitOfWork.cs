using System;
using ICSproject.DAL.Factories;
using Microsoft.EntityFrameworkCore;

namespace ICSproject.DAL.UnitOfWork
{
    public class UnitOfWork : IDisposable
    {
        public UnitOfWork(DbContext dbContext, IDbFactoryCommon dbFactory)
        {
            _dbFactory = dbFactory ?? throw new ArgumentNullException(nameof(dbFactory));
            DbContext = dbContext ?? throw new ArgumentNullException(nameof(dbContext));
        }

        private IDbFactoryCommon _dbFactory;

        /* This method is used to prevent errors of type
         'The instance of entity type X cannot be tracked because another 
          instance with the key value XX is already being tracked.'*/
        public void Refresh()
        {
            DbContext = _dbFactory.Create();
        }

        public DbContext DbContext { get; set; }

        public void Dispose() => DbContext.Dispose();

        public void Commit() => DbContext.SaveChanges();
    }
}