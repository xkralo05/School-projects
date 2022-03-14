using System;
using System.Collections.Generic;
using System.Text;

namespace ICSproject.DAL.Entities
{
    public record TimeSlotEntity : EntityBase
    {
        public DateTime SlotStart { get; set; }
        public DateTime SlotEnd { get; set; }

        public Guid StageId { get; set; }
        public StageEntity Stage { get; set; }
        
        public Guid InterpretId { get; set; }
        public InterpretEntity Interpret { get; set; }
        
        private sealed class TimeSlotWithoutStageEntityEqualityComparer : IEqualityComparer<TimeSlotEntity>
        {
            public bool Equals(TimeSlotEntity x, TimeSlotEntity y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (x is null) return false;
                if (y is null) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.SlotStart.Equals(y.SlotStart)
                    && x.SlotEnd.Equals(y.SlotEnd)
                    && x.InterpretId.Equals(y.InterpretId)
                    && Equals(x.Interpret,y.Interpret);
            }

            public int GetHashCode(TimeSlotEntity obj)
                => HashCode.Combine(obj.SlotStart, obj.SlotEnd, obj.InterpretId, obj.StageId, obj.Interpret);
        }

        public static IEqualityComparer<TimeSlotEntity> TimeSlotWithoutStageEntityComparer { get; }
            = new TimeSlotWithoutStageEntityEqualityComparer();

        private sealed class TimeSlotWithoutInterpretEntityEqualityComparer : IEqualityComparer<TimeSlotEntity>
        {
            public bool Equals(TimeSlotEntity x, TimeSlotEntity y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (x is null) return false;
                if (y is null) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.SlotStart.Equals(y.SlotStart)
                    && x.SlotEnd.Equals(y.SlotEnd)
                    && x.StageId.Equals(y.StageId);
            }

            public int GetHashCode(TimeSlotEntity obj)
                => HashCode.Combine(obj.SlotStart, obj.SlotEnd, obj.InterpretId, obj.StageId, obj.Stage);
        }

        public static IEqualityComparer<TimeSlotEntity> TimeSlotWithoutInterpretEntityComparer { get; }
            = new TimeSlotWithoutInterpretEntityEqualityComparer();
    }
}
