using System;
using System.Collections.Generic;
using System.Text;
using ICSproject.BL.Mappers;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;
using ICSproject.DAL.Repositories;
using ICSproject.DAL.UnitOfWork;

namespace ICSproject.BL.Facades
{
    public class TimeSlotFacade : CrudFacadeBase<TimeSlotEntity, TimeSlotConcert, TimeSlotDetailModel>
    {
        public TimeSlotFacade(UnitOfWork unitOfWork, 
            RepositoryBase<TimeSlotEntity> repository, 
            IMapper<TimeSlotEntity, TimeSlotConcert, TimeSlotDetailModel> mapper,
            IEntityBaseFactory entityFactory) 
            : base(unitOfWork, repository, mapper, entityFactory)
        {
        }


    }
}