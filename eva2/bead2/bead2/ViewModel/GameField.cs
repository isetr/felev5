using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using bead2.Persistance;

namespace bead2.ViewModel
{
    public class GameField : ViewModelBase
    {
        private Field type;

        public Field Type
        {
            get { return type; }
            set
            {
                type = value;
                OnPropertyChanged();
            }
        }

        public Int32 X { get; set; }
        public Int32 Y { get; set; }
        public Int32 Number { get; set; }
    }
}
