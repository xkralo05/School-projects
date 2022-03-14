using System;
using System.Collections.Generic;

namespace ICSproject.BL.Models.ListModels
{
    public class InterpretListModel : IModel
    {
        public Guid Id { get; set; }
        public string Name { get; set; }

        private sealed class IdNameEqualityComparer : IEqualityComparer<InterpretListModel>
        {
            public bool Equals(InterpretListModel x, InterpretListModel y)
            {
                if (ReferenceEquals(x, y)) return true;
                if (ReferenceEquals(x, null)) return false;
                if (ReferenceEquals(y, null)) return false;
                if (x.GetType() != y.GetType()) return false;
                return x.Id.Equals(y.Id) && x.Name == y.Name;
            }

            public int GetHashCode(InterpretListModel obj)
            {
                return HashCode.Combine(obj.Id, obj.Name);
            }
        }

        public static IEqualityComparer<InterpretListModel> IdNameComparer { get; } = new IdNameEqualityComparer();
    }
}