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

#include <chrono>
#include <iostream>
#include <ctime>
#include <vector>

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

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

void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);
extern CHAR_INFO *m_bufScreen;
extern int nScreenWidth;			// Console Screen Size X (columns)
extern int nScreenHeight;			// Console Screen Size Y (rows)
std::wstring GetHex(char o);
std::wstring GetHexPadded8(int num);
std::wstring GetChar(char o);
extern char* memblock2;
void DrawStringSkipSpace(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);
extern bool searchMode;
extern std::wstring searchHex;

namespace SimpleXTree
{
  const byte * begin(const byte* str) { return str; }
  const byte * end(const byte* str, int numBytes) { return str + numBytes; }

  enum COLOUR
  {
    FG_BLACK = 0x0000,
    FG_DARK_BLUE = 0x0001,
    FG_DARK_GREEN = 0x0002,
    FG_DARK_CYAN = 0x0003,
    FG_DARK_RED = 0x0004,
    FG_DARK_MAGENTA = 0x0005,
    FG_DARK_YELLOW = 0x0006,
    FG_GREY = 0x0007, // Thanks MS :-/
    FG_DARK_GREY = 0x0008,
    FG_BLUE = 0x0009,
    FG_GREEN = 0x000A,
    FG_CYAN = 0x000B,
    FG_RED = 0x000C,
    FG_MAGENTA = 0x000D,
    FG_YELLOW = 0x000E,
    FG_WHITE = 0x000F,
    BG_BLACK = 0x0000,
    BG_DARK_BLUE = 0x0010,
    BG_DARK_GREEN = 0x0020,
    BG_DARK_CYAN = 0x0030,
    BG_DARK_RED = 0x0040,
    BG_DARK_MAGENTA = 0x0050,
    BG_DARK_YELLOW = 0x0060,
    BG_GREY = 0x0070,
    BG_DARK_GREY = 0x0080,
    BG_BLUE = 0x0090,
    BG_GREEN = 0x00A0,
    BG_CYAN = 0x00B0,
    BG_RED = 0x00C0,
    BG_MAGENTA = 0x00D0,
    BG_YELLOW = 0x00E0,
    BG_WHITE = 0x00F0,
  };

  enum eCursor
  {
    CUR_ONE,
    CUR_TWO
  };

  __int64 GetYCoord(__int64 cursorPosition1, eCursor cur)
  {
    return cursorPosition1 / 16;
  }

  __int64 GetXCoord(__int64 cursorPosition1, eCursor cur)
  {
    __int64 cursorPosition = cursorPosition1 % 16;

    // Group 1
    if (cursorPosition == 0 && cur == CUR_ONE)
    {
      return 10;
    }
    if (cursorPosition == 0 && cur == CUR_TWO)
    {
      return 11;
    }
    if (cursorPosition == 1 && cur == CUR_ONE)
    {
      return 13;
    }
    if (cursorPosition == 1 && cur == CUR_TWO)
    {
      return 14;
    }
    if (cursorPosition == 2 && cur == CUR_ONE)
    {
      return 16;
    }
    if (cursorPosition == 2 && cur == CUR_TWO)
    {
      return 17;
    }
    if (cursorPosition == 3 && cur == CUR_ONE)
    {
      return 19;
    }
    if (cursorPosition == 3 && cur == CUR_TWO)
    {
      return 20;
    }

    // Group 2
    if (cursorPosition == 4 && cur == CUR_ONE)
    {
      return 23;
    }
    if (cursorPosition == 4 && cur == CUR_TWO)
    {
      return 24;
    }
    if (cursorPosition == 5 && cur == CUR_ONE)
    {
      return 26;
    }
    if (cursorPosition == 5 && cur == CUR_TWO)
    {
      return 27;
    }
    if (cursorPosition == 6 && cur == CUR_ONE)
    {
      return 29;
    }
    if (cursorPosition == 6 && cur == CUR_TWO)
    {
      return 30;
    }
    if (cursorPosition == 7 && cur == CUR_ONE)
    {
      return 32;
    }
    if (cursorPosition == 7 && cur == CUR_TWO)
    {
      return 33;
    }

    // Group 3
    if (cursorPosition == 8 && cur == CUR_ONE)
    {
      return 36;
    }
    if (cursorPosition == 8 && cur == CUR_TWO)
    {
      return 37;
    }
    if (cursorPosition == 9 && cur == CUR_ONE)
    {
      return 39;
    }
    if (cursorPosition == 9 && cur == CUR_TWO)
    {
      return 40;
    }
    if (cursorPosition == 10 && cur == CUR_ONE)
    {
      return 42;
    }
    if (cursorPosition == 10 && cur == CUR_TWO)
    {
      return 43;
    }
    if (cursorPosition == 11 && cur == CUR_ONE)
    {
      return 45;
    }
    if (cursorPosition == 11 && cur == CUR_TWO)
    {
      return 46;
    }

    // Group 4
    if (cursorPosition == 12 && cur == CUR_ONE)
    {
      return 49;
    }
    if (cursorPosition == 12 && cur == CUR_TWO)
    {
      return 50;
    }
    if (cursorPosition == 13 && cur == CUR_ONE)
    {
      return 52;
    }
    if (cursorPosition == 13 && cur == CUR_TWO)
    {
      return 53;
    }
    if (cursorPosition == 14 && cur == CUR_ONE)
    {
      return 55;
    }
    if (cursorPosition == 14 && cur == CUR_TWO)
    {
      return 56;
    }
    if (cursorPosition == 15 && cur == CUR_ONE)
    {
      return 58;
    }
    if (cursorPosition == 15 && cur == CUR_TWO)
    {
      return 59;
    }
    return 0;
  }

  __int64 m_cursorPosition = 1;



  eCursor m_cursor = CUR_ONE;

  Search::Search() :  m_exitThread(false),
    m_threadReadyToSearch(false), m_startBeforeSearch(0), m_numFoundBeforeSearch(0), m_theSearchPosBeforeSearch(0), m_editing(false), 
    m_activated(false), m_timePassed(0), m_timeSet(false), m_renderCursor(true)
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
//    m_startBeforeSearch = thestart;
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

    m_startBeforeSearch = thestart;
    m_numFoundBeforeSearch = numFound;
    m_theSearchPosBeforeSearch = theSearchPos;
  }

  void Search::StartSearch(std::wstring const& theSearchHex)
  {
    m_theSearchHex = theSearchHex;
    m_startBeforeSearch = thestart;
    m_threadReadyToSearch = true;
  }

  void Search::StartSearch(std::wstring const& theSearchHex, __int64 startBeforeSearch)
  {
    m_theSearchHex = theSearchHex;
    m_startBeforeSearch = startBeforeSearch;
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

  void Search::RenderNow()
  {
    m_renderCursor = true;
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    m_timePassed = millisec_since_epoch;
    m_timeSet = true;
  }

  void Search::KeyEvent(WCHAR ch)
  {
    if (!m_editing && ch == 'e')
    {
      m_editing = true;
      m_cursorPosition = 0;
      m_cursor = CUR_ONE;
      RenderNow();
    }
  }

  void Search::VK(DWORD vk)
  {
    if (vk == VK_ESCAPE)
    {
      m_editing = false;
    }
    else if (vk == VK_RIGHT)
    {
      RenderNow();
      if (m_cursor == CUR_ONE)
      {
        m_cursor = CUR_TWO;
      }
      else if (m_cursor == CUR_TWO)
      {
        m_cursorPosition++;
//        if (m_cursorPosition > 15)
        {
//          m_cursorPosition = 0;
        }
        m_cursor = CUR_ONE;

        if (m_cursorPosition > 703)
        {
          m_cursorPosition -= 16;
          bool inc = true;
          if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
            inc = false;

          if (inc)
          {
            thestart += 16;
            ReadFile();
          }
        }

      }
    }
    else if (vk == VK_LEFT)
    {
      RenderNow();
      if (m_cursor == CUR_TWO)
      {
        m_cursor = CUR_ONE;
      }
      else if (m_cursor == CUR_ONE)
      {
        m_cursorPosition--;
        if (m_cursorPosition < 0)
        {
          m_cursorPosition = 0;
          m_cursor = CUR_ONE;
        }
        else
        {
          m_cursor = CUR_TWO;
        }
      }
    }
    else if (vk == VK_DOWN)
    {
      RenderNow();
      m_cursorPosition+=16;
      if (m_cursorPosition > 703)
      {
        m_cursorPosition -= 16;
        bool inc = true;
        if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
          inc = false;

        if (inc)
        {
          thestart += 16;
          ReadFile();
        }
      }
    }
    else if (vk == VK_UP)
    {
      RenderNow();
      if (m_cursorPosition < 16)
      {
        if (thestart < 16)
        {
          thestart = 0;
        }
        else
        {
          thestart -= 16;
        }
        ReadFile();
      }
      else if (m_cursorPosition > 15)
      {
        m_cursorPosition -= 16;
      }


    }
  }

  void Search::Render()
  {
    std::wstring file = StrUtil::s2ws(theFile);
    std::wstringstream ss;
    ss << file;// << L"_" << lastChar;
    file = ss.str();
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          HEX   (no mask)  ", FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 6, 0, file, FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"────────────────────────────────────────────────────────────────────────────────", FG_GREY | BG_BLACK);

    __int64 start = thestart;
    int lNumFound = numFound;
    __int64 lTheSearchPos = theSearchPos;

    if (Searching())
    {
      start = m_startBeforeSearch;
      lNumFound = m_numFoundBeforeSearch;
//      lTheSearchPos = 0;// searchObj.m_theSearchPosBeforeSearch;
      lTheSearchPos = m_theSearchPosBeforeSearch;
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"Searching...────────────────────────────────────────────────────────────────────", FG_BLACK | BG_CYAN);
    }

    //			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, L"00000000  DA C4 C4 C4  C4 5B 20 42  49 45 57 20  56 65 72 73   ┌────[ BIEW Vers", FG_CYAN | BG_BLACK);
    //			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 3, L"00000010  69 6F 6E 20  36 2E 31 2E  30 20 5D C4  C4 C4 C4 C4   ion 6.1.0 ]─────", FG_CYAN | BG_BLACK);

    int line = 2;
    __int64 index = 0;
    __int64 endy = start + 688;
    if (endy > theend)
      endy = theend;

    __int64 printed = 0;
    __int64 off = 0;
    bool isFirst = false;
    //        for (int i = start; i <= endy; i += 16)
    bool isPrinted = false;
    bool startedPrinting = false;
    for (__int64 i = start; i <= endy;)
    {
      isPrinted = false;
      //          if (i >= numMemBlock2-std::streampos(start))
      //            break;

      std::wstringstream buf;
      std::wstringstream side;
      std::wstringstream ss;

      //          bool extra = false;
      //				  buf << GetHexPadded8(i) << L"  DA C4 C4 C4  C4 5B 20 42  49 45 57 20  56 65 72 73   ┌────[ BIEW Vers";
      if (endy != 0)
      {
        buf << GetHexPadded8(i) << L"  ";
        if (!startedPrinting)
          off += 10;
      }
      else
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
      }

      for (int j = 0; j < 4; ++j)
      {
        if (index + j + start < theend)
        {
          if (i >= lTheSearchPos && i <= lTheSearchPos + std::streampos(lNumFound) && printed < lNumFound)
          {
            ss << GetHex(memblock2[index + j]);
            isPrinted = true;
            startedPrinting = true;
            printed++;
            if (printed != lNumFound)
            {
              ss << L" ";
            }
          }

          buf << GetHex(memblock2[index + j]) << L" ";
          side << GetChar(memblock2[index + j]);
          if (!startedPrinting)
            off += 3;
        }
        else
        {
          buf << L"   ";
          side << L" ";
        }
        ++i;
      }
      index += 4;
      buf << L" ";
      if (isPrinted)
      {
        ss << L" ";
      }
      //          if (printed < lNumFound-1)
      if (!startedPrinting)
      {
        off += 1;
      }
      isPrinted = false;

      for (int j = 0; j < 4; ++j)
      {
        if (index + j + start < theend)
        {
          if (i >= lTheSearchPos && i <= lTheSearchPos + std::streampos(lNumFound) && printed < lNumFound)
          {
            ss << GetHex(memblock2[index + j]);
            isPrinted = true;
            startedPrinting = true;
            printed++;
            if (printed != lNumFound)
            {
              ss << L" ";
            }
          }

          buf << GetHex(memblock2[index + j]) << L" ";
          side << GetChar(memblock2[index + j]);
          if (!startedPrinting)
            off += 3;
        }
        else
        {
          buf << L"   ";
          //              off += 3;
          side << L" ";
        }
        ++i;
      }
      index += 4;
      buf << L" ";
      if (isPrinted)
      {
        ss << L" ";
      }
      //          if (printed < lNumFound-2)
      if (!startedPrinting)
      {
        off += 1;
      }
      isPrinted = false;

      for (int j = 0; j < 4; ++j)
      {
        if (index + j + start < theend)
        {
          if (i >= lTheSearchPos && i <= lTheSearchPos + std::streampos(lNumFound) && printed < lNumFound)
          {
            ss << GetHex(memblock2[index + j]);
            isPrinted = true;
            startedPrinting = true;
            printed++;
            if (printed != lNumFound)
            {
              ss << L" ";
            }
          }

          buf << GetHex(memblock2[index + j]) << L" ";
          side << GetChar(memblock2[index + j]);
          if (!startedPrinting)
            off += 3;
        }
        else
        {
          buf << L"   ";
          //              off += 3;
          side << L" ";
        }
        ++i;
      }
      index += 4;
      buf << L" ";
      if (isPrinted)
      {
        ss << L" ";
      }
      //          if (printed < lNumFound-1)
      if (!startedPrinting)
      {
        off += 1;
      }
      isPrinted = false;

      for (int j = 0; j < 4; ++j)
      {
        //            if (i+index + j -start- tweak< theend)
        if (index + j + start < theend)
        {
          if (i >= lTheSearchPos && i <= lTheSearchPos + std::streampos(lNumFound) && printed < lNumFound)
          {
            ss << GetHex(memblock2[index + j]);
            isPrinted = true;
            startedPrinting = true;
            printed++;
            if (printed != lNumFound)
            {
              ss << L" ";
            }
          }

          buf << GetHex(memblock2[index + j]) << L" ";
          side << GetChar(memblock2[index + j]);
          if (!startedPrinting)
            off += 3;
        }
        else
        {
          buf << L"   ";
          //              off += 3;
          side << L" ";
        }
        ++i;
      }
      index += 4;
      buf << L"  ";
      if (isPrinted)
      {
        ss << L" ";
      }
      //          if (printed < lNumFound-1)
      if (!startedPrinting)
      {
        off += 1;
      }
      isPrinted = false;

      //				  buf << L"┌────[ BIEW Vers";
      buf << side.str() << L" ";

      //				  "DA C4 C4 C4  C4 5B 20 42  49 45 57 20  56 65 72 73   ┌────[BIEW Vers";
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buf.str(), FG_CYAN | BG_BLACK);

      if (lNumFound > 0)
      {
        std::wstring oneLess = ss.str();

        if (oneLess.length()>0 && oneLess[oneLess.length() - 1] == 32)
        {
          oneLess = oneLess.substr(0, oneLess.length() - 1);
        }
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, off, line, oneLess, FG_BLACK | BG_GREY);
      }
      line++;
      off = 0;
    }

    for (int i = line; i <= 45; ++i)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
      line++;
    }

    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"═══════════════════════════════════════════════════════════════─────────────────", FG_GREY | BG_BLACK);
    line++;

    {
      //https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
      if (!m_timeSet)
      {
        auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        m_timePassed = millisec_since_epoch;
        m_timeSet = true;
      }

      auto currentMillisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
      if (currentMillisec_since_epoch - m_timePassed > 500)
      {
        m_timePassed = currentMillisec_since_epoch;
        m_timeSet = true;
        m_renderCursor = !m_renderCursor;
      }

      if (m_editing && m_renderCursor)
      {
        __int64 x = GetXCoord(m_cursorPosition, m_cursor);
        __int64 y = GetYCoord(m_cursorPosition, m_cursor);
        std::wstring hex = GetHex(memblock2[m_cursorPosition]);
        std::wstring hex1 = hex.substr(0, 1);
        std::wstring hex2 = hex.substr(1, 1);
        std::wstring hexChar = L"";
        if (m_cursor == CUR_ONE)
        {
          hexChar = hex1;
        }
        else if (m_cursor == CUR_TWO)
        {
          hexChar = hex2;
        }
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, hexChar, FG_RED | BG_WHITE);
      }
    }

    if (searchMode)
    {
      if (endOfSearch)
      {
        int a = 1;
        a++;
      }
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"Search for text:", FG_GREY | BG_BLACK);
      std::wstringstream sb;
      sb << L"                 " << searchHex;
      //          DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                 CC 39 38", FG_CYAN | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, sb.str(), FG_CYAN | BG_BLACK);
      line++;
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                               F2 case sensitive (no )  F4 search for (hex    ) ", FG_GREY | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                               F2                       F4                     ", FG_CYAN | BG_BLACK);
      line++;
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"Enter search string                                 ◄─┘ ok  F1 help  ESC cancel ", FG_GREY | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                    ◄─┘     F1       ESC        ", FG_CYAN | BG_BLACK);
      line++;
    }
    else
    {
      if (searchHex != L"" && endOfSearch)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"End of Search                                                                   ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                         ◄─┘ ok ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                         ◄─┘    ", FG_CYAN | BG_BLACK);
      }
      else if (m_editing)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"HEX EDIT  ◄─┘ save and exit  F8 undo  TAB hex/ascii  CTRL V paste               ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          ◄─┘                F8       TAB            CTRL V                     ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"COMMANDS                                                                        ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"←↑↓→ position cursor                                        F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                            F1       ESC        ", FG_CYAN | BG_BLACK);
        line++;
      }
      else
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"VIEW      ASCII  Dump  Edit  Find  Hex  Mask  Wordwrap  Jump                    ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          A      D     E     F     H    M     W         J                       ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"COMMANDS  0..9 goto bookmark         F9 search  F5 seedos  SPACE search again   ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          0..9                       F9         F5         SPACE                ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"←↑↓→ scroll  SHIFT 0..9 set ALT 0..9 clear bookmarks        F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"             SHIFT 0..9     ALT 0..9                        F1       ESC        ", FG_CYAN | BG_BLACK);
        line++;
      }
    }

  }
}
