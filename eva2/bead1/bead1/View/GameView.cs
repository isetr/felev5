using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using bead1.Model;
using bead1.Persistance;

namespace bead1.View
{
    public partial class GameView : Form
    {
        private GameModel gameModel;
        private Timer timer;
        private Timer fuelTimer;
        private Timer stepTimer;

        public GameView()
        {
            InitializeComponent();

            saveFileDialog = new SaveFileDialog();
            openFileDialog = new OpenFileDialog();

            if (gameModel == null)
            {
                gameModel = new GameModel();
                gameModel.GameOver += new EventHandler<GameEventArgs>(Game_GameOver);
                gameModel.GameStep += new EventHandler<GameEventArgs>(Game_GameStep);
            }

            if (timer == null)
            {
                timer = new Timer();
                timer.Interval = 1000;
                timer.Tick += new EventHandler(Timer_Tick);
            }

            if (fuelTimer == null)
            {
                fuelTimer = new Timer();
                fuelTimer.Interval = 3000;
                fuelTimer.Tick += new EventHandler(FuelTimer_Tick);
            }

            if (stepTimer == null)
            {
                stepTimer = new Timer();
                stepTimer.Interval = 1000;
                stepTimer.Tick += new EventHandler(StepTimer_Tick);
            }
        }

        private void GameView_Load(object sender, EventArgs e)
        {

        }

        private void újJátékToolStripMenuItem_Click(object sender, EventArgs e)
        {
            gameModel.NewGame();

            timer.Start();
            fuelTimer.Start();
            stepTimer.Start();
        }

        private void Game_GameStep(Object sender, GameEventArgs e)
        {
            toolStripStatusLabel2.Text = TimeSpan.FromSeconds(e.GameTime).ToString("g");
            toolStripStatusLabel4.Text = e.Fuel.ToString();

            panel1.Refresh();
        }
        private void Game_GameOver(Object sender, GameEventArgs e)
        {
            timer.Stop();
            fuelTimer.Stop();
            stepTimer.Stop();
            
            MessageBox.Show(
                    "Sajnos elfogyott az üzemanyag." + Environment.NewLine +
                    "Az időd: " + TimeSpan.FromSeconds(e.GameTime).ToString("g"),
                    "Vegtelen futam",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Asterisk
                );
        }

        private void Timer_Tick(Object sender, EventArgs e)
        {
            gameModel.AdvanceTime();
            panel1.Refresh();
            if(gameModel.GameTime < 20)
            {
                fuelTimer.Interval = 3000;
                stepTimer.Interval = 1000;
            }
            else if (gameModel.GameTime < 40)
            {
                fuelTimer.Interval = 2500;
                stepTimer.Interval = 700;
            }
            else if (gameModel.GameTime < 60)
            {
                fuelTimer.Interval = 2000;
                stepTimer.Interval = 500;
            }
            else if (gameModel.GameTime < 120)
            {
                stepTimer.Interval = 500;
            }
            else if (gameModel.GameTime < 180)
            {
                stepTimer.Interval = 200;
            }
        }

        private void FuelTimer_Tick(Object sender, EventArgs e)
        {
            gameModel.SpawnNewFuel();
            panel1.Refresh();
        }

        private void StepTimer_Tick(Object sender, EventArgs e)
        {
            gameModel.AdvancePlayer();
            panel1.Refresh();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            if(gameModel != null)
            {
                var p = sender as Panel;
                var g = e.Graphics;

                for(int i = 0; i < 10; i++)
                {
                    for(int j = 0; j < 10; j++)
                    {
                        Point[] points = new Point[4];
                        points[0] = new Point(i * 50, j * 50);
                        points[1] = new Point(i * 50, j * 50 + 50);
                        points[2] = new Point(i * 50 + 50, j * 50 + 50);
                        points[3] = new Point(i * 50 + 50, j * 50);

                        switch (gameModel.GameTable.GetValue(i, j)) {
                            case Field.ROAD:
                                g.FillPolygon(new SolidBrush(Color.Black), points);
                                break;
                            case Field.PLAYER:
                                g.FillPolygon(new SolidBrush(Color.White), points);
                                break;
                            case Field.FUEL:
                                g.FillPolygon(new SolidBrush(Color.Green), points);
                                break;
                        }
                    }
                }
            }
        }

        private void GameView_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(e.KeyChar == 'a')
            {
                gameModel.Step(Direction.LEFT);
                panel1.Refresh();
            }
            else if (e.KeyChar == 'd')
            {
                gameModel.Step(Direction.RIGHT);
                panel1.Refresh();
            }
        }

        private void kilépésToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private async void mentésToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    await gameModel.SaveGameAsync(saveFileDialog.FileName);
                }
                catch (Exception)
                {
                    gameModel.NewGame();
                }
            }
        }

        private async void betöltésToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    await gameModel.LoadGameAsync(openFileDialog.FileName);
                    panel1.Refresh();
                    timer.Start();
                    fuelTimer.Start();
                    stepTimer.Start();
                }
                catch (Exception)
                {
                    MessageBox.Show("Játék betöltése sikertelen!" + Environment.NewLine + "Hibás az elérési út, vagy a fájlformátum.", "Hiba!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void szünetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(timer.Enabled)
            {
                timer.Stop();
                fuelTimer.Stop();
                stepTimer.Stop();
            }
            else
            {
                timer.Start();
                fuelTimer.Start();
                stepTimer.Start();
            }
        }
    }
}
