using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace SimpleXTreeWpf
{
  public class CharInfo
  {
    public Brush Background = Brushes.Black;
    public Brush Foreground = Brushes.Black;
    public char Ch = ' ';
    public bool Dirty = false;
    public string Font = "Consolas";

    public CharInfo(Brush background, Brush foreground, char ch)
    {
      Background = background;
      Foreground = foreground;
      Ch = ch;
    }

    public CharInfo()
    {
    }
  }
}
