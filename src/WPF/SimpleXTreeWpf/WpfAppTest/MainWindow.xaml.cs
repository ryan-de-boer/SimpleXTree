using System.Globalization;
using System.Windows;
using System.Windows.Media;

namespace WpfAppTest
{




  public partial class MainWindow : Window
  {
    const int TargetCols = 80;
    const int TargetRows = 50;
    const int TargetWidth = 880;
    const int TargetHeight = 1200;

    public MainWindow()
    {
      InitializeComponent();
      this.Width = TargetWidth;
      this.Height = TargetHeight;

      var bestFontSize = FindBestFontSize("Consolas", TargetCols, TargetRows, TargetWidth, TargetHeight);

      // Draw test output
      this.Content = new DrawingVisualHost(bestFontSize);
    }

    double FindBestFontSize(string fontName, int cols, int rows, double maxWidth, double maxHeight)
    {
      var dpi = VisualTreeHelper.GetDpi(this).PixelsPerDip;
      double fontSize = 1.0;

      // Increment font size until we hit the limit
      while (true)
      {
        var formatted = new FormattedText("W", CultureInfo.InvariantCulture, FlowDirection.LeftToRight,
            new Typeface(fontName), fontSize, Brushes.Black, dpi);

        double totalWidth = formatted.Width * cols;
        double totalHeight = formatted.Height * rows;

        if (totalWidth > maxWidth || totalHeight > maxHeight)
          break;

        fontSize += 0.1;
      }

      return fontSize - 0.1; // Backtrack a step to last working size
    }
  }


}
