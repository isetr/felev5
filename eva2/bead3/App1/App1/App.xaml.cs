using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Xamarin.Forms;
using App1.Model;
using App1.Persistance;
using App1.ViewModel;
using DLToolkit.Forms.Controls;


namespace App1
{
	public partial class App : Application
    {
        private GameModel model;
        private GameViewModel viewModel;
        private MainPage view;
        private Boolean isPaused;
        private Boolean timer;
        private Boolean spawnTimer;

        public App ()
		{
            InitializeComponent();
            FlowListView.Init();

            model = new GameModel();
            model.GameOver += new EventHandler<GameEventArgs>(Game_GameOver);

            viewModel = new GameViewModel(model);
            viewModel.NewGame += new EventHandler(ViewModel_NewGame);
            viewModel.PauseGame += new EventHandler(ViewModel_PauseGame);

            view = new App1.MainPage();
            view.BindingContext = viewModel;

            MainPage = view;
        }

        private void ViewModel_NewGame(Object sender, EventArgs e)
        {
            model.NewGame();
            timer = true;
            spawnTimer = true;
            Device.StartTimer(TimeSpan.FromSeconds(1), () => {
                model.AdvanceTime();
                model.SpawnNewFuel();
                return timer;
            });
            Device.StartTimer(TimeSpan.FromMilliseconds(1000), () => { model.AdvancePlayer(); return spawnTimer; });
        }

        private void ViewModel_PauseGame(Object sender, EventArgs e)
        {
            if (timer)
            {
                timer = false;
                spawnTimer = false;
            }
            else
            {
                timer = true;
                spawnTimer = true;
                Device.StartTimer(TimeSpan.FromSeconds(1), () => { model.AdvanceTime(); model.SpawnNewFuel(); return timer; });
                Device.StartTimer(TimeSpan.FromMilliseconds(1), () => { model.AdvancePlayer(); return spawnTimer; });
            }
        }

        private void Game_GameOver(Object sender, GameEventArgs e)
        {
            timer = false;
            spawnTimer = false;

            MainPage.DisplayAlert(
                    "Vegtelen futam",
                    "Sajnos elfogyott az üzemanyag." + Environment.NewLine +
                    "Az időd: " + TimeSpan.FromSeconds(e.GameTime).ToString("g"),
                    "OK"
                );
        }

        protected override void OnStart ()
		{
            // Handle when your app starts

            isPaused = false;
            timer = true;
            spawnTimer = true;
            
            Device.StartTimer(TimeSpan.FromSeconds(1), () => { model.AdvanceTime(); model.SpawnNewFuel(); return timer; });
            Device.StartTimer(TimeSpan.FromMilliseconds(1), () => { model.AdvancePlayer(); return spawnTimer; });
        }

		protected override void OnSleep ()
		{
			// Handle when your app sleeps
		}

		protected override void OnResume ()
		{
			// Handle when your app resumes
		}
	}
}
