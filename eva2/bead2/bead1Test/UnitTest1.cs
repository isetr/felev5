using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using bead1.Model;
using bead1.Persistance;

namespace bead1Test
{
    [TestClass]
    public class UnitTest1
    {
        private GameModel model;

        [TestInitialize]
        public void Initialize()
        {
            model = new GameModel();

            model.GameStep += new EventHandler<GameEventArgs>(Model_GameStep);
            model.GameOver += new EventHandler<GameEventArgs>(Model_GameOver);
        }

        private void Model_GameStep(Object sender, GameEventArgs e)
        {
            Assert.IsTrue(model.GameTime >= 0);
            Assert.AreEqual(model.GameTime == 0, model.IsOver);

            Assert.AreEqual(e.Fuel, model.Fuel); // a két értéknek egyeznie kell
            Assert.AreEqual(e.GameTime, model.GameTime); // a két értéknek egyeznie kell
            Assert.IsFalse(e.IsWon); // még nem nyerték meg a játékot
        }

        private void Model_GameOver(Object sender, GameEventArgs e)
        {
            Assert.IsTrue(model.IsOver); // biztosan vége van a játéknak
            Assert.AreEqual(0, e.Fuel); // a tesztben csak akkor váltódhat ki, ha elfogy az idő
            Assert.IsFalse(e.IsWon);
        }

        [TestMethod]
        public void NewGame()
        {
            model.NewGame();

            Assert.AreEqual(20, model.Fuel);
            Assert.AreEqual(0, model.GameTime);

            Int32 player = 0;
            Int32 fuel = 0;

            for(Int32 i = 0; i < 10; i++)
            {
                for(Int32 j = 0; j < 10; i++)
                {
                    switch(model.GameTable.GetValue(i, j))
                    {
                        case Field.FUEL: fuel++; break;
                        case Field.PLAYER: player++; break;
                    }
                }
            }

            Assert.AreEqual(0, fuel);
            Assert.AreEqual(1, player);
        }

        public void NewFuel()
        {
            model.NewGame();

            Assert.AreEqual(20, model.Fuel);
            Assert.AreEqual(0, model.GameTime);

            Int32 player = 0;
            Int32 fuel = 0;

            model.SpawnNewFuel();

            for (Int32 i = 0; i < 10; i++)
            {
                for (Int32 j = 0; j < 10; i++)
                {
                    switch (model.GameTable.GetValue(i, j))
                    {
                        case Field.FUEL: fuel++; break;
                        case Field.PLAYER: player++; break;
                    }
                }
            }

            Assert.AreEqual(1, fuel);
            Assert.AreEqual(1, player);
        }

        public void Step()
        {
            model.NewGame();

            Assert.AreEqual(20, model.Fuel);
            Assert.AreEqual(0, model.GameTime);

            Int32 player = 0;
            Int32 fuel = 0;

            for (Int32 i = 0; i < 10; i++)
            {
                for (Int32 j = 0; j < 10; i++)
                {
                    switch (model.GameTable.GetValue(i, j))
                    {
                        case Field.FUEL: fuel++; break;
                        case Field.PLAYER: player++; break;
                    }
                }
            }

            Assert.AreEqual(0, fuel);
            Assert.AreEqual(1, player);

            Assert.AreEqual(Field.PLAYER, model.GameTable.GetValue(8, 5));
            model.Step(Direction.LEFT);
            Assert.AreEqual(Field.PLAYER, model.GameTable.GetValue(8, 4));
            model.Step(Direction.RIGHT);
            Assert.AreEqual(Field.PLAYER, model.GameTable.GetValue(8, 5));
        }
    }
}
