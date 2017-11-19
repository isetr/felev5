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

        public GameView()
        {
            InitializeComponent();
        }

        private void GameView_Load(object sender, EventArgs e)
        {

        }

        private void toolStripStatusLabel1_Click(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
