#pragma once
#include <string>
#include <fstream>

namespace SimpleXTree
{
  class RandomAccessBufferedReader
  {
  public:
    RandomAccessBufferedReader();
    ~RandomAccessBufferedReader();
    void Open(std::wstring const& pathToFile);
    std::streampos GetEnd() const;
    void SeekAndRead(std::streampos pos, char* memblock, std::streamsize size);
    void Close();
  private:
    std::wstring m_pathToFile;
    std::streampos m_end;
  public:
    std::ifstream m_source;
  private:
    bool m_isOpen;
    std::streampos m_from;
    char* m_memblock;
    std::streamsize m_size;
  };
}