using System;
using System.Collections.Generic;
using System.Text;
using Nemesis.Essentials.Design;

namespace ICSproject.DAL.Entities
{

    public record StageEntity : EntityBase
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public string ImgSrc { get; set; }
        public ICollection<TimeSlotEntity> Interprets { get; set; } = new ValueCollection<TimeSlotEntity>(TimeSlotEntity.TimeSlotWithoutStageEntityComparer);

        private sealed class StageEntityEqualityComparer : IEqualityComparer<StageEntity>
        {
            public bool Equals(StageEntity x, StageEntity y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.Name == y.Name && x.Description == y.Description && x.ImgSrc == y.ImgSrc && Equals(x.Interprets, y.Interprets);
            }

            public int GetHashCode(StageEntity obj)
            {
                return HashCode.Combine(obj.Name, obj.Description, obj.ImgSrc, obj.Interprets);
            }
        }

        public static IEqualityComparer<StageEntity> StageEntityComparer { get; } = new StageEntityEqualityComparer();
    }
}
