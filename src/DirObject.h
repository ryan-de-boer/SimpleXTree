#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>

namespace SimpleXTree
{
  class DirObject
  {
  public:
    DirObject(std::string const& path, DirObject* parent);
	DirObject(DirObject const& copy);
    ~DirObject();

    DirObject* Parent;
    bool IsExpanded;
    std::string Path;
	std::wstring PathW() const;
	std::wstring PathChar() const;

    void Expand();
	void ExpandAll();
	void Collapse();
    void Expand(int initialCount, int selectedPath);
	void ApplyFileSpec(std::wstring const& fileSpec);
    std::vector<DirObject> ChildrenPaths;
	std::vector<std::wstring> Files;
	std::vector<std::wstring> FileSpecFiles;
	std::wstring FileSpec;
	std::vector<bool> Tags;
	bool GetTag(int index) const;
  std::vector<std::wstring> GetTaggedFiles() const;
  void Tag(int fileIndex);
  void Tag();
	void Untag();
	void TagAndChildren();
	void UntagAndChildren();
	unsigned long long DirSize;
	DWORD Attrib;
	std::wstring GetAttributes();
	std::wstring GetCreatedDate();
	std::wstring GetCreatedTime();
	//std::vector<long> FileSizes;
	//unsigned long GetFileSizeL(int i);
	unsigned long long GetAllFilesSize();
	std::string GetFileName(int index) const;
  std::wstring GetFileNameW(int index) const;
  std::wstring GetFileNameFileSpecW(int index) const;

    int RecursiveSize();
	int NumFilesRecursive();
	std::wstring NumFilesRecursiveW();

    std::string GetIndexValue(int currentIndex, int selectedPath);

    bool IsLastOfParent();

	void WriteToFile();
	void Write(std::ofstream& file);

	std::vector<DirObject*> AllPaths;
	void Compile();
	void Compile(std::vector<DirObject*>& vec);

  std::string GetName() const;
  std::wstring GetNameW() const;

  private:
	  //unsigned long GetFileSizeL(std::wstring filename);
	  //unsigned long GetFileSizeWin(std::wstring filename);

	  typedef std::wstring String;
	  typedef std::vector<String> StringVector;
	  typedef unsigned long long uint64_t;

	  uint64_t CalculateDirSize(const String &path, StringVector *errVect = NULL, uint64_t size = 0);
	  bool IsBrowsePath(const String& path);
  };
}