using System;

namespace UI_of_app.ViewModels
{
    public interface IDetailViewModel<TDetail> : IViewModel
    {
        TDetail Model { get; set; }

        void Load(Guid id);
    }
}