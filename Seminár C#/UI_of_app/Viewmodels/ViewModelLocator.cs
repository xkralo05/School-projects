using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using ICSproject.BL.Facades;
using ICSproject.BL.Mappers;
using ICSproject.DAL.Entities;
using ICSproject.DAL.Factories;
using ICSproject.DAL.Repositories;
using ICSproject.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;
using UI_of_app.Services;

namespace UI_of_app.ViewModels
{
    public class ViewModelLocator
    {
        private readonly IMediator _mediator;
        private readonly InterpretFacade _interpretFacade;
        private readonly StageFacade _stageFacade;
        private readonly TimeSlotFacade _timeSlotFacade;
        private readonly IMessageBoxService _messageBoxService;
        public InterpretListViewModel InterpretListViewModel => new(_interpretFacade, _mediator);
        public InterpretDetailViewModel InterpretDetailViewModel => new(_interpretFacade, _messageBoxService, _mediator);
        public StageListViewModel StageListViewModel => new(_stageFacade, _mediator);
        public StageDetailViewModel StageDetailViewModel => new(_stageFacade, _messageBoxService, _mediator);
        public TimeSlotConcertViewModel TimeSlotConcertViewModel => new(_timeSlotFacade,_interpretFacade,_stageFacade,_mediator);
        public TimeSlotDetailViewModel TimeSlotDetailViewModel => new(_messageBoxService, _timeSlotFacade, _interpretFacade, _stageFacade, _mediator);

        public MainViewModel MainViewModel { get; }

        public ViewModelLocator()
        {
            _mediator = new Mediator();
            _messageBoxService = new MessageBoxService();

            var dbContextFactory = new DesignTimeDbContextFactory();

            var dbx = dbContextFactory.Create();
            dbx.Database.EnsureCreated();
            var unitOfWork = new UnitOfWork(dbx, dbContextFactory);
            var entityFactory = new EntityBaseFactory(dbx.ChangeTracker);

            var interpretRepository = new RepositoryBase<InterpretEntity>(unitOfWork);
            var interpretMapper = new InterpretMapper();

            _interpretFacade = new InterpretFacade(unitOfWork, interpretRepository, interpretMapper, entityFactory);

            var stageRepository = new RepositoryBase<StageEntity>(unitOfWork);
            var stageMapper = new StageMapper();

            _stageFacade = new StageFacade(unitOfWork, stageRepository, stageMapper, entityFactory);

            var timeSlotRepository = new RepositoryBase<TimeSlotEntity>(unitOfWork);
            var timeSlotMapper = new TimeSlotMapper();

            _timeSlotFacade = new TimeSlotFacade(unitOfWork, timeSlotRepository, timeSlotMapper, entityFactory);
          
            MainViewModel = new MainViewModel(
                InterpretListViewModel,
                InterpretDetailViewModel,
                StageListViewModel,
                StageDetailViewModel,
                TimeSlotConcertViewModel,
                TimeSlotDetailViewModel
                );

        }
    }
}
