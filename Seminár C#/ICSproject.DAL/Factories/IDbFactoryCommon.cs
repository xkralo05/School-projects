using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;

namespace ICSproject.DAL.Factories
{
    public interface IDbFactoryCommon
    {
        ProjectDbContext Create();
    }
}