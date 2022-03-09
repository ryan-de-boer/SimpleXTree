#include "stdafx.h"
#include "RandomAccessBufferedReader.h"
#include <Windows.h>

namespace SimpleXTree
{

  RandomAccessBufferedReader::RandomAccessBufferedReader() : m_isOpen(false)
  {
  }

  void RandomAccessBufferedReader::Open(std::wstring const& pathToFile)
  {
    if (m_isOpen && m_pathToFile.compare(pathToFile)==0)
    {
      return;
    }
    if (m_isOpen)
    {
      Close();
    }
    m_pathToFile = pathToFile;
    m_source.open(m_pathToFile, std::ios::in | std::ios::binary);
    m_source.seekg(std::streampos(0), std::ios::end);
    m_end = m_source.tellg();
    if (m_end == std::streampos(-1))
    {
      int a = 2;
      a++;
    }
    m_isOpen = true;

//    m_size = 5000;
    m_size = 5000000;
//    m_size = 15000000;
    m_memblock = new char[m_size];

    m_from = std::streampos(0);
    m_source.seekg(m_from, std::ios::beg);
    m_source.read(m_memblock, m_size);
  }

  RandomAccessBufferedReader::~RandomAccessBufferedReader()
  {
    Close();
  }

  std::streampos RandomAccessBufferedReader::GetEnd() const
  {
    if (m_end == std::streampos(-1))
    {
      int a = 2;
      a++;
    }
    return m_end;
  }

  void RandomAccessBufferedReader::SeekAndRead(std::streampos pos, char* memblock, std::streamsize size)
  {
    if (pos >= m_from && pos+size < m_from+m_size)
    {
      memcpy_s(memblock, size, m_memblock+pos-m_from, size);
    }
    else
    {
      m_from = pos - m_size / 2;
      if (m_from + m_size > m_end)
      {
        m_from = pos;
        m_size = m_end - m_from;
      }
      else
      {
        m_size = 5000000;
        m_from = pos - m_size / 2;
        if (m_from + m_size > m_end)
        {
          m_from = pos;
          m_size = m_end - m_from;
        }
      }
      if (m_from < 0)
      {
        m_from = std::streampos(0);
      }
      ZeroMemory(m_memblock, m_size);
      bool isO = m_source.is_open();
      m_source.seekg(m_from, std::ios::beg);
//      m_source.read(m_memblock, m_size);

      m_source.read(m_memblock, m_size);

      std::streamsize re = m_source.gcount();
      if (re != m_size)
      {
        int c = 1;
        c++;
      }

//      if (strcmp(m_memblock, "") == 0)
//      {
//        int a = 1;
//          a++;
//          Close();
//
//          m_source.open(m_pathToFile, std::ios::in | std::ios::binary);
//          m_isOpen = true;
// //         m_size = 5000;
//
////          m_from = std::streampos(0);
//          m_source.seekg(m_from, std::ios::beg);
//          m_source.read(m_memblock, m_size);
//
//      }
      if (m_from == std::streampos(0))
      {
        memcpy_s(memblock, size, m_memblock, size);
      }
      else if (m_from != pos)
      {
        memcpy_s(memblock, size, m_memblock + m_size / 2, size);
      }
      else
      {
        memcpy_s(memblock, size, m_memblock, size);
      }
    }
  }

  void RandomAccessBufferedReader::Close()
  {
    m_source.close();
    m_isOpen = false;
  }
}
