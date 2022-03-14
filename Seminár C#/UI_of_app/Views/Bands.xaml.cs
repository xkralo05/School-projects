using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using UI_of_app.Commands;

namespace UI_of_app.Views
{
    /// <summary>
    /// Interaction logic for Bands.xaml
    /// </summary>
    public partial class Bands : UserControlBase
    {
        //public ICommand InterpretNewCommand { get; set; }

        public Bands()
        {
            InitializeComponent();
            //InterpretNewCommand = new RelayCommand(InterpretNew);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
