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
using Xunit;

namespace ICSproject.BL.Tests
{
    public class InterpretFacadeTests
    {
        private readonly InterpretFacade _facadeSUT;
        private readonly RepositoryBase<InterpretEntity> _repository;
        private readonly InterpretMapper _mapper;

        public InterpretFacadeTests()
        {
            IDbFactoryCommon dbContextFactory = new DesignTimeDbContextFactory();
            var dbx = dbContextFactory.Create();
            dbx.Database.EnsureCreated();
            var unitOfWork = new UnitOfWork(dbx, dbContextFactory);
            _repository = new RepositoryBase<InterpretEntity>(unitOfWork);
            _mapper = new InterpretMapper();
            var entityFactory = new EntityBaseFactory(dbx.ChangeTracker);
            _facadeSUT = new InterpretFacade(unitOfWork, _repository, _mapper, entityFactory);
        }
        

        [Fact]
        public void NewInterpret_InsertOrUpdate_Persisted()
        {
            var detail = new InterpretDetailModel()
            {
                Name = "Gojira",
                Genre = "Heavy Metal",
                ImageSrc = "https://i.scdn.co/image/1ad6844cd3a0d5a195f2df98f44abfcbfe2f4d5e",
                Country = "France",
                LongDescription = "Very long description",
                ShortDescription = "Flying Whales",
                Concerts = new List<TimeSlotConcert>()
                {
                    new TimeSlotConcert {
                        Id = new Guid(),
                        SlotStart = DateTime.Parse("9:00"),
                        SlotEnd = DateTime.Parse("10:15"),
                        Stage = "Zero",
                    }
                }
            };


            detail = _facadeSUT.Save(detail);

            Assert.NotEqual(Guid.Empty, detail.Id);

            var entityFromDb = _repository.GetById(detail.Id);
            Assert.Equal(detail, _mapper.Map(entityFromDb), InterpretDetailModel.InterpretDetailModelComparer);
        }

        [Fact]
        public void GetById_Luna()
        {
            var detail = _facadeSUT.GetById(Seed.InterpretLuna.Id);

            Assert.Equal(detail, _mapper.Map(Seed.InterpretLuna), InterpretDetailModel.InterpretDetailModelComparer);
        }
        [Fact]
        public void DeleteById_Gojira()
        {
            _facadeSUT.Delete(Seed.InterpretGojira.Id);

            var detail = _facadeSUT.GetById(Seed.InterpretGojira.Id);

            Assert.Null(detail);
        }

        [Fact]
        public void NewInterpret_Insert_Persisted()
        {
            var detail = new InterpretDetailModel()
            {
                Id = new Guid(),
                Name = "Bring me the horizon",
                Genre = "Alternative",
                ImageSrc = "https://i.scdn.co/image/1ad6844cd3a0d5a195f2df98f44abfcbfe2f4d5e",
                Country = "Great Britain",
                LongDescription = "Very long description",
                ShortDescription = "Nihilist Blues",
            };

            detail = _facadeSUT.Save(detail);

            Assert.NotEqual(Guid.Empty, detail.Id);

            Guid hell = detail.Id;

            Assert.NotEqual(Guid.Empty, detail.Id);

            var entityFromDb = _repository.GetById(detail.Id);
            Assert.Equal(detail, _mapper.Map(entityFromDb), InterpretDetailModel.InterpretDetailModelComparer);

            _facadeSUT.Delete(hell);

            detail = _facadeSUT.GetById(hell);

            Assert.Null(detail);
        }
    }
}
