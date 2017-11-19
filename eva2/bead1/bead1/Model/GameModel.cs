using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using bead1.Persistance;

namespace bead1.Model
{
    public enum Direction { LEFT, RIGHT }

    class GameModel
    {
        private GameTable gameTable;
        private Int32 gameTime;
        private Int32 fuel;
        private Int32 playerPos;

        public GameTable GameTable { get { return gameTable; } }
        public Int32 GameTime { get { return gameTime; } }
        public Int32 Fuel { get { return fuel; } }
        public Boolean IsOver { get { return fuel == 0; } }

        public event EventHandler<GameEventArgs> GameOver;
        public event EventHandler<GameEventArgs> GameStep;

        public GameModel()
        {
            gameTable = new GameTable();
            gameTime = 0;
            fuel = 10;
            playerPos = gameTable.Size / 2;
        }

        public void NewGame()
        {
            gameTable = new GameTable();
            gameTime = 0;
            fuel = 10;
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
            Int32 place = rng.Next(0, gameTable.Size - 1);
            gameTable.SetValue(0, place, Field.FUEL);
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
                for (Int32 j = gameTable.Size - 0; j >= 0; --j)
                {
                    if(gameTable.GetValue(i, j) == Field.FUEL)
                    {
                        if (gameTable.GetValue(i + 1, j) == Field.PLAYER)
                        {
                            fuel++;
                        }
                        else if (i < gameTable.Size - 2)
                        {
                            gameTable.SetValue(i + 1, j, Field.FUEL);
                        }
                        gameTable.SetValue(i, j, Field.ROAD);
                    }
                }
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
                        gameTable.SetValue(gameTable.Size - 1, playerPos, Field.PLAYER);
                        gameTable.SetValue(gameTable.Size - 1, playerPos + 1, Field.ROAD);
                    }
                    break;
                case Direction.RIGHT:
                    if (playerPos < gameTable.Size)
                    {
                        playerPos++;
                        gameTable.SetValue(gameTable.Size - 1, playerPos, Field.PLAYER);
                        gameTable.SetValue(gameTable.Size - 1, playerPos - 1, Field.ROAD);
                    }
                    break;
            }
        }

        private void OnGameAdvance()
        {
            if(GameStep != null)
            {
                GameStep(this, new GameEventArgs(gameTime, fuel, IsOver));
            }
        }

    }
}
