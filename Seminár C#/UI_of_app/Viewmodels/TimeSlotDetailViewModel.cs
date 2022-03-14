using UI_of_app.Commands;
using ICSproject.BL.Facades;
using ICSproject.BL.Mappers;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using ICSproject.BL.Models.DetailModels;
using ICSproject.BL.Models.ListModels;
using UI_of_app.Messages;
using UI_of_app.Services;
using UI_of_app.ViewModels;
using System.Collections.ObjectModel;
using UI_of_app.Extensions;

namespace UI_of_app.ViewModels
{
    public class TimeSlotDetailViewModel : ViewModelBase
    {
        private readonly TimeSlotFacade _timeSlotFacade;
        private readonly InterpretFacade _interpretFacade;
        private readonly StageFacade _stageFacade;
        private readonly IMessageBoxService _messageBoxService;
        private readonly IMediator _mediator;
        public ICollection<InterpretListModel> Interprets { get; set; }
        public ICollection<StageListModel> Stages { get; set; }
        public TimeSlotDetailModel Model { get; set; }
        public ICommand SaveCommand { get; set; }
        public ICommand DeleteCommand { get; set; }
        public ICommand LoadImageCommand { get; set; }


        public TimeSlotDetailViewModel(
            IMessageBoxService messageBoxService, TimeSlotFacade timeSlotFacade, InterpretFacade interpretFacade, StageFacade stageFacade, IMediator mediator
        )
        {
            _timeSlotFacade = timeSlotFacade;
            _messageBoxService = messageBoxService;
            _mediator = mediator;
            _interpretFacade = interpretFacade;
            _stageFacade = stageFacade;
            SaveCommand = new RelayCommand(Save, CanSave);
            DeleteCommand = new RelayCommand(Delete);
            LoadImageCommand = new RelayCommand(LoadImage);

            var interprets = _interpretFacade.GetAllList();
            Interprets = (ICollection<InterpretListModel>)interprets;
            var stages = _stageFacade.GetAllList();
            Stages = (ICollection<StageListModel>)stages;

            _mediator.Register<TimeSlotSelectedMessage>(TimeSlotSelected);
            _mediator.Register<TimeSlotNewMessage>(TimeSlotNew);
        }

        private void TimeSlotNew(TimeSlotNewMessage interpretNewMessage)
        {
            Interprets = (ICollection<InterpretListModel>)_interpretFacade.GetAllList();
            Stages = (ICollection<StageListModel>)_stageFacade.GetAllList();
            Model = new TimeSlotDetailModel();
        }

        private void TimeSlotSelected(TimeSlotSelectedMessage interpretSelectedMessage)
            => Model = _timeSlotFacade.GetById(interpretSelectedMessage.Id);

        public void Save()
        {
            if (Model.Id == Guid.Empty)
            {
                _timeSlotFacade.Save(Model);
                _mediator.Send(new TimeSlotAddedMessage { Id = Model.Id });
            }
            else
            {
                _timeSlotFacade.Save(Model);
                _mediator.Send(new TimeSlotUpdatedMessage { Id = Model.Id });
            }
        }

        private bool CanSave() =>
            Model != null
            && Model.SlotStart.CompareTo(Model.SlotEnd) < 0;

        public void Delete()
        {
            if (Model.Id != Guid.Empty)
            {
                try
                {
                    _timeSlotFacade.Delete(Model.Id);
                }
                catch
                {
                    _messageBoxService.Show($"Deleting of {Model?.Id} failed!", "Deleting failed", MessageBoxButton.OK);
                }

                _mediator.Send(new TimeSlotDeletedMessage { Id = Model.Id });
            }

            Model = null;
        }

        


        public void LoadImage()
        {
            ImageSource = "";
        }


        public string ImageSource
        {
            get
            {
                if (Model==null || Model.Interpret == null)
                    return null;
                var interpret= _interpretFacade.GetById(Model.Interpret.Id);
                if (interpret == null)
                    return null;

                return interpret.ImageSrc;
            }
            set
            {
                OnPropertyChanged();
            }
        }
    }
}