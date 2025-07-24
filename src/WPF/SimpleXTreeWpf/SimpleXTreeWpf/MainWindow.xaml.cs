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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.IO;
using System.Globalization;
using System.Windows.Media.Media3D;
using Path = System.IO.Path;
using System.Windows.Threading;
using System.Diagnostics;

namespace SimpleXTreeWpf
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private const string FONT_NAME = "Consolas";
    private const string FONT_NAME_ARROW = "Lucida Console";

    VisualHost m_visualHost;
    string m_currentTabPath = "D:\\";
    string m_selectedPath = "D:\\";

    Folder root = new Folder("My Computer");
    DriveInfo[] drives = DriveInfo.GetDrives();
    Dictionary<string, Folder> driveLookup = new Dictionary<string, Folder>();

    ImageSource m_darkBlock;
    ImageSource m_lightBlock;

    public MainWindow()
    {
        
      InitializeComponent();


      Uri uri = new Uri("pack://application:,,,/SimpleXTreeWpf;component/DarkBlock.png", UriKind.Absolute);
      m_darkBlock = new BitmapImage(uri);

      uri = new Uri("pack://application:,,,/SimpleXTreeWpf;component/LightBlock.png", UriKind.Absolute);
      m_lightBlock = new BitmapImage(uri);

      //      RenderOptions.SetBitmapScalingMode(TerminalImage, BitmapScalingMode.HighQuality);
      //      RenderOptions.SetBitmapScalingMode(TerminalImage, BitmapScalingMode.NearestNeighbor);
      //      RenderOptions.SetEdgeMode(TerminalImage, EdgeMode.Unspecified);

      m_bmp = new RenderTargetBitmap(width, height, 96, 96, PixelFormats.Pbgra32);
//      m_bmp = new RenderTargetBitmap(587, 800, 96, 96, PixelFormats.Pbgra32);
      TerminalImage.Source = m_bmp;


      Loaded += OnLoaded;

      //this.KeyDown += MainWindow_KeyDown;
      this.PreviewKeyDown += MainWindow_PreviewKeyDown;

      //m_visualHost = new VisualHost(this);


      ////var rect = new DrawingVisual();
      ////using (var dc = rect.RenderOpen())
      ////{
      ////  dc.DrawRectangle(Brushes.Black, null, new Rect(0, 0, 11, 24));
      ////}

      ////  host.AddVisual(rect);
      //this.Content = m_visualHost;
      CreateContextMenu();

      //this.SnapsToDevicePixels = true;
      //this.UseLayoutRounding= true;


      foreach (DriveInfo drive in drives)
      {
        Folder driveFolder = new Folder(drive.Name);
        if (m_selectedPath.Equals(drive.Name))
        {
          driveFolder.Selected = true;
        }
        driveLookup[drive.Name] = driveFolder;
        root.Children.Add(driveFolder);

        foreach (string path in Directory.GetDirectories(drive.Name))
        {
          string folderName = Path.GetFileName(path);
          Folder newFolder = new Folder(folderName);
          if (newFolder.Name.Equals(m_selectedPath))
          {
            newFolder.Selected = true;
          }
          driveFolder.Children.Add(newFolder);
        }
      }


      DrawTerminal();
      UpdateTime();

      _timer = new DispatcherTimer();
      //      _timer.Interval = TimeSpan.FromSeconds(1); // update every second
//      _timer.Interval = TimeSpan.FromSeconds(0.016); // update every second
      _timer.Interval = TimeSpan.FromSeconds(0.016*2); // update every second
      _timer.Tick += _timer_Tick;
      _timer.Start();

      //      this.Width = 590;
      //      this.Height = 800;

      this.MouseDown += MainWindow_MouseDown;
      this.MouseMove += MainWindow_MouseMove;
      this.MouseLeave += MainWindow_MouseLeave;
    }

    private void MainWindow_MouseLeave(object sender, MouseEventArgs e)
    {
      driveLookup["D:\\"].MouseOver = false;
      foreach (Folder f in driveLookup["D:\\"].Children)
      {
        f.MouseOver = false;
      }
      DrawTerminal(true);
    }

    private void MainWindow_MouseMove(object sender, MouseEventArgs e)
    {
      Point p = e.GetPosition(TerminalImage);
      double mx = p.X;
      double my = p.Y;
      //      MessageBox.Show(mx + "," + my);
      double cellW = TerminalImage.DesiredSize.Width / 80.0;
      double cellH = TerminalImage.DesiredSize.Height / 50.0;
      if (mx > 0 && mx <= MXL * cellW && my>0 && my < 37*cellH)
      {
//        DrawTerminal(true, false, true, cellW, cellH, mx, my);
MouseParams mouseParams = new MouseParams();
        mouseParams.CellW = cellW;
        mouseParams.CellH = cellH;
        mouseParams.Mx= mx;
        mouseParams.My = my;

        DrawTerminal(true, eMouseTrace.MouseOver, mouseParams);
      }
      else
      {
        driveLookup["D:\\"].MouseOver = false;
        foreach (Folder f in driveLookup["D:\\"].Children)
        {
          f.MouseOver = false;
        }
        DrawTerminal(true);
      }
    }

    enum eMouseTrace
    {
      None,
      MouseDown,
      MouseOver
    }

    public class MouseParams
    {
      public double CellW = 0.0;
      public double CellH = 0.0;
      public double Mx = 0.0;
      public double My = 0.0;
    }

    private void MainWindow_MouseDown(object sender, MouseButtonEventArgs e)
    {
      Point p = e.GetPosition(TerminalImage);
      double mx = p.X;
      double my = p.Y;
//      MessageBox.Show(mx + "," + my);
      double cellW = TerminalImage.DesiredSize.Width / 80.0;
      double cellH = TerminalImage.DesiredSize.Height / 50.0;
      //      DrawTerminal(true, true, false, cellW, cellH, mx, my);

      MouseParams mouseParams = new MouseParams();
      mouseParams.CellW = cellW;
      mouseParams.CellH = cellH;
      mouseParams.Mx = mx;
      mouseParams.My = my;
      DrawTerminal(true, eMouseTrace.MouseDown, mouseParams);


    }

    private void MainWindow_PreviewKeyDown(object sender, KeyEventArgs e)
    {
      KeyDown(e);
    }

    private void KeyDown(KeyEventArgs e)
    {
      if (e.Key == Key.Down)
      {
        Next();
        //        DrawTerminal();
        //        UpdateTime();

        Dispatcher.Invoke(() =>
        {
          Stopwatch sw = Stopwatch.StartNew();
          DrawTerminal(true);
          UpdateTime();
          sw.Stop();

          System.Diagnostics.Debug.WriteLine("sw: " + sw.Elapsed.ToString());
        });
      }
      else if (e.Key == Key.Up)
      {
        Prev();
        //        DrawTerminal();
        //        UpdateTime();

        Dispatcher.Invoke(() =>
        {
          DrawTerminal(true);
          UpdateTime();
        });
      }
    }

    private void MainWindow_KeyDown(object sender, KeyEventArgs e)
    {
//      KeyDown(e);
    }

    private void Next()
    {
      if (driveLookup["D:\\"].Selected)
      {
        driveLookup["D:\\"].Selected = false;
        driveLookup["D:\\"].Children[0].Selected = true;
      }
      else
      {
        for (int i = 0; i < driveLookup["D:\\"].Children.Count; ++i)
        {
          if (driveLookup["D:\\"].Children[i].Selected && i + 1 < driveLookup["D:\\"].Children.Count)
          {
            driveLookup["D:\\"].Children[i].Selected = false;
            driveLookup["D:\\"].Children[i + 1].Selected = true;
            break;
          }
        }
      }
    }

    private void Prev()
    {
      if (driveLookup["D:\\"].Selected)
      {
        // stay selected
      }
      else if (driveLookup["D:\\"].Children[0].Selected)
      {
        driveLookup["D:\\"].Children[0].Selected = false;
        driveLookup["D:\\"].Selected = true;
      }
      else
      {
        for (int i = driveLookup["D:\\"].Children.Count - 1; i >= 0; --i)
        {
          if (driveLookup["D:\\"].Children[i].Selected && i - 1 >= 0)
          {
            driveLookup["D:\\"].Children[i].Selected = false;
            driveLookup["D:\\"].Children[i - 1].Selected = true;
            break;
          }
        }
      }
    }

    private void PrintString(CharInfo[,] screen, int x, int y, string str, Brush bg, Brush fg, string fontName=FONT_NAME)
    {
      for (int i = 0; i < str.Length; i++)
      {
        screen[x + i, y] = new CharInfo();
        screen[x + i, y].Ch = str[i];
        screen[x + i, y].Background = bg;
        screen[x + i, y].Foreground = fg;
        screen[x + i, y].Font = fontName;
        screen[x + i, y].Dirty = true;
      }
    }

    private void PrintImage(CharInfo[,] screen, int x, int y, string str, bool isDark)
    {
      for (int i = 0; i < str.Length; i++)
      {
        screen[x + i, y] = new CharInfo();
        screen[x + i, y].HasDarkImage = isDark;
        screen[x + i, y].HasLightImage = !isDark;
        screen[x + i, y].Dirty = true;
      }
    }

    private void PrintStringSkipSpace(CharInfo[,] screen, int x, int y, string str, Brush bg, Brush fg, string fontName = FONT_NAME)
    {
      for (int i = 0; i < str.Length; i++)
      {
        if (str[i]!=' ')
        {
          screen[x + i, y] = new CharInfo();
          screen[x + i, y].Ch = str[i];
          screen[x + i, y].Background = bg;
          screen[x + i, y].Foreground = fg;
          screen[x + i, y].Font = fontName;
          screen[x + i, y].Dirty = true;
        }
      }
    }

    

    string TabPath = "D:\\";

    const int cols = 80;
    const int rows = 50;
    const int width = 880;
    const int height = 1200;
    double charWidth = (double)width / cols;
    double charHeight = (double)height / rows;

    SolidColorBrush lightGrey = new SolidColorBrush(Color.FromRgb(204, 204, 204));
    SolidColorBrush lightBlue = new SolidColorBrush(Color.FromRgb(97, 214, 214));

    double MXL = 58.0;

//    void DrawTerminal(bool justDirs = false, bool traceMouse = false, bool traceMouseOver = false, double cellW = 0.0, double cellH = 0.0, double mx = 0.0, double my = 0.0)
    void DrawTerminal(bool justDirs = false, eMouseTrace mouseTrace = eMouseTrace.None, MouseParams mouseParams = null)
    {
      bool redraw = false;

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

      if (!justDirs)
      {
      PrintString(screen, xa, 0, str10, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += str10.Length;
      PrintString(screen, xa, 0, str11, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += str11.Length;
//      PrintString(screen, xa, 0, str12, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      PrintString(screen, xa, 0, "", Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));


      //        PrintString(screen, 0, 0, str1, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      string str2 = "┌─────────────────────────────────────────────────────────┬────────────────────┐";
      PrintString(screen, 0, 1, str2, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        //        string str3 = "│ D:\\                                                     │FILE  *.*           │";
      }
      string str30 = "│";
        //        string str31 = " D:\\
        //        ";

      string str310 = " ";
      string str311 = "D:\\";
      str311 = this.TabPath;
      string str312 = "                                                     ";

      string str32 = "│FILE ";
      string str33 = "*.*";
      string str34 = "            │";
      xa = 0;
      PrintString(screen, xa, 2, str30, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += str30.Length;

      if (driveLookup["D:\\"].Selected)
      {
        PrintString(screen, xa, 2, str310, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str310.Length;
        PrintString(screen, xa, 2, str311, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(118, 118, 118)));
        xa += str311.Length;
        PrintString(screen, xa, 2, str312, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str312.Length;
      }
      else if (driveLookup["D:\\"].MouseOver)
      {
        PrintString(screen, xa, 2, str310, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str310.Length;
        PrintString(screen, xa, 2, str311, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(255, 255, 8)));
        xa += str311.Length;
        PrintString(screen, xa, 2, str312, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str312.Length;
      }
      else
      {
        PrintString(screen, xa, 2, str310, Brushes.Black, Brushes.Black);
        xa += str310.Length;
        PrintString(screen, xa, 2, str311, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
        xa += str311.Length;
        PrintString(screen, xa, 2, str312, Brushes.Black, Brushes.Black);
        xa += str312.Length;
      }
      if (mouseTrace==eMouseTrace.MouseOver && 
        mouseParams.My >= 2 * mouseParams.CellH && 
        mouseParams.My <= 3 * mouseParams.CellH && 
        mouseParams.Mx <= MXL * mouseParams.CellW)
      {
        //        MessageBox.Show("sel drive");
        driveLookup["D:\\"].MouseOver = true;
        foreach (Folder f in driveLookup["D:\\"].Children)
        {
          f.MouseOver = false;
        }
        redraw = true;// DrawTerminal(true);
      }
      if (mouseTrace==eMouseTrace.MouseDown && 
        mouseParams.My >= 2 * mouseParams.CellH &&
        mouseParams.My <= 3 * mouseParams.CellH &&
        mouseParams.Mx <= MXL * mouseParams.CellW)
      {
//        MessageBox.Show("sel drive");
        driveLookup["D:\\"].Selected = true;
        foreach (Folder f in driveLookup["D:\\"].Children)
        {
          f.Selected = false;
        }
        redraw = true;// DrawTerminal(true);
      }

      PrintString(screen, xa, 2, str32, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += str32.Length;
      PrintString(screen, xa, 2, str33, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
      xa += str33.Length;
      PrintString(screen, xa, 2, str34, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += str34.Length;

      xa = 0;
      //        string str4 = "│+├──$RECYCLE.BIN                                         ├────────────────────┤";
      /*
              string str40 = "│";
              string str41 = "+├──$RECYCLE.BIN";
              string str42 = "                                         ";
              string str43 = "├────────────────────┤";
              PrintString(screen, xa, 3, str40, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
              xa += str40.Length;
              PrintString(screen, xa, 3, str41, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
              xa += str41.Length;
              PrintString(screen, xa, 3, str42, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
              xa += str42.Length;
              PrintString(screen, xa, 3, str43, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
              xa += str43.Length;
      */





      int yUpTo = 3;
      foreach (Folder folder in driveLookup["D:\\"].Children)
      {
        if (yUpTo== 37)
        {
          break;
        }

        xa = 0;
        if (yUpTo == 50)
          break;
        string str50 = "│";
        string str51 = "+├──" + folder.Name;
        if (folder.Name== driveLookup["D:\\"].Children[driveLookup["D:\\"].Children.Count - 1].Name)
        {
          //last
          str51 = "+└──" + folder.Name;
        }

        PrintString(screen, xa, yUpTo, str50, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str50.Length;
        if (folder.Selected)
        {
          PrintString(screen, xa, yUpTo, str51, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(118, 118, 118)));
          xa += str51.Length;

          string restOfLine = " ";
          for (int i = str51.Length; i < 56; ++i)
          {
            restOfLine += " ";
          }
          PrintString(screen, xa, yUpTo, restOfLine, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(118, 118, 118)));
        }
        else if (folder.MouseOver)
        {
          PrintString(screen, xa, yUpTo, str51, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(255, 255, 8)));
          xa += str51.Length;

          string restOfLine = " ";
          for (int i = str51.Length; i < 56; ++i)
          {
            restOfLine += " ";
          }
          PrintString(screen, xa, yUpTo, restOfLine, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(118, 118, 118)));
        }
        else
        {
          PrintString(screen, xa, yUpTo, str51, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
          xa += str51.Length;

          string restOfLine = " ";
          for (int i = str51.Length; i < 56; ++i)
          {
            restOfLine += " ";
          }
          PrintString(screen, xa, yUpTo, restOfLine, Brushes.Black, Brushes.Black);
        }

        if (mouseTrace==eMouseTrace.MouseOver && mouseParams.My >= yUpTo * mouseParams.CellH && mouseParams.My <= (yUpTo + 1) * mouseParams.CellH && mouseParams.Mx <= MXL * mouseParams.CellW)
        {
          //          MessageBox.Show("sel folder: "+folder.Name);

          driveLookup["D:\\"].MouseOver = false;
          foreach (Folder f in driveLookup["D:\\"].Children)
          {
            f.MouseOver = false;
          }
          folder.MouseOver = true;
          redraw = true;// DrawTerminal(true);
        }
        if (mouseTrace == eMouseTrace.MouseDown && mouseParams.My >= yUpTo * mouseParams.CellH && mouseParams.My <= (yUpTo+1) * mouseParams.CellH && mouseParams.Mx <= MXL* mouseParams.CellW)
        {
//          MessageBox.Show("sel folder: "+folder.Name);

          driveLookup["D:\\"].Selected = false;
          foreach (Folder f in driveLookup["D:\\"].Children)
          {
            f.Selected = false;
          }
          folder.Selected= true;
          redraw = true;// DrawTerminal(true);
        }

        yUpTo++;
      }



      while (yUpTo < 37)
      //      for (int i=yUpTo;i<37;++i)
      {
        xa = 0;
        string after37 = "│                                                         │                    │";
        PrintString(screen, xa, yUpTo, after37, Brushes.Black, lightGrey);
        yUpTo++;
      }


      if (!justDirs)
      {
      string bot = "├─────────────────────────────────────────────────────────┴────────────────────┤";
      PrintString(screen, 0, yUpTo, bot, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      yUpTo++;

      DrawRightPanel(screen);

      string b1 = "│ ";
      xa = 0;
      PrintString(screen, xa, yUpTo, b1, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += b1.Length;
      string dirNotLogged = "Dir Not Logged";
      PrintString(screen, xa, yUpTo, dirNotLogged, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
      yUpTo++;
      xa = 0;
      for (int i=0;i<7;++i)
      {
        PrintString(screen, xa, yUpTo, "│", Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        yUpTo++;
      }
      PrintString(screen, xa, yUpTo, "│", Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      yUpTo++;

      DrawBottomPanel(screen);

      PrintString(screen, xa, yUpTo, "DIR       Avail  Branch  Compare  Delete  Filespec  Global  Invert  Log  Make", Brushes.Black, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "          A      B       C        D       F         G       I       L    M   ", Brushes.Black, lightBlue);
      yUpTo++;

      PrintString(screen, xa, yUpTo, "COMMANDS  Open  Pick  Rename  Showall  Tag  Untag  Volume  eXecute  Quit", Brushes.Black, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "          O     P     R       S        T    U      V        X       Q   ", Brushes.Black, lightBlue);
      yUpTo++;

      PrintString(screen, xa, yUpTo, "◄─┘", Brushes.Black, lightBlue, FONT_NAME_ARROW);
      PrintString(screen, 3, yUpTo, " file  F5 rescan  F6 newview  F8 split    F11 clipboard  F1 help  < > select", Brushes.Black, lightGrey);
      PrintStringSkipSpace(screen, 3, yUpTo, "       F5         F6          F8          F11            F1       < >       ", Brushes.Black, lightBlue);
      yUpTo++;

      }


      // Create DrawingVisual for off-screen rendering
      DrawingVisual visual = new DrawingVisual();
      using (DrawingContext dc = visual.RenderOpen())
      {
        string currentFont = FONT_NAME;
        Typeface typeface = new Typeface(currentFont);
        double fontSize = charHeight * 0.9;

        // Draw grid
        for (int y = 0; y < rows; y++)
        {
          for (int x = 0; x < cols; x++)
          {
            if (!screen[x, y].Dirty)
            {
              break;
            }
            Rect rect = new Rect(x * charWidth, y * charHeight, charWidth, charHeight);

            // Background
//            dc.DrawRectangle(Brushes.Black, null, rect);
            dc.DrawRectangle(screen[x, y].Background, null, rect);
            if (screen[x,y].HasLightImage)
            {
              dc.DrawImage(m_lightBlock, rect);
            }
            if (screen[x, y].HasDarkImage)
            {
              dc.DrawImage(m_darkBlock, rect);
            }

            // Character (alternating ─ and │)
            //            string ch = (y % 2 == 0) ? "─" : "│";
            string ch = "│";
            ch = screen[x,y].Ch.ToString();
            if (!screen[x,y].Font.Equals(currentFont))
            {
              currentFont = screen[x, y].Font;
              typeface = new Typeface(currentFont);
            }
            FormattedText ft = new FormattedText(
                ch,
                CultureInfo.InvariantCulture,
                FlowDirection.LeftToRight,
                typeface,
                fontSize,
                //                Brushes.White,
                screen[x, y].Foreground,
                VisualTreeHelper.GetDpi(this).PixelsPerDip
            );

            // Align top-left
            dc.DrawText(ft, new Point(x * charWidth, y * charHeight));
          }
        }
      }

      //// Render to bitmap
      //RenderTargetBitmap bmp = new RenderTargetBitmap(
      //    width, height, 96, 96, PixelFormats.Pbgra32);
      //bmp.Render(visual);

      //// Display in Image
      //TerminalImage.Source = bmp;

      //m_bmp = bmp;


      // Render to bitmap
      m_bmp.Render(visual);

      // Display in Image

      if (redraw)
      {
        DrawTerminal(true);
      }


      //      TerminalImage.Width = 590;
      //      TerminalImage.Height = 831;

      //      Height = 830;
      //      Width = 586.7;
      //      Width = m_bmp.Width/1.5;
      //      Height = m_bmp.Height/1.5;
      //      Width = 603;
      //      Height = 838;

    }

    private void DrawRightPanel(CharInfo[,] screen)
    {
      /*
┬────────────────────┐
│FILE *.*            │
├────────────────────┤
│DISK D:DATA         │
│ Available Bytes    │
│    375,287,500,800 │
│ Total Bytes        │
│  1,000,186,310,656 │
├────────────────────┤
│DISK              1 │
│ Total              │
│  Files           4 │
│  Bytes       7,077 │
│ Matching           │
│  Files           4 │
│  Bytes       7,077 │
│ Tagged             │
│  Files           0 │
│  Bytes           0 │
│ Current Directory  │
│  D:\               │
│  Bytes       7,077 │
│  Attr              │
│  Create            │
│  Time              │
│ Directory Items    │
│  Dirs           33 │
│  Files           4 │
│  Match           4 │
│                    │
│                    │
│                    │
│                    │
│                    │
│                    │
│                    │
┴────────────────────┤
*/
      int xa = 58;
      int yUpTo = 1;
      SolidColorBrush bg = Brushes.Black;
      PrintString(screen, xa, yUpTo, "┬────────────────────┐", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│FILE *.*            │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "      *.*", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "├────────────────────┤", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│DISK D:DATA         │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "      D:DATA", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Available Bytes    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│    375,287,500,800 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "     375,287,500,800", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Total Bytes        │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  1,000,186,310,656 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "   1,000,186,310,656", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "├────────────────────┤", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│DISK              1 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, " DISK              1", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Total              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Files           4 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                   4", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Bytes       7,077 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "               7,077", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Matching           │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Files           4 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                   4", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Bytes       7,077 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "               7,077", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Tagged             │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Files           0 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                   0", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Bytes           0 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                   0", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Current Directory  │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  D:\\               │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "   D:\\", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Bytes       7,077 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "               7,077", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Attr              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Create            │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Time              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Directory Items    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Dirs           33 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                  33", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Files           4 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                   4", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│  Match           4 │", bg, lightGrey);
      PrintStringSkipSpace(screen, xa, yUpTo, "                   4", bg, lightBlue);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                    │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "┴────────────────────┤", bg, lightGrey);
      yUpTo++;
    }

    private void DrawBottomPanel(CharInfo[,] screen)
    {
/*
├─────────────────────────────────────────────────────────┴────────────────────┤
│ Dir Not Logged                                                               │
│                                                                              │
│                                                                              │
│                                                                              │
│                                                                              │
│                                                                              │
│                                                                              │
│                                                                              │
└▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓┘
 */
      int xa = 0;
      int yUpTo = 37;
      SolidColorBrush bg = Brushes.Black;
      PrintString(screen, xa, yUpTo, "├─────────────────────────────────────────────────────────┴────────────────────┤", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│ Dir Not Logged                                                               │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      PrintString(screen, xa, yUpTo, "│                                                                              │", bg, lightGrey);
      yUpTo++;
      //      PrintString(screen, xa, yUpTo, "└▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓┘", bg, lightGrey);
      //      yUpTo++;
      //      PrintString(screen, xa, yUpTo, "└░░▒▒▓▓██┘", bg, lightGrey, "Terminal");
      //     yUpTo++;
      //            PrintString(screen, xa, yUpTo,
      //                                       "└▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒┘", bg, lightGrey);
      //            yUpTo++;

      //      PrintString(screen, xa, yUpTo, "└░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓┘", bg, lightGrey);
      //      yUpTo++;

      PrintString(screen, xa, yUpTo, "└                                                                              ┘", bg, lightGrey);
      PrintImage(screen, 1, yUpTo, "                                                                              ", false);
      PrintImage(screen, 1, yUpTo, "   ", true);
      yUpTo++;

    }

    RenderTargetBitmap m_bmp;

    private DispatcherTimer _timer;

    private void UpdateTime()
    {
      CharInfo[,] screen = new CharInfo[80, 50];
      for (int r = 0; r < 80; r++)
      {
        for (int c = 0; c < 50; c++)
        {
          screen[r, c] = new CharInfo();
        }
      }

      DateTime now = DateTime.Now;//.AddHours(-2);

      //      string str12 = " Fr 18-07-25  6:46:12 pm ";
      string before = now.ToString("ddd dd-MM-yy");
      string after = now.ToString("h:mm:ss tt");
      if (after[1] == ':')
      {
        before += " ";
      }

      string str12 = " " + before + " " + after + " ";
      string ampm = str12.Substring(str12.Length - 3).ToLower();
      str12 = str12.Substring(0, str12.Length - 3) + ampm;
      PrintString(screen, 54, 0, str12, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));

      DrawScreen(screen);
    }

    private void _timer_Tick(object? sender, EventArgs e)
    {
      //Width was 602
      //Height was 838, at scale 1, times 0.66666...
      UpdateTime();
    }

    DrawingVisual m_screenVisual = new DrawingVisual();

    private void DrawScreen(CharInfo[,] screen)
    {

      // Create DrawingVisual for off-screen rendering
      using (DrawingContext dc = m_screenVisual.RenderOpen())
      {
        Typeface typeface = new Typeface(FONT_NAME);
        double fontSize = charHeight * 0.9;

        // Draw grid
        for (int y = 0; y < rows; y++)
        {
          for (int x = 0; x < cols; x++)
          {
            CharInfo cell = screen[x, y];
            if (!cell.Dirty)
            {
              continue;
            }

            Rect rect = new Rect(x * charWidth, y * charHeight, charWidth, charHeight);

            // Background
            //            dc.DrawRectangle(Brushes.Black, null, rect);
            dc.DrawRectangle(screen[x, y].Background, null, rect);



            // Create or get cached FormattedText
            string cacheKey = $"{cell.Ch}-{cell.Foreground}";
            if (!m_textCache.TryGetValue(cacheKey, out var ft))
            {
              ft = new FormattedText(
                  cell.Ch.ToString(),
                  CultureInfo.InvariantCulture,
                  FlowDirection.LeftToRight,
                  typeface,
                  fontSize,
                  cell.Foreground,
                  VisualTreeHelper.GetDpi(m_screenVisual).PixelsPerDip
              );
              m_textCache[cacheKey] = ft;
            }

            /*
            // Character (alternating ─ and │)
            //            string ch = (y % 2 == 0) ? "─" : "│";
            string ch = "│";
            ch = screen[x, y].Ch.ToString();
            FormattedText ft = new FormattedText(
                ch,
                CultureInfo.InvariantCulture,
                FlowDirection.LeftToRight,
                typeface,
                fontSize,
                //                Brushes.White,
                screen[x, y].Foreground,
                VisualTreeHelper.GetDpi(this).PixelsPerDip
            );
*/

      // Align top-left
      dc.DrawText(ft, new Point(x * charWidth, y * charHeight));

            screen[x, y].Dirty = false;
          }
        }
      }

      // Render to bitmap
      m_bmp.Render(m_screenVisual);

//      TerminalImage.
    }

    private Dictionary<string, FormattedText> m_textCache = new Dictionary<string, FormattedText>();

    MenuItem m_tabItem;
    MenuItem m_newTab;
    MenuItem m_removeTab;
    MenuItem m_currentTab;

    private void CreateContextMenu()
    {
      ContextMenu contextMenu = new ContextMenu();

      m_tabItem = new MenuItem { Header = "Tabs" };

      //   MenuItem child1 = new MenuItem { Header = "* D:\\" };
      //    m_currentTab = child1;

      m_newTab = new MenuItem { Header = "New Tab" };
      m_removeTab = new MenuItem { Header = "Remove Tab" };

      m_newTab.Click += NewTab_Click;
      m_removeTab.Click += RemoveTab_Click;

      DriveInfo[] drives = DriveInfo.GetDrives();
      foreach (DriveInfo drive in drives)
      {
        MenuItem child = new MenuItem { Header = drive.Name };
        child.Click += Child_Click;
        if (drive.Name==m_currentTabPath)
        {
          m_currentTab = child;
          child.Header = "* " + drive.Name;
        }
        m_tabItem.Items.Add(child);
      }

      m_tabItem.Items.Add(m_newTab);
      m_tabItem.Items.Add(m_removeTab);


      contextMenu.Items.Add(m_tabItem);

      // Attach to a control (e.g., a TextBox)
      this.ContextMenu = contextMenu;
    }

    private void RemoveTab_Click(object sender, RoutedEventArgs e)
    {
      m_tabItem.Items.Remove(m_currentTab);
      m_tabItem.Items.Remove(m_newTab);
      m_tabItem.Items.Remove(m_removeTab);

      m_tabItem.Items.Add(m_newTab);
      m_tabItem.Items.Add(m_removeTab);

      m_currentTab = (MenuItem)m_tabItem.Items[m_tabItem.Items.Count - 3];
      string text = m_currentTab.Header.ToString().Replace("* ", "");

      if (m_tabItem.Items.Count==3)
      {
        m_removeTab.Visibility = Visibility.Collapsed;
      }

      m_currentTab.Header = "* " + text;
      m_currentTabPath = text;
//      m_visualHost.TabPath = m_currentTabPath;
//      m_visualHost.UpdateCurrrentTab();
    }

    private void NewTab_Click(object sender, RoutedEventArgs e)
    {
      m_currentTab.Header = m_currentTab.Header.ToString().Replace("* ", "");

      m_removeTab.Visibility = Visibility.Visible;
      MenuItem child = new MenuItem { Header = "* "+m_currentTabPath };
      m_currentTab = child;
      child.Click += Child_Click;
      m_tabItem.Items.Remove(m_newTab);
      m_tabItem.Items.Remove(m_removeTab);
      m_tabItem.Items.Add(child);
      m_tabItem.Items.Add(m_newTab);
      m_tabItem.Items.Add(m_removeTab);
    }

    private void Child_Click(object sender, RoutedEventArgs e)
    {
      MenuItem senderMenu = (sender as MenuItem);
      m_currentTab.Header = m_currentTab.Header.ToString().Replace("* ", "");
      string text = senderMenu.Header.ToString().Replace("* ", "");
      m_currentTab = senderMenu;
      m_currentTab.Header = "* " + text;
      m_currentTabPath = text;
      //      m_visualHost.TabPath = m_currentTabPath;
      //      m_visualHost.UpdateCurrrentTab();
      TabPath = m_currentTabPath;
      UpdateCurrrentTab();
    }

    public void UpdateCurrrentTab()
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
      string str10 = " Path: ";
      int numCharsLeft = pathSpot.Length - TabPath.Length;
      string strCharsLeft = "";
      for (int i = 0; i < numCharsLeft; i++)
      {
        strCharsLeft += " ";
      }
      string str11 = TabPath + strCharsLeft;
      string str12 = " Fr 18-07-25  6:46:12 pm ";

      xa += str10.Length;
      PrintString(screen, xa, 0, str11, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      xa += str11.Length;
      //      PrintString(screen, xa, 0, str12, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));

      //      PrintString(screen, 0, 0, "hello", Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));

      DrawCurrentSelected(screen);

      DrawScreen(screen);

      UpdateTime();

      //      DrawWholeScreen();

    }

    private void DrawCurrentSelected(CharInfo[,] screen)
    {
      string str310 = " ";
      string str311 = "D:\\";
      str311 = this.TabPath;
      string str312 = "                                                     ";

      int xa = 1;
      PrintString(screen, xa, 2, str310, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
      xa += str310.Length;
      PrintString(screen, xa, 2, str311, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
      xa += str311.Length;
      PrintString(screen, xa, 2, str312, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
      xa += str312.Length;
    }



    private void OnLoaded(object sender, RoutedEventArgs e)
    {
      double oldX = this.Left;
      double oldY = this.Top;

      // Desired window size in actual physical pixels
      //      int targetWidth = 880-2;
      //      int targetHeight = 1200-32;
            int targetWidth = 880;
            int targetHeight = 1200;

      //int targetWidth = 587;
      //int targetHeight = 800;

      // Get window handle
      var hwnd = new WindowInteropHelper(this).Handle;

      //double scaleX = 1;
      //double scaleY = 1;

      // Get DPI scale
      var dpi = VisualTreeHelper.GetDpi(this);
      double scaleX = dpi.DpiScaleX;
      double scaleY = dpi.DpiScaleY;

      //works on laptop, does it work on desktop?
      scaleX = dpi.DpiScaleX / 1.5;
      scaleY = dpi.DpiScaleY / 1.5;

//      scaleX = 1;
//      scaleY = 1;

      //if (Math.Abs(dpi.DpiScaleX - 1.0) < 0.1 && Math.Abs(dpi.DpiScaleY - 1.0) < 0.1)
      //{
      //  scaleX = 0.66666666;
      //  scaleY = 0.66666666;
      //}
      //else if (Math.Abs(dpi.DpiScaleX - 1.5) < 0.1 && Math.Abs(dpi.DpiScaleY - 1.5) < 0.1)
      //{
      //  scaleX = 1;
      //  scaleY = 1;
      //}

      targetWidth = (int)(targetWidth * scaleX);
      targetHeight = (int)(targetHeight * scaleY);

      // Get window styles (to calculate border size)
      GetWindowRect(hwnd, out RECT rect);
      GetClientRect(hwnd, out RECT clientRect);

      int borderWidth = (rect.Right - rect.Left) - (clientRect.Right - clientRect.Left);
      int borderHeight = (rect.Bottom - rect.Top) - (clientRect.Bottom - clientRect.Top);

      // Calculate total size including borders
      int totalWidth = (int)(targetWidth + borderWidth);
      int totalHeight = (int)(targetHeight + borderHeight);

      // Resize the window
      SetWindowPos(hwnd, IntPtr.Zero, (int)oldX, (int)oldY, totalWidth, totalHeight,
          SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // Win32 interop
    private const int SWP_NOZORDER = 0x0004;
    private const int SWP_NOACTIVATE = 0x0010;

    [DllImport("user32.dll", SetLastError = true)]
    static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter,
        int X, int Y, int cx, int cy, uint uFlags);

    [DllImport("user32.dll", SetLastError = true)]
    static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

    [DllImport("user32.dll", SetLastError = true)]
    static extern bool GetClientRect(IntPtr hWnd, out RECT lpRect);

    [StructLayout(LayoutKind.Sequential)]
    public struct RECT
    {
      public int Left, Top, Right, Bottom;
    }

    private void Button_Click(object sender, RoutedEventArgs e)
    {
      var dpi = VisualTreeHelper.GetDpi(this);
      MessageBox.Show($"DPI: X={dpi.PixelsPerInchX}, Y={dpi.PixelsPerInchY}");
      //      Console.WriteLine($"DPI: X={dpi.PixelsPerInchX}, Y={dpi.PixelsPerInchY}");


      double widthDIPs = 880 / dpi.DpiScaleX;
      double heightDIPs = 1200 / dpi.DpiScaleY;

      MessageBox.Show($"DIPs: {widthDIPs} x {heightDIPs}");
    }


  }
}
