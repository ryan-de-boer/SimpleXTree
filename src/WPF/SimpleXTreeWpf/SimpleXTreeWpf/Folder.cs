﻿using System;
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

    public string Name { get; set; } = "";

    public bool Selected { get; set; } = false;
    public bool MouseOver { get; set; } = false;

    public List<Folder> Children { get; set; } = new List<Folder>();

    public Folder Parent { get; set; } = null;

    public string GetAbsolutePath()
    {
      Stack<Folder> stack = new Stack<Folder>();
      stack.Push(this);
      stack.Push(Parent);
      Folder nextFolder = Parent;
      while (nextFolder.Parent != null)
      {
        stack.Push(nextFolder);
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
