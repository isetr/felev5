using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace bead1.Persistance
{
    public interface IGameDataAccess
    {
        Task<GameTable> LoadAsync(String path);
        Task SaveAsync(String file, GameTable path);
    }
}
