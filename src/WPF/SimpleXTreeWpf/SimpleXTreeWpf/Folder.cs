using System;
using System.Collections.Generic;
using System.IO;
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

    public int Indent { get; set; } = 0;
    public bool IsLastChild 
    {
      get
      {
        if (Name=="D:\\")
        {
          return true;
        }
        return m_isLastChild;
      }
      set
      {
        m_isLastChild = value;
      }
    }
      private bool m_isLastChild = false;

    public string Name { get; set; } = "";

    public bool Selected { get; set; } = false;
    public bool MouseOver { get; set; } = false;

    public List<Folder> Children { get; set; } = new List<Folder>();

    public Folder Parent { get; set; } = null;

    public bool IsExpanded { get; set; } = false;

    public void Expand()
    {
      if (!IsExpanded)
      {
        string absolutePath = GetAbsolutePath();
        string[] dirs = Directory.GetDirectories(absolutePath);
        for (int i=0;i<dirs.Length;++i)
        {
          string path = dirs[i];
          string folderName = Path.GetFileName(path);
          Folder newFolder = new Folder(folderName);
          newFolder.Indent = Indent + 1;
          Children.Add(newFolder);
          newFolder.Parent = this;
          if (i==dirs.Length-1)
          {
            newFolder.IsLastChild = true;
          }
        }
        IsExpanded = true;

        if (Parent!=null)
        {
          string parentAbsolutePath = Parent.GetAbsolutePath();
          string[] parentDirs = Directory.GetDirectories(parentAbsolutePath);
          if (this.GetAbsolutePath().Equals(parentDirs[parentDirs.Length-1]+"\\"))
          {
            IsLastChild = true;
          }
        }
      }
    }

    public List<Folder> GetAllChildren()
    {
      List<Folder> retList = new List<Folder>();
      foreach (Folder f in Children)
      {
        retList.Add(f);
        retList.AddRange(f.GetAllChildren());
      }
      return retList;
    }

    public string GetAbsolutePath()
    {
      Stack<Folder> stack = new Stack<Folder>();
      stack.Push(this);
      if (Parent!=null)
      {
        stack.Push(Parent);
      }
      Folder nextFolder = Parent;
      while (nextFolder!=null && nextFolder.Parent != null)
      {
        stack.Push(nextFolder.Parent);
        nextFolder = nextFolder.Parent;
      }

      StringBuilder stringBuilder= new StringBuilder();
      foreach (Folder folder in stack)
      { 
        stringBuilder.Append(folder.Name);
        if (!folder.Name.EndsWith("\\"))
        {
          stringBuilder.Append("\\");
        }
      }

      return stringBuilder.ToString();
      
    }
  }
}
