#include "stdafx.h"
#include "Search.h"
#include <filesystem>
#include "StrUtil.h"
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <windows.h>
#include <filesystem>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include <direct.h>
#include <thread>
#include "FreeDiskSpace.h"
#include "Stopwatch.h"

using namespace std;
int HexToInt2(std::wstring str);
void ReadFile();

extern std::streampos thestart;
extern std::streampos theend;
extern std::streampos theSearchPos;
extern std::streampos theAfterSearchPos;
extern bool endOfSearch;
extern std::string pathToFile;
extern std::string theFile;
extern int numMemBlock2;
extern int numFound;

namespace SimpleXTree
{
  const byte * begin(const byte* str) { return str; }
  const byte * end(const byte* str, int numBytes) { return str + numBytes; }

  Search::Search() :  m_exitThread(false),
    m_threadReadyToSearch(false), m_startBeforeSearch(0), m_numFoundBeforeSearch(0), m_theSearchPosBeforeSearch(0)
  {
    //https://softwareengineering.stackexchange.com/questions/382195/is-it-okay-to-start-a-thread-from-within-a-constructor-of-a-class
    m_member_thread = std::thread(&Search::ThreadFn, this);
  }

  Search::~Search()
  {
    m_exitThread = true;
    m_member_thread.join();
  }

  void Search::Search3(std::wstring theSearchHex)
  {
    m_startBeforeSearch = thestart;
    m_numFoundBeforeSearch = numFound;
    m_theSearchPosBeforeSearch = theSearchPos;

    win32::Stopwatch starta;
    starta.Start();

    std::vector<char> chSearch;
    std::vector<bool> chFound;
    for (std::wstring::iterator pos = theSearchHex.begin(); pos != theSearchHex.end(); ++pos)
    {
      wchar_t ch = *pos;
      if (ch == 32) //space
      {
        ++pos;
      }
      std::wstringstream hex;
      hex << *pos;
      ++pos;
      hex << *pos;

      int num = HexToInt2(hex.str());
      chSearch.push_back((char)num);
      chFound.push_back(false);
    }
    byte* aSearch = (byte*)&chSearch[0];

    std::streampos searchPos = thestart;

    ifstream source;
    source.open(pathToFile, ios::in | ios::binary);
    source.seekg(std::streampos(0), ios::end);
    theend = source.tellg();
    source.seekg(std::streampos(0), ios::beg);

    if (theend < 100000000)
    {
      numMemBlock2 = theend;
    }
    else
    {
      numMemBlock2 = 100000000; //100 meg try
    }

    byte* memblock3 = new byte[numMemBlock2];

    source.seekg(std::streampos(thestart), ios::beg);
    source.read((char*)memblock3, numMemBlock2);

    int currentFound = -1;
    bool foundEmAll = false;
    endOfSearch = false;

    bool atTheEnd = false;
    while (!atTheEnd)
    {
      auto it = std::search(SimpleXTree::begin(memblock3), SimpleXTree::end(memblock3, numMemBlock2), SimpleXTree::begin(aSearch), SimpleXTree::end(aSearch, chSearch.size()));

      if (it == SimpleXTree::end(memblock3, numMemBlock2))
      {
        // not found
        int b = 1;
        b++;
      }
      else
      {
        // subrange found at std::distance(std::begin(Buffer), it)
        int index = std::distance(SimpleXTree::begin(memblock3), it);
        searchPos = thestart + std::streampos(index);
        numFound = chSearch.size();
        int b = 1;
        b++;
        break;
      }

      thestart += numMemBlock2 - chSearch.size();
      if (thestart >= theend)
      {
        atTheEnd = true;
        endOfSearch = true;
        numFound = 0;
        break;
      }
      source.seekg(std::streampos(thestart), ios::beg);
      source.read((char*)memblock3, numMemBlock2);
    }

    starta.Stop();
    double seconds = starta.ElapsedSeconds();

    thestart = searchPos;
    theSearchPos = searchPos;
    theAfterSearchPos = searchPos + std::streampos(chSearch.size());
    ReadFile();
  }

  void Search::StartSearch(std::wstring const& theSearchHex)
  {
    m_theSearchHex = theSearchHex;
    m_threadReadyToSearch = true;
  }

  bool Search::Searching() const
  {
    return m_threadReadyToSearch;
  }

  void Search::ThreadFn()
  {
    if (m_exitThread)
      return;

    while (!m_exitThread)
    {

      if (m_threadReadyToSearch)
      {
        Search3(m_theSearchHex);
        m_threadReadyToSearch = false;
      }
      Sleep(10);
    }
  }
}
