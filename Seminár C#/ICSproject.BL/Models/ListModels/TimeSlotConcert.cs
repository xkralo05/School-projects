using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ICSproject.BL.Models.ListModels
{
    public class TimeSlotConcert : IModel
    {
        public Guid Id { get; set; }
        public DateTime SlotStart { get; set; }
        public DateTime SlotEnd { get; set; }

        public String InterpretName { get; set; }

        public String Stage { get; set; }


        public sealed class TimeSlotConcertEqualityComparer : IEqualityComparer<TimeSlotConcert>
        {
            public bool Equals(TimeSlotConcert x, TimeSlotConcert y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                if (x.Id.Equals(y.Id) && x.SlotStart.Equals(y.SlotStart) && x.SlotEnd.Equals(y.SlotEnd))
                {
                    if (x.InterpretName == null || y.InterpretName == null)
                        return true;
                    if(x.Stage == null || y.Stage == null)
                        return true;
                    return x.InterpretName.Equals(y.InterpretName) && x.Stage.Equals(y.Stage);
                }
                return false;
            }

            public int GetHashCode(TimeSlotConcert obj)
            {
                return HashCode.Combine(obj.Id, obj.InterpretName, obj.SlotStart, obj.SlotEnd, obj.Stage);
            }
        }
        public static IEqualityComparer<TimeSlotConcert> TimeSlotConcertComparer { get; } = new TimeSlotConcertEqualityComparer();
    }
}
