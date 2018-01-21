using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using App1.Persistance;

namespace App1.Model
{
    public enum Direction { LEFT, RIGHT }

    public class GameModel
    {
        private GameTable gameTable;
        private Int32 gameTime;
        private Int32 fuel;
        private Int32 playerPos;
        private GameDataAccess dataAccess;

        public GameTable GameTable { get { return gameTable; } }
        public Int32 GameTime { get { return gameTime; } }
        public Int32 Fuel { get { return fuel; } }
        public Boolean IsOver { get { return fuel == 0; } }

        public event EventHandler<GameEventArgs> GameOver;
        public event EventHandler<GameEventArgs> GameStep;

        public GameModel()
        {
            dataAccess = new GameDataAccess();
            gameTable = new GameTable();
            gameTime = 0;
            fuel = 20;
            playerPos = gameTable.Size / 2;
        }

        public void NewGame()
        {
            gameTable = new GameTable();
            gameTime = 0;
            fuel = 20;
            playerPos = gameTable.Size / 2;
        }

        public void AdvanceTime()
        {
            if (IsOver)
            {
                return;
            }

            gameTime++;
            OnGameAdvance();
        }

        public void SpawnNewFuel()
        {
            if (IsOver)
            {
                return;
            }
            Random rng = new Random();
            Int32 place = rng.Next(0, gameTable.Size - 2);
            gameTable.SetValue(place, 0, Field.FUEL);
            OnGameAdvance();
        }

        public void AdvancePlayer()
        {
            if (IsOver)
            {
                return;
            }
            for(Int32 i = 0; i < gameTable.Size; ++i)
            {
                for (Int32 j = gameTable.Size - 1; j >= 0 ; --j)
                {
                    if(gameTable.GetValue(i, j) == Field.FUEL)
                    {
                        if (gameTable.GetValue(i, j + 1) == Field.PLAYER)
                        {
                            fuel += 11;
                        }
                        else if (j < gameTable.Size - 2)
                        {
                            gameTable.SetValue(i, j + 1, Field.FUEL);
                        }
                        gameTable.SetValue(i, j, Field.ROAD);
                    }
                }
            }
            --fuel;
            if(fuel == 0)
            {
                OnGameOver();
            }
            OnGameAdvance();
        }

        public void Step(Direction d)
        {
            switch(d)
            {
                case Direction.LEFT:
                    if(playerPos > 0)
                    {
                        playerPos--;
                        gameTable.SetValue(playerPos, gameTable.Size - 2, Field.PLAYER);
                        gameTable.SetValue(playerPos + 1, gameTable.Size - 2, Field.ROAD);
                    }
                    break;
                case Direction.RIGHT:
                    if (playerPos < gameTable.Size - 1)
                    {
                        playerPos++;
                        gameTable.SetValue(playerPos, gameTable.Size - 2, Field.PLAYER);
                        gameTable.SetValue(playerPos - 1, gameTable.Size - 2, Field.ROAD);
                    }
                    break;
            }
            OnGameAdvance();
        }

        private void OnGameAdvance()
        {
            if(GameStep != null)
            {
                GameStep(this, new GameEventArgs(gameTime, fuel, IsOver));
            }
        }

        private void OnGameOver()
        {
            if (GameOver != null)
            {
                GameOver(this, new GameEventArgs(gameTime, fuel, IsOver));
            }
        }

        public async Task SaveGameAsync(String path)
        {
            if(dataAccess == null)
            {
                throw new Exception();
            }
            gameTable.SetToSave(fuel, gameTime);
            await dataAccess.SaveAsync(path, gameTable);
        }

        public async Task LoadGameAsync(String path)
        {
            if (dataAccess == null)
            {
                throw new Exception();
            }

            gameTable = await dataAccess.LoadAsync(path);
            gameTime = gameTable.Time;
            fuel = gameTable.Fuel;
        }
    }
}
