using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using ICSproject.BL.Models.ListModels;
using UI_of_app.Commands;
using UI_of_app.Factories;

namespace UI_of_app.ViewModels
{
    public class MainViewModel : ViewModelBase
    {
        public ICommand InterpretNewCommand { get; }
        public ICommand InterpretSelectedCommand { get; }

        public ICommand StageNewCommand { get; }
        public ICommand StageSelectedCommand { get; }

        public ICommand ApplyFilterCommand { get; }
        public ICommand ResetFilterCommand { get; }

        public ICommand TimeSlotNewCommand { get; }
        public ICommand TimeSlotSelectedCommand { get; }

        public InterpretListModel InterpretFilter { get; set; }
        public StageListModel StageFilter { get; set; }

        public ObservableCollection<InterpretListModel> Interprets { get; }
        public ObservableCollection<StageListModel> Stages { get; }
        public ObservableCollection<TimeSlotConcert> TimeSlots { get; }


        public InterpretListViewModel InterpretListViewModel;
        public InterpretDetailViewModel InterpretDetailViewModel;
        public StageListViewModel StageListViewModel;
        public StageDetailViewModel StageDetailViewModel;
        public TimeSlotConcertViewModel TimeSlotConcertViewModel;
        public TimeSlotDetailViewModel TimeSlotDetailViewModel;

        public MainViewModel(
            InterpretListViewModel interpretListViewModel, 
            InterpretDetailViewModel interpretDetailViewModel,
            StageListViewModel stageListViewModel,
            StageDetailViewModel stageDetailViewModel,
            TimeSlotConcertViewModel timeSlotConcertViewModel,
            TimeSlotDetailViewModel timeSlotDetailViewModel) 
        {
            InterpretNewCommand = interpretListViewModel.InterpretNewCommand;
            InterpretSelectedCommand = interpretListViewModel.InterpretSelectedCommand;
            StageNewCommand = stageListViewModel.StageNewCommand;
            StageSelectedCommand = stageListViewModel.StageSelectedCommand;
            TimeSlotNewCommand = timeSlotConcertViewModel.TimeSlotNewCommand;
            TimeSlotSelectedCommand = timeSlotConcertViewModel.TimeSlotSelectedCommand;

            Interprets = interpretListViewModel.Interprets;
            Stages = stageListViewModel.Stages;
            TimeSlots = timeSlotConcertViewModel.TimeSlots;

            InterpretListViewModel = interpretListViewModel;
            InterpretDetailViewModel = interpretDetailViewModel;
            StageListViewModel = stageListViewModel;
            StageDetailViewModel = stageDetailViewModel;
            TimeSlotConcertViewModel = timeSlotConcertViewModel;
            TimeSlotDetailViewModel = timeSlotDetailViewModel;

            InterpretListViewModel.Load();
            StageListViewModel.Load();
            TimeSlotConcertViewModel.Load();

            MenuVisibility = Visibility.Hidden;
            HomeVisibility = Visibility.Visible;
            StagesVisibility = Visibility.Hidden;
            BandsVisibility = Visibility.Hidden;
            ProgramVisibility = Visibility.Hidden;
            GoToMenuCommand = new RelayCommand(GoToMenuExecute);
            GoToStagesCommand = new RelayCommand(GoToStagesExecute);
            GoToProgramCommand = new RelayCommand(GoToProgramExecute);
            GoToBandsCommand = new RelayCommand(GoToBandsExecute);

            ApplyFilterCommand = timeSlotConcertViewModel.ApplyFilterCommand;
            ResetFilterCommand = timeSlotConcertViewModel.ResetFilterCommand;
            InterpretFilter = timeSlotConcertViewModel.InterpretFilter;
            StageFilter = timeSlotConcertViewModel.StageFilter;
            TimeSlots = timeSlotConcertViewModel.SelectedTimeSlots;
            
        }
        public ICommand GoToMenuCommand { get; }
        public ICommand GoToStagesCommand { get; }
        public ICommand GoToProgramCommand { get; }
        public ICommand GoToBandsCommand { get; }
        public Visibility MenuVisibility
        {
            get => menuVisibility; set
            {
                menuVisibility = value;
                OnPropertyChanged();
            }
        }
        public Visibility HomeVisibility
        {
            get => homeVisibility; set
            {
                homeVisibility = value;
                OnPropertyChanged();
            }
        }
        public Visibility StagesVisibility
        {
            get => stagesVisibility; set
            {
                stagesVisibility = value;
                OnPropertyChanged();
            }
        }
        public Visibility BandsVisibility
        {
            get => bandsVisibility; set
            {
                bandsVisibility = value;
                OnPropertyChanged();
            }
        }
        public Visibility ProgramVisibility
        {
            get => programVisibility; set
            {
                programVisibility = value;
                OnPropertyChanged();
            }
        }

        private Visibility menuVisibility;
        private Visibility homeVisibility;
        private Visibility stagesVisibility;
        private Visibility programVisibility;
        private Visibility bandsVisibility;

        private void GoToMenuExecute()
        {
            MenuVisibility = Visibility.Visible;
            HomeVisibility = Visibility.Hidden;
            StagesVisibility = Visibility.Hidden;
            BandsVisibility = Visibility.Hidden;
            ProgramVisibility = Visibility.Hidden;
        }
        private void GoToStagesExecute()
        {
            MenuVisibility = Visibility.Hidden;
            HomeVisibility = Visibility.Hidden;
            StagesVisibility = Visibility.Visible;
            BandsVisibility = Visibility.Hidden;
            ProgramVisibility = Visibility.Hidden;
        }
        private void GoToProgramExecute()
        {
            MenuVisibility = Visibility.Hidden;
            HomeVisibility = Visibility.Hidden;
            StagesVisibility = Visibility.Hidden;
            BandsVisibility = Visibility.Hidden;
            ProgramVisibility = Visibility.Visible;
        }
        private void GoToBandsExecute()
        {
            MenuVisibility = Visibility.Hidden;
            HomeVisibility = Visibility.Hidden;
            StagesVisibility = Visibility.Hidden;
            BandsVisibility = Visibility.Visible;
            ProgramVisibility = Visibility.Hidden;
        }
    }
    

}
