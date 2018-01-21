using System;
using System.Threading.Tasks;

namespace App1.Persistance
{
    public interface IGameDataAccess
    {
        Task<GameTable> LoadAsync(String path);
        Task SaveAsync(String file, GameTable path);
    }
}
