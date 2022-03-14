using System;
using System.Collections.Generic;
using System.Text;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;

namespace ICSproject.BL.Factories
{
    public class CreateNewEntityFactory : IEntityBaseFactory 
    {
        public TEntity Create<TEntity>(Guid id) where TEntity : EntityBase, new() => new TEntity();
    }
}
