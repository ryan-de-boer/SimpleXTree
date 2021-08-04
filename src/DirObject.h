#pragma once
#include <string>
#include <vector>
#include <fstream>

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
    void Collapse();
    void Expand(int initialCount, int selectedPath);
    std::vector<DirObject> ChildrenPaths;
	std::vector<std::wstring> Files;
	std::string GetFileName(int index) const;
  std::wstring GetFileNameW(int index) const;

    int RecursiveSize();

    std::string GetIndexValue(int currentIndex, int selectedPath);

    bool IsLastOfParent();

	void WriteToFile();
	void Write(std::ofstream& file);

	std::vector<DirObject*> AllPaths;
	void Compile();
	void Compile(std::vector<DirObject*>& vec);

  std::string GetName() const;
  std::wstring GetNameW() const;

  };
}