using System;
using System.Collections.Generic;
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
    public class StageFacadeTests
    {
        private readonly StageFacade _facadeSUT;
        private readonly RepositoryBase<StageEntity> _repository;
        private readonly StageMapper _mapper;

        public StageFacadeTests()
        {
            var dbContextFactory = new DbContextInMemoryFactory(nameof(StageFacadeTests));
            var dbx = dbContextFactory.Create();
            dbx.Database.EnsureCreated();
            var unitOfWork = new UnitOfWork(dbx, dbContextFactory);
            _repository = new RepositoryBase<StageEntity>(unitOfWork);
            _mapper = new StageMapper();
            var entityFactory = new EntityBaseFactory(dbx.ChangeTracker);
            _facadeSUT = new StageFacade(unitOfWork, _repository, _mapper, entityFactory);
        }

        [Fact]
        public void NewStage_InsertOrUpdate_Persisted()
        {
            var detail = new StageDetailModel()
            {
                Id = new Guid(),
                Name = "StageNorth",
                ImgSrc = "https://i.scdn.co/image/1ad6844cd3a0d5a195f2df98f44abfcbfe2f4d5e",
                Description = "Stage",
                TimeSlots = new List<TimeSlotConcert>()
                {
                    new TimeSlotConcert()
                    {
                        Id = new Guid(),
                        SlotStart=DateTime.Parse("9:00"),
                        SlotEnd=DateTime.Parse("10:15"),
                        InterpretName = "Alice in Chains"
                    },
                    new TimeSlotConcert()
                    {
                        Id = new Guid(),
                        SlotStart=DateTime.Parse("10:30"),
                        SlotEnd=DateTime.Parse("11:30"),
                        InterpretName = "Massive attack"
                    }
                }
            };

            detail = _facadeSUT.Save(detail);

            Assert.NotEqual(Guid.Empty, detail.Id);

            var entityFromDb = _repository.GetById(detail.Id);
            Assert.Equal(detail, _mapper.Map(entityFromDb), StageDetailModel.StageDetailModelComparer);
        }

        [Fact]
        public void GetById_StageNorth()
        {
            var detail = _facadeSUT.GetById(Seed.StageNorth.Id);

            Assert.Equal(detail, _mapper.Map(Seed.StageNorth), StageDetailModel.StageDetailModelComparer);
        }
        [Fact]
        public void DeleteById_StageNorth()
        {
            _facadeSUT.Delete(Seed.StageNorth.Id);

            var detail = _facadeSUT.GetById(Seed.StageNorth.Id);

            Assert.Null(detail);
        }
    }
}
