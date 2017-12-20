using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bead1.Model
{
    public class GameEventArgs : EventArgs
    {
        private Int32 gameTime;
        private Int32 fuel;
        private Boolean isWon;

        public Int32 GameTime { get { return gameTime; } }
        public Int32 Fuel { get { return fuel; } }
        public Boolean IsWon { get { return isWon; } }

        public GameEventArgs(Int32 gt, Int32 f, Boolean iw)
        {
            gameTime = gt;
            fuel = f;
            isWon = iw;
        }
    }
}
