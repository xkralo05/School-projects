using System;
using System.Collections.Generic;
using System.Linq;
using ICSproject.BL.Factories;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;

namespace ICSproject.BL.Mappers
{
    public class TimeSlotMapper : IMapper<TimeSlotEntity, TimeSlotConcert, TimeSlotDetailModel>
    {
        public IEnumerable<TimeSlotConcert> Map(IQueryable<TimeSlotEntity> entities)
        {
            return entities?.Select(entity => new TimeSlotConcert()
            {
                Id = entity.Id,
                SlotStart = entity.SlotStart,
                SlotEnd = entity.SlotEnd,
                InterpretName = entity.Interpret.Name,
                Stage = entity.Stage.Name
            }).ToArray();
        }

        public TimeSlotDetailModel Map(TimeSlotEntity entity) 
            => entity == null? null : new TimeSlotDetailModel
            {
                Id = entity.Id,
                SlotStart = entity.SlotStart,
                SlotEnd = entity.SlotEnd,
                Stage = new StageListModel()
                {
                    Id = entity.StageId,
                    Name = entity.Stage == null ? null : entity.Stage.Name
                },
                Interpret = new InterpretListModel()
                {
                    Id = entity.InterpretId,
                    Name = entity.Interpret == null ? null : entity.Interpret.Name
                } 
            };

        public TimeSlotEntity Map(TimeSlotDetailModel detailModel, IEntityBaseFactory entityFactory)
        {
            var entity = (entityFactory ??= new CreateNewEntityFactory()).Create<TimeSlotEntity>(detailModel.Id);

            entity.Id = detailModel.Id;
            entity.SlotStart = detailModel.SlotStart;
            entity.SlotEnd = detailModel.SlotEnd;
            entity.StageId = detailModel.Stage.Id;
            entity.InterpretId = detailModel.Interpret.Id;
            if (entity.Interpret == null)
                entity.Interpret = entityFactory.Create<InterpretEntity>(detailModel.Interpret.Id);
            entity.Interpret.Name = detailModel.Interpret.Name;
            if (entity.Stage == null)
                entity.Stage = entityFactory.Create<StageEntity>(detailModel.Stage.Id);
            entity.Stage.Name = detailModel.Stage.Name;

            return entity;
        }
    }
}