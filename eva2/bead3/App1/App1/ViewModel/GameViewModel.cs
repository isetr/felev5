using System;
using System.Collections.ObjectModel;
using App1.Model;
using App1.Persistance;

namespace App1.ViewModel
{
    public class GameViewModel : ViewModelBase
    {
        private GameModel model;

        public DelegateCommand NewGameCommand { get; private set; }
        public DelegateCommand ExitGameCommand { get; private set; }
        public DelegateCommand PauseGameCommand { get; private set; }
        public DelegateCommand StepLeft { get; private set; }
        public DelegateCommand StepRight { get; private set; }
        public ObservableCollection<GameField> Fields { get; set; }
        public Int32 Fuel { get { return model.Fuel; } }
        public String GameTime { get { return TimeSpan.FromSeconds(model.GameTime).ToString("g"); } }

        public event EventHandler NewGame;
        public event EventHandler ExitGame;
        public event EventHandler PauseGame;

        public GameViewModel(GameModel m)
        {
            model = m;
            model.GameOver += new EventHandler<GameEventArgs>(Game_GameOver);
            model.GameStep += new EventHandler<GameEventArgs>(Game_GameStep);

            NewGameCommand = new DelegateCommand(param => { OnNewGame(); RefreshTable(); });
            PauseGameCommand = new DelegateCommand(param => { OnPauseGame(); });
            ExitGameCommand = new DelegateCommand(param => OnExitGame());
            StepLeft = new DelegateCommand(param => { model.Step(Direction.LEFT); });
            StepRight = new DelegateCommand(param => { model.Step(Direction.RIGHT); });

            Fields = new ObservableCollection<GameField>();
            for(int i = 0; i < model.GameTable.Size; ++i)
            {
                for(int j = 0; j < model.GameTable.Size; ++j)
                {
                    Fields.Add(new GameField
                    {
                        X = j,
                        Y = i,
                        Type = model.GameTable.GetValue(i, j),
                        Number = i * model.GameTable.Size + j
                    });
                }
            }
        }

        private void RefreshTable()
        {
            foreach(GameField field in Fields)
            {
                field.Type = model.GameTable.GetValue(field.X, field.Y);
            }

            OnPropertyChanged("GameTime");
        }

        private void Game_GameOver(object sender, GameEventArgs e)
        {
            // empty
        }

        private void Game_GameStep(object sender, GameEventArgs e)
        {
            foreach (GameField field in Fields)
            {
                field.Type = model.GameTable.GetValue(field.X, field.Y);
            }
            OnPropertyChanged("GameTime");
            OnPropertyChanged("Fuel");
        }

        private void OnNewGame()
        {
            if(NewGame != null)
            {
                NewGame(this, EventArgs.Empty);
            }
        }

        private void OnPauseGame()
        {
            if(PauseGame != null)
            {
                PauseGame(this, EventArgs.Empty);
            }
        }

        private void OnExitGame()
        {
            if(ExitGame != null)
            {
                ExitGame(this, EventArgs.Empty);
            }
        }
    }
}
