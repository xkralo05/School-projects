using System;
using System.Collections.Generic;
using System.Linq;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using Nemesis.Essentials.Design;

namespace ICSproject.BL.Models.DetailModels
{
    public class InterpretDetailModel : IModel
    {
        public Guid Id { get; set; }

        public string Name { get; set; }
        public string Genre { get; set; }
        public string ImageSrc { get; set; }
        public string Country { get; set; }
        public string LongDescription { get; set; }
        public string ShortDescription { get; set; }
        public ICollection<TimeSlotConcert> Concerts { get; set; }

        private sealed class InterpretDetailModelEqualityComparer : IEqualityComparer<InterpretDetailModel>
        {
            public bool Equals(InterpretDetailModel x, InterpretDetailModel y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.Id.Equals(y.Id) && x.Name == y.Name && x.Genre == y.Genre && x.ImageSrc == y.ImageSrc &&
                       x.Country == y.Country && x.LongDescription == y.LongDescription &&
                       x.ShortDescription == y.ShortDescription &&
                       x.Concerts.OrderBy(i => i.Id).SequenceEqual(y.Concerts.OrderBy(i => i.Id), TimeSlotConcert.TimeSlotConcertComparer);
            }

            public int GetHashCode(InterpretDetailModel obj)
            {
                return HashCode.Combine(obj.Id, obj.Name, obj.Genre, obj.ImageSrc, obj.Country, obj.LongDescription, obj.ShortDescription, obj.Concerts);
            }
        }

        public static IEqualityComparer<InterpretDetailModel> InterpretDetailModelComparer { get; } = new InterpretDetailModelEqualityComparer();


    }
}