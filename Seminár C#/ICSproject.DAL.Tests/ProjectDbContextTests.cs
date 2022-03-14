using System;
using Xunit;
using Microsoft.EntityFrameworkCore;
using ICSproject.DAL.Entities;
using ICSproject;
using System.Linq;
using System.Diagnostics;
using ICSproject.DAL;

namespace Tests
{

    public class ProjectDbContextTests : IDisposable
    {
        private readonly ProjectDbContext _dbContextSUT;
        private readonly DbContextInMemoryFactory _dbContextFactory;

        public ProjectDbContextTests()
        {
            _dbContextFactory = new DbContextInMemoryFactory(nameof(ProjectDbContextTests));
            _dbContextSUT = _dbContextFactory.Create();
        }

        [Fact]
        public void  AddNew_Interpret_Persisted()
        {
            //arrange
            var interpretEntity = new InterpretEntity() 
            {
                Name = "Skupina1",
                ShortDescription = "shortdesc",
                ImageSrc="source",
                LongDescription="longdesc",
                Country="Slovakia",
                Concerts =
                {
                    new TimeSlotEntity()
                    {
                        SlotStart = DateTime.Parse("9:00"),
                        SlotEnd = DateTime.Parse("10:15"),
                        Stage = new StageEntity { Name = "Stage1", Description = "shortdesc", ImgSrc = "/img/img2" }
                    },
                    new TimeSlotEntity()
                    {
                        SlotStart = DateTime.Parse("10:30"),
                        SlotEnd = DateTime.Parse("11:30"),
                        Stage = new StageEntity { Name = "Stage2", Description = "shortdesc2", ImgSrc = "/img/img3" }
                    }
                }


            };

            //act
            _dbContextSUT.Interprets.Add(interpretEntity);
            _dbContextSUT.SaveChanges();

            //assert
            using var dbx = _dbContextFactory.Create();
            var interpretFromDatabase = dbx.Interprets
                .Include(interpret => interpret.Concerts)
                .ThenInclude(timeSlot => timeSlot.Stage)
                .FirstOrDefault(i => i.Id == interpretEntity.Id);
            Assert.Equal(interpretEntity, interpretFromDatabase);
        }

        [Fact]
        public void AddNew_Stage_Persisted()
        {
            //arrange
            var stageEntity = new StageEntity()
            {
                Name = "stage1",
                Description = "popis",
                ImgSrc = "/img/img1",
                Interprets =
                {
                    new TimeSlotEntity()
                    {
                        SlotStart=DateTime.Parse("9:00"),
                        SlotEnd=DateTime.Parse("10:15"),
                        Interpret= new InterpretEntity{ Name = "Skupina1", ShortDescription = "shortdesc", ImageSrc="source", LongDescription="longdesc", Country="Slovakia" }

                    },
                    new TimeSlotEntity()
                    {
                        SlotStart=DateTime.Parse("10:30"),
                        SlotEnd=DateTime.Parse("11:30"),
                        Interpret= new InterpretEntity{ Name = "Skupina2", ShortDescription = "shortdesc2", ImageSrc="source2", LongDescription="longdesc2", Country="Czech Republic" }

                    }
                }
            };

            //act
            _dbContextSUT.Stages.Add(stageEntity);
            _dbContextSUT.SaveChanges();

            //assert     
            using var dbx = _dbContextFactory.Create();
            var stageFromDatabase = dbx.Stages
                .Include(stage => stage.Interprets)
                .ThenInclude(timeSlot=>timeSlot.Interpret)
                .FirstOrDefault(i => i.Id == stageEntity.Id);
            Assert.Equal(stageEntity, stageFromDatabase);

        }

        public void Dispose()
        {
            _dbContextSUT.Dispose();
        }
    }
}
