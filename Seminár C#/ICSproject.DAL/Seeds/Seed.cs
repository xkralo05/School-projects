using System;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using ICSproject.DAL.Entities;

namespace ICSproject.DAL.Seeds
{
    public static class Seed
    {
        public static readonly InterpretEntity InterpretGojira = new InterpretEntity()
        {
            Id = Guid.Parse("88D642BD-6D3E-403C-AA15-8552DEDDFA8A"),
            Name = "Gojira",
            Genre = "Heavy Metal",
            ImageSrc = "https://i.scdn.co/image/1ad6844cd3a0d5a195f2df98f44abfcbfe2f4d5e",
            Country = "France",
            LongDescription = "Very long description",
            ShortDescription = "Fying Whales"
        };

        public static readonly InterpretEntity InterpretLuna = new InterpretEntity()
        {
            Id = Guid.Parse("88D682BD-6D3E-403C-AB15-8552DEDDFA8A"),
            Name = "Luna",
            Genre = "Pop",
            ImageSrc = "https://img.youtube.com/vi/oDvXu8aZ-kw/hq720.jpg",
            Country = "Ukraine",
            LongDescription = "Very long description",
            ShortDescription = "Бутылочка"
        };

        public static readonly StageEntity StageNorth = new StageEntity()
        {
            Id = Guid.Parse("A464EAA4-867E-45F4-81F1-48465E2C23A2"),
            Name = "Stage North",
            Description = "A lot of seats",
            ImgSrc = "https://image.freepik.com/free-vector/red-stage-curtain-theater-opera-scene-drape_107791-1552.jpg"
        };


        public static readonly TimeSlotEntity Slot1 = new TimeSlotEntity()
        {
            Id = Guid.Parse("06869db4-e41b-4d2c-ac78-033c7817d9b1"),
            SlotStart = DateTime.Parse("9:00"),
            SlotEnd = DateTime.Parse("10:15"),
            Stage = StageNorth,
            Interpret = InterpretGojira,
            StageId = StageNorth.Id,
            InterpretId = InterpretGojira.Id
        };

        public static readonly TimeSlotEntity Slot2 = new TimeSlotEntity()
        {
            Id = Guid.Parse("c4162790-550c-425c-8c96-5d2fe71cb444"),
            SlotStart = DateTime.Parse("10:30"),
            SlotEnd = DateTime.Parse("11:30"),
            Stage = StageNorth,
            Interpret = InterpretLuna,
            StageId = StageNorth.Id,
            InterpretId = InterpretLuna.Id
        };

        public static void SeedInterprets(this ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<InterpretEntity>()
                .HasData(new  
                {
                    Id = InterpretGojira.Id,
                    Name = InterpretGojira.Name,
                    Genre = InterpretGojira.Genre,
                    ImageSrc = InterpretGojira.ImageSrc,
                    Country = InterpretGojira.Country,
                    LongDescription = InterpretGojira.LongDescription,
                    ShortDescription = InterpretGojira.ShortDescription,
                    Concerts = InterpretGojira.Concerts
                });
            modelBuilder.Entity<InterpretEntity>()
                .HasData(new
                {
                    Id = InterpretLuna.Id,
                    Name = InterpretLuna.Name,
                    Genre = InterpretLuna.Genre,
                    ImageSrc = InterpretLuna.ImageSrc,
                    Country = InterpretLuna.Country,
                    LongDescription = InterpretLuna.LongDescription,
                    ShortDescription = InterpretLuna.ShortDescription,
                    Concerts = InterpretLuna.Concerts
                });
        }

        public static void SeedStages(this ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<StageEntity>()
                .HasData(new
                {
                    Id = StageNorth.Id,
                    Name = StageNorth.Name,
                    Description = StageNorth.Description,
                    ImgSrc = StageNorth.ImgSrc,
                    Interprets = StageNorth.Interprets
                });
        }

        public static void SeedTimeSlots(this ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<TimeSlotEntity>()
                .HasData(new TimeSlotEntity()
                {
                    Id = Slot1.Id,
                    SlotStart = Slot1.SlotStart,
                    SlotEnd = Slot1.SlotEnd,
                    Stage = Slot1.Stage,
                    Interpret = Slot1.Interpret,
                    StageId = Slot1.StageId,
                    InterpretId = Slot1.InterpretId
                });
            modelBuilder.Entity<TimeSlotEntity>()
                .HasData(new TimeSlotEntity()
                {
                    Id = Slot2.Id,
                    SlotStart = Slot2.SlotStart,
                    SlotEnd = Slot2.SlotEnd,
                    Stage = Slot2.Stage,
                    Interpret = Slot2.Interpret,
                    StageId = Slot2.StageId,
                    InterpretId = Slot2.InterpretId
                });
        }

    }
}