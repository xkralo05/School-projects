using UI_of_app.Commands;
using ICSproject.BL.Facades;
using ICSproject.BL.Mappers;
using System;
using System.Collections.ObjectModel;
using System.Windows.Input;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using UI_of_app.Extensions;
using UI_of_app.Messages;
using UI_of_app.Services;
using UI_of_app.ViewModels;

namespace UI_of_app.ViewModels
{
    public class InterpretListViewModel : ViewModelBase
    {
        private readonly InterpretFacade _interpretFacade;
        private readonly IMediator _mediator;

        public ObservableCollection<InterpretListModel> Interprets { get; set; } = new ObservableCollection<InterpretListModel>();

        public ICommand InterpretSelectedCommand { get; }
        public ICommand InterpretNewCommand { get; }

        public InterpretListViewModel(InterpretFacade interpretFacade, IMediator mediator)
        {
            _interpretFacade = interpretFacade;
            _mediator = mediator;

            InterpretSelectedCommand = new RelayCommand<InterpretListModel>(InterpretSelected);
            InterpretNewCommand = new RelayCommand(InterpretNew);

            _mediator.Register<InterpretAddedMessage>(InterpretAdded);
            _mediator.Register<InterpretUpdatedMessage>(InterpretUpdated);
            _mediator.Register<InterpretDeletedMessage>(InterpretDeleted);
        }

        private void InterpretNew() => _mediator.Send(new InterpretNewMessage());

        private void InterpretSelected(InterpretListModel interpret) => _mediator.Send(new InterpretSelectedMessage { Id = interpret.Id });

        private void InterpretAdded(InterpretAddedMessage interpret) => Load();

        private void InterpretUpdated(InterpretUpdatedMessage interpret) => Load();

        private void InterpretDeleted(InterpretDeletedMessage interpret) => Load();

        public override void Load()
        {
            Interprets.Clear();
            Interprets.AddRange(_interpretFacade.GetAllList());
        }
    }
}
