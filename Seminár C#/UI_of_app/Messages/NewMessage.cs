using ICSproject.BL.Models;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;

namespace UI_of_app.Messages
{
    public class NewMessage<T> : Message<T>
        where T : IModel
    {
    }
}
