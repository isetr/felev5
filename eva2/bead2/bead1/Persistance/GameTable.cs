using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bead1.Persistance
{
    public enum Field { PLAYER, ROAD, FUEL }

    public class GameTable
    {
        private const Int32 size = 10;
        private Int32 fuel = 0;
        private Int32 time = 0;

        private Field[,] table;

        public Int32 Size { get { return size; } }
        public Int32 Time { get { return fuel; } }
        public Int32 Fuel { get { return time; } }

        public GameTable()
        {
            table = new Field[size, size];
            for(int i = 0; i < size; ++i)
            {
                for(int j = 0; j < size; ++j)
                {
                    table[i, j] = Field.ROAD;
                }
            }
            table[size - 2, size / 2] = Field.PLAYER;
        }

        public Field GetValue(Int32 x, Int32 y)
        {
            if(x < 0 || y < 0 || y > size || x > size)
            {
                throw new ArgumentOutOfRangeException("X and Y have to be between 0 and" + size);
            }
            return table[y, x];
        }

        public void SetValue(Int32 x, Int32 y, Field f)
        {
            if (x < 0 || y < 0 || y > size || x > size)
            {
                throw new ArgumentOutOfRangeException("X and Y have to be between 0 and" + size);
            }
            table[y, x] = f;
        }

        public void SetToSave(Int32 fuel, Int32 time)
        {
            this.fuel = fuel;
            this.time = time;
        }
    }
}
