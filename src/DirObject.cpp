#include "stdafx.h"
#include "DirObject.h"
#include <filesystem>
#include "StrUtil.h"
#include <fstream>
#include <windows.h>

namespace SimpleXTree
{
  DirObject::DirObject(std::string const& path, DirObject* parent) : Parent(parent), IsExpanded(false), Path(path)
  {
  }

  DirObject::DirObject(DirObject const& copy)
  {
    this->Path = copy.Path;
    this->Parent = copy.Parent;
    this->IsExpanded = copy.IsExpanded;
	  this->ChildrenPaths = copy.ChildrenPaths;
	  this->AllPaths = copy.AllPaths;
  }

  DirObject::~DirObject()
  {
  }

  std::string DirObject::GetName() const
  {
    if (Path.find("\\")!=std::string::npos)
    {
      return Path.substr(Path.rfind("\\")+1);
    }
    return Path;
  }

  std::wstring DirObject::GetNameW() const
  {
	  return StrUtil::s2ws(GetName());
  }

  std::string DirObject::GetFileName(int index) const
  {
	  std::string path = StrUtil::ws2s(Files[index]);
	  if (path.find("\\") != std::string::npos)
	  {
      std::string p2 = path.substr(path.rfind("\\") + 1);
      if (p2 == "")
      {
        int a = 1;
        a++;
      }
      return p2;
	  }
    if (path == "")
    {
      int a = 1;
      a++;
    }
    return path;
  }

  std::wstring DirObject::GetFileNameW(int index) const
  {
    std::wstring path = Files[index];
    if (path.find(L"\\") != std::wstring::npos)
    {
      std::wstring p2 = path.substr(path.rfind(L"\\") + 1);
      if (p2 == L"")
      {
        int a = 1;
        a++;
      }
      return p2;
    }
    if (path == L"")
    {
      int a = 1;
      a++;
    }
    return path;
  }

  std::wstring DirObject::PathW() const
  {
	  return StrUtil::s2ws(Path);
  }

  std::wstring DirObject::PathChar() const
  {
	  return PathW().substr(0, 1);
  }

  void DirObject::Expand()
  {
    Collapse();
    IsExpanded = true;

    //std::vector<std::string> dFiles;
    for (const auto & entry : std::experimental::filesystem::directory_iterator(Path))
    {
      if (std::experimental::filesystem::is_directory(entry.path()))
      {
        std::wstring f = entry.path();
        std::wstring sub = f.substr(Path.length());
        DirObject newPath = DirObject(StrUtil::ws2s(f), this);
        ChildrenPaths.push_back(newPath);
      }
      else
      {
        std::wstring f = entry.path();
        std::wstring sub = f.substr(Path.length());
        //dFiles.push_back(StrUtil::ws2s(f));
//        Files.push_back(StrUtil::ws2s(f));
        Files.push_back(f);
      }
    }
  }

  void DirObject::Collapse()
  {
    IsExpanded = false;
    ChildrenPaths.clear();
	Files.clear();
  }
  
  bool DirObject::IsLastOfParent()
  {
    if (Parent != NULL)
    {
      return &Parent->ChildrenPaths[Parent->ChildrenPaths.size() - 1] == this;
    }
    return false;
  }


  void DirObject::Expand(int initialCount, int selectedPath)
  {
    int count = initialCount;
    if (selectedPath == initialCount)
    {
      Expand();
      return;
    }
    for (int i = 0; i < ChildrenPaths.size(); ++i)
    {
      count++;
      if (selectedPath == count)
      {
        ChildrenPaths[i].Expand();
        return;
      }
      ChildrenPaths[i].Expand(count, selectedPath);
    }
  }

  int DirObject::RecursiveSize()
  {
    if (ChildrenPaths.size() == 0)
    {
      return 1;
    }

    int size = 0;
    for (int i = 0; i < ChildrenPaths.size(); ++i)
    {
      size += ChildrenPaths[i].RecursiveSize();
    }
    return size;
  }

  std::string DirObject::GetIndexValue(int currentIndex, int selectedPath)
  {
    int count = currentIndex;
    if (selectedPath == currentIndex)
    {
      return Path;
    }
    for (int i = 0; i < ChildrenPaths.size(); ++i)
    {
      count++;
      if (selectedPath == count)
      {
        return ChildrenPaths[i].Path;
      }
      return ChildrenPaths[i].GetIndexValue(count, selectedPath);
    }
  }


  
  void DirObject::WriteToFile()
  {
	  	std::ofstream myfile;
	  	myfile.open("wtf.txt");

		myfile << std::hex << GetConsoleWindow() << std::endl;

	  //int size = RecursiveSize();
	  //int currentIndex = 0;
	  //for (int i = 0; i < size; ++i)
	  //{
		 // std::string value = GetIndexValue(currentIndex, i);

		 // myfile << value << std::endl;
	  //}

		//for (int i = 0; i < ChildrenPaths.size(); ++i)
		//{

		//}
		Write(myfile);

	  myfile.close();
  }

  void DirObject::Write(std::ofstream& file)
  {
	  file << Path << std::endl;
	  for (int i = 0; i < ChildrenPaths.size(); ++i)
	  {
		  ChildrenPaths[i].Write(file);
	  }
  }

  void DirObject::Compile()
  {
	  AllPaths.clear();
	  Compile(AllPaths);
  }

  void DirObject::Compile(std::vector<DirObject*>& vec)
  {
	  vec.push_back(this);
	  for (int i = 0; i < ChildrenPaths.size(); ++i)
	  {
		  ChildrenPaths[i].Compile(vec);
	  }
  }

}

