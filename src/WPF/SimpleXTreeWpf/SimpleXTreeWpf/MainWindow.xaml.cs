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

namespace SimpleXTreeWpf
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    VisualHost m_visualHost;
    string m_currentTabPath = "D:\\";
    string m_selectedPath = "D:\\";

    Folder root = new Folder("My Computer");
    DriveInfo[] drives = DriveInfo.GetDrives();
    Dictionary<string, Folder> driveLookup = new Dictionary<string, Folder>();


    public MainWindow()
    {
        
      InitializeComponent();
      Loaded += OnLoaded;

      this.KeyDown += MainWindow_KeyDown;

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
    }

    private void MainWindow_KeyDown(object sender, KeyEventArgs e)
    {
      if (e.Key == Key.Down)
      {
        Next();
        DrawTerminal();
        UpdateTime();
      }
      else if (e.Key == Key.Up)
      {
        Prev();
        DrawTerminal();
        UpdateTime();
      }
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

    const int cols = 80;
    const int rows = 50;
    const int width = 880;
    const int height = 1200;
    double charWidth = (double)width / cols;
    double charHeight = (double)height / rows;
    void DrawTerminal()
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
//      PrintString(screen, xa, 0, str12, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      PrintString(screen, xa, 0, "", Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));


      //        PrintString(screen, 0, 0, str1, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      string str2 = "┌─────────────────────────────────────────────────────────┬────────────────────┐";
      PrintString(screen, 0, 1, str2, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
      //        string str3 = "│ D:\\                                                     │FILE  *.*           │";
      string str30 = "│";
      //        string str31 = " D:\\
      //        ";

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

      if (driveLookup["D:\\"].Selected)
      {
        PrintString(screen, xa, 2, str310, new SolidColorBrush(Color.FromRgb(204, 204, 204)), Brushes.Black);
        xa += str310.Length;
        PrintString(screen, xa, 2, str311, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(118, 118, 118)));
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
        xa = 0;
        if (yUpTo == 50)
          break;
        string str50 = "│";
        string str51 = "+├──" + folder.Name;

        PrintString(screen, xa, yUpTo, str50, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
        xa += str50.Length;
        if (folder.Selected)
        {
          PrintString(screen, xa, yUpTo, str51, new SolidColorBrush(Color.FromRgb(204, 204, 204)), new SolidColorBrush(Color.FromRgb(118, 118, 118)));
          xa += str51.Length;
        }
        else
        {
          PrintString(screen, xa, yUpTo, str51, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
          xa += str51.Length;
        }

        yUpTo += 1;
      }


      // Create DrawingVisual for off-screen rendering
      DrawingVisual visual = new DrawingVisual();
      using (DrawingContext dc = visual.RenderOpen())
      {
        Typeface typeface = new Typeface("Consolas");
        double fontSize = charHeight * 0.9;

        // Draw grid
        for (int y = 0; y < rows; y++)
        {
          for (int x = 0; x < cols; x++)
          {
            Rect rect = new Rect(x * charWidth, y * charHeight, charWidth, charHeight);

            // Background
//            dc.DrawRectangle(Brushes.Black, null, rect);
            dc.DrawRectangle(screen[x, y].Background, null, rect);

            // Character (alternating ─ and │)
            //            string ch = (y % 2 == 0) ? "─" : "│";
            string ch = "│";
            ch = screen[x,y].Ch.ToString();
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

      // Render to bitmap
      RenderTargetBitmap bmp = new RenderTargetBitmap(
          width, height, 96, 96, PixelFormats.Pbgra32);
      bmp.Render(visual);

      // Display in Image
      TerminalImage.Source = bmp;

      m_bmp = bmp;

      //      TerminalImage.Width = 590;
      //      TerminalImage.Height = 831;

      //      Height = 830;
      //      Width = 586.7;
      //      Width = m_bmp.Width/1.5;
      //      Height = m_bmp.Height/1.5;
//      Width = 603;
//      Height = 838;

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
        Typeface typeface = new Typeface("Consolas");
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
      m_visualHost.TabPath = m_currentTabPath;
      m_visualHost.UpdateCurrrentTab();
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

      // Get window handle
      var hwnd = new WindowInteropHelper(this).Handle;

      // Get DPI scale
      var dpi = VisualTreeHelper.GetDpi(this);
      double scaleX = dpi.DpiScaleX;
      double scaleY = dpi.DpiScaleY;
      if (Math.Abs(dpi.DpiScaleX - 1.0) < 0.1 && Math.Abs(dpi.DpiScaleY - 1.0) < 0.1)
      {
        scaleX = 0.66666666;
        scaleY = 0.66666666;
      }
      else if (Math.Abs(dpi.DpiScaleX - 1.5) < 0.1 && Math.Abs(dpi.DpiScaleY - 1.5) < 0.1)
      {
        scaleX = 1;
        scaleY = 1;
      }
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
