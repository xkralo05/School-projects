using System.Collections.Generic;
using System.Linq;
using ICSproject.BL.Models;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;

namespace ICSproject.BL.Mappers
{
    public interface IMapper<TEntity, out TListModel, TDetailModel>
        where TEntity : EntityBase, new()
        where TListModel : IModel, new()
        where TDetailModel : IModel, new()
    {
        IEnumerable<TListModel> Map(IQueryable<TEntity> entities);
        TDetailModel Map(TEntity entity);
        TEntity Map(TDetailModel detailModel, IEntityBaseFactory entityFactory);
    }
}