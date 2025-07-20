using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows;
using System.Diagnostics;
using System.Globalization;
using static System.Net.Mime.MediaTypeNames;
using System.Runtime.Intrinsics.Arm;
using System.Windows.Threading;
using System.IO;
using System.Windows.Controls;
using static SimpleXTreeWpf.MainWindow;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;

namespace SimpleXTreeWpf
{
  public class VisualHost : FrameworkElement
  {
    private readonly List<DrawingVisual> visuals = new();

    public string TabPath = "D:\\";

    public VisualHost(MainWindow mainWindow)
    {
      _timer = new DispatcherTimer();
//      _timer.Interval = TimeSpan.FromSeconds(1); // update every second
      _timer.Interval = TimeSpan.FromSeconds(0.016); // update every second
      _timer.Tick += _timer_Tick;
      _timer.Start();

      //for (int i = 0; i < 4000; i++)
      //{
      //  var vis = new DrawingVisual();
      //  using (var dc = vis.RenderOpen())
      //  {
      //    double x = (i % 100) * 12; // example layout
      //    double y = (i / 100) * 26;

      //    dc.DrawRectangle(Brushes.Black, null, new Rect(x, y, 11, 24));
      //    FormattedText text = new FormattedText(
      //        "A", System.Globalization.CultureInfo.InvariantCulture,
      //        FlowDirection.LeftToRight, new Typeface("Consolas"),
      //        14, Brushes.White, 1.0);
      //    dc.DrawText(text, new Point(x + 1, y));
      //  }

      //  visuals.Add(vis);
      //  AddVisualChild(vis);
      //}

      /*
      var vis = new DrawingVisual();
      using (DrawingContext dc = vis.RenderOpen())
      {
        for (int y = 0; y < 50; y++)
        {
          for (int x = 0; x < 80; x++)
          {

            Rect rect = new Rect(x * 11, y * 24, 11.2, 24);
            dc.DrawRectangle(Brushes.Black, null, rect);

            FormattedText text = new FormattedText(
                "A", System.Globalization.CultureInfo.InvariantCulture,
                FlowDirection.LeftToRight, new Typeface("Consolas"),
                14, Brushes.White, 1.0);
            dc.DrawText(text, new Point(x * 11 + 1, y * 24));
          }
        }
      }
      */

      /*

      var sampleText = new FormattedText(
    "A", CultureInfo.InvariantCulture,
    FlowDirection.LeftToRight,
    new Typeface("Consolas"),
    14,
    Brushes.Black,
    1.0); // pixelsPerDip = 1.0 here, might want to pass actual

      //double charWidth = sampleText.WidthIncludingTrailingWhitespace;
      //Debug.WriteLine($"Character width: {charWidth}");

      //var dpi = VisualTreeHelper.GetDpi(this);
      //Debug.WriteLine($"DPI scale: {dpi.DpiScaleX}, {dpi.DpiScaleY}");


      double somePadding = 3.3;
      double dpiScaleX = VisualTreeHelper.GetDpi(this).DpiScaleX;
      double charWidth = 7.7; // logical pixels (DIPs)
      double cellWidth = charWidth + somePadding; // say 11 logical pixels

      double physicalCellWidth = cellWidth * dpiScaleX; // physical pixels per cell

      double totalWidthPhysical = physicalCellWidth * 80; // physical pixels for 80 chars
      double totalWidthLogical = totalWidthPhysical / dpiScaleX; // back to logical pixels for layout





      int cols = 80;
      int rows = 50;

      // Define total width and height of your target render area (e.g., 880 x 1200)
      double totalWidth = 880;
      double totalHeight = 1200;

      // Compute width and height per character cell
//      double cellWidth = totalWidth / cols;     // = 11
      double cellHeight = totalHeight / rows;   // = 24

      var vis = new DrawingVisual();
      using (DrawingContext dc = vis.RenderOpen())
      {
        for (int y = 0; y < rows; y++)
        {
          for (int x = 0; x < cols; x++)
          {
            Rect rect = new Rect(x * cellWidth, y * cellHeight, cellWidth+0.2, cellHeight);
            dc.DrawRectangle(Brushes.Black, null, rect);

            var text = new FormattedText(
                "A", System.Globalization.CultureInfo.InvariantCulture,
                FlowDirection.LeftToRight, new Typeface("Consolas"),
                14, Brushes.White, 1.0);
            dc.DrawText(text, new Point(x * cellWidth + 1, y * cellHeight));
          }
        }
      }

      visuals.Add(vis);
      AddVisualChild(vis);
      */

      /*

      var vis = new DrawingVisual();
      var dpi = VisualTreeHelper.GetDpi(vis);

      // Define character size in logical units (DIPs)
      double charWidth = 7.7;  // measured in DIPs (Consolas 14pt)
      //double charHeight = 14;  // font size
      double charHeight = 13.3;

      // Define cell size (add a bit of padding so letters aren't cut off)
      double cellWidth = charWidth + 1.0;  // 8.7 DIPs
      double cellHeight = 24;              // line height

      // You can tweak these if spacing looks off

      using (DrawingContext dc = vis.RenderOpen())
      {
        for (int y = 0; y < 50; y++)
        {
          for (int x = 0; x < 80; x++)
          {
            // Calculate position using logical pixels
            double xPos = x * cellWidth;
            double yPos = y * cellHeight;

            // Draw background rectangle
            Rect rect = new Rect(xPos, yPos, cellWidth, cellHeight);
            dc.DrawRectangle(Brushes.Black, null, rect);

            // Create text
            FormattedText text = new FormattedText(
                "A",
                System.Globalization.CultureInfo.InvariantCulture,
                FlowDirection.LeftToRight,
                new Typeface("Consolas"),
                charHeight,
                Brushes.White,
                dpi.PixelsPerDip // VERY IMPORTANT
            );

            // Draw text with small padding (1 DIP)
            dc.DrawText(text, new Point(xPos + 1, yPos));
          }
        }


        string line = "┌─────────────────────────────────────────────────────────┬────────────────────┐";
        FormattedText text2 = new FormattedText(
      line,
      System.Globalization.CultureInfo.InvariantCulture,
      FlowDirection.LeftToRight,
      new Typeface("Consolas"),
      13.3,
      Brushes.White,
      dpi.PixelsPerDip // VERY IMPORTANT
  );

        // Draw text with small padding (1 DIP)
        dc.DrawText(text2, new Point(0, 0));
      }

      */
      m_vis = new DrawingVisual();
      RenderOptions.SetEdgeMode(m_vis, EdgeMode.Aliased);
      RenderOptions.SetEdgeMode(m_visTab, EdgeMode.Aliased);
      RenderOptions.SetEdgeMode(m_visTime, EdgeMode.Aliased);
      RenderOptions.SetBitmapScalingMode(m_vis, BitmapScalingMode.NearestNeighbor);
      RenderOptions.SetBitmapScalingMode(m_visTab, BitmapScalingMode.NearestNeighbor);
      RenderOptions.SetBitmapScalingMode(m_visTime, BitmapScalingMode.NearestNeighbor);

      TextOptions.SetTextFormattingMode(this, TextFormattingMode.Display);
      TextOptions.SetTextFormattingMode(m_vis, TextFormattingMode.Display);
      TextOptions.SetTextFormattingMode(m_visTab, TextFormattingMode.Display);
      TextOptions.SetTextFormattingMode(m_visTime, TextFormattingMode.Display);

      /*
      DrawWholeScreen();

      visuals.Add(m_vis);
      AddVisualChild(m_vis);

      visuals.Add(m_visTab);
      AddVisualChild(m_visTab);

      visuals.Add(m_visTime);
      AddVisualChild(m_visTime);
*/

      DrawTerminal(mainWindow);

    }


    void DrawTerminal(MainWindow mw)
    {
      const int cols = 80;
      const int rows = 50;
      const int width = 880;
      const int height = 1200;
      double charWidth = (double)width / cols;
      double charHeight = (double)height / rows;

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
            dc.DrawRectangle(Brushes.Black, null, rect);

            // Character (alternating ─ and │)
            //            string ch = (y % 2 == 0) ? "─" : "│";
            string ch = "│";
            FormattedText ft = new FormattedText(
                ch,
                CultureInfo.InvariantCulture,
                FlowDirection.LeftToRight,
                typeface,
                fontSize,
                Brushes.White,
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
      //      terminalImage.Source = bmp;
      mw.TerminalImage.Source = bmp;
    }

    private void _timer_Tick(object? sender, EventArgs e)
    {
      UpdateTime();
    }

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
      if (after[1]==':')
      {
        before += " ";
      }

      string str12 = " " + before + " " + after + " ";
      string ampm = str12.Substring(str12.Length - 3).ToLower();
      str12 = str12.Substring(0, str12.Length - 3) + ampm;
      PrintString(screen, 54, 0, str12, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));

      DrawScreen(screen, m_visTime);
    }

    private void DrawWholeScreen()
    {
      using (DrawingContext dc = m_vis.RenderOpen())
      {

               double fontSize = 13.3; // Experimentally good for Consolas at 80 cols
//        double fontSize = 13.4; // Experimentally good for Consolas at 80 cols
//        double fontSize = 20; // Experimentally good for Consolas at 80 cols
        Typeface typeface = new Typeface("Consolas");
//        string testChar = "W"; // widest in monospace
        string testChar = "─"; // widest in monospace
        

        // Important: Get accurate DIP scaling
        var dpi = VisualTreeHelper.GetDpi(this);

        // Measure character size in DIPs
        FormattedText measure = new FormattedText(
            testChar,
            System.Globalization.CultureInfo.InvariantCulture,
            FlowDirection.LeftToRight,
            typeface,
            fontSize,
            Brushes.White,
            dpi.PixelsPerDip);

        double charWidth = measure.Width;
        double charHeight = measure.Height;
        double padding = (24 - charHeight) / 2;
        //        charWidth = 7.375;
        //        charWidth = 8;
        double charWidth2 = (880) / 80;
        double charHeight2 = (1200) / 50;

        CharInfo[,] screen = new CharInfo[80, 50];
        for (int r = 0; r < 80; r++)
        {
          for (int c = 0; c < 50; c++)
          {
            screen[r, c] = new CharInfo();
          }
        }
        //screen[0, 0] = new CharInfo(Brushes.Black, Brushes.Black, ' ');
        //screen[1, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), 'P');
        //screen[2, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), 'a');
        //screen[3, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), 't');
        //screen[4, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), 'h');
        //screen[5, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), ':');
        //screen[6, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), ' ');
        //screen[7, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), 'D');
        //screen[8, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), ':');
        //screen[9, 0] = new CharInfo(Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)), '\\');

        //        PrintString(screen, 0, 0, " Path: D:\\", Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));

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

        Folder root = new Folder("My Computer");
        DriveInfo[] drives = DriveInfo.GetDrives();
        Dictionary<string, Folder> driveLookup = new Dictionary<string, Folder>();
        foreach (DriveInfo drive in drives)
        {
          Folder driveFolder = new Folder(drive.Name);
          driveLookup[drive.Name] = driveFolder;
          root.Children.Add(driveFolder);

          foreach (string path in Directory.GetDirectories(drive.Name))
          {
            string folderName = Path.GetFileName(path);
            driveFolder.Children.Add(new Folder(folderName));
          }
        }

        
        int yUpTo = 3;
        foreach (Folder folder in driveLookup["D:\\"].Children)
        {
          xa = 0;
          if (yUpTo == 50)
            break;
          string str50 = "│";
          string str51 = "+├──"+folder.Name;

          PrintString(screen, xa, yUpTo, str50, Brushes.Black, new SolidColorBrush(Color.FromRgb(204, 204, 204)));
          xa += str50.Length;
          PrintString(screen, xa, yUpTo, str51, Brushes.Black, new SolidColorBrush(Color.FromRgb(97, 214, 214)));
          xa += str51.Length;

          yUpTo += 1;
        }

        int col = 0;
        // Draw 80 x 50 grid
        for (int y = 0; y < 50; y++)
        {
          for (int x = 0; x < 80; x++)
          {
//            double posX = Math.Round(x * charWidth);
//            double posY = Math.Round(y * charHeight);// +padding;
            double posX = x * charWidth;
            double posY = y * charHeight;// +padding;

            // Draw background rectangle
            //            Rect rect = new Rect(posX, posY, charWidth + 0.1, charHeight + 0.2);
            //            Rect rect = new Rect(posX, posY, charWidth + 0.3, charHeight + 0.4);
            Rect rect = new Rect(posX, posY, charWidth, charHeight);
            col++;
            Brush b = Brushes.Black;
            switch (col % 7)
            {
              case 0:
                b = Brushes.Red;
                break;
              case 1:
                b = Brushes.Yellow;
                break;
              case 2:
                b = Brushes.Blue;
                break;
              case 3:
                b = Brushes.Pink;
                break;
              case 4:
                b = Brushes.PeachPuff;
                break;
              case 5:
                b = Brushes.Yellow;
                break;
              case 6:
                b = Brushes.YellowGreen;
                break;
            }
            //dc.DrawRectangle(b, null, rect);
            if (y <= 50)
            {
              
               GuidelineSet guidelines2 = new GuidelineSet();
              guidelines2.GuidelinesX.Add(posX);
              guidelines2.GuidelinesX.Add(posX + charWidth);
              guidelines2.GuidelinesY.Add(posY);
              guidelines2.GuidelinesY.Add(posY + charHeight);
              dc.PushGuidelineSet(guidelines2);
              

              dc.DrawRectangle(screen[x, y].Background, null, rect);

              dc.Pop(); // Pop the guideline set
//              */
            }
            else
              dc.DrawRectangle(b, null, rect);

            Brush f = Brushes.White;
            String str = "A";
            if (y <= 50)
            {
              str = screen[x, y].Ch.ToString();
              f = screen[x, y].Foreground;
            }


            // Draw character
            //FormattedText text = new FormattedText(
            //    str,
            //    System.Globalization.CultureInfo.InvariantCulture,
            //    FlowDirection.LeftToRight,
            //    typeface,
            //    fontSize,
            //    f,
            //    dpi.PixelsPerDip);



            GuidelineSet guidelines = new GuidelineSet();
            guidelines.GuidelinesX.Add(posX);
            guidelines.GuidelinesX.Add(posX + charWidth);
            guidelines.GuidelinesY.Add(posY);
            guidelines.GuidelinesY.Add(posY + charHeight);
            dc.PushGuidelineSet(guidelines);

            //            text.TextFormattingMode = TextFormattingMode.Display; // <-- Important!

            //            dc.DrawText(text, new Point(posX, posY));


            string theS = str; // "-"
            var text = new FormattedText(theS, CultureInfo.InvariantCulture, FlowDirection.LeftToRight,
typeface, fontSize, f, dpi.PixelsPerDip);


            //            charWidth = 11;
            //            charHeight = 24;

            dc.DrawText(text, new Point(x * charWidth, y * charHeight));



            dc.Pop(); // Pop the guideline set


          }
        }
//        dc.DrawRectangle(Brushes.AliceBlue, null, new Rect(0, 0, 590, 760));
      }



      UpdateTime();
    }

    DrawingVisual m_vis;

    DrawingVisual m_visTab = new DrawingVisual();
    DrawingVisual m_visTime = new DrawingVisual();

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

    private void DrawScreen(CharInfo[,] screen, DrawingVisual vis)
    {
      using (DrawingContext dc = vis.RenderOpen())
      {

        double fontSize = 13.3; // Experimentally good for Consolas at 80 cols
        Typeface typeface = new Typeface("Consolas");
        string testChar = "W"; // widest in monospace

        // Important: Get accurate DIP scaling
        var dpi = VisualTreeHelper.GetDpi(this);

        // Measure character size in DIPs
        FormattedText measure = new FormattedText(
            testChar,
            System.Globalization.CultureInfo.InvariantCulture,
            FlowDirection.LeftToRight,
            typeface,
            fontSize,
            Brushes.White,
            dpi.PixelsPerDip);

        double charWidth = measure.Width;
        double charHeight = measure.Height;
        double padding = (24 - charHeight) / 2;

        int col = 0;
        // Draw 80 x 50 grid
        for (int y = 0; y < 50; y++)
        {
          for (int x = 0; x < 80; x++)
          {
            if (!screen[x, y].Dirty)
            {
              continue;
            }

            double posX = x * charWidth;
            double posY = y * charHeight;// +padding;

            // Draw background rectangle
            Rect rect = new Rect(posX, posY, charWidth + 0.1, charHeight + 0.2);
            col++;
            Brush b = Brushes.Black;
            switch (col % 7)
            {
              case 0:
                b = Brushes.Red;
                break;
              case 1:
                b = Brushes.Yellow;
                break;
              case 2:
                b = Brushes.Blue;
                break;
              case 3:
                b = Brushes.Pink;
                break;
              case 4:
                b = Brushes.PeachPuff;
                break;
              case 5:
                b = Brushes.Yellow;
                break;
              case 6:
                b = Brushes.YellowGreen;
                break;
            }
            //dc.DrawRectangle(b, null, rect);
            if (y <= 2)
            {
              dc.DrawRectangle(screen[x, y].Background, null, rect);
            }
            else
              dc.DrawRectangle(b, null, rect);

            Brush f = Brushes.White;
            String str = "A";
            if (y <= 2)
            {
              str = screen[x, y].Ch.ToString();
              f = screen[x, y].Foreground;
            }

            if (str!=" ")
            {
              // Draw character
              FormattedText text = new FormattedText(
                  str,
                  System.Globalization.CultureInfo.InvariantCulture,
                  FlowDirection.LeftToRight,
                  typeface,
              fontSize,
                  f,
                  dpi.PixelsPerDip);

              dc.DrawText(text, new Point(posX, posY));
            }

          }
        }
      }
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

      DrawScreen(screen, m_visTab);

      UpdateTime();

      //      DrawWholeScreen();

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

    protected override int VisualChildrenCount => visuals.Count;
    protected override Visual GetVisualChild(int index) => visuals[index];
  }

}
