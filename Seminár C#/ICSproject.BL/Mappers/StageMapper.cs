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
    public class StageMapper : IMapper<StageEntity, StageListModel, StageDetailModel>
    {
        public IEnumerable<StageListModel> Map(IQueryable<StageEntity> entities){
            return entities?.Select(entity => new StageListModel()
            {
                Id = entity.Id,
                Name = entity.Name
            }).ToArray();
        }

        public StageDetailModel Map(StageEntity entity) 
            => entity == null ? null : new StageDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Description = entity.Description,
                ImgSrc = entity.ImgSrc,
                TimeSlots = entity.Interprets.Select(
                timeSlotEntity => new TimeSlotConcert()
                {
                    Id = timeSlotEntity.Id,
                    SlotStart = timeSlotEntity.SlotStart,
                    SlotEnd = timeSlotEntity.SlotEnd,
                    InterpretName = timeSlotEntity.Interpret.Name
                }).ToArray()
            };

        public StageEntity Map(StageDetailModel detailModel, IEntityBaseFactory entityFactory)
        {
            var entity = (entityFactory ??= new CreateNewEntityFactory()).Create<StageEntity>(detailModel.Id);

            entity.Id = detailModel.Id;
            entity.Name = detailModel.Name;
            entity.Description = detailModel.Description;
            entity.ImgSrc = detailModel.ImgSrc;

            if (detailModel.TimeSlots != null)
                entity.Interprets = detailModel.TimeSlots.Select(interpret =>
                {
                    var timeSlot = entityFactory.Create<TimeSlotEntity>(interpret.Id);
                    timeSlot.SlotStart = interpret.SlotStart;
                    timeSlot.SlotEnd = interpret.SlotEnd;
                    timeSlot.Interpret = entityFactory.Create<InterpretEntity>(interpret.Id);
                    timeSlot.Interpret.Name = interpret.Stage;
                    return timeSlot;
                }).ToList();

            return entity;
        }
    }
}