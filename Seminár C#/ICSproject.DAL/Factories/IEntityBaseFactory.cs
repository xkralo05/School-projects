using System;
using ICSproject.DAL.Entities;

namespace ICSproject.DAL.Factories
{
    public interface IEntityBaseFactory
    {
        TEntity Create<TEntity>(Guid id) where TEntity : EntityBase, new();
    }
}