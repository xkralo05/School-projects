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
    public class InterpretDetailViewModel : ViewModelBase
    {
        private readonly InterpretFacade _interpretFacade;
        private readonly IMessageBoxService _messageBoxService;
        private readonly IMediator _mediator;

        public InterpretDetailModel Model { get; set; }
        public ICommand SaveCommand { get; set; }
        public ICommand DeleteCommand { get; set; }

        public InterpretDetailViewModel(
            InterpretFacade stageFacade,
            IMessageBoxService messageDialogService,
            IMediator mediator)
        {
            _interpretFacade = stageFacade;
            _messageBoxService = messageDialogService;
            _mediator = mediator;

            SaveCommand = new RelayCommand(Save, CanSave);
            DeleteCommand = new RelayCommand(Delete);

            _mediator.Register<InterpretSelectedMessage>(InterpretSelected);
            _mediator.Register<InterpretNewMessage>(InterpretNew);
        }

        private void InterpretNew(InterpretNewMessage interpretNewMessage)
            => Model = new InterpretDetailModel();

        private void InterpretSelected(InterpretSelectedMessage interpretSelectedMessage)
            => Model = _interpretFacade.GetById(interpretSelectedMessage.Id);


        public void Save()
        {
            if (Model.Id == Guid.Empty)
            {
                _interpretFacade.Save(Model);
                _mediator.Send(new InterpretAddedMessage { Id = Model.Id });
            }
            else
            {
                _interpretFacade.Save(Model);
                _mediator.Send(new InterpretUpdatedMessage { Id = Model.Id });
            }
        }

        private bool CanSave() =>
            Model != null
            && !string.IsNullOrWhiteSpace(Model.Name)
            && !string.IsNullOrWhiteSpace(Model.Country)
            && !string.IsNullOrWhiteSpace(Model.Genre)
            && !string.IsNullOrWhiteSpace(Model.ImageSrc);

        public void Delete()
        {
            if (Model.Id != Guid.Empty)
            {
                try
                {
                    _interpretFacade.Delete(Model.Id);
                }
                catch
                {
                    _messageBoxService.Show($"Deleting of {Model?.Name} failed!", "Deleting failed", MessageBoxButton.OK);
                }

                _mediator.Send(new InterpretDeletedMessage { Id = Model.Id });
            }

            Model = null;
        }
    }
}