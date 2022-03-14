using System;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Query;
using ICSproject.BL.Mappers;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;
using ICSproject.DAL.Repositories;
using ICSproject.DAL.UnitOfWork;

namespace ICSproject.BL.Facades
{
    public class InterpretFacade : CrudFacadeBase<InterpretEntity, InterpretListModel, InterpretDetailModel>
    {
        public InterpretFacade(
            UnitOfWork unitOfWork, 
            RepositoryBase<InterpretEntity> repository, 
            IMapper<InterpretEntity, InterpretListModel, InterpretDetailModel> mapper,
            IEntityBaseFactory entityFactory) 
            : base(unitOfWork, repository, mapper, entityFactory)
        {
        }

        protected override Func<IQueryable<InterpretEntity>, IIncludableQueryable<InterpretEntity, object>>[] Includes
        {
            get;
        } = new Func<IQueryable<InterpretEntity>, IIncludableQueryable<InterpretEntity, object>>[]
        {
            entities => entities.Include(i=>i.Concerts)
                .ThenInclude(i=>i.Stage),
        };
    }
}