using UI_of_app.Commands;
using ICSproject.BL.Facades;
using ICSproject.BL.Mappers;
using System;
using System.Windows;
using System.Windows.Input;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using UI_of_app.Messages;
using UI_of_app.Services;
using UI_of_app.ViewModels;

namespace UI_of_app.ViewModels
{
    public class StageDetailViewModel : ViewModelBase
    {
        private readonly StageFacade _stageFacade;
        private readonly IMessageBoxService _messageBoxService;
        private readonly IMediator _mediator;

        public StageDetailModel Model { get; set; }
        public ICommand SaveCommand { get; set; }
        public ICommand DeleteCommand { get; set; }

        public StageDetailViewModel(
            StageFacade stageFacade,
            IMessageBoxService messageBoxService,
            IMediator mediator)
        {
            _stageFacade = stageFacade;
            _messageBoxService = messageBoxService;
            _mediator = mediator;

            SaveCommand = new RelayCommand(Save, CanSave);
            DeleteCommand = new RelayCommand(Delete);

            _mediator.Register<StageSelectedMessage>(StageSelected);
            _mediator.Register<StageNewMessage>(StageNew);
        }

        private void StageNew(StageNewMessage interpretNewMessage)
            => Model = new StageDetailModel();

        private void StageSelected(StageSelectedMessage interpretSelectedMessage)
            => Model = _stageFacade.GetById(interpretSelectedMessage.Id);

        public void Save()
        {

            if (Model.Id == Guid.Empty)
            {
                _stageFacade.Save(Model);
                _mediator.Send(new StageAddedMessage { Id = Model.Id });
            }
            else
            {
                _stageFacade.Save(Model);
                _mediator.Send(new StageUpdatedMessage { Id = Model.Id });
            }
        }

        private bool CanSave() =>
            Model != null
            && !string.IsNullOrWhiteSpace(Model.Name)
            && !string.IsNullOrWhiteSpace(Model.Description);

        public void Delete()
        {
            if (Model.Id != Guid.Empty)
            {
                try
                {
                    _stageFacade.Delete(Model.Id);
                }
                catch
                {
                    _messageBoxService.Show($"Deleting of {Model?.Name} failed!", "Deleting failed", MessageBoxButton.OK);
                }

                _mediator.Send(new StageDeletedMessage { Id = Model.Id });
            }
            Model = null;
        }
    }
}