using System;
using System.Collections.Generic;
using System.Text;
using Nemesis.Essentials.Design;

namespace ICSproject.DAL.Entities
{
    public record InterpretEntity : EntityBase
    {
        public string Name { get; set; }
        public string Genre { get; set; }
        public string ImageSrc { get; set; }
        public string Country { get; set; }
        public string LongDescription { get; set; }
        public string ShortDescription { get; set; }
        public ICollection<TimeSlotEntity> Concerts { get; set; } = new ValueCollection<TimeSlotEntity>(TimeSlotEntity.TimeSlotWithoutInterpretEntityComparer);

        private sealed class InterpretEntityEqualityComparer : IEqualityComparer<InterpretEntity>
        {
            public bool Equals(InterpretEntity x, InterpretEntity y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.Name == y.Name && x.Genre == y.Genre && x.ImageSrc == y.ImageSrc && x.Country == y.Country && x.LongDescription == y.LongDescription && x.ShortDescription == y.ShortDescription && Equals(x.Concerts, y.Concerts);
            }

            public int GetHashCode(InterpretEntity obj)
            {
                return HashCode.Combine(obj.Name, obj.Genre, obj.ImageSrc, obj.Country, obj.LongDescription, obj.ShortDescription, obj.Concerts);
            }
        }

        public static IEqualityComparer<InterpretEntity> InterpretEntityComparer { get; } = new InterpretEntityEqualityComparer();
    }
}
