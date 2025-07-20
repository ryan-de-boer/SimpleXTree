using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleXTreeWpf
{
  public class Folder
  {
    public Folder(string name) {
      this.Name = name;
    }

    public string Name { get; set; }

    public List<Folder> Children { get; set; } = new List<Folder>();
  }
}
