using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Threading;
using bead2.Model;
using bead2.Persistance;
using bead2.ViewModel;

namespace bead2
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private GameModel model;
        private GameViewModel viewModel;
        private MainWindow view;
        private DispatcherTimer timer;
        private DispatcherTimer fuelTimer;
        private DispatcherTimer stepTimer;

        public App()
        {
            Startup += new StartupEventHandler(App_Startup);
        }

        private void App_Startup(object sender, StartupEventArgs e)
        {
            model = new GameModel();
            model.GameOver += new EventHandler<GameEventArgs>(Game_GameOver);

            viewModel = new GameViewModel(model);
            viewModel.NewGame += new EventHandler(ViewModel_NewGame);
            viewModel.ExitGame += new EventHandler(ViewModel_ExitGame);
            viewModel.PauseGame += new EventHandler(ViewModel_PauseGame);

            view = new bead2.MainWindow();
            view.DataContext = viewModel;
            view.Show();

            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(1000);
            timer.Tick += new EventHandler(Timer_Tick);

            fuelTimer = new DispatcherTimer();
            fuelTimer.Interval = TimeSpan.FromMilliseconds(3000);
            fuelTimer.Tick += new EventHandler(FuelTimer_Tick);

            stepTimer = new DispatcherTimer();
            stepTimer.Interval = TimeSpan.FromMilliseconds(1000);
            stepTimer.Tick += new EventHandler(StepTimer_Tick);
        }

        private void Timer_Tick(Object sender, EventArgs e)
        {
            model.AdvanceTime();
            model.SpawnNewFuel();
            if (model.GameTime < 20)
            {
                fuelTimer.Interval = TimeSpan.FromMilliseconds(3000);
                stepTimer.Interval = TimeSpan.FromMilliseconds(1000);
            }
            else if (model.GameTime < 40)
            {
                fuelTimer.Interval = TimeSpan.FromMilliseconds(2500);
                stepTimer.Interval = TimeSpan.FromMilliseconds(700);
            }
            else if (model.GameTime < 60)
            {
                fuelTimer.Interval = TimeSpan.FromMilliseconds(2000);
                stepTimer.Interval = TimeSpan.FromMilliseconds(500);
            }
            else if (model.GameTime < 120)
            {
                stepTimer.Interval = TimeSpan.FromMilliseconds(500);
            }
            else if (model.GameTime < 180)
            {
                stepTimer.Interval = TimeSpan.FromMilliseconds(200);
            }
        }

        private void FuelTimer_Tick(Object sender, EventArgs e)
        {
            model.SpawnNewFuel();
        }

        private void StepTimer_Tick(Object sender, EventArgs e)
        {
            model.AdvancePlayer();
        }

        private void ViewModel_NewGame(Object sender, EventArgs e)
        {
            model.NewGame();
            timer.Start();
            fuelTimer.Start();
            stepTimer.Start();
        }

        private void ViewModel_ExitGame(Object sender, EventArgs e)
        {
            view.Close();
        }

        private void ViewModel_PauseGame(Object sender, EventArgs e)
        {
            if (timer.IsEnabled)
            {
                timer.Stop();
                fuelTimer.Stop();
                stepTimer.Stop();
            }
            else
            {
                timer.Start();
                fuelTimer.Start();
                stepTimer.Start();
            }
        }

        private void Game_GameOver(Object sender, GameEventArgs e)
        {
            timer.Stop();
            fuelTimer.Stop();
            stepTimer.Stop();

            MessageBox.Show(
                    "Sajnos elfogyott az üzemanyag." + Environment.NewLine +
                    "Az időd: " + TimeSpan.FromSeconds(e.GameTime).ToString("g"),
                    "Vegtelen futam",
                    MessageBoxButton.OK,
                    MessageBoxImage.Asterisk
                );
        }
    }
}
