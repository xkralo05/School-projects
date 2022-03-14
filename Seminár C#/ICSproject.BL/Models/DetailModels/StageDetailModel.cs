using System;
using System.Collections.Generic;
using System.Linq;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;
using Nemesis.Essentials.Design;

namespace ICSproject.BL.Models.DetailModels
{
    public class StageDetailModel : IModel
    {
        public Guid Id { get; set; }

        public string Name { get; set; }
        public string Description { get; set; }
        public string ImgSrc { get; set; }

        public ICollection<TimeSlotConcert> TimeSlots { get; set; }

        private sealed class StageDetailModelEqualityComparer : IEqualityComparer<StageDetailModel>
        {
            public bool Equals(StageDetailModel x, StageDetailModel y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.Id.Equals(y.Id) && x.Name == y.Name && x.Description == y.Description &&
                       x.ImgSrc == y.ImgSrc &&
                       x.TimeSlots.OrderBy(i => i.Id).SequenceEqual(y.TimeSlots.OrderBy(i => i.Id), TimeSlotConcert.TimeSlotConcertComparer);
            }

            public int GetHashCode(StageDetailModel obj)
            {
                return HashCode.Combine(obj.Id, obj.Name, obj.Description, obj.ImgSrc, obj.TimeSlots);
            }
        }

        public static IEqualityComparer<StageDetailModel> StageDetailModelComparer { get; } = new StageDetailModelEqualityComparer();
    }
}