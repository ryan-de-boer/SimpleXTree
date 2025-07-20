using SimpleXTreeWpf;
using System.Globalization;
using System.Windows;
using System.Windows.Media;

namespace WpfAppTest
{


  public class DrawingVisualHost : FrameworkElement
  {
    private readonly DrawingVisual _visual = new DrawingVisual();

    private void PrintString(CharInfo[,] screen, int x, int y, string str, Brush bg, Brush fg)
    {
      for (int i = 0; i < str.Length; i++)
      {
        screen[x + i, y] = new CharInfo();
        screen[x + i, y].Ch = str[i];
        screen[x + i, y].Background = bg;
        screen[x + i, y].Foreground = fg;
        screen[x + i, y].Dirty = true;
      }
    }
    string TabPath = "D:\\";

    public DrawingVisualHost(double fontSize)
    {
      AddVisualChild(_visual);


      //

      //      using (DrawingContext dc = m_vis.RenderOpen())
      {

        CharInfo[,] screen = new CharInfo[80, 50];
        for (int r = 0; r < 80; r++)
        {
          for (int c = 0; c < 50; c++)
          {
            screen[r, c] = new CharInfo();
          }
        }

        int xa = 0;
        string pathSpot = "                                                ";
        //        TabPath
        //        string str1 = " Path: D:\\                                              Fr 18-07-25  6:46:12 pm ";
        string str10 = " Path: ";
        //        string str1 = " Path: D:\\                                              Fr 18-07-25  6:46:12 pm ";

        int numCharsLeft = pathSpot.Length - TabPath.Length;
        string strCharsLeft = "";
        for (int i = 0; i < numCharsLeft; i++)
        {
          strCharsLeft += " ";
        }
        string str11 = TabPath + strCharsLeft;
        string str12 = " Fr 18-07-25  6:46:12 pm ";

        PrintString(screen, xa, 0, str10, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str10.Length;
        PrintString(screen, xa, 0, str11, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str11.Length;
        PrintString(screen, xa, 0, str12, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));


        //        PrintString(screen, 0, 0, str1, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        string str2 = "┌─────────────────────────────────────────────────────────┬────────────────────┐";
        PrintString(screen, 0, 1, str2, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        //        string str3 = "│ D:\\                                                     │FILE  *.*           │";
        string str30 = "│";
        //        string str31 = " D:\\                                                     ";
        string str310 = " ";
        string str311 = "D:\\";
        str311 = this.TabPath;
        string str312 = "                                                     ";
        string str32 = "│FILE  ";
        string str33 = "*.*";
        string str34 = "           │";
        xa = 0;
        PrintString(screen, xa, 2, str30, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str30.Length;

        PrintString(screen, xa, 2, str310, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str310.Length;
        PrintString(screen, xa, 2, str311, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str311.Length;
        PrintString(screen, xa, 2, str312, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str312.Length;

        PrintString(screen, xa, 2, str32, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str32.Length;
        PrintString(screen, xa, 2, str33, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
        xa += str33.Length;
        PrintString(screen, xa, 2, str34, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str34.Length;

        //

        var context = _visual.RenderOpen();
        var typeface = new Typeface("Consolas");
        var brush = Brushes.Black;
        double x = 0, y = 0;

        var dpi = VisualTreeHelper.GetDpi(this).PixelsPerDip;
        var ft = new FormattedText("W", CultureInfo.InvariantCulture, FlowDirection.LeftToRight,
            typeface, fontSize, brush, dpi);

        double charWidth = ft.Width;
        double charHeight = ft.Height;

        for (int row = 0; row < 50; row++)
        {
          for (int col = 0; col < 80; col++)
          {
            //var text = new FormattedText("│", CultureInfo.InvariantCulture, FlowDirection.LeftToRight,
            //    typeface, fontSize, brush, dpi);
            //context.DrawText(text, new Point(col * charWidth, row * charHeight));

            Point p = new Point(col * charWidth, row * charHeight);

            GuidelineSet guidelines = new GuidelineSet();
            guidelines.GuidelinesX.Add(p.X);
            guidelines.GuidelinesX.Add(p.X + charWidth);
            guidelines.GuidelinesY.Add(p.Y);
            guidelines.GuidelinesY.Add(p.Y + charHeight);
            context.PushGuidelineSet(guidelines);

            Rect rect = new Rect(p.X, p.Y, charWidth, charHeight);
            context.DrawRectangle(screen[col, row].Background, null, rect);





            var text = new FormattedText("─", CultureInfo.InvariantCulture, FlowDirection.LeftToRight,
  typeface, fontSize, brush, dpi);
            charWidth = 11;
            charHeight = 24;
            context.DrawText(text, new Point(col * charWidth, row * charHeight));

            context.Pop(); // Pop the guideline set
          }
        }
        context.Close();
      }

    }

    protected override int VisualChildrenCount => 1;
    protected override Visual GetVisualChild(int index) => _visual;
  }

}
