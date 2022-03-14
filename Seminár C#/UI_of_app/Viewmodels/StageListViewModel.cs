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
    public class StageListViewModel : ViewModelBase
    {
        private readonly StageFacade _stageFacade;
        private readonly IMediator _mediator;

        public ObservableCollection<StageListModel> Stages { get; set; } = new ObservableCollection<StageListModel>();

        public ICommand StageSelectedCommand { get; }
        public ICommand StageNewCommand { get; }

        public StageListViewModel(StageFacade stageFacade, IMediator mediator)
        {
            _stageFacade = stageFacade;
            _mediator = mediator;

            StageSelectedCommand = new RelayCommand<StageListModel>(StageSelected);
            StageNewCommand = new RelayCommand(StageNew);

            _mediator.Register<StageAddedMessage>(StageAdded);
            _mediator.Register<StageUpdatedMessage>(StageUpdated);
            _mediator.Register<StageDeletedMessage>(StageDeleted);

        }

        private void StageNew() => _mediator.Send(new StageNewMessage());

        private void StageSelected(StageListModel stage) => _mediator.Send(new StageSelectedMessage { Id = stage.Id });


        private void StageAdded(StageAddedMessage interpret) => Load();

        private void StageUpdated(StageUpdatedMessage interpret) => Load();

        private void StageDeleted(StageDeletedMessage interpret) => Load();

        public override void Load()
        {
            Stages.Clear();
            var stages = _stageFacade.GetAllList();
            Stages.AddRange(stages);
        }
    }
}
