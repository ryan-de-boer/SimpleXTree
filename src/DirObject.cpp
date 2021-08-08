#include "stdafx.h"
#include "DirObject.h"
#include <filesystem>
#include "StrUtil.h"
#include <fstream>
#include <sstream>
#include <windows.h>

namespace SimpleXTree
{
  DirObject::DirObject(std::string const& path, DirObject* parent) : Parent(parent), IsExpanded(false), Path(path), DirSize(0)
  {
  }

  DirObject::DirObject(DirObject const& copy)
  {
    this->Path = copy.Path;
    this->Parent = copy.Parent;
    this->IsExpanded = copy.IsExpanded;
	  this->ChildrenPaths = copy.ChildrenPaths;
	  this->AllPaths = copy.AllPaths;
	  this->DirSize = copy.DirSize;
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

	DirSize = CalculateDirSize(this->PathW());

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
//		FileSizes.push_back(GetFileSizeL(f));
      }
    }
  }

  void DirObject::Collapse()
  {
    IsExpanded = false;
    ChildrenPaths.clear();
	Files.clear();
	DirSize = 0;
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

  int DirObject::NumFilesRecursive()
  {
	  int numFiles = this->Files.size();
	  for (int i = 0; i < this->ChildrenPaths.size(); ++i)
	  {
		  numFiles += this->ChildrenPaths[i].NumFilesRecursive();
	  }
	  return numFiles;
  }

  std::wstring DirObject::NumFilesRecursiveW()
  {
	  std::wstringstream buf;
	  buf << NumFilesRecursive();
	  return buf.str();
  }

  //https://stackoverflow.com/questions/10015341/size-of-a-directory
  //support pagefile.sys
  //https://stackoverflow.com/questions/16772931/getfileattributes-on-locked-system-file

  uint64_t DirObject::CalculateDirSize(const String &path, StringVector *errVect, uint64_t size)
  {
	  WIN32_FIND_DATA data;
	  HANDLE sh = NULL;
	  sh = FindFirstFile((path + L"\\*").c_str(), &data);

	  if (sh == INVALID_HANDLE_VALUE)
	  {
		  //if we want, store all happened error  
		  if (errVect != NULL)
			  errVect->push_back(path);
		  return size;
	  }

	  do
	  {
		  // skip current and parent
		  if (!IsBrowsePath(data.cFileName))
		  {
			  // if found object is ...
			  if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				  // directory, then search it recursievly
				  ;//				  size = CalculateDirSize(path + L"\\" + data.cFileName, NULL, size);
			  else
			  {
				  // otherwise get object size and add it to directory size
//				  size += (uint64_t)(data.nFileSizeHigh * (MAXDWORD)+data.nFileSizeLow);

				  _int64 filesize = data.nFileSizeHigh;
				  filesize <<= 32;
				  filesize |= data.nFileSizeLow;
				  size += (uint64_t)(filesize);
			  }
		  }

	  } while (FindNextFile(sh, &data)); // do

	  FindClose(sh);

	  return size;
  }

  bool DirObject::IsBrowsePath(const String& path)
  {
	  return (path == _T(".") || path == _T(".."));
  }


  ////https://stackoverflow.com/questions/5840148/how-can-i-get-a-files-size-in-c
  //unsigned long DirObject::GetFileSizeL(std::wstring filename)
  //{
	 // //std::string str = StrUtil::ws2s(filename);
	 // //struct stat stat_buf;
	 // //int rc = stat(str.c_str(), &stat_buf);
	 // //return rc == 0 ? stat_buf.st_size : -1;

	 // unsigned long size = GetFileSizeWin(filename);
	 // return size;
  //}

  //unsigned long DirObject::GetFileSizeWin(std::wstring filename)
  //{
	 // HANDLE hFile = CreateFile(filename.c_str(),               // file to open
		//  GENERIC_READ,          // open for reading
		//  FILE_SHARE_READ,       // share for reading
		//  NULL,                  // default security
		//  OPEN_EXISTING,         // existing file only
		//  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
		//  NULL);                 // no attr. template
	 // if (hFile == INVALID_HANDLE_VALUE)
	 // {
		//  int a = 1;
		//  a++;
	 // }
	 // DWORD dwFileSize = GetFileSize(hFile, NULL);
	 // return dwFileSize;
  //}

  //unsigned long DirObject::GetFileSizeL(int i)
  //{
	 // return GetFileSizeL(Files[i]);
  //}

  unsigned long long DirObject::GetAllFilesSize()
  {
	  unsigned long long total = 0;
	  total += DirSize;
	  //for (int i = 0; i < Files.size(); ++i)
	  //{
		 // for (int j = 0; j < ChildrenPaths.size(); ++j)
		 // {
			//  total += ChildrenPaths[j].GetAllFilesSize();
		 // }
	  //}

	   for (int j = 0; j < ChildrenPaths.size(); ++j)
	   {
	    total += ChildrenPaths[j].GetAllFilesSize();
	   }

	  return total;
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

