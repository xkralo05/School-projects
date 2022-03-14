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
    public class InterpretMapper : IMapper<InterpretEntity, InterpretListModel, InterpretDetailModel>
    {
        public IEnumerable<InterpretListModel> Map(IQueryable<InterpretEntity> entities){
            return entities?.Select(entity => new InterpretListModel()
            {
                Id = entity.Id,
                Name = entity.Name
            }).ToArray();
        }

        public InterpretDetailModel Map(InterpretEntity entity) 
            => entity == null ? null : new InterpretDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Genre = entity.Genre,
                ImageSrc = entity.ImageSrc,
                Country = entity.Country,
                LongDescription = entity.LongDescription,
                ShortDescription = entity.ShortDescription,
                Concerts = entity.Concerts.Select(
                timeSlotEntity => new TimeSlotConcert()
                {
                    Id = timeSlotEntity.Id,
                    SlotStart = timeSlotEntity.SlotStart,
                    SlotEnd = timeSlotEntity.SlotEnd,
                    Stage = timeSlotEntity.Stage.Name
                }).ToArray()
            };

        public InterpretEntity Map(InterpretDetailModel detailModel, IEntityBaseFactory entityFactory)
        {
            var entity = (entityFactory ??= new CreateNewEntityFactory()).Create<InterpretEntity>(detailModel.Id);

            entity.Id = detailModel.Id;
            entity.Name = detailModel.Name;
            entity.Genre = detailModel.Genre;
            entity.ImageSrc = detailModel.ImageSrc;
            entity.Country = detailModel.Country;
            entity.LongDescription = detailModel.LongDescription;
            entity.ShortDescription = detailModel.ShortDescription;
            if (detailModel.Concerts != null)
                entity.Concerts = detailModel.Concerts.Select(concert =>
                {
                    var timeSlot = entityFactory.Create<TimeSlotEntity>(concert.Id);
                    timeSlot.SlotStart = concert.SlotStart;
                    timeSlot.SlotEnd = concert.SlotEnd;
                    timeSlot.Stage = entityFactory.Create<StageEntity>(concert.Id);
                    timeSlot.Stage.Name = concert.Stage;
                    return timeSlot;
                }).ToList();
        
            return entity;
        }
    }

}