using ICSproject.BL.Mappers;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;
using ICSproject.DAL.Repositories;
using ICSproject.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Query;
using System;
using System.Linq;

namespace ICSproject.BL.Facades
{
    public class StageFacade : CrudFacadeBase<StageEntity, StageListModel, StageDetailModel>
    {
        public StageFacade(
            UnitOfWork unitOfWork,
            RepositoryBase<StageEntity> repository,
            IMapper<StageEntity, StageListModel, StageDetailModel> mapper,
            IEntityBaseFactory entityFactory)
            : base(unitOfWork, repository, mapper, entityFactory)
        {
        }

        protected override Func<IQueryable<StageEntity>, IIncludableQueryable<StageEntity, object>>[] Includes
        {
            get;
        } = new Func<IQueryable<StageEntity>, IIncludableQueryable<StageEntity, object>>[]
        {
            entities => entities.Include(i=>i.Interprets)
                .ThenInclude(i=>i.Interpret),
        };
    }
}