using UI_of_app.Commands;
using ICSproject.BL.Facades;
using ICSproject.BL.Mappers;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Input;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using UI_of_app.Extensions;
using UI_of_app.Messages;
using UI_of_app.Services;
using UI_of_app.ViewModels;
using System.Windows;
using System.Linq;

namespace UI_of_app.ViewModels
{
    public class TimeSlotConcertViewModel : ViewModelBase
    {
        private readonly TimeSlotFacade _timeSlotFacade;
        private readonly IMediator _mediator;
        public ObservableCollection<TimeSlotConcert> TimeSlots { get; set; } = new ObservableCollection<TimeSlotConcert>();

        public ObservableCollection<TimeSlotConcert> SelectedTimeSlots { get; set; } = new ObservableCollection<TimeSlotConcert>();
        public ObservableCollection<InterpretListModel> Interprets { get; set; } = new ObservableCollection<InterpretListModel>();
        public ObservableCollection<StageListModel> Stages { get; set; } = new ObservableCollection<StageListModel>();

        public ICommand TimeSlotSelectedCommand { get; }
        public ICommand TimeSlotNewCommand { get; }
        public ICommand ResetFilterCommand { get; }
        public ICommand ApplyFilterCommand { get; }


        public StageListModel StageFilter { get; set; } = new StageListModel();

        public InterpretListModel InterpretFilter { get; set; } = new InterpretListModel();


        public void ResetFilter()
        {
            InterpretFilter.Name = null;
            StageFilter.Name = null;
            SelectedTimeSlots.Clear();
            var stages = _timeSlotFacade.GetAllList();
            SelectedTimeSlots.AddRange(stages);
        }

        public void ApplyFilter(){
            SelectedTimeSlots.Clear();

            IEnumerable<TimeSlotConcert> toAdd;
            if(string.IsNullOrEmpty(StageFilter.Name))
                toAdd = (TimeSlots.Where(slot => slot.InterpretName == InterpretFilter.Name).OrderBy(slot => slot.SlotStart).ToArray()) as IEnumerable<TimeSlotConcert>;
            else if (string.IsNullOrEmpty(InterpretFilter.Name))
                toAdd = (TimeSlots.Where(slot => slot.Stage == StageFilter.Name).OrderBy(slot => slot.SlotStart).ToArray()) as IEnumerable<TimeSlotConcert>;
            else
                toAdd = (TimeSlots.Where(slot => slot.Stage == StageFilter.Name && slot.InterpretName == InterpretFilter.Name)
                    .OrderBy(slot => slot.SlotStart).ToArray()) as IEnumerable<TimeSlotConcert>;

            SelectedTimeSlots.AddRange(toAdd);
        }


        public TimeSlotConcertViewModel(TimeSlotFacade timeSlotFacade, InterpretFacade interpretFacade, StageFacade stageFacade, IMediator mediator
            )
        {
            _timeSlotFacade = timeSlotFacade;
            _mediator = mediator;

            TimeSlotSelectedCommand = new RelayCommand<TimeSlotConcert>(TimeSlotSelected);
            TimeSlotNewCommand = new RelayCommand(TimeSlotNew);
            ApplyFilterCommand = new RelayCommand(ApplyFilter);
            ResetFilterCommand = new RelayCommand(ResetFilter);
            var interprets = interpretFacade.GetAllList();
            Interprets.AddRange<InterpretListModel>(interprets);
            var stages = stageFacade.GetAllList();
            Stages.AddRange<StageListModel>(stages);
            _mediator.Register<TimeSlotAddedMessage>(TimeSlotAdded);
            _mediator.Register<TimeSlotUpdatedMessage>(TimeSlotUpdated);
            _mediator.Register<TimeSlotDeletedMessage>(TimeSlotDeleted);

            EditViewVisibility = Visibility.Hidden;
        }




        private void TimeSlotNew()
        {
            EditViewVisibility = Visibility.Visible;
            _mediator.Send(new TimeSlotNewMessage());

        }

        private void TimeSlotSelected(TimeSlotConcert interpret)
        {
            EditViewVisibility = Visibility.Visible;
            _mediator.Send(new TimeSlotSelectedMessage { Id = interpret.Id });
        }

        private void TimeSlotAdded(TimeSlotAddedMessage interpret) => Load();

        private void TimeSlotUpdated(TimeSlotUpdatedMessage interpret) => Load();

        private void TimeSlotDeleted(TimeSlotDeletedMessage interpret) => Load();

        public Visibility EditViewVisibility
        {
            get => editViewVisibility; set
            {
                editViewVisibility = value;
                OnPropertyChanged();
            }
        }

        private Visibility editViewVisibility;

        public override void Load()
        {

            TimeSlots.Clear();
            SelectedTimeSlots.Clear();
            var stages = _timeSlotFacade.GetAllList();
            TimeSlots.AddRange(stages);
            SelectedTimeSlots.AddRange(stages);

        }
    }
}
