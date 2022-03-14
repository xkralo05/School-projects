using System;
using System.Collections.Generic;
using ICSproject.DAL.Entities;

namespace ICSproject.DAL.Repositories
{
    public static class PrimaryKeyComparers
    {
        public static IEqualityComparer<EntityBase> IdComparer { get; } = new IdEqualityComparer<Guid>();

        private sealed class IdEqualityComparer<TKey> : IEqualityComparer<EntityBase>
        {
            public bool Equals(EntityBase x, EntityBase y)
            {
                if (ReferenceEquals(x, y))
                {
                    return true;
                }

                if (ReferenceEquals(x, null))
                {
                    return false;
                }

                if (ReferenceEquals(y, null))
                {
                    return false;
                }

                if (x.GetType() != y.GetType())
                {
                    return false;
                }

                return x.Id.Equals(y.Id);
            }

            public int GetHashCode(EntityBase obj) => obj.Id.GetHashCode();
        }
    }
}