using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bead1.Persistance
{
    public enum Field { PLAYER, ROAD, FUEL }

    class GameTable
    {
        private const Int32 size = 10;

        private Field[,] table;

        public Int32 Size { get { return size; } }

        public GameTable()
        {
            table = new Field[size, size];
            table[size - 1, size / 2] = Field.PLAYER;
        }

        public Field GetValue(Int32 x, Int32 y)
        {
            if(x < 0 || y < 0 || y > size || x > size)
            {
                throw new ArgumentOutOfRangeException("X and Y have to be between 0 and" + size);
            }
            return table[x,y];
        }

        public void SetValue(Int32 x, Int32 y, Field f)
        {
            if (x < 0 || y < 0 || y > size || x > size)
            {
                throw new ArgumentOutOfRangeException("X and Y have to be between 0 and" + size);
            }
            table[x, y] = f;
        }
    }
}
