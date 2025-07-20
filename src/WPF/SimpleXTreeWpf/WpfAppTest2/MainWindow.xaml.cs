using System.Globalization;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace WpfTerminal
{
  public partial class MainWindow : Window
  {
    const int cols = 80;
    const int rows = 50;
    const int width = 880;
    const int height = 1200;
    double charWidth = (double)width / cols;
    double charHeight = (double)height / rows;

    public MainWindow()
    {
      InitializeComponent();
      DrawTerminal();
    }

    void DrawTerminal()
    {
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
      TerminalImage.Source = bmp;
    }
  }
}
