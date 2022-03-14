using System;
using System.Collections.Generic;
using ICSproject.BL.Models.ListModels;
using ICSproject.DAL.Entities;

namespace ICSproject.BL.Models.DetailModels
{
    public class TimeSlotDetailModel : IModel
    {
        public Guid Id { get; set; }
        public DateTime SlotStart { get; set; } = DateTime.Today;
        public DateTime SlotEnd { get; set; } = DateTime.Today;

        public StageListModel Stage { get; set; }

        public InterpretListModel Interpret { get; set; }

        private sealed class TimeSlotDetailModelEqualityComparer : IEqualityComparer<TimeSlotDetailModel>
        {
            public bool Equals(TimeSlotDetailModel x, TimeSlotDetailModel y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.Id.Equals(y.Id) && x.SlotStart.Equals(y.SlotStart) && x.SlotEnd.Equals(y.SlotEnd)
                       && x.Stage.Id == y.Stage.Id && x.Stage.Name == y.Stage.Name 
                       && x.Interpret.Id == y.Interpret.Id && x.Interpret.Name == y.Interpret.Name;
            }

            public int GetHashCode(TimeSlotDetailModel obj)
            {
                return HashCode.Combine(obj.Id, obj.SlotStart, obj.SlotEnd, obj.Stage, obj.Interpret);
            }
        }

        public static IEqualityComparer<TimeSlotDetailModel> TimeSlotDetailModelComparer { get; } = new TimeSlotDetailModelEqualityComparer();
    }
}