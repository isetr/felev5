using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace bead2.Persistance
{
    class GameDataAccess : IGameDataAccess
    {
        public async Task<GameTable> LoadAsync(String path)
        {
            try
            {
                using (StreamReader reader = new StreamReader(path))
                {
                    String line = await reader.ReadLineAsync();
                    String[] parts = line.Split(' ');
                    Int32 size = Int32.Parse(parts[0]);
                    Int32 fuel = Int32.Parse(parts[1]);
                    Int32 time = Int32.Parse(parts[2]);
                    GameTable table = new GameTable();
                    for(Int32 i = 0; i < size; ++i)
                    {
                        line = await reader.ReadLineAsync();
                        parts = line.Split(' ');
                        for(Int32 j = 0; j < size; ++j)
                        {
                            switch(parts[j])
                            {
                                case "P": table.SetValue(i, j, Field.PLAYER); break;
                                case "F": table.SetValue(i, j, Field.FUEL); break;
                                case "R": table.SetValue(i, j, Field.ROAD); break;
                            }
                        }
                    }
                    table.SetToSave(fuel, time);
                    return table;
                }
            }
            catch
            {
                throw new Exception();
            }
        }

        public async Task SaveAsync(String path, GameTable table)
        {
            try
            {
                using (StreamWriter writer = new StreamWriter(path))
                {
                    writer.Write(table.Size);
                    await writer.WriteLineAsync(" " + table.Fuel + " " + table.Time);
                    for(Int32 i = 0; i < table.Size; ++i)
                    {
                        for(Int32 j = 0; j < table.Size; ++j)
                        {
                            switch(table.GetValue(i, j))
                            {
                                case Field.FUEL: await writer.WriteAsync("F "); break;
                                case Field.PLAYER: await writer.WriteAsync("P "); break;
                                case Field.ROAD: await writer.WriteAsync("R "); break;
                            }
                            
                        }
                        await writer.WriteLineAsync();
                    }
                }
            }
            catch
            {
                throw new Exception();
            }
        }
    }
}
