using System;
using ICSproject.BL.Facades;
using ICSproject.BL.Mappers;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;
using ICSproject.DAL.Repositories;
using ICSproject.DAL.Seeds;
using ICSproject.DAL.UnitOfWork;
using Nemesis.Essentials.Design;
using Xunit;

namespace ICSproject.BL.Tests
{
    public class TimeSlotFacadeTests
    {
        private readonly TimeSlotFacade _facadeSUT;
        private readonly RepositoryBase<TimeSlotEntity> _repository;
        private readonly TimeSlotMapper _mapper;

        public TimeSlotFacadeTests()
        {
            var dbContextFactory = new DbContextInMemoryFactory(nameof(TimeSlotFacadeTests));
            var dbx = dbContextFactory.Create();
            dbx.Database.EnsureCreated();
            var unitOfWork = new UnitOfWork(dbx, dbContextFactory);
            _repository = new RepositoryBase<TimeSlotEntity>(unitOfWork);
            _mapper = new TimeSlotMapper();
            var entityFactory = new EntityBaseFactory(dbx.ChangeTracker);
            _facadeSUT = new TimeSlotFacade(unitOfWork, _repository, _mapper, entityFactory);
        }


        [Fact]
        public void TimeSlot_InsertOrUpdate_Persisted()
        {
            var detail = new TimeSlotDetailModel()
            {
                Id = new Guid(),
                SlotStart = DateTime.Parse("9:10"),
                SlotEnd = DateTime.Parse("10:20"),
                Stage = new StageListModel { Id = new Guid(), Name = "Stage1"},
                Interpret = new InterpretListModel { Id = new Guid(), Name = "Alice in Chains"}
            };

            detail = _facadeSUT.Save(detail);
            
            var entityFromDb = _repository.GetById(detail.Id);
            Assert.Equal(detail, _mapper.Map(entityFromDb), TimeSlotDetailModel.TimeSlotDetailModelComparer);
        }

        [Fact]
        public void GetById_Slot1()
        {
            var detail = _facadeSUT.GetById(Seed.Slot1.Id);

            Assert.Equal(detail, _mapper.Map(Seed.Slot1), TimeSlotDetailModel.TimeSlotDetailModelComparer);
        }
        [Fact]
        public void DeleteById_Slot2()
        {
            _facadeSUT.Delete(Seed.Slot2.Id);

            var detail = _facadeSUT.GetById(Seed.Slot2.Id);

            Assert.Null(detail);
        }

    }
}
