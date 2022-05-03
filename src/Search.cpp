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
//#include "rope.hpp"
#include "076_gapbuf.h"

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
void RunProcessHide(std::wstring const& filePath, std::wstring const& arg);

extern std::streampos thestart;
extern std::streampos theend;
extern std::streampos theSearchPos;
extern std::streampos theAfterSearchPos;
extern bool endOfSearch;
extern std::string pathToFile;
extern std::string theFile;
extern int numMemBlock2;
extern int numFound;
extern int m_lastI;
extern wchar_t* chars;

void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);
extern CHAR_INFO *m_bufScreen;
extern int nScreenWidth;			// Console Screen Size X (columns)
extern int nScreenHeight;			// Console Screen Size Y (rows)
std::wstring GetHex(char o);
std::wstring GetHexPadded8(int num);
int HexToInt2(std::wstring str);
std::wstring GetChar(char o);
extern char* memblock2;
extern char* rmemblock2;
extern int rnumMemBlock2;
extern std::streampos rtheend;
void rReadFile();
void DrawStringSkipSpace(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);
extern bool searchMode;
extern std::wstring searchHex;
std::wstring GetClipboardText();
bool HasFocus();
std::wstring GetAppPath();
void ClearFileCache();

int g_itemsp = 0;
int g_itemsleft = 0;
int g_leftSpace = 0;
int g_rightSpace = 0;
extern HANDLE ghConsole;
GapBuf* gbuf;

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

  enum eSearchType
  {
    S_HEX,
    S_TEXT
  };

  __int64 Search::GetYCoord(__int64 cursorPosition1, eCursor cur)
  {
    return cursorPosition1 / 16;
  }

  __int64 Search::GetDumpYCoord(__int64 cursorPosition1, eCursor cur)
  {
    return cursorPosition1 / 64;
  }

  __int64 Search::GetXCoord(__int64 cursorPosition1, eCursor cur)
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

  __int64 Search::GetDumpXCoord(__int64 cursorPosition1, eCursor cur)
  {
    __int64 cursorPosition = 10 + cursorPosition1 % 64;
    return cursorPosition;
  }

  __int64 m_cursorPosition = 1;



  eCursor m_cursor = CUR_ONE;

  Search::Search() : m_exitThread(false),
    m_threadReadyToSearch(false), m_startBeforeSearch(0), m_numFoundBeforeSearch(0), m_theSearchPosBeforeSearch(0), m_editing(false),
    m_editingAscii(false), m_editingDump(false), m_editingText(false), m_jumping(false), m_jumpingFirstChar(false), m_saving(false),
    m_activated(false), m_timePassed(0), m_timeSet(false), m_renderCursor(true), m_hasFocus(false), m_typed(L""), m_renderAscii(false), 
    m_renderDump(false), m_wordwrap(false), m_newline(true), m_showingLastLine(false), m_searchType(S_HEX)
  {
    //https://softwareengineering.stackexchange.com/questions/382195/is-it-okay-to-start-a-thread-from-within-a-constructor-of-a-class
    m_member_thread = std::thread(&Search::ThreadFn, this);

    for (int i = 0; i < 9; ++i)
    {
      m_bookmarks[i] = 0;
      m_bookmarksSet[i] = false;
    }
  }

  Search::~Search()
  {
    m_exitThread = true;
    m_member_thread.join();
  }

  std::wstring currentTest = L"Untitled";
  bool testPassed = true;
  std::wstring errorMessage = L"";
  bool allTestsPassed = true;

  //std::vector<std::wstring> currentTests;
  //std::vector<bool> testPasseds;
  //std::vector<std::wstring> errorMessages;

  std::vector<TestSuite> testSuites;

  void SetCurrentTestSuite(std::wstring const& testSuiteName)
  {
    TestSuite ts;
    ts.Name = testSuiteName;
    testSuites.push_back(ts);
  }

  void SetCurrentTest(std::wstring const& testName)
  {
    if (!StrUtil::EqualsIgnoreCase(currentTest, L"Untitled"))
    {
      testSuites[testSuites.size()-1].currentTests.push_back(currentTest);
      testSuites[testSuites.size() - 1].testPasseds.push_back(testPassed);
      testSuites[testSuites.size() - 1].errorMessages.push_back(errorMessage);
    }
    currentTest = testName;
    testPassed = true;
    errorMessage = L"";
  }

  void EndTestsSuite()
  {
    testSuites[testSuites.size() - 1].currentTests.push_back(currentTest);
    testSuites[testSuites.size() - 1].testPasseds.push_back(testPassed);
    testSuites[testSuites.size() - 1].errorMessages.push_back(errorMessage);
    currentTest = L"Untitled";
  }

  bool Search::Assert(int actualValue, int expectedValue, std::wstring const& customAssertMessage)
  {
    if (actualValue != expectedValue)
    {
      std::wstringstream msg;
      msg << customAssertMessage;
      if (!StrUtil::EndsWith(customAssertMessage, L"."))
      {
        msg << L".";
      }
      msg << L" Actual: " << actualValue << L", expected: " << expectedValue << L". Quit?";
//      MessageBox(NULL, msg.str().c_str(), L"Assert Failure", MB_OK);

      const int result = MessageBox(NULL, msg.str().c_str(), L"Assert Failure", MB_YESNOCANCEL);

      switch (result)
      {
      case IDYES:
        exit(0);
        break;
      case IDNO:
        // Do something
        break;
      case IDCANCEL:
        // Do something
        break;
      }

      testPassed = false;
      if (StrUtil::EqualsIgnoreCase(errorMessage, L""))
      {
        errorMessage = msg.str();
      }
      else
      {
        errorMessage = errorMessage + L"_BR_"+ msg.str();
      }
      allTestsPassed = false;
      return false;
    }
    return true;
  }

  bool Search::Assert(int actualValue, int expectedValue, std::wstringstream const& customAssertMessage)
  {
    return Assert(actualValue, expectedValue, customAssertMessage.str());
  }

  void TestSuiteWriteTestReport(TestSuite const& testSuite, std::wofstream& fileStream)
  {
    unsigned int numFailed = 0;
    unsigned int numTests = 0;
    for (size_t i = 0; i<testSuite.testPasseds.size(); ++i)
    {
        numTests++;
        if (!testSuite.testPasseds[i])
        {
          numFailed++;
        }
    }

    fileStream << L"    <testsuite name=\"" << testSuite.Name << L"\" tests=\"" <<
      numTests << L"\" failures=\"0\" errors=\"" << numFailed <<
      L"\" time=\"0.0\">" << std::endl;

    for (size_t i = 0; i<testSuite.testPasseds.size(); ++i)
    {
      {
        fileStream << L"        <testcase name=\"" << testSuite.currentTests[i] << L"\" time=\"0.0\">";
        if (!testSuite.testPasseds[i])
        {
          fileStream << L"<error message=\"" << testSuite.errorMessages[i] << L"\"/>";
        }
        fileStream << L"</testcase>" << std::endl;
      }
    }

    fileStream << L"    </testsuite>" << std::endl;
  }

  void DeleteTestReport()
  {
    std::wstring path = GetAppPath() + L"\\TestData\\TestReport_AutomatedTesting.xml";
    std::experimental::filesystem::remove(path);

    path = GetAppPath() + L"\\TestData\\TestReport.html";
    std::experimental::filesystem::remove(path);
  }

  void WriteTestReport()
  {
    std::wofstream fileStream(GetAppPath() + L"\\TestData\\TestReport_AutomatedTesting.xml", std::ios::out);
    fileStream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" << std::endl;
    fileStream << "<?xml-stylesheet type=\"text/xsl\" href=\"junit-noframes_mod_notime.xsl\"?>" << std::endl;
    fileStream << "<testsuites>" << std::endl;

    //for (size_t i = 0; i<m_testSuites.size(); ++i)
    //{
    //  TestSuite* testSuite = m_testSuites[i];
    //  if (testSuite->GetEnabled())
    //  {
    //    testSuite->WriteTestReport(fileStream);
    //  }
    //}

    for (size_t i = 0; i<testSuites.size(); ++i)
    {
      TestSuiteWriteTestReport(testSuites[i], fileStream);
    }

    fileStream << "</testsuites>" << std::endl;
    fileStream.close();
  }

  void ConvertToHtml()
  {
    RunProcessHide(GetAppPath() + L"\\TestData\\msxsl.exe", GetAppPath() + L"\\TestData\\msxsl.exe " + GetAppPath() + L"\\TestData\\TestReport_AutomatedTesting.xml " + GetAppPath() + L"\\TestData\\junit-noframes_mod_notime.xsl -o " + GetAppPath() + L"\\TestData\\TestReport.html");

    std::wifstream t(GetAppPath() + L"\\TestData\\TestReport.html");
    std::wstringstream buffer;
    buffer << t.rdbuf();
    std::wstring contents = buffer.str();
    t.close();
    StrUtil::ReplaceAll(contents, L"_BR_", L"<br>");

    std::wofstream fs;
    fs.open(GetAppPath() + L"\\TestData\\TestReport.html", std::wofstream::out);
    fs << contents.c_str();
    fs.close();
  }

  void OpenTestReport()
  {
    std::wstring path = (GetAppPath() + L"\\TestData\\TestReport.html");

    std::wstringstream wstr2;
    wstr2 << " /c " << "\"" << path << "\"";

    ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", wstr2.str().c_str(), NULL, SW_HIDE);
  }

  inline bool exists_test31(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
  }

  void Search::SetupTest(std::wstring const& viewFile)
  {
    TCHAR szPath[MAX_PATH];

    GetModuleFileName(NULL, szPath, MAX_PATH);

    if (viewFile.find(L":") != std::wstring::npos) // If rooted don't append current dir.
    {
      pathToFile = StrUtil::ws2s(viewFile);
    }
    else
    {
      // Else append current dir.
      std::wstringstream ss2;
      ss2 << szPath;
      std::wstring p = ss2.str();
      StrUtil::Replace(p, L"SimpleXTree.exe", viewFile);
      pathToFile = StrUtil::ws2s(p);
    }

    rReadFile();

    gbuf = new_buffer(2);

    for (int i = 0; i < rtheend; ++i)
    {
      insert_character(gbuf, rmemblock2[i]);
    }

    while (gb_front(gbuf) != 0)
    {
      cursor_left(gbuf);
    }
    m_cursorPosition = 0;

    while (m_cursorPosition < gb_front(gbuf))
    {
      cursor_right(gbuf);
    }

    m_renderAscii = true;
    m_wordwrap = true;
    m_renderDump = false;

    std::wstringstream ss2;
    ss2.str(L"");
    ss2 << szPath;
    std::wstring p = ss2.str();
    StrUtil::Replace(p, L"SimpleXTree.exe", L"ExtendedAciiValue.txt");
    std::string srr2 = StrUtil::ws2s(p);

    if (exists_test31(srr2))
    {
      ifstream rf(srr2, ios::out | ios::binary);
      if (!rf) {
        cout << "Cannot open file!" << endl;
        return;
      }
      wchar_t  c;
      rf.read((char *)&c, 2); // skip FF FE header
      for (int i = 0; i <= 254; i += 1)
      {
        rf.read((char *)&c, 2);
        chars[i] = c;
        //      std::wstring ws = GetChar(c);
        int a = 1;
      }
      rf.close();
    }
  }

  void Search::Test(std::wstring const& args)
  {

    if (StrUtil::EqualsIgnoreCase(args, L"test") || args.find(L"TestCursorDown") != std::wstring::npos)
    {
      SetupTest(L"TestData\\_short.txt");
      SetCurrentTestSuite(L"TestCursorDown");
      SetCurrentTest(L"TestShortTxtCursorDown_Start");
      int cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 9, L"Expected cursorToEndOfLine to be 9");
      int nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 0, L"Expected nextLineToCursor to be 0");

      SetCurrentTest(L"TestShortTxtCursorDown_Right1");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 8, L"Expected cursorToEndOfLine to be 8");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 1, L"Expected nextLineToCursor to be 1");

      SetCurrentTest(L"TestShortTxtCursorDown_Right2");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 7, L"Expected cursorToEndOfLine to be 7");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 2, L"Expected nextLineToCursor to be 2");

      SetCurrentTest(L"TestShortTxtCursorDown_Right3");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 6, L"Expected cursorToEndOfLine to be 6");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 3, L"Expected nextLineToCursor to be 3");

      SetCurrentTest(L"TestShortTxtCursorDown_Right4");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 5, L"Expected cursorToEndOfLine to be 5");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 4, L"Expected nextLineToCursor to be 4");

      SetCurrentTest(L"TestShortTxtCursorDown_Right5");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 4, L"Expected cursorToEndOfLine to be 4");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 5, L"Expected nextLineToCursor to be 5");

      SetCurrentTest(L"TestShortTxtCursorDown_Right6");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 3, L"Expected cursorToEndOfLine to be 3");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 5, L"Expected nextLineToCursor to be 5");

      SetCurrentTest(L"TestShortTxtCursorDown_Right7");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 2, L"Expected cursorToEndOfLine to be 2");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 5, L"Expected nextLineToCursor to be 5");

      SetCurrentTest(L"TestShortTxtCursorDown_Right8");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 1, L"Expected cursorToEndOfLine to be 1");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 5, L"Expected nextLineToCursor to be 5");

      SetCurrentTest(L"TestShortTxtCursorDown_Right9");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 0, L"Expected cursorToEndOfLine to be 0");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 5, L"Expected nextLineToCursor to be 5");

      SetCurrentTest(L"TestShortTxtCursorDown_Line2");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 5, L"Expected cursorToEndOfLine to be 5");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 0, L"Expected nextLineToCursor to be 0");

      SetCurrentTest(L"TestShortTxtCursorDown_Line2_Right1");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 4, L"Expected cursorToEndOfLine to be 4");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 1, L"Expected nextLineToCursor to be 1");

      SetCurrentTest(L"TestShortTxtCursorDown_Line2_Right2");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 3, L"Expected cursorToEndOfLine to be 3");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 2, L"Expected nextLineToCursor to be 2");

      SetCurrentTest(L"TestShortTxtCursorDown_Line2_Right3");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 2, L"Expected cursorToEndOfLine to be 2");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 2, L"Expected nextLineToCursor to be 2");

      SetCurrentTest(L"TestShortTxtCursorDown_Line2_Right4");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 1, L"Expected cursorToEndOfLine to be 1");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 2, L"Expected nextLineToCursor to be 2");

      SetCurrentTest(L"TestShortTxtCursorDown_Line2_Right5");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 0, L"Expected cursorToEndOfLine to be 0");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 2, L"Expected nextLineToCursor to be 2");

      SetCurrentTest(L"TestShortTxtCursorDown_Line3");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 2, L"Expected cursorToEndOfLine to be 2");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 0, L"Expected nextLineToCursor to be 0");

      SetCurrentTest(L"TestShortTxtCursorDown_Line3_Right1");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 1, L"Expected cursorToEndOfLine to be 1");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 0, L"Expected nextLineToCursor to be 0");

      SetCurrentTest(L"TestShortTxtCursorDown_Line3_Right2");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 0, L"Expected cursorToEndOfLine to be 0");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, 0, L"Expected nextLineToCursor to be 0");

      SetCurrentTest(L"TestShortTxtCursorDown_Line4");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 0, L"Expected cursorToEndOfLine to be 0");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, -1, L"Expected nextLineToCursor to be -1");

      SetCurrentTest(L"TestShortTxtCursorDown_Line4_Right1");
      VK(VK_RIGHT);
      cursorToEndOfLine = GetCursorToEndOfLine();
      Assert(cursorToEndOfLine, 0, L"Expected cursorToEndOfLine to be 0");
      nextLineToCursor = GetNextLineToCursor();
      Assert(nextLineToCursor, -1, L"Expected nextLineToCursor to be -1");

      SetupTest(L"TestData\\_short2.txt");

      SetCurrentTest(L"TestShort2TxtCursorDown_Loop");
      // bore ten. Parish any chatty can elinor direct for former. Up as meant
      int j = 0;
      for (int i = 69; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 27)
        {
          // should stop at 27
          ++j;
        }
      }

      // widow equal an share least.
      j = 0;
      for (int i = 27; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be -1";
        Assert(nextLineToCursor, -1, buf);
        VK(VK_RIGHT);
      }

      SetupTest(L"TestData\\along2.txt");

      SetCurrentTest(L"TestAlongTxtCursorDown_Loop");
      //std::vector<int> endOfLines;
      //std::vector<int> nextLines;
      //endOfLines.push_back(65);
      //nextLines.push_back(64);
      //endOfLines.push_back(64);
      //nextLines.push_back(62); //62line or 72?

      //for (int k = 0; k < endOfLines.size(); ++k)
      //{
      //  j = 0;
      //  for (int i = endOfLines[k]; i >= 0; --i)
      //  {
      //    cursorToEndOfLine = GetCursorToEndOfLine();
      //    std::wstringstream buf;
      //    buf << L"Expected cursorToEndOfLine to be " << i;
      //    Assert(cursorToEndOfLine, i, buf);
      //    nextLineToCursor = GetNextLineToCursor();
      //    buf.str(L"");
      //    buf << L"Expected nextLineToCursor to be " << j;
      //    Assert(nextLineToCursor, j, buf);
      //    VK(VK_RIGHT);
      //    if (j < nextLines[k])
      //    {
      //      // should stop here
      //      ++j;
      //    }
      //  }
      //}

      // 1Folly words widow one downs few age every seven. If miss part by 
      j = 0;
      for (int i = 65; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 64)
        {
          // should stop at 64
          ++j;
        }
      }

      // fact he park just shew. Discovered had get considered projection
      j = 0;
      for (int i = 64; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 72)
        {
          // should stop at 72
          ++j;
        }
      }

      // who favourable. Necessary up knowledge it tolerably. Unwilling departure
      j = 0;
      for (int i = 72; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 64)
        {
          // should stop at 64
          ++j;
        }
      }

      // education is be dashwoods or an. Use off agreeable law unwilling
      j = 0;
      for (int i = 64; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 67)
        {
          // should stop at 67
          ++j;
        }
      }

      // sir deficient curiosity instantly. Easy mind life fact with see has
      j = 0;
      for (int i = 67; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 69)
        {
          // should stop at 69
          ++j;
        }
      }

      // bore ten. Parish any chatty can elinor direct for former. Up as meant
      j = 0;
      for (int i = 69; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 27)
        {
          // should stop at 27
          ++j;
        }
      }

      // widow equal an share least.
      j = 0;
      for (int i = 27; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
      }

      // ""
      j = 0;
      for (int i = 0; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
      }

      // Talent she for lively eat led sister. Entrance strongly packages
      j = 0;
      for (int i = 64; i >= 0; --i)
      {
        cursorToEndOfLine = GetCursorToEndOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToEndOfLine to be " << i;
        Assert(cursorToEndOfLine, i, buf);
        nextLineToCursor = GetNextLineToCursor();
        buf.str(L"");
        buf << L"Expected nextLineToCursor to be " << j;
        Assert(nextLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j < 70)
        {
          // should stop at 70
          ++j;
        }
      }


      //SetCurrentTest(L"TestShort2TxtCursorDown_Start");
      //cursorToEndOfLine = GetCursorToEndOfLine();
      //Assert(cursorToEndOfLine, 69, L"Expected cursorToEndOfLine to be 69");
      //nextLineToCursor = GetNextLineToCursor();
      //Assert(nextLineToCursor, 0, L"Expected nextLineToCursor to be 0");

      //SetCurrentTest(L"TestShort2TxtCursorDown_Right1");
      //VK(VK_RIGHT);
      //cursorToEndOfLine = GetCursorToEndOfLine();
      //Assert(cursorToEndOfLine, 68, L"Expected cursorToEndOfLine to be 68");
      //nextLineToCursor = GetNextLineToCursor();
      //Assert(nextLineToCursor, 1, L"Expected nextLineToCursor to be 1");

      //SetCurrentTest(L"TestShort2TxtCursorDown_Right2");
      //VK(VK_RIGHT);
      //cursorToEndOfLine = GetCursorToEndOfLine();
      //Assert(cursorToEndOfLine, 67, L"Expected cursorToEndOfLine to be 67");
      //nextLineToCursor = GetNextLineToCursor();
      //Assert(nextLineToCursor, 2, L"Expected nextLineToCursor to be 2");

      //SetCurrentTest(L"TestShort2TxtCursorDown_Right3");
      //VK(VK_RIGHT);
      //cursorToEndOfLine = GetCursorToEndOfLine();
      //Assert(cursorToEndOfLine, 66, L"Expected cursorToEndOfLine to be 66");
      //nextLineToCursor = GetNextLineToCursor();
      //Assert(nextLineToCursor, 3, L"Expected nextLineToCursor to be 3");

      //SetCurrentTest(L"TestShort2TxtCursorDown_Right4");
      //VK(VK_RIGHT);
      //cursorToEndOfLine = GetCursorToEndOfLine();
      //Assert(cursorToEndOfLine, 65, L"Expected cursorToEndOfLine to be 65");
      //nextLineToCursor = GetNextLineToCursor();
      //Assert(nextLineToCursor, 4, L"Expected nextLineToCursor to be 4");

      //SetCurrentTest(L"TestShort2TxtCursorDown_Right5");
      //VK(VK_RIGHT);
      //cursorToEndOfLine = GetCursorToEndOfLine();
      //Assert(cursorToEndOfLine, 64, L"Expected cursorToEndOfLine to be 64");
      //nextLineToCursor = GetNextLineToCursor();
      //Assert(nextLineToCursor, 5, L"Expected nextLineToCursor to be 5");

      //Assert(0, 1, L"Expected differs");

      EndTestsSuite();

    }
    SetupTest(L"TestData\\_short.txt");

    if (StrUtil::EqualsIgnoreCase(args, L"test") || args.find(L"TestCursorUp") != std::wstring::npos)
    {

      SetCurrentTestSuite(L"TestCursorUp");
      SetCurrentTest(L"TestShortTxtCursorUp_Loop");

      //for (int i = 0; i < 10; ++i)
      //{
      //  VK(VK_RIGHT);
      //}


//      int c = GetCursorToStartOfLine(); //0
//      int d = GetPreviousLineToCursor(); //9
//      Assert(c, 0, L"c unexpected");
//      Assert(d, 9, L"d unexpected");

      // some line
      for (int i = 0; i <= 9; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be -1";
        Assert(previousLineToCursor, -1, buf);
        VK(VK_RIGHT);
      }

      // short
      int j = 9;
      for (int i = 0; i <= 5; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 4)
        {
          // should stop at 4
          --j;
        }
      }

      // mo
      j = 5;
      for (int i = 0; i <= 2; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 3)
        {
          // should stop at 3
          --j;
        }
      }

      // ""
      j = 2;
      for (int i = 0; i <= 0; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 2)
        {
          // should stop at 2
          --j;
        }
      }

      SetupTest(L"TestData\\_short2.txt");
      SetCurrentTest(L"TestShort2TxtCursorUp_Loop");

      // bore ten. Parish any chatty can elinor direct for former. Up as meant
      for (int i = 0; i <= 69; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be -1";
        Assert(previousLineToCursor, -1, buf);
        VK(VK_RIGHT);
      }

      // widow equal an share least.
      j = 69;
      for (int i = 0; i <= 27; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 27)
        {
          // should stop at 27
          --j;
        }
      }

      SetupTest(L"TestData\\along2.txt");
      SetCurrentTest(L"TestAlongTxtCursorUp_Loop");

      // 1Folly words widow one downs few age every seven. If miss part by
      int lineCount = std::wstring(L"1Folly words widow one downs few age every seven. If miss part by").length(); //65
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be -1";
        Assert(previousLineToCursor, -1, buf);
        VK(VK_RIGHT);
      }

      // fact he park just shew. Discovered had get considered projection
      lineCount = std::wstring(L"n").length(); //64
      j = std::wstring(L"1Folly words widow one downs few age every seven. If miss part by").length(); //65
      for (int i = 0; i <= 64; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 1)
        {
          // should stop at 1
          --j;
        }
      }

      // who favourable. Necessary up knowledge it tolerably. Unwilling departure
      lineCount = std::wstring(L"who favourable. Necessary up knowledge it tolerably. Unwilling departure").length(); //72
      j = std::wstring(L"fact he park just shew. Discovered had get considered projection").length(); //64
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      // education is be dashwoods or an. Use off agreeable law unwilling
      lineCount = std::wstring(L"education is be dashwoods or an. Use off agreeable law unwilling").length(); //64
      j = std::wstring(L"who favourable. Necessary up knowledge it tolerably. Unwilling departure").length(); //72
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      // sir deficient curiosity instantly. Easy mind life fact with see has
      lineCount = std::wstring(L"sir deficient curiosity instantly. Easy mind life fact with see has").length(); //67
      j = std::wstring(L"education is be dashwoods or an. Use off agreeable law unwilling").length(); //64
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      // bore ten. Parish any chatty can elinor direct for former. Up as meant
      lineCount = std::wstring(L"bore ten. Parish any chatty can elinor direct for former. Up as meant").length(); //69
      j = std::wstring(L"sir deficient curiosity instantly. Easy mind life fact with see has").length(); //67
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      // widow equal an share least.
      lineCount = std::wstring(L"widow equal an share least.").length(); //27
      j = std::wstring(L"bore ten. Parish any chatty can elinor direct for former. Up as meant").length(); //69
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      // ""
      lineCount = 0; //0
      j = std::wstring(L"widow equal an share least.").length(); //27
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      // Talent she for lively eat led sister. Entrance strongly packages
      lineCount = std::wstring(L"Talent she for lively eat led sister. Entrance strongly packages").length(); //64
      j = 0; //0
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      //she out rendered get quitting denoting led. Dwelling confined improved
      lineCount = std::wstring(L"she out rendered get quitting denoting led. Dwelling confined improved").length(); //70
      j = std::wstring(L"Talent she for lively eat led sister. Entrance strongly packages").length(); //64
      for (int i = 0; i <= lineCount; ++i)
      {
        int cursorToStartOfLine = GetCursorToStartOfLine();
        std::wstringstream buf;
        buf << L"Expected cursorToStartOfLine to be " << i;
        Assert(cursorToStartOfLine, i, buf);
        int previousLineToCursor = GetPreviousLineToCursor();
        buf.str(L"");
        buf << L"Expected previousLineToCursor to be " << j;
        Assert(previousLineToCursor, j, buf);
        VK(VK_RIGHT);
        if (j > 0)
        {
          // should stop at 0
          --j;
        }
      }

      EndTestsSuite();
    }

    if (allTestsPassed)
    {
      MessageBox(NULL, L"Test results: PASSED", L"Automated Testing", MB_OK);
    }
    else
    {
      MessageBox(NULL, L"Test results: FAILED", L"Automated Testing", MB_OK);
    }

    const int result = MessageBox(NULL, L"View Test Report?", L"Test Report", MB_YESNOCANCEL);

    switch (result)
    {
    case IDYES:
      DeleteTestReport();
      WriteTestReport();
      ConvertToHtml();
      OpenTestReport();
      break;
    case IDNO:
      // Do something
      break;
    case IDCANCEL:
      // Do something
      break;
    }

  }

  void Search::Search3(std::wstring theSearchHex)
  {
    //    m_startBeforeSearch = thestart;
    m_numFoundBeforeSearch = numFound;
    m_theSearchPosBeforeSearch = theSearchPos;

    if (m_searchType == S_TEXT)
    {
      std::wstringstream hex;
      for (std::wstring::size_type i = 0; i < theSearchHex.size(); ++i)
      {
        hex << GetHex(theSearchHex[i]);
        if (i < theSearchHex.size() - 1)
        {
          hex << L" ";
        }
      }
      theSearchHex = hex.str();
    }

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
      if (pos == theSearchHex.end())
      {
        std::wstringstream hex2;
        hex2 << L"0";
        hex2 << hex.str();
        hex.str(L"");
        hex << hex2.str();
      }
      else
      {
        hex << *pos;
      }

      int num = HexToInt2(hex.str());
      chSearch.push_back((char)num);
      chFound.push_back(false);
      if (pos == theSearchHex.end())
      {
        break;
      }
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
    //if (m_searchType == S_TEXT)
    //{
    //  std::wstringstream hex;
    //  for (std::wstring::size_type i = 0; i < theSearchHex.size(); ++i)
    //  {
    //    hex << GetHex(theSearchHex[i]);
    //    if (i < theSearchHex.size() - 1)
    //    {
    //      hex << L" ";
    //    }
    //  }
    //  m_theSearchHex = hex.str();
    //}

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
      RenderCursor();

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

  class AnEdit
  {
  public:
    WCHAR One;
    bool OneSet;
    WCHAR Two;
    bool TwoSet;
    AnEdit::AnEdit() : OneSet(false), TwoSet(false), One(0), Two(0)
    {
    }
  };

  std::map<__int64, AnEdit> m_edits;
  std::vector<__int64> m_orderOfEdits;

  char ReadIt(__int64 pos)
  {
    std::ifstream source;
    source.open(pathToFile, ios::in | ios::binary);
    bool fo = source.is_open();
    source.seekg(std::streampos(pos), ios::beg);
    char* memblock = new char[1];
    source.read(memblock, 1);
    char ch = memblock[0];
    source.close();
    delete[] memblock;
    return ch;
  }

  void WriteIt(__int64 pos, char value)
  {
    std::fstream source;
    source.open(pathToFile, ios::in | ios::out | ios::binary);
    bool fo = source.is_open();
    source.seekp(std::streampos(pos), ios::beg);
    source.write(&value, 1);
    source.close();
  }

  //https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array
  int char2int(char input)
  {
    if (input >= '0' && input <= '9')
      return input - '0';
    if (input >= 'A' && input <= 'F')
      return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
      return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
  }

  // This function assumes src to be a zero terminated sanitized string with
  // an even number of [0-9a-f] characters, and target to be sufficiently large
  void hex2bin(const char* src, char* target)
  {
    while (*src && src[1])
    {
      *(target++) = char2int(*src) * 16 + char2int(src[1]);
      src += 2;
    }
  }

  int g_15 = -506;

  void Search::KeyEvent(WCHAR ch)
  {
    bool control = (0x8000 & GetAsyncKeyState((unsigned char)(VK_CONTROL))) != 0;
    bool shift = (0x8000 & GetAsyncKeyState((unsigned char)(VK_SHIFT))) != 0;
    bool alt = (0x8000 & GetAsyncKeyState((unsigned char)(VK_MENU))) != 0;

    int numberKey = IsNumberKey(ch);

    if (ch == '\t' || ch == '\r')
    {
      ; // skip tab and enter
    }
    else if (searchMode)
    {
      ; // skip if search mode
    }
    else if (!m_editing && !m_jumping && ch == 'e')
    {
      m_editing = true;
      if (m_renderDump)
      {
        m_editingDump = true;
      }
      if (m_renderAscii)
      {
        m_editingText = true;
      }
      m_cursorPosition = 0;
      m_cursor = CUR_ONE;
      RenderNow();
    }
    //else if (m_editing && !m_jumping && ch == 'e')
    //{
    //  m_editing = false;
    //  if (m_renderDump)
    //  {
    //    m_editingDump = false;
    //  }
    //  if (m_renderAscii)
    //  {
    //    m_editingText = false;
    //  }
    //  RenderNow();
    //}
    else if (!m_jumping && ch == 'j')
    {
      m_jumping = true;
      m_jumpingFirstChar = true;
      RenderNow();
    }
    else if (m_jumping)
    {
      if (ch == '\r' || ch == '\n' || ch == '\x1b')
      {
      }
      else if (ch == '\b')
      {
        m_jumpingFirstChar = false;
        if (m_typed.length() > 0)
        {
          m_typed = m_typed.substr(0, m_typed.length() - 1);
        }
      }
      else
      {
        if (m_jumpingFirstChar)
        {
          m_typed = L"";
        }
        m_typed += ch;
        m_jumpingFirstChar = false;
      }
    }
    else if (m_saving && ch == 'y')
    {
      m_saving = false;
      m_editing = false;
      m_editingAscii = false;
      m_editingDump = false;
      // save edits:

      std::map<__int64, AnEdit>::iterator it;
      for (it = m_edits.begin(); it != m_edits.end(); it++)
      {
        __int64 address = it->first;
        char ch = ReadIt(address);
        std::wstring hexStr = GetHex(ch);
        std::wstring one = hexStr.substr(0, 1);
        std::wstring two = hexStr.substr(1, 1);
        if (it->second.OneSet)
        {
          one = it->second.One;
        }
        if (it->second.TwoSet)
        {
          two = it->second.Two;
        }
        std::wstringstream buf;
        buf << one << two;

        char target[1];
        hex2bin(StrUtil::ws2s(buf.str()).c_str(), target);

        WriteIt(address, target[0]);
      }

      m_edits.clear();
      m_orderOfEdits.clear();
      ClearFileCache();
      ReadFile();
    }
    else if (m_saving && ch == 'n')
    {
      m_editing = false;
      m_editingAscii = false;
      m_editingDump = false;
      m_saving = false;
      m_edits.clear();
      m_orderOfEdits.clear();
    }
    else if (!m_editingText && (m_editingAscii || m_editing) && ch == '\b')
    {
      VK(VK_LEFT);
    }
    else if (m_editingAscii||m_editingDump && control && ch == 22) //ctrl-v
    {
      std::wstring clipboard = GetClipboardText();
      for (std::wstring::size_type i = 0; i < clipboard.size(); ++i)
      {
        std::wstring str = GetHex(clipboard[i]);
        m_cursor = CUR_ONE;
        InsertHexChar(str[0]);
        m_cursor = CUR_TWO;
        InsertHexChar(str[1]);
      }
    }
    else if (m_editingAscii || m_editingDump)
    {
      std::wstring str = GetHex(ch);
      m_cursor = CUR_ONE;
      InsertHexChar(str[0]);
      m_cursor = CUR_TWO;
      InsertHexChar(str[1]);
    }
    else if (m_editingText)
    {
      if (ch == '\r' || ch == '\n' || ch == '\x1b')
      {
      }
      else if (ch == '\b')
      {
        backspace(gbuf);
      }
      else
      {
        insert_character(gbuf, ch);
        m_cursorPosition++;
      }
    }
    else if (m_editingText)
    {
      int a = 1;
      a++;
    }
    else if (m_editing && ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F') || (ch >= '0' && ch <= '9')))
    {
      InsertHexChar(ch);
      int a = 1;
      a++;
    }
    else if (m_editing && control && ch == 22) //ctrl-v
    {
      std::wstring clipboard = GetClipboardText();
      for (std::wstring::size_type i = 0; i < clipboard.size(); ++i)
      {
        InsertHexChar(clipboard[i]);
      }
    }
    else if (ch == 'a')
    {
      m_renderAscii = true;
      m_renderDump = false;
      ReadFile();
    }
    else if (ch == 'h')
    {
      m_renderAscii = false;
      m_renderDump = false;
    }
    else if (ch == 'w')
    {
      m_renderAscii = true;
      m_renderDump = false;
      m_wordwrap = !m_wordwrap;
      ReadFile();
    }
    else if (ch == 'd')
    {
      m_renderDump = true;
      m_renderAscii = false;
      ReadFile();
    }
    else if (ch == 'm')
    {
      if (thestart >= 1)
      {
        thestart -= std::streampos(1);
        ReadFile();
        RenderNow();
      }
    }
    else if (ch == 'b')
    {
      thestart += std::streampos(1);
      ReadFile();
      RenderNow();
    }
    else if (ch == 'n')
    {
      m_newline = !m_newline;
      /*
      thestart = theend - std::streampos((44 * 80));
      ReadFile();
      RenderNow();
      */

      if (m_newline && false)
      {
        /*
        std::streampos estimateEnd = thestart + std::streampos((44 * 80));
        int col = 0;
//        int row = -2;
        int row = 0;
        std::streampos count = 0;
        for (std::streampos pointer = thestart; pointer < theend; pointer=pointer+std::streampos(1))
        {
          col++;
          if (col>80)
          {
            col = 0;
            row++;
            if (row >= 44)
              break;
          }
          else
          {
            count+=1;
          }
          if (memblock2[pointer-thestart] == 0x0A)
          {
            col = 0;
            row++;
            if (row >= 44)
              break;
          }
        }
        thestart = theend - std::streampos(row * 80)+(80-col-1) + g_15;

        thestart = 371874;
        */

        int col = 0;
        int row = 0;
        std::streampos pointer = theend;
        for (pointer = theend; row<44; pointer = pointer - std::streampos(1))
        {
          col++;
          if (col>80)
          {
            col = 0;
            row++;
            if (row >= 44)
            {
              pointer += row;
              break;
            }
          }
          if (memblock2[pointer-thestart] == 0x0A)
          {
            col = 0;
            row++;
            if (row >= 44)
            {
              pointer += row;
              break;
            }
          }
        }

//        thestart = theend - std::streampos(row * 80);// +(80 - col);
//        thestart = theend - std::streampos(row * 80) + 200;
        thestart = pointer;

        //now go to end?
        Down();
        while (Down() != -1)
        {
          int a = 1;
          a++;
        }

        ReadFile();
        RenderNow();
      }
    }
    else if (numberKey != -1)
    {
      if (shift)
      {
        m_bookmarks[numberKey] = thestart;
        m_bookmarksSet[numberKey] = true;
      }
      else if (alt)
      {
        m_bookmarks[numberKey] = 0;
        m_bookmarksSet[numberKey] = false;
      }
      else if (m_bookmarksSet[numberKey])
      {
        thestart = m_bookmarks[numberKey];
        ReadFile();
      }
    }
  }

  int Search::IsNumberKey(char ch) const
  {
    if (ch >= '0' && ch <= '9')
    {
      return ch - '0';
    }
    switch (ch)
    {
    case '!':
      return 1;
    case '@':
      return 2;
    case '#':
      return 3;
    case '$':
      return 4;
    case '%':
      return 5;
    case '^':
      return 6;
    case '&':
      return 7;
    case '*':
      return 8;
    case '(':
      return 9;
    case ')':
      return 0;
    }
    return -1;
  }

  void Search::End()
  {
    thestart = theend - std::streampos((44 * 80));
    ReadFile();

    int col = 0;
    int row = 0;
    std::streampos pointer = theend;
    for (pointer = theend; row<44; pointer = pointer - std::streampos(1))
    {
      col++;
      if (col>80)
      {
        col = 0;
        row++;
        if (row >= 44)
        {
          pointer += row;
          break;
        }
      }
      if (memblock2[pointer - thestart] == 0x0A)
      {
        col = 0;
        row++;
        if (row >= 44)
        {
          pointer += row;
          break;
        }
      }
    }

    thestart = pointer;
    ReadFile();

    if (GetNumLines() > 5)
    {
      Up();

      //now go to end?
      Down();
      while (Down() != -1)
      {
        int a = 1;
        a++;
      }

      ReadFile();
    }
    RenderNow();
  }

  void Search::InsertHexChar(char ch)
  {
    bool isValid = (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F') || (ch >= '0' && ch <= '9');
    if (!isValid)
    {
      return;
    }

    ch = std::toupper(ch);
    if (m_cursor == CUR_ONE)
    {
      std::map<__int64, AnEdit>::iterator it = m_edits.find(thestart + m_cursorPosition);
      if (it != m_edits.end())
      {
        it->second.One = ch;
        it->second.OneSet = true;
      }
      else
      {
        AnEdit edit;
        edit.One = ch;
        edit.OneSet = true;
        m_edits[thestart + m_cursorPosition] = edit;
      }
      m_orderOfEdits.push_back(thestart + m_cursorPosition);
      VK(VK_RIGHT);
    }
    else if (m_cursor == CUR_TWO)
    {
      std::map<__int64, AnEdit>::iterator it = m_edits.find(thestart + m_cursorPosition);
      if (it != m_edits.end())
      {
        it->second.Two = ch;
        it->second.TwoSet = true;
      }
      else
      {
        AnEdit edit;
        edit.Two = ch;
        edit.TwoSet = true;
        m_edits[thestart + m_cursorPosition] = edit;
      }
      m_orderOfEdits.push_back(thestart + m_cursorPosition);
      VK(VK_RIGHT);
    }
  }

  int Search::Down()
  {
    if (DontRenderAscii())
      return -1;

    bool inc = true;
    int numLines = GetNumLines();
    if (numLines == -1 || numLines==0)
    {
      int b = 1;
      b++;
      inc = false;
      return -1;
    }

    if (inc)
    {
      bool foundNewLine = false;
      for (int j = 0; j < theend - thestart && !foundNewLine; ++j)
      {
        if (m_wordwrap && memblock2[j] == ' ' && j > 63)
        {
          foundNewLine = true;
          if (thestart + std::streampos(j + 1) < theend)
          {
            thestart += j + 1;
            ReadFile();
          }
          break;
        }
        else if (memblock2[j] == 0x0A)
        {
          foundNewLine = true;
          if (thestart + std::streampos(j + 1) < theend)
          {
            thestart += j + 1;
            ReadFile();
          }
          break;
        }
        else if (j>79)
        {
          foundNewLine = true;
          if (thestart + std::streampos(j + 1) < theend)
          {
            thestart += j + 1;
            ReadFile();
          }
          break;
        }
      }
    }
    return numLines;
  }

  void Search::Up()
  {
    double sec2 = 0.0;
    double sec3 = 0.0;
    win32::Stopwatch sw;
    sw.Start();
    int oldStart = thestart;
    bool oneMore = false;
    bool atStart = false;
    if (thestart > 0)
    {
      int upTo80 = 0;
      while (memblock2[0] != 0x0A && thestart>0 && upTo80<80)
      {
        thestart -= 1;
        upTo80++;
        ReadFile();
      }
      if (thestart == std::streampos(0))
      {
        atStart = true;
      }
      sw.Stop();
      double sec = sw.ElapsedSeconds();
      sw.Start();

      if (memblock2[0] == 0x0A)
      {
        thestart -= 1;
        ReadFile();
        if (memblock2[0] == 0x0D)
        {
          thestart -= 1;
          ReadFile();
        }

        if (memblock2[0] == 0x0A)
        {
          thestart -= 1;
          ReadFile();
          //                   oneMore = true;

          if (memblock2[0] == 0x0D)
          {
            thestart -= 1;
            ReadFile();
            oneMore = true;
          }
        }
        if (!oneMore)
        {
          while (memblock2[0] != 0x0A && thestart>0)
          {
            thestart -= 1;
            ReadFile();
          }
        }
      }


      int newStart = oldStart;
      //                while (newStart - thestart >80)
      //while (thestart < newStart-80)
      //{
      //  searchObj.Down();
      //}
      //                if (!atStart)
      {
        //while (thestart < oldStart)
        //{
        //  searchObj.Down();
        //}
      }

      sw.Stop();
      sw.Start();
      sec3 = 0.0;

      int oldStart2 = oldStart;
      newStart = thestart;
      while (oldStart > std::streampos(newStart))
      {
        oldStart2 = thestart;
        win32::Stopwatch sw2;
        sw2.Start();
        if (Down() == -1)
        {
          break;
        }
        //if (searchObj.m_showingLastLine)
        //{
        //  break;
        //}
        sw2.Stop();
        sec3 += sw2.ElapsedSeconds();
        newStart = thestart;
      }

      thestart = oldStart2;
      ReadFile();

      sec2 = sw.ElapsedSeconds();

      //                if (oneMore && oldStart > thestart+std::streampos(80))
      //if (oneMore)
      //{
      //  searchObj.Down();
      //}
      int c = 1;
      c++;

      //72 was 64
      //for (int j = 0; j <= 64; ++j)
      //{
      //  if ((memblock2[0] != 0x0D || j <= 64) && thestart > 0)
      //  {
      //    thestart -= 1;
      //    ReadFile();
      //  }
      //  //if ((memblock2[0] == 0x0A || j<63) && thestart>0)
      //  //{
      //  //  bool end = false;
      //  //  if (memblock2[0] == 0x0A)
      //  //    end = true;
      //  //  thestart -= 1;
      //  //  ReadFile();
      //  //  if (end)
      //  //    break;
      //  //}
      //}
    }
    /*             if (memblock2[0] == ' ')
    {
    thestart += 1;
    ReadFile();
    }*/

    sw.Stop();
    double sec = sw.ElapsedSeconds();
    int z = 1;
    z++;
  }

  void Search::VK(DWORD vk)
  {
    if (m_jumping)
    {
      if (vk == VK_RETURN)
      {
        m_jumping = false;
        std::wstring typed = m_typed;
        StrUtil::ReplaceAll(typed, L"$", L"");
        if (typed.find(L"%") != std::wstring::npos)
        {
          StrUtil::ReplaceAll(typed, L"%", L"");
          int p = std::stoi(typed); // convert to int
          if (p == 100)
          {
            thestart = theend;
            if (theend >= std::streampos(688))
            {
              thestart = theend - std::streampos(688);
            }
            if (thestart >= thestart % 16)
            {
              thestart = thestart - thestart % 16;

              if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
              {
                if (thestart >= 16)
                {
                  thestart -= 16;
                }
              }

              bool inc = true;
              if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
                inc = false;
              if (inc)
              {
                thestart += 16;
              }
            }
          }
          else
          {
            double percent = (double)p / 100.0;
            thestart = theend*percent;
          }
          ReadFile();
        }
        else
        {
          int offset = HexToInt2(typed);
          thestart = offset;
          ReadFile();
        }
      }
      else if (vk == VK_ESCAPE)
      {
        m_jumping = false;
      }
      else if (vk == VK_F1)
      {
        std::wstringstream wstr;
        wstr << " /c " << "\"" << GetAppPath() << "\\help\\Jump.html" << "\"";
        ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", wstr.str().c_str(), NULL, SW_HIDE);
      }
    }
    //else if (vk == VK_ESCAPE)
    //{
    //  m_editing = false;
    //  m_editingAscii = false;
    //  m_saving = false;
    //  m_edits.clear();
    //  m_orderOfEdits.clear();
    //}
    else if (searchMode && vk == VK_F4)
    {
      if (m_searchType == S_HEX)
      {
        m_searchType = S_TEXT;
      }
      else if (m_searchType == S_TEXT)
      {
        m_searchType = S_HEX;
      }
      RenderNow();
    }
    else if (m_editing && vk == VK_F8)
    {
      m_edits.clear();
      m_orderOfEdits.clear();
    }
    else if (m_editingAscii && vk == VK_TAB)
    {
      m_editingAscii = false;
      m_cursor = CUR_ONE;
      RenderNow();
    }
    else if (m_editing && vk == VK_TAB)
    {
      m_editingAscii = true;
      RenderNow();
    }
    else if (vk == VK_RIGHT)
    {
      RenderNow();

      if (m_renderAscii)
      {
        if (m_cursorPosition < rtheend)
        {
////          char* fr = extractFront(gbuf);
//          char* bk = extractBack(gbuf);
//          if (bk[0] != '\r' && bk[0] != '\n')
//          {
// //           free(fr);
//            m_cursorPosition++;
//            cursor_right(gbuf);
//          }
//          while (bk[0] == '\r' || bk[0] == '\n')
//          {
// //           free(fr);
//            m_cursorPosition++;
//            cursor_right(gbuf);
//            bk = extractBack(gbuf);
//          }


          char* bk = extractBack(gbuf);
          if (bk[0] != '\r' && bk[0] != '\n')
          {
            m_cursorPosition++;
            cursor_right(gbuf);
            return;
          }
          if (bk[0] == '\r')
          {
            m_cursorPosition++;
            cursor_right(gbuf);
          }
          bk = extractBack(gbuf);
          if (bk[0] == '\n')
          {
            m_cursorPosition++;
            cursor_right(gbuf);
          }

        }
        return;
      }

      if (m_cursor == CUR_ONE)
      {
        m_cursor = CUR_TWO;
      }
      else if (m_renderDump && m_cursor == CUR_TWO)
      {
        if (m_cursorPosition >= (64*44-1))
        {
          bool inc = true;
          if ((thestart + std::streampos(64 * 43) + std::streampos(64)) >= (theend))
            inc = false;

          if (inc)
          {
            m_cursorPosition++;
            m_cursor = CUR_ONE;

            m_cursorPosition -= 64;
            thestart += 64;
            ReadFile();
          }
        }
        else
        {
          m_cursorPosition++;
          m_cursor = CUR_ONE;
        }
      }
      else if (!m_renderDump && m_cursor == CUR_TWO)
      {
        //        m_cursorPosition++;
        //        if (m_cursorPosition > 15)
        {
          //          m_cursorPosition = 0;
        }
        //        m_cursor = CUR_ONE;

        if (m_cursorPosition >= 703)
        {
          bool inc = true;
          if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
            inc = false;

          if (inc)
          {
            m_cursorPosition++;
            m_cursor = CUR_ONE;

            m_cursorPosition -= 16;
            thestart += 16;
            ReadFile();
          }
        }
        //else if (m_cursorPosition == 703)
        //{

        //}
        else
        {
          m_cursorPosition++;
          m_cursor = CUR_ONE;
        }

      }
    }
    else if (vk == VK_LEFT && m_renderAscii)
    {
      if (m_cursorPosition > 0)
      {

        char* fr = extractFront(gbuf);
        char* bk = extractBack(gbuf);

        if (fr[strlen(fr) - 2] != '\n' && fr[strlen(fr) - 2] != '\r')
        {
          m_cursorPosition--;
          cursor_left(gbuf);
          return;
        }
        if (fr[strlen(fr) - 2] == '\n')
        {
          m_cursorPosition--;
          cursor_left(gbuf);
        }
        fr = extractFront(gbuf);
        if (fr[strlen(fr) - 2] == '\r')
        {
          m_cursorPosition--;
          cursor_left(gbuf);
        }
      }
    }
    else if (vk == VK_LEFT)
    {
      RenderNow();
      if (m_editingAscii || m_editingDump)
      {
        // Want to go back one if editing ascii.
        m_cursor = CUR_ONE;
      }
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
      if (m_renderDump)
      {
        RenderNow();
        m_cursorPosition += 64;
        if (m_cursorPosition > (64*44-1))
        {
          m_cursorPosition -= 64;

          bool inc = true;
          if ((thestart + std::streampos(64 * 43) + std::streampos(64)) >= (theend))
            inc = false;

          if (inc)
          {
            thestart += 64;
            ReadFile();
          }
        }
      }
      else if (!m_renderAscii)
      {
        RenderNow();
        m_cursorPosition += 16;
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
      else if (m_renderAscii)
      {
        FindNextLine();
      }
    }
    else if (vk == VK_UP)
    {
      if (m_renderDump)
      {
        RenderNow();

        if (m_cursorPosition < 64)
        {
          if (thestart < 64)
          {
            thestart = 0;
          }
          else
          {
            thestart -= 64;
          }
          ReadFile();
        }
        else if (m_cursorPosition > 63)
        {
          m_cursorPosition -= 64;
        }
      }
      else if (!m_renderAscii)
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
      else if (m_renderAscii)
      {
        FindPrevLine();
      }
    }
    else if (vk == VK_END)
    {
      RenderNow();
      m_cursorPosition = 703;
      m_cursor = CUR_TWO;
    }
    else if (vk == VK_HOME)
    {
      RenderNow();
      m_cursorPosition = 0;
      m_cursor = CUR_ONE;
    }
    else if (vk == VK_RETURN && m_editing)
    {
      if (m_edits.size() == 0)
      {
        // No edits to save.
        m_editing = false;
        m_editingAscii = false;
        m_editingDump = false;
        m_saving = false;
      }
      else
      {
        m_saving = true;
      }
    }
    else if (vk == VK_F1 && m_editing)
    {
      std::wstringstream wstr;
      wstr << " /c " << "\"" << GetAppPath() << "\\help\\HexEdit.html" << "\"";
      ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", wstr.str().c_str(), NULL, SW_HIDE);
    }
  }

  void Search::VKUp(DWORD vk)
  {
    if (vk == VK_ESCAPE)
    {
      m_editing = false;
      m_editingAscii = false;
      m_editingDump = false;
      m_editingText = false;
      m_saving = false;
      m_edits.clear();
      m_orderOfEdits.clear();
    }
  }

  bool Search::HasCoord(__int64 x, __int64 y, std::wstring& hexChar)
  {
    std::map<__int64, AnEdit>::iterator it;
    for (it = m_edits.begin(); it != m_edits.end(); it++)
    {
      if (m_cursor == CUR_ONE && it->second.OneSet)
      {
        __int64 address = it->first;
        __int64 offset = address - thestart;
        if (offset >= 0 && offset <= 703)
        {
          __int64 ox = GetXCoord(offset, CUR_ONE);
          __int64 oy = GetYCoord(offset, CUR_ONE);
          if (ox == x && oy == y)
          {
            std::wstringstream out;
            out << it->second.One;
            hexChar = out.str();
            return true;
          }
        }
      }
      else if (m_cursor == CUR_TWO && it->second.TwoSet)
      {
        __int64 address = it->first;
        __int64 offset = address - thestart;
        if (offset >= 0 && offset <= 703)
        {
          __int64 ox = GetXCoord(offset, CUR_TWO);
          __int64 oy = GetYCoord(offset, CUR_TWO);
          if (ox == x && oy == y)
          {
            std::wstringstream out;
            out << it->second.Two;
            hexChar = out.str();
            return true;
          }
        }
      }
    } //endFor
    return false;
  }

  bool Search::HasCoordDump(__int64 x, __int64 y, std::wstring& hexChar)
  {
    std::map<__int64, AnEdit>::iterator it;
    for (it = m_edits.begin(); it != m_edits.end(); it++)
    {
      if (m_cursor == CUR_ONE && it->second.OneSet)
      {
        __int64 address = it->first;
        __int64 offset = address - thestart;
        if (offset >= 0 && offset <= 64*44)
        {
          __int64 ox = GetDumpXCoord(offset, CUR_ONE);
          __int64 oy = GetDumpYCoord(offset, CUR_ONE);
          if (ox == x && oy == y)
          {
            std::wstringstream out;
            out << it->second.One;
            hexChar = out.str();
            return true;
          }
        }
      }
      else if (m_cursor == CUR_TWO && it->second.TwoSet)
      {
        __int64 address = it->first;
        __int64 offset = address - thestart;
        if (offset >= 0 && offset <= 64*44)
        {
          __int64 ox = GetDumpXCoord(offset, CUR_TWO);
          __int64 oy = GetDumpYCoord(offset, CUR_TWO);
          if (ox == x && oy == y)
          {
            std::wstringstream out;
            out << it->second.Two;
            hexChar = out.str();
            return true;
          }
        }
      }
    } //endFor
    return false;
  }

  //int OldGetNumLines()
  //{
  //  std::wstringstream side;
  //  int line = 2;
  //  for (int i = 0; i < (std::streampos(numMemBlock2) - thestart) && line <= 45; ++i)
  //  {
  //    if (memblock2[i] == 0x0D)
  //    {
  //      // skip \r
  //    }
  //    else if (memblock2[i] == 0x0A)
  //    {
  //      // parse \n as new line, render the current lineint 
  //      for (int j = side.str().length(); j < 80; ++j)
  //      {
  //        side << ' ';
  //      }
  //      //if (side.str().find(L"ye whole") != std::wstring::npos)
  //      //{
  //      //  int a = 1;
  //      //  a++;
  //      //}
  //      //        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
  //      line++;
  //      side.str(L"");
  //    }
  //    else
  //    {
  //      // keep on printing chars
  //      // no point printing beyond 80
  //      if (m_wordwrap && memblock2[i] == ' ' && side.str().length() > 63)
  //      {
  //        side << GetChar(memblock2[i]);
  //        // fill in spaces
  //        for (int j = side.str().length(); j < 80; ++j)
  //        {
  //          side << ' ';
  //        }
  //        //if (side.str().find(L"ye whole") != std::wstring::npos)
  //        //{
  //        //  int a = 1;
  //        //  a++;
  //        //}
  //        //          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
  //        line++;
  //        side.str(L"");
  //      }
  //      else if (side.str().length() < 79 && m_wordwrap && i < numMemBlock2)
  //      {
  //        side << GetChar(memblock2[i]);
  //        if (i + 1 >= (std::streampos(numMemBlock2) - thestart))
  //        {
  //          // Last char needs to write current line.

  //          // fill in spaces
  //          for (int j = side.str().length(); j < 80; ++j)
  //          {
  //            side << ' ';
  //          }
  //          //if (side.str().find(L"ye whole") != std::wstring::npos)
  //          //{
  //          //  int a = 1;
  //          //  a++;
  //          //}
  //          //            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
  //          line++;
  //          side.str(L"");

  //          return -1;
  //        }
  //      }
  //      else if (side.str().length() < 79)
  //      {
  //        side << GetChar(memblock2[i]);
  //      }
  //      else if (side.str().length() < 79 && m_wordwrap)
  //      {
  //        // fill in spaces
  //        for (int j = side.str().length(); j < 80; ++j)
  //        {
  //          side << ' ';
  //        }
  //        //if (side.str().find(L"ye whole") != std::wstring::npos)
  //        //{
  //        //  int a = 1;
  //        //  a++;
  //        //}
  //        //          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
  //        line++;
  //        side.str(L"");
  //      }
  //      //else if (m_wordwrap)
  //      //{
  //      //  side << GetChar(memblock2[i]);
  //      //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
  //      //  line++;
  //      //  side.str(L"");
  //      //}
  //    }
  //  }
  //  return line - 2;
  //}

  int Search::GetNumLines() const
  {
    int length = 0;
    int line = 2;
//    for (int i = 0; i < (std::streampos(numMemBlock2) - thestart) && line <= 45; ++i)
    for (int i = 0; i < std::streampos(numMemBlock2) && line <= 45; ++i)
    {
      if (i + 1 >= std::streampos(numMemBlock2))
      {
        // Last char needs to write current line.
        return line-2;
      }

      if (memblock2[i] == 0x0D)
      {
        // skip \r
      }
      else if (memblock2[i] == 0x0A)
      {
        // parse \n as new line, render the current lineint 
        for (int j = length; j < 80; ++j)
        {
          length++;
        }
        line++;
        length = 0;
      }
      else
      {
        // keep on printing chars
        // no point printing beyond 80
        if (m_wordwrap && memblock2[i] == ' ' && length > 63)
        {
          length++;
          // fill in spaces
          for (int j = length; j < 80; ++j)
          {
            length++;
          }
          line++;
          length = 0;
        }
        else if (length < 79 && m_wordwrap && i < numMemBlock2)
        {
          length++;
//          if (i + 1 >= (std::streampos(numMemBlock2) - thestart))
          if (i + 1 >= std::streampos(numMemBlock2))
          {
            // Last char needs to write current line.
            return -1;
          }
        }
        else if (length < 79)
        {
          length++;
        }
        else if (length > 79/* && m_wordwrap*/)
        {
          // fill in spaces
          for (int j = length; j < 80; ++j)
          {
            length++;
          }
          line++;
          length = 0;
        }
      }
    }
    return line - 2;
  }

  int Search::GetNumChars() const
  {
    int numChars = 0;
    int length = 0;
    int line = 2;
//    for (int i = 0; i < (std::streampos(numMemBlock2) - thestart) && line <= 45; ++i)
    for (int i = 0; i < std::streampos(numMemBlock2) && line <= 45; ++i)
    {
      if (memblock2[i] == 0x0D)
      {
        // skip \r
        numChars++;
      }
      else if (memblock2[i] == 0x0A)
      {
        numChars++;
        // parse \n as new line, render the current lineint 
        for (int j = length; j < 80; ++j)
        {
          length++;
//          numChars++;
        }
        line++;
        length = 0;
      }
      else
      {
        // keep on printing chars
        // no point printing beyond 80
        if (m_wordwrap && memblock2[i] == ' ' && length > 63)
        {
          length++;
          numChars++;
          // fill in spaces
          for (int j = length; j < 80; ++j)
          {
            length++;
//            numChars++;
          }
          line++;
          length = 0;
        }
        else if (length < 79 && m_wordwrap && i < numMemBlock2)
        {
          length++;
          numChars++;
//          if (i + 1 >= (std::streampos(numMemBlock2) - thestart))
          if (i + 1 >= std::streampos(numMemBlock2))
          {
            // Last char needs to write current line.

            // fill in spaces
            for (int j = length; j < 80; ++j)
            {
//              numChars++;
            }
            line++;
            length = 0;

            return numChars;
          }
        }
        else if (length < 79)
        {
          length++;
          numChars++;
        }
        else if (length < 79 && m_wordwrap)
        {
          // fill in spaces
          for (int j = length; j < 80; ++j)
          {
            length++;
//            numChars++;
          }
          line++;
          length = 0;
        }
      }
    }
    return numChars;
  }
//  proj::rope rope;

  //how much from cursor to end of line?
  int Search::GetCursorToEndOfLine()
  {
    std::wstringstream buff;
    char* extf = extractFront(gbuf);
    char* extb = extractBack(gbuf);
    int line = 2;
    for (int i = 0; i < gb_front(gbuf); ++i)
    {
      if (extf[i] == 0x0D)
      {
        buff.str(L"");
      }
      else if (extf[i] == 0x0A)
      {
        buff.str(L"");
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        buff.str(L"");
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 64)
      {
        buff.str(L"");
        line++;
      }
      else
      {
        buff << GetChar(extf[i]);
      }
    }
    int cursorX = buff.str().length();
    int cursorY = line;
    buff << L"X"; // cursor
    for (int i = 0; i < gb_back(gbuf); ++i)
    {
      if (extb[i] == 0x0D)
      {
        //skip
      }
      else if (extb[i] == 0x0A)
      {
        if (i > 0)
        {
          return i - 1;
        }
        else
        {
          return i;
        }
      }
      else if (line != cursorY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 63)
      {
        return i;
      }
      else if (line == cursorY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 64)
      {
        return i;
      }
      else
      {
        buff << GetChar(extb[i]);
      }
    }
    return strlen(extb)-1;
  }

  //how much next line to cursorX index or end of line or end of file
  int Search::GetNextLineToCursor()
  {
    std::wstringstream buff;
    char* extf = extractFront(gbuf);
    char* extb = extractBack(gbuf);
    int line = 2;
    for (int i = 0; i < gb_front(gbuf); ++i)
    {
      if (extf[i] == 0x0D)
      {
        buff.str(L"");
      }
      else if (extf[i] == 0x0A)
      {
        buff.str(L"");
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        buff.str(L"");
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 64)
      {
        buff.str(L"");
        line++;
      }
      else
      {
        buff << GetChar(extf[i]);
      }
    }
    int cursorX = buff.str().length();
    int cursorY = line;
    buff << L"X"; // cursor
    bool foundNextLine = false;
    int indexFound = -1;
    int i = 0;
    for (i = 0; i < gb_back(gbuf); ++i)
    {
      if (extb[i] == 0x0D)
      {
        //skip
      }
      else if (line != cursorY && extb[i] == 0x0A)
      {
        foundNextLine = true;
        indexFound = i+1; // one char after \n

        //try return here?
        if (cursorX >= buff.str().length())
          return buff.str().length();
        else
          return cursorX; //try this?
        break;
      }
      else if (line == cursorY && extb[i] == 0x0A)
      {
        buff.str(L"");
        line++;
      }
      else if (line != cursorY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 63)
      {
        foundNextLine = true;
        indexFound = i;
//        indexFound = i+1; //try one after?

        //try return heer?
        if (cursorX >= buff.str().length())
          return buff.str().length();
        else
          return cursorX; //try this?
        break;
      }
      else if (line == cursorY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 64)
      {
        //same line is not next line?
//        foundNextLine = true;
//        indexFound = i;
//        break;
        buff.str(L"");
        line++;
      }
      else
      {
        buff << GetChar(extb[i]);
      }
    }

    if (extb[i] == 'Í')
    {
      if (line == cursorY && !foundNextLine)
        return -1; //work around so does not move cursor (would prefer 0)

      //try return heer?
      if (cursorX >= buff.str().length())
        return buff.str().length();
      else
        return cursorX; //try this?
    }

    buff.str(L"");
    i = indexFound;
    if (foundNextLine)
    for (i = indexFound; i < gb_back(gbuf) && (i - indexFound)<cursorX; ++i)
    {
      if (extb[i] == 0x0D)
      {
        //skip
      }
      else if (extb[i] == 0x0A)
      {
        int ret = i - indexFound - 1; // one char before \n
        if (ret > 0)
        {
          return ret;
        }
        else
        {
          return 0;
        }
        return ret;
      }
      else if (m_wordwrap && extb[i] == ' ' && buff.str().length() > 63)
      {
        return i - indexFound;
        //int ret = i - indexFound - 1; // one char before \n
        //if (ret > 0)
        //{
        //  return ret;
        //}
        //else
        //{
        //  return 0;
        //}
        //return ret;
      }
      else if (m_wordwrap && extb[i] == ' ' && buff.str().length() > 64)
      {
        return i - indexFound;
        //int ret = i - indexFound - 1; // one char before \n
        //if (ret > 0)
        //{
        //  return ret;
        //}
        //else
        //{
        //  return 0;
        //}
        //return ret;
      }
      else
      {
        buff << GetChar(extb[i]);

        //if (extb[i] == 'Í')
        //{
        //  return i - indexFound - 1; // one char before end of file
        //}
      }
    }
    if (extb[i] == 'Í')
    {
      int ret = i - indexFound - 1; // one char before end of file
      if (ret > 0)
      {
        return ret;
      }
      else
      {
        return 0;
      }
      return ret;
    }

    return buff.str().length();

  }

  int Search::GetCursorToStartOfLine()
  {
    std::wstringstream buff;
    char* extf = extractFront(gbuf);
    char* extb = extractBack(gbuf);
    int line = 2;
    bool lastCharNewLine = false;
    int i = 0;
    for (i = 0; i < gb_front(gbuf); ++i)
    {
      lastCharNewLine = false;
      if (extf[i] == 0x0D)
      {
        buff.str(L"");
      }
      else if (extf[i] == 0x0A)
      {
        buff.str(L"");
        lastCharNewLine = true;
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        buff.str(L"");
        lastCharNewLine = true;
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 64)
      {
        buff.str(L"");
        lastCharNewLine = true;
        line++;
      }
      else
      {
        buff << GetChar(extf[i]);
      }
    }
    //if (extf[i] == 'Í')
    //{
    //  return 0;
    //}
    int cursorX = buff.str().length();
    int cursorY = line;

    return cursorX;
    //if (lastCharNewLine && cursorX == 0)
    //{
    //  return 0;
    //}


    //buff << L"X"; // cursor


    //return 1;
  }

  int Search::GetPreviousLineToCursor()
  {
    std::wstringstream buff;
    char* extf = extractFront(gbuf);
    char* extb = extractBack(gbuf);
    int line = 2;
    bool lastCharNewLine = false;
    std::wstring previousLine = L"";
    for (int i = 0; i < gb_front(gbuf); ++i)
    {
//      lastCharNewLine = false;
      if (extf[i] == 0x0D)
      {
      }
      else if (extf[i] == 0x0A)
      {
        previousLine = buff.str();
        buff.str(L"");
        lastCharNewLine = true;
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        previousLine = buff.str();
        buff.str(L"");
        lastCharNewLine = true;
        line++;
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 64)
      {
        previousLine = buff.str();
        buff.str(L"");
        lastCharNewLine = true;
        line++;
      }
      else
      {
        buff << GetChar(extf[i]);
      }
    }
    int cursorX = buff.str().length();
    int cursorY = line;

    if (previousLine == L"")
    {
      if (lastCharNewLine)
      {
        return 0;
      }
      else
      {
        return -1;
      }
    }

    int diff = previousLine.length() - cursorX;
    if (diff > 0)
    {
      return diff;
    }
    else
    {
      return 0;
    }


//    buff << L"X"; // cursor


//    return 10;
  }

  void Search::FindNextLine()
  {
    int a = GetCursorToEndOfLine();
    int r = GetNextLineToCursor();
    std::wstringstream rb;
    rb << L"EOL:" << a << L", NL:" << r;
//    MessageBox(NULL, rb.str().c_str(), L"Info", MB_OK);
    //add the 2 to get how many rights
//    if (r != -1)
    {
      int sum = a + r + 1;
      //    if (sum > 1)
      {
        for (int j = 0; j < sum; ++j)
        {
          VK(VK_RIGHT);
        }
      }
    }
    return;

    std::wstringstream buff;

    int eighty = 0;
    int line = 2;
    bool lastLineNewLine = false;
    int lastLineCount = 0;

    char* extf = extractFront(gbuf);
    char* extb = extractBack(gbuf);

    for (int i = 0; i < gb_front(gbuf) && line <= 45; ++i)
    {
      eighty++;
      lastLineNewLine = false;
      if (extf[i] == 0x0D)
      {
        // skip
        eighty--;
      }
      else if (extf[i] == 0x0A)
      {
        if (m_newline && line <= 45)
        {
          eighty--;
          // parse \n as new line, render the current lineint 
          for (int j = buff.str().length(); j < 80; ++j)
          {
            buff << ' ';
          }
//          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          buff.str(L"");
          lastLineNewLine = true;
          lastLineCount = eighty;
        }
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        buff << GetChar(extf[i]);
        // fill in spaces
        for (int j = buff.str().length(); j < 80; ++j)
        {
          buff << ' ';
        }
//        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
        line++;
        buff.str(L"");
        lastLineNewLine = true;
        lastLineCount = eighty;
        eighty = 0;
      }
      else
      {
        buff << GetChar(extf[i]);
        if (eighty >= 80 && line <= 45)
        {
//          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          buff.str(L"");
          lastLineNewLine = true;
          lastLineCount = eighty;
          eighty = 0;
        }
      }
    }

    //cursor
    buff << 'X';
    int cursX = eighty;
    int cursY = line;
    bool foundNextLine = false;
    int offset = 0;
    std::vector<int> lineCounts;

    int i = 0;
    for (i = 0; i < gb_back(gbuf) && line <= 45; ++i)
    {
      if (lineCounts.size() >= 2 && cursX > lastLineCount)
      {
        for (int j = 0; j < (offset + 1); ++j)
        {
          VK(VK_RIGHT);
        }
        return;
      }
      else if (line>cursY && eighty >= cursX)
      {
        //if (cursX == 1 || cursX == 65)
        //  offset -= 1;
        for (int j = 0; j < (offset + 1); ++j)
        {
          VK(VK_RIGHT);
        }
        return;
      }

      //if (lastLineNewLine)// && line>cursY && eighty >= cursX)
      //{
      //  //if (cursX > lastLineCount)
      //  //{
      //  //  //          int actual = cursX - lastLineCount;
      //  //  int actual = lineCounts[0] - cursX + lastLineCount - 1;
      //  //  for (int j = 0; j < (actual); ++j)
      //  //  {
      //  //    VK(VK_RIGHT);
      //  //  }
      //  //  return;
      //  //}


      //  for (int j = 0; j < (offset+1); ++j)
      //  {
      //    VK(VK_RIGHT);
      //  }
      //  return;
      //}
      //else if (line>cursY && eighty >= cursX)
      //{
      //  if (cursX == 0)
      //    offset -= 2;
      //  if (cursX == 1 || cursX == 65)
      //    offset -= 1;
      //  //if (cursX >= 1 && cursX <= 65)
      //  //  offset -= 3;
      //  for (int j = 0; j < offset; ++j)
      //  {
      //    VK(VK_RIGHT);
      //  }
      //  return;
      //}

      eighty++;
      offset++;    

      lastLineNewLine = false;
      if (extb[i] == 0x0D)
      {
        // skip
        offset--;
        eighty--;
      }
      else if (extb[i] == 0x0A)
      {
        if (m_newline && line <= 45)
        {
          offset--;
          eighty--;

          // parse \n as new line, render the current lineint 
          int sz = buff.str().length();
          for (int j = sz; j < 80; ++j)
          {
            buff << ' ';
          }
          std::wstring buffstr = buff.str();
//          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buffstr, FG_CYAN | BG_BLACK);
          line++;
          foundNextLine = true;
          lastLineNewLine = true;
          lastLineCount = eighty;
          lineCounts.push_back(eighty);
          buff.str(L"");
          eighty = 0;
        }
      }
      else if (line != cursY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 63)
      {
        offset--;
        buff << GetChar(extb[i]);
        // fill in spaces
        for (int j = buff.str().length(); j < 80; ++j)
        {
          buff << ' ';
        }
//        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
        line++;
        foundNextLine = true;
        lastLineNewLine = true;
        lastLineCount = eighty;
        lineCounts.push_back(eighty);
        buff.str(L"");
        eighty = 0;
      }
      else if (line == cursY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 64)
      {
        offset--;
        buff << GetChar(extb[i]);
        // fill in spaces
        for (int j = buff.str().length(); j < 80; ++j)
        {
          buff << ' ';
        }
        //        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
        line++;
        foundNextLine = true;
        lastLineNewLine = true;
        lastLineCount = eighty;
        lineCounts.push_back(eighty);
        buff.str(L"");
        eighty = 0;
      }
      else
      {
        buff << GetChar(extb[i]);
        if (extb[i] == 'Í')
        {
          line++;
          foundNextLine = true;
          lastLineNewLine = true;
          lastLineCount = eighty;
          lineCounts.push_back(eighty);
          buff.str(L"");
          eighty = 0;
        }
        else if (eighty >= 80 && line <= 45)
        {
//          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          foundNextLine = true;
          lastLineNewLine = true;
          lastLineCount = eighty;
          lineCounts.push_back(eighty);
          buff.str(L"");
          eighty = 0;
        }
      }
    }
    if (eighty > 0 && line <= 45)
    {
//      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_GREY | BG_BLACK);
      buff.str(L"");
      eighty = 0;
      line++;
      foundNextLine = true;
    }

    if (extb[i] == 'Í')
    {
      line++;
      foundNextLine = true;
      lastLineNewLine = true;
      lastLineCount = eighty;
      lineCounts.push_back(eighty);
      buff.str(L"");
      eighty = 0;

      //for (int j = 0; j < offset+1; ++j)
      //{
      //  VK(VK_RIGHT);
      //}

      for (int j = 0; j < lineCounts[0]-8; ++j)
      {
        VK(VK_RIGHT);
      }
    }

  }


  void Search::FindPrevLine()
  {
    int a = GetCursorToStartOfLine();
    int r = GetPreviousLineToCursor();
    std::wstringstream rb;
    rb << L"SOL:" << a << L", PLC:" << r;
//    MessageBox(NULL, rb.str().c_str(), L"Info", MB_OK);

    //add the 2 to get how many lefts
    if (r != -1)
    {
      int sum = a + r + 1;
      //    if (sum > 1)
      {
        for (int j = 0; j < sum; ++j)
        {
          VK(VK_LEFT);
        }
      }
    }
    return;


    std::wstringstream buff;

    int eighty = 0;
    int line = 2;
    int less = 0;
    int lastLine = 0;

    char* extf = extractFront(gbuf);
    char* extb = extractBack(gbuf);
    std::vector<int> lineCounts;

    for (int i = 0; i < gb_front(gbuf) && line <= 45; ++i)
    {
      eighty++;
      if (extf[i] == 0x0D)
      {
        // skip
      }
      else if (extf[i] == 0x0A)
      {
        if (m_newline && line <= 45)
        {
          // parse \n as new line, render the current lineint 
          for (int j = buff.str().length(); j < 80; ++j)
          {
            buff << ' ';
          }
          //          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          lineCounts.push_back(eighty);
          line++;
          buff.str(L"");
        }
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        buff << GetChar(extf[i]);
        // fill in spaces
        less = 0;
        for (int j = buff.str().length(); j < 80; ++j)
        {
          buff << ' ';
          less++;
        }
        //        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
        line++;
        buff.str(L"");
        lastLine = eighty;
        lineCounts.push_back(eighty);
        eighty = 0;
      }
      else
      {
        buff << GetChar(extf[i]);
        if (eighty >= 80 && line <= 45)
        {
          //          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          buff.str(L"");
          lineCounts.push_back(eighty);
//          eighty = 0;
        }
      }
    }

    int cursX = eighty;
    int cursY = line;

    //for (int j = 0; j < 80-less; ++j)
    //{
    //  VK(VK_LEFT);
    //}

    if (lineCounts.size() > 0)
    {
      if (cursX > lineCounts[lineCounts.size() - 1])
      {
        //      lineCounts[lineCounts.size() - 1]
        for (int j = 0; j < cursX + 1; ++j)
        {
          VK(VK_LEFT);
        }
        return;
      }
    }

    int diff = cursX - lastLine;

    for (int j = 0; j < 80-less; ++j)
    {
      VK(VK_LEFT);
    }
    return;


    //cursor
    buff << 'X';

    bool foundPrevLine = false;
    int offset = 0;


    for (int i = gb_front(gbuf); i > 0; --i)
    {

      eighty--;
      offset++;
      if (eighty <= 0)
      {
        foundPrevLine = true;
      }
      else if (extf[i] == 0x0D)
      {
        // skip
      }
      else if (extf[i] == 0x0A)
      {
        if (m_newline)
        {
          // parse \n as new line, render the current lineint 
          for (int j = buff.str().length(); j < 80; ++j)
          {
            buff << ' ';
//            offset++;
          }
          line--;
          foundPrevLine = true;
          buff.str(L"");
        }
      }
      else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
      {
        buff << GetChar(extf[i]);
        // fill in spaces
        for (int j = buff.str().length(); j < 80; ++j)
        {
          buff << ' ';
//          offset++;
        }
        //        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
        line--;
        foundPrevLine = true;
        buff.str(L"");
        eighty = 0;
      }
      else
      {
        buff << GetChar(extf[i]);
        if (eighty >= 80 && line <= 45)
        {
          //          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line--;
          foundPrevLine = true;
          buff.str(L"");
          eighty = 0;
        }
        if (eighty <= -80 && line <= 45)
        {
          //          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line--;
          foundPrevLine = true;
          buff.str(L"");
          eighty = 0;
        }
      }

      if (foundPrevLine)
      {
        for (int j = 0; j < offset; ++j)
        {
          VK(VK_LEFT);
        }
        return;
      }
    }




  }

  void Search::RenderAscii2()
  {
    if (rmemblock2 == NULL)
    {
      rReadFile();

      gbuf = new_buffer(2);

      for (int i = 0; i < rtheend; ++i)
      {
        insert_character(gbuf, rmemblock2[i]);
      }

      while (gb_front(gbuf)!=0)
      {
        cursor_left(gbuf);
      }

      while (m_cursorPosition < gb_front(gbuf))
      {
        cursor_right(gbuf);
      }
      //for (int i = 0; i < rtheend; ++i)
      //{
      //  std::string s(1, rmemblock2[i]);
      //  rope.append(s);
      //}
      //rope.balance();
    }

      std::wstringstream buff;


      size_t aa = gb_front(gbuf);
      size_t ab = gb_back(gbuf);

      int eighty = 0;
      int line = 2;
      char* ext = extract_text(gbuf);

      char* extf = extractFront(gbuf);
      char* extb = extractBack(gbuf);


      for (int i = 0; i < gb_front(gbuf) && line <= 45; ++i)
      {
        //        buff << rmemblock2[i];
        //        buff << rope.at(i);
    //    buff << extf[i];

        eighty++;
        if (extf[i] == 0x0D)
        {
          // skip
        }
        else if (extf[i] == 0x0A)
        {
          if (m_newline && line <= 45)
          {
            // parse \n as new line, render the current lineint 
            for (int j = buff.str().length(); j < 80; ++j)
            {
              buff << ' ';
            }
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
            line++;
            buff.str(L"");
            eighty = 0;
          }
        }
        else if (m_wordwrap && extf[i] == ' ' && buff.str().length() > 63)
        {
          buff << GetChar(extf[i]);
          // fill in spaces
          for (int j = buff.str().length(); j < 80; ++j)
          {
            buff << ' ';
          }
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          buff.str(L"");
          eighty = 0;
        }
        else
        {
          buff << GetChar(extf[i]);
          if (eighty >= 80 && line <= 45)
          {
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
            line++;
            buff.str(L"");
            eighty = 0;
          }
        }
      }
      //if (eighty > 0 && line <= 45)
      //{
      //  buff << 'X';
      //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_GREY | BG_BLACK);
      //  buff.str(L"");
      //  eighty = 0;
      //  line++;
      //}

      buff << 'X';
      int cursX = eighty;
      int cursY = line;



      for (int i = 0; i < gb_back(gbuf) && line <= 45; ++i)
      {
        //        buff << rmemblock2[i];
        //        buff << rope.at(i);


        eighty++;
        if (extb[i] == 0x0D)
        {
          // skip
        }
        else if (extb[i] == 0x0A)
        {
          if (m_newline && line <= 45)
          {
            // parse \n as new line, render the current lineint 
            int sz = buff.str().length();
            for (int j = sz; j < 80; ++j)
            {
              buff << ' ';
            }
            std::wstring buffstr = buff.str();
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buffstr, FG_CYAN | BG_BLACK);
            line++;
            buff.str(L"");
            eighty = 0;
          }
        }
        else if (line != cursY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 63)
        {
          buff << GetChar(extb[i]);
          // fill in spaces
          for (int j = buff.str().length(); j < 80; ++j)
          {
            buff << ' ';
          }
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          buff.str(L"");
          eighty = 0;
        }
        else if (line == cursY && m_wordwrap && extb[i] == ' ' && buff.str().length() > 64)
        {
          buff << GetChar(extb[i]);
          // fill in spaces
          for (int j = buff.str().length(); j < 80; ++j)
          {
            buff << ' ';
          }
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
          line++;
          buff.str(L"");
          eighty = 0;
        }
        else
        {
          buff << GetChar(extb[i]);
          if (eighty >= 80 && line <= 45)
          {
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_CYAN | BG_BLACK);
            line++;
            buff.str(L"");
            eighty = 0;
          }
        }
    }
    if (eighty > 0 && line <= 45)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_GREY | BG_BLACK);
      buff.str(L"");
      eighty = 0;
      line++;
    }
  
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, cursX, cursY, L"░", FG_GREY | BG_GREY);




//      for (int i = 0; i < rtheend; ++i)
//      {
////        buff << rmemblock2[i];
////        buff << rope.at(i);
//        buff << ext[i];
//
//        eighty++;
//        if (eighty >= 80 && line<=45)
//        {
//          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_GREY | BG_BLACK);
//          line++;
//          buff.str(L"");
//          eighty = 0;
//        }
//      }
//      if (eighty > 0 && line<=45)
//      {
//        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buff.str(), FG_GREY | BG_BLACK);
//        buff.str(L"");
//        eighty = 0;
//        line++;
//      }
//    }

    std::wstring file = StrUtil::s2ws(theFile);
    std::wstringstream ss;
    ss << file;// << L"_" << lastChar;
    file = ss.str();
    if (m_wordwrap)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          WRAP  (no mask)  ", FG_GREY | BG_BLACK);
      if (m_newline)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          WRAPN (no mask)  ", FG_GREY | BG_BLACK);
      }
    }
    else
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          ASCII (no mask)  ", FG_GREY | BG_BLACK);
      if (m_newline)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          ASCIIN(no mask)  ", FG_GREY | BG_BLACK);
      }
    }
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 6, 0, file, FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"────────────────────────────────────────────────────────────────────────────────", FG_GREY | BG_BLACK);

    m_showingLastLine = false;
    std::wstringstream side;
//    int line = 2;
//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, lines, FG_GREY | BG_BLACK);

    
  }

  void Search::RenderAscii()
  {
    RenderAscii2();
      return;

    ////https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
    //if (!m_timeSet)
    //{
    //  auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    //  m_timePassed = millisec_since_epoch;
    //  m_timeSet = true;
    //}

    //auto currentMillisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    //if (currentMillisec_since_epoch - m_timePassed > 500)
    //{
    //  m_timePassed = currentMillisec_since_epoch;
    //  m_timeSet = true;
    //  m_renderCursor = !m_renderCursor;
    //}

    //if (m_editingText)
    //{
    //  CONSOLE_CURSOR_INFO     cursorInfo;
    //  GetConsoleCursorInfo(ghConsole, &cursorInfo);
    //  //if (!cursorInfo.bVisible)
    //  //{
    //  //  //      if (m_renderCursor)
    //  //  cursorInfo.bVisible = true; // set the cursor visibility
    //  //                              //      else
    //  //                              //        cursorInfo.bVisible = false; // set the cursor visibility

    //  //  SetConsoleCursorInfo(ghConsole, &cursorInfo);
    //  //}
    //  if (m_renderCursor)
    //  {
    //    cursorInfo.bVisible = true; // set the cursor visibility
    //  }
    //  else
    //  {
    //    cursorInfo.bVisible = false; // set the cursor visibility
    //  }
    //  SetConsoleCursorInfo(ghConsole, &cursorInfo);

    //  COORD coord;
    //  coord.X = 0 + m_cursor;
    //  coord.Y = 2;
    //  SetConsoleCursorPosition(ghConsole, coord);
    //}


    std::wstring file = StrUtil::s2ws(theFile);
    std::wstringstream ss;
    ss << file;// << L"_" << lastChar;
    file = ss.str();
    if (m_wordwrap)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          WRAP  (no mask)  ", FG_GREY | BG_BLACK);
      if (m_newline)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          WRAPN (no mask)  ", FG_GREY | BG_BLACK);
      }
    }
    else
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          ASCII (no mask)  ", FG_GREY | BG_BLACK);
      if (m_newline)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          ASCIIN(no mask)  ", FG_GREY | BG_BLACK);
      }
    }
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 6, 0, file, FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"────────────────────────────────────────────────────────────────────────────────", FG_GREY | BG_BLACK);

    m_showingLastLine = false;
    std::wstringstream side;
    int line = 2;
//    for (int i = 0; i < (std::streampos(numMemBlock2)-thestart) && line<=45; ++i)
//    for (int i = 0; i < std::streampos(numMemBlock2) && line<=45; ++i)
//    for (int i = 0; i < std::streampos(numMemBlock2) && line<=45; ++i)
//    for (int i = 0; i < std::streampos(numMemBlock2) && line<=47; ++i)
    for (int i = 0; i < std::streampos(numMemBlock2) && line<=45; ++i)
    {
      m_lastI = i;

      if (i + 1 >= (std::streampos(numMemBlock2)))
      {
        m_showingLastLine = true;
      }

      if (memblock2[i] == 0x0D)
      {
        // skip \r
        if (!m_newline)
        {
          side << GetChar(memblock2[i]);
        }
      }
      else if (memblock2[i] == 0x0A)
      {
        if (m_newline)
        {
          // parse \n as new line, render the current lineint 
          for (int j = side.str().length(); j < 80; ++j)
          {
            side << ' ';
          }
          if (side.str().find(L"ye whole") != std::wstring::npos)
          {
            int a = 1;
            a++;
          }
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
          line++;
          side.str(L"");
        }
        else
        {
          side << GetChar(memblock2[i]);
        }
      }
      else
      {
        // keep on printing chars
        // no point printing beyond 80
        if (m_wordwrap && memblock2[i] == ' ' && side.str().length() > 63)
        {
          side << GetChar(memblock2[i]);
          // fill in spaces
          for (int j = side.str().length(); j < 80; ++j)
          {
            side << ' ';
          }
          if (side.str().find(L"ye whole") != std::wstring::npos)
          {
            int a = 1;
            a++;
          }
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
          line++;
          side.str(L"");
        }
        else if (side.str().length() < 79 && m_wordwrap && i < numMemBlock2)
        {
          side << GetChar(memblock2[i]);
          if (i+1 >= (std::streampos(numMemBlock2)))
          {
            // Last char needs to write current line.

            // fill in spaces
            for (int j = side.str().length(); j < 80; ++j)
            {
              side << ' ';
            }
            if (side.str().find(L"ye whole") != std::wstring::npos)
            {
              int a = 1;
              a++;
            }
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
            line++;
            side.str(L"");

            m_showingLastLine = true;
          }
        }
        else if (side.str().length() < 79)
        {
          side << GetChar(memblock2[i]);
        }
        else if (side.str().length() < 79 && m_wordwrap)
        {
          // fill in spaces
          for (int j = side.str().length(); j < 80; ++j)
          {
            side << ' ';
          }
          if (side.str().find(L"ye whole") != std::wstring::npos)
          {
            int a = 1;
            a++;
          }
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
          line++;
          side.str(L"");
        }
        else
        {
          side << GetChar(memblock2[i]);
          if (side.str().length() > 79 && m_wordwrap)
          {
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
            line++;
            side.str(L"");
          }
          else if (side.str().length() > 79)
          {
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
            line++;
            side.str(L"");
          }
          else if (side.str().length() > 1024)
          {
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
            line++;
            side.str(L"");
          }
        }
        //else if (m_wordwrap)
        //{
        //  side << GetChar(memblock2[i]);
        //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
        //  line++;
        //  side.str(L"");
        //}
      }
    }

    if (side.str().length() > 0)
    {
      for (int j = side.str().length(); j < 80; ++j)
      {
        side << ' ';
      }
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
      line++;
    }

    side.str(L"");
    for (int j = 0; j < 80; ++j)
    {
      side << ' ';
    }

    for (int i = line; i < 46; ++i)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, side.str(), FG_CYAN | BG_BLACK);
      line++;
    }

//    if (m_editingText)
//    {
//      CONSOLE_CURSOR_INFO     cursorInfo;
//      GetConsoleCursorInfo(ghConsole, &cursorInfo);
////      if (m_renderCursor)
//        cursorInfo.bVisible = true; // set the cursor visibility
////      else
////        cursorInfo.bVisible = false; // set the cursor visibility
//
//      SetConsoleCursorInfo(ghConsole, &cursorInfo);
//
//      COORD coord;
//      coord.X = 0 + m_cursor;
//      coord.Y = 2;
//      SetConsoleCursorPosition(ghConsole, coord);
//    }

    if (m_editingText)
    {
      std::wstring ch = GetChar(memblock2[m_cursorPosition]);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, ch, FG_RED | BG_WHITE);
    }

    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"═══════════════════════════════════════════════════════════════─────────────────", FG_GREY | BG_BLACK);
    line++;

//    int endit = 703 + 1;
    int endit = GetNumChars();
    if (theend < endit)
    {
      endit = theend;
    }

    double percent = (double)(thestart + std::streampos(endit)) / (double)theend * 100.0;
    int ipercent = (int)percent;
    std::wstringstream wpercent;
    if (ipercent < 100)
    {
      wpercent << " ";
    }
    if (ipercent < 10)
    {
      wpercent << " ";
    }
    wpercent << ipercent << " %";
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"VIEW      ASCII  Dump  Edit  Find  Hex  Mask  Wordwrap  Jump  Indent            ", FG_GREY | BG_BLACK);
    DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                          " + wpercent.str(), FG_CYAN | BG_BLACK);
    DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          A      D     E     F     H    M     W         J     I                 ", FG_CYAN | BG_BLACK);
    line++;
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"COMMANDS  0..9 goto bookmark         F9 search             SPACE search again   ", FG_GREY | BG_BLACK);
    DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          0..9                       F9 s                  SPACE                ", FG_CYAN | BG_BLACK);
    line++;
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"←↑↓→ scroll  SHIFT 0..9 set ALT 0..9 clear bookmarks        F1 help  ESC cancel ", FG_GREY | BG_BLACK);
    DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"             SHIFT 0..9     ALT 0..9                        F1       ESC        ", FG_CYAN | BG_BLACK);
    line++;

    
  }

  void Search::RenderCursor()
  {
    return;

    if (m_editingText)
    {
      CONSOLE_CURSOR_INFO     cursorInfo;
      GetConsoleCursorInfo(ghConsole, &cursorInfo);
      //      if (m_renderCursor)
      cursorInfo.bVisible = true; // set the cursor visibility
                                  //      else
                                  //        cursorInfo.bVisible = false; // set the cursor visibility

      SetConsoleCursorInfo(ghConsole, &cursorInfo);

      COORD coord;
      coord.X = 0 + m_cursor;
      coord.Y = 2;
      SetConsoleCursorPosition(ghConsole, coord);
    }
  }

  bool Search::DontRenderAscii()
  {
    bool showingLastLine = false;
    int length = 0;
    int line = 2;
    for (int i = 0; i < std::streampos(numMemBlock2) && line <= 45; ++i)
    {
      if (i + 1 >= (std::streampos(numMemBlock2)))
      {
        showingLastLine = true;
        return showingLastLine;
      }

      if (memblock2[i] == 0x0D)
      {
        // skip \r
        if (!m_newline)
        {
          length++;
        }
      }
      else if (memblock2[i] == 0x0A)
      {
        if (m_newline)
        {
          // parse \n as new line, render the current lineint 
          for (int j = length; j < 80; ++j)
          {
            length++;
          }
          line++;
          length = 0;
        }
        else
        {
          length++;
        }
      }
      else
      {
        // keep on printing chars
        // no point printing beyond 80
        if (m_wordwrap && memblock2[i] == ' ' && length > 63)
        {
          length++;
          // fill in spaces
          for (int j = length; j < 80; ++j)
          {
            length++;
          }
          line++;
          length = 0;
        }
        else if (length < 79 && m_wordwrap && i < numMemBlock2)
        {
          length++;
          if (i + 1 >= (std::streampos(numMemBlock2)))
          {
            // Last char needs to write current line.

            // fill in spaces
            for (int j = length; j < 80; ++j)
            {
              length++;
            }
            line++;
            length = 0;

            showingLastLine = true;
            return showingLastLine;
          }
        }
        else if (length < 79)
        {
          length++;
        }
        else if (length < 79 && m_wordwrap)
        {
          // fill in spaces
          for (int j = length; j < 80; ++j)
          {
            length++;
          }
          line++;
          length = 0;
        }
        else
        {
          length++;
          if (length > 79 && m_wordwrap)
          {
            line++;
            length = 0;
          }
          else if (length > 79)
          {

            line++;
            length = 0;
          }
          else if (length > 1024)
          {

            line++;
            length = 0;
          }
        }
      }
    }

    if (length > 0)
    {
      for (int j = length; j < 80; ++j)
      {
        length++;
      }

      line++;
    }

    length = 0;
    for (int j = 0; j < 80; ++j)
    {
      length++;
    }

    for (int i = line; i < 46; ++i)
    {

      line++;
    }

    return showingLastLine;
  }

  void Search::Render()
  {
    if (m_renderAscii)
    {
      RenderAscii();
      return;
    }
    if (m_renderDump)
    {
      RenderDump();
      return;
    }

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

    //https://mycurvefit.com/
    //power
    //y = 60450.23*x^-0.8831565
    /*
    consoleapp.csproj
    {_Myoff=0 _Fpos=2425 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ════════════════════════════════════════════════════════════════════════────────
    2425 => 72/80, 8, 0.9, 0.1

    program.cs
    {_Myoff=0 _Fpos=8595 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ══════════════──────────────────────────────────────────────────────────────────
    8595 => 14/80, 66, 0.175, 0.825

    hookdll.dll
    {_Myoff=0 _Fpos=32768 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ════════────────────────────────────────────────────────────────────────────────
    32768 => 8/80, 72, 0.1, 0.9

    hookexe.exe
    {_Myoff=0 _Fpos=28672 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ═════════───────────────────────────────────────────────────────────────────────
    28672 => 9/80, 71, 0.1125, 0.8875

    hookdll.cpp
    {_Myoff=0 _Fpos=2011 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ══════════════════════════════════════════════════════════════════──────────────
    2011 => 66/80, 14, 0.825, 0.175
    */
    double percent = (double)(thestart + std::streampos(703 + 1)) / (double)theend * 100.0;
    double percents = (double)(thestart) / (double)theend;
    if (percent >= 100)
    {
      percents = 1.0;
    }
    double percentp = (double)(thestart) / (double)theend * 100.0;
    double percentpx = 1.0 - (double)(thestart) / (double)theend;
    double percentc = (double)(thestart + std::streampos(703 + 1)) / (double)theend;
    int ipercent = (int)percent;
    int ipercentp = (int)percentp;
    int ipercentc = (int)percentc;


    //double thepercent = 0.77;
    double thepercent = (double)(0 + std::streampos(703 + 1)) / (double)theend;
    int itemsp = (int)(thepercent * 80.0);
    itemsp *= 2;
      if (itemsp  >79)
      {
        itemsp = 79;
      }
      if (itemsp < 1)
      {
        itemsp = 1;
      }
//    int iy = 1;
    //if (theend > 0)
    //{
    //  double y = 60450.23*pow(theend, -0.8831565);
    //  itemsp = (int)y;
    //  int m = 80 - itemsp;
    //  //if (iy > m)
    //  //{
    //  //  iy = m;
    //  //}
    //  //if (iy < 1)
    //  //{
    //  //  iy = 1;
    //  //}
    //  if (itemsp  >80)
    //  {
    //    itemsp = 79;
    //  }
    //  if (itemsp < 1)
    //  {
    //    itemsp = 1;
    //  }
    //}

      g_itemsp = itemsp;
    int itemsleft = 80 - itemsp;
    int leftSpace = (int)((percents*(double)itemsleft)+0.5);
    int rightSpace = (int)(((1.0 - percents)*(double)itemsleft)+0.5);
    g_itemsleft = itemsleft;
    g_leftSpace = leftSpace;
    g_rightSpace = rightSpace;

//
//    if (ipercent==0

    std::wstringstream buf;
    for (int i = 0; i < leftSpace; ++i)
    {
      buf << L"─";
    }
    for (int i = 0; i < itemsp; ++i)
    {
      buf << L"═";
    }
    for (int i = 0; i < rightSpace; ++i)
    {
      buf << L"─";
    }
    //for (int i = 0; i < itemsp; ++i)
    //{
    //  buf << L"─";
    //}
    //for (int i = itemsp; i < itemsp+iy; ++i)
    //{
    //  buf << L"═";
    //}
    //for (int i = itemsp+iy; i < 80; ++i)
    //{
    //  buf << L"─";
    //}
    //int spare = 80 - iy;
    //std::wstringstream buf;
    //for (int i = thestart; i < 703; ++i)
    //{

    //}
    //std::wstringstream buf;
    //for (int i = 0; i < iy; ++i)
    //{
    //  buf << L"═";
    //}
    //for (int i = iy; i < 80; ++i)
    //{
    //  buf << L"─";
    //}

//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"═══════════════════════════════════════════════════════════════─────────────────", FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buf.str(), FG_GREY | BG_BLACK);
    line++;

    if (!m_hasFocus && HasFocus())
    {
      RenderNow();
    }
    m_hasFocus = HasFocus();

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

      std::map<__int64, AnEdit>::iterator it;
      for (it = m_edits.begin(); it != m_edits.end(); it++)
      {
        __int64 address = it->first;
        __int64 offset = address - thestart;
        char ch = memblock2[offset];
        std::wstring str = GetHex(ch);
        std::wstring str1 = str.substr(0, 1);
        std::wstring str2 = str.substr(1, 1);
        if (it->second.OneSet)
        {
          if (offset >= 0 && offset <= 703)
          {
            __int64 x = GetXCoord(offset, CUR_ONE);
            __int64 y = GetYCoord(offset, CUR_ONE);
            std::wstringstream out;
            out << it->second.One;
            str1 = out.str();
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, out.str(), FG_CYAN | BG_BLACK);
          }
        }
        if (it->second.TwoSet)
        {
          if (offset >= 0 && offset <= 703)
          {
            __int64 x = GetXCoord(offset, CUR_TWO);
            __int64 y = GetYCoord(offset, CUR_TWO);
            std::wstringstream out;
            out << it->second.Two;
            str2 = out.str();
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, out.str(), FG_CYAN | BG_BLACK);
          }
        }
        
        char finalChar = (char)HexToInt2(str1 + str2);
        std::wstring finalStr = GetChar(finalChar);

        int y2 = offset / 16;
        int x2 = offset % 16;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 63 + x2, 2 + y2, finalStr, FG_CYAN | BG_BLACK);
      }

      if (m_editingAscii && m_renderCursor && HasFocus())
      {
        __int64 x2 = m_cursorPosition % 16 + 63;
        __int64 y2 = m_cursorPosition / 16 + 2;
        std::wstring ch = GetChar(memblock2[m_cursorPosition]);

        __int64 xc1 = GetXCoord(m_cursorPosition, CUR_ONE);
        __int64 yc1 = GetYCoord(m_cursorPosition, CUR_ONE);
        __int64 xc2 = GetXCoord(m_cursorPosition, CUR_TWO);
        __int64 yc2 = GetYCoord(m_cursorPosition, CUR_TWO);
        std::wstring hex = GetHex(memblock2[m_cursorPosition]);
        std::wstring hex1 = hex.substr(0, 1);
        std::wstring hex2 = hex.substr(1, 1);
        m_cursor = CUR_ONE;
        HasCoord(xc1, yc1, hex1);
        m_cursor = CUR_TWO;
        HasCoord(xc2, yc2, hex2);
        hex = hex1 + hex2;
        ch = GetChar((char)HexToInt2(hex));

        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x2, y2, ch, FG_RED | BG_WHITE);
      }
      else if (m_editing && m_renderCursor && HasFocus())
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
        HasCoord(x, y, hexChar);
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, hexChar, FG_RED | BG_WHITE);
      }
    }

    if (m_editing)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 63, 0, L"Byte:            ", FG_GREY | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 63, 0, L"Byte: "+GetHexPadded8(thestart+m_cursorPosition), FG_GREY | BG_BLACK);
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
      if (m_searchType == S_TEXT)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                               F2 case sensitive (no )  F4 search for (text   ) ", FG_GREY | BG_BLACK);
      }
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
      else if (m_saving)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L" Save modifications to disk?                                                    ", FG_GREY | BG_BLACK);
        if (m_renderCursor && HasFocus())
        {
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 29, line, L" ", FG_WHITE | BG_WHITE);
        }
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                   Yes  No  F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                   Y    N   F1       ESC        ", FG_CYAN | BG_BLACK);
        line++;
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
      else if (m_jumping)
      {
        std::wstring beforeCursor = std::wstring(L"Goto address: ") + m_typed;

        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"Goto address:                                                                   ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"Goto address: ").length(), line, m_typed, FG_CYAN | BG_BLACK);
        if (m_renderCursor && HasFocus())
        {
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), line, L"▄", FG_GREY | BG_BLACK);
        }
        else
        {
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), line, L" ", FG_GREY | BG_BLACK);
        }
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"Enter location as $ hex % percentage                ◄─┘ ok  F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                    ◄─┘     F1    p  ESC        ", FG_CYAN | BG_BLACK);
        line++;
      }
      else
      {
        int endit = 703 + 1;
        if (theend < endit)
        {
          endit = theend;
        }

        double percent = (double)(thestart+std::streampos(endit)) / (double)theend * 100.0;
        int ipercent = (int)percent;
        std::wstringstream wpercent;
        if (ipercent < 100)
        {
          wpercent << " ";
        }
        if (ipercent < 10)
        {
          wpercent << " ";
        }
        wpercent << ipercent << " %";
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"VIEW      ASCII  Dump  Edit  Find  Hex  Mask  Wordwrap  Jump                    ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                          " + wpercent.str(), FG_CYAN | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          A      D     E     F     H    M     W         J                       ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"COMMANDS  0..9 goto bookmark         F9 search             SPACE search again   ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          0..9                       F9 s                  SPACE                ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"←↑↓→ scroll  SHIFT 0..9 set ALT 0..9 clear bookmarks        F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"             SHIFT 0..9     ALT 0..9                        F1       ESC        ", FG_CYAN | BG_BLACK);
        line++;
      }
    }

  }

  void Search::RenderDump()
  {
    std::wstring file = StrUtil::s2ws(theFile);
    std::wstringstream ss;
    ss << file;// << L"_" << lastChar;
    file = ss.str();
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"File:                                                          DUMP  (no mask)  ", FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 6, 0, file, FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"────────────────────────────────────────────────────────────────────────────────", FG_GREY | BG_BLACK);

    ss.str(L"");
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
    __int64 endy = start + 2752;
    if (endy > theend)
      endy = theend;

    __int64 printed = 0;
    __int64 off = 10;
    bool isFirst = false;
    //        for (int i = start; i <= endy; i += 16)
    bool isPrinted = false;
    bool startedPrinting = false;
    for (__int64 i = start; i <= endy;)
    {
      std::wstringstream buf;

      if (endy != 0 && (index + start < theend))
      {
        buf << GetHexPadded8(i) << L"  ";
      }
      else
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
      }

      for (int j = 0; j < 64; ++j)
      {
        if (i >= lTheSearchPos && i <= lTheSearchPos + std::streampos(lNumFound) && printed < lNumFound)
        {
          ss << GetChar(memblock2[index + j]);
          isPrinted = true;
          startedPrinting = true;
          printed++;
          if (printed != lNumFound)
          {
//            ss << L" ";
          }
        }

        if (index + j + start < theend)
        {
          buf << GetChar(memblock2[index + j]);
        }
        else
        {
          buf << L" ";
        }
        ++i;
      }
      index += 64;
      buf << L"      ";

      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buf.str(), FG_CYAN | BG_BLACK);

      if (lNumFound > 0 && ss.str().length()>=1)
      {
        std::wstring oneLess = ss.str();
        ss.str(L"");

        if (oneLess.length()>0 && oneLess[oneLess.length() - 1] == 32)
        {
          oneLess = oneLess.substr(0, oneLess.length() - 1);
        }
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, off, line, oneLess, FG_BLACK | BG_GREY);
      }

      line++;
    }

    for (int i = line; i <= 45; ++i)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
      line++;
    }

    //https://mycurvefit.com/
    //power
    //y = 60450.23*x^-0.8831565
    /*
    consoleapp.csproj
    {_Myoff=0 _Fpos=2425 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ════════════════════════════════════════════════════════════════════════────────
    2425 => 72/80, 8, 0.9, 0.1

    program.cs
    {_Myoff=0 _Fpos=8595 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ══════════════──────────────────────────────────────────────────────────────────
    8595 => 14/80, 66, 0.175, 0.825

    hookdll.dll
    {_Myoff=0 _Fpos=32768 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ════════────────────────────────────────────────────────────────────────────────
    32768 => 8/80, 72, 0.1, 0.9

    hookexe.exe
    {_Myoff=0 _Fpos=28672 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ═════════───────────────────────────────────────────────────────────────────────
    28672 => 9/80, 71, 0.1125, 0.8875

    hookdll.cpp
    {_Myoff=0 _Fpos=2011 _Mystate={_Wchar=0 _Byte=0 _State=0 } }
    ══════════════════════════════════════════════════════════════════──────────────
    2011 => 66/80, 14, 0.825, 0.175
    */
    double percent = (double)(thestart + std::streampos(703 + 1)) / (double)theend * 100.0;
    double percents = (double)(thestart) / (double)theend;
    if (percent >= 100)
    {
      percents = 1.0;
    }
    double percentp = (double)(thestart) / (double)theend * 100.0;
    double percentpx = 1.0 - (double)(thestart) / (double)theend;
    double percentc = (double)(thestart + std::streampos(703 + 1)) / (double)theend;
    int ipercent = (int)percent;
    int ipercentp = (int)percentp;
    int ipercentc = (int)percentc;


    //double thepercent = 0.77;
    double thepercent = (double)(0 + std::streampos(703 + 1)) / (double)theend;
    int itemsp = (int)(thepercent * 80.0);
    itemsp *= 2;
    if (itemsp  >79)
    {
      itemsp = 79;
    }
    if (itemsp < 1)
    {
      itemsp = 1;
    }
    //    int iy = 1;
    //if (theend > 0)
    //{
    //  double y = 60450.23*pow(theend, -0.8831565);
    //  itemsp = (int)y;
    //  int m = 80 - itemsp;
    //  //if (iy > m)
    //  //{
    //  //  iy = m;
    //  //}
    //  //if (iy < 1)
    //  //{
    //  //  iy = 1;
    //  //}
    //  if (itemsp  >80)
    //  {
    //    itemsp = 79;
    //  }
    //  if (itemsp < 1)
    //  {
    //    itemsp = 1;
    //  }
    //}

    g_itemsp = itemsp;
    int itemsleft = 80 - itemsp;
    int leftSpace = (int)((percents*(double)itemsleft) + 0.5);
    int rightSpace = (int)(((1.0 - percents)*(double)itemsleft) + 0.5);
    g_itemsleft = itemsleft;
    g_leftSpace = leftSpace;
    g_rightSpace = rightSpace;

    //
    //    if (ipercent==0

    std::wstringstream buf;
    for (int i = 0; i < leftSpace; ++i)
    {
      buf << L"─";
    }
    for (int i = 0; i < itemsp; ++i)
    {
      buf << L"═";
    }
    for (int i = 0; i < rightSpace; ++i)
    {
      buf << L"─";
    }
    //for (int i = 0; i < itemsp; ++i)
    //{
    //  buf << L"─";
    //}
    //for (int i = itemsp; i < itemsp+iy; ++i)
    //{
    //  buf << L"═";
    //}
    //for (int i = itemsp+iy; i < 80; ++i)
    //{
    //  buf << L"─";
    //}
    //int spare = 80 - iy;
    //std::wstringstream buf;
    //for (int i = thestart; i < 703; ++i)
    //{

    //}
    //std::wstringstream buf;
    //for (int i = 0; i < iy; ++i)
    //{
    //  buf << L"═";
    //}
    //for (int i = iy; i < 80; ++i)
    //{
    //  buf << L"─";
    //}

    //    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"═══════════════════════════════════════════════════════════════─────────────────", FG_GREY | BG_BLACK);
    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, buf.str(), FG_GREY | BG_BLACK);
    line++;

    if (!m_hasFocus && HasFocus())
    {
      RenderNow();
    }
    m_hasFocus = HasFocus();

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

      std::map<__int64, AnEdit>::iterator it;
      for (it = m_edits.begin(); it != m_edits.end(); it++)
      {
        __int64 address = it->first;
        __int64 offset = address - thestart;
        char ch = memblock2[offset];
        std::wstring str = GetHex(ch);
        std::wstring str1 = str.substr(0, 1);
        std::wstring str2 = str.substr(1, 1);
        if (it->second.OneSet)
        {
          if (offset >= 0 && offset <= (44*64-1))
          {
            __int64 x = GetDumpXCoord(offset, CUR_ONE);
            __int64 y = GetDumpYCoord(offset, CUR_ONE);
            std::wstringstream out;
            out << it->second.One;
            str1 = out.str();
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, out.str(), FG_CYAN | BG_BLACK);
          }
        }
        if (it->second.TwoSet)
        {
          if (offset >= 0 && offset <= (44 * 64 - 1))
          {
            __int64 x = GetDumpXCoord(offset, CUR_TWO);
            __int64 y = GetDumpYCoord(offset, CUR_TWO);
            std::wstringstream out;
            out << it->second.Two;
            str2 = out.str();
            DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, out.str(), FG_CYAN | BG_BLACK);
          }
        }

        char finalChar = (char)HexToInt2(str1 + str2);
        std::wstring finalStr = GetChar(finalChar);

        int y2 = offset / 64;
        int x2 = offset % 64;
//        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10 + x2, 2 + y2, finalStr, FG_CYAN | BG_BLACK);
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10 + x2, 2 + y2, finalStr, FG_CYAN | BG_BLACK);
      }

      if (m_editingDump && m_renderCursor && HasFocus())
      {
        __int64 x2 = m_cursorPosition % 64 + 10;
        __int64 y2 = m_cursorPosition / 64 + 2;
        std::wstring ch = GetChar(memblock2[m_cursorPosition]);

        __int64 xc1 = GetDumpXCoord(m_cursorPosition, CUR_ONE);
        __int64 yc1 = GetDumpYCoord(m_cursorPosition, CUR_ONE);
        __int64 xc2 = GetDumpXCoord(m_cursorPosition, CUR_TWO);
        __int64 yc2 = GetDumpYCoord(m_cursorPosition, CUR_TWO);
        std::wstring hex = GetHex(memblock2[m_cursorPosition]);
        std::wstring hex1 = hex.substr(0, 1);
        std::wstring hex2 = hex.substr(1, 1);
        m_cursor = CUR_ONE;
        HasCoordDump(xc1, yc1, hex1);
        m_cursor = CUR_TWO;
        HasCoordDump(xc2, yc2, hex2);
        hex = hex1 + hex2;
        ch = GetChar((char)HexToInt2(hex));

        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x2, y2, ch, FG_RED | BG_WHITE);
      }
      //else if (m_editingDump && m_renderCursor && HasFocus())
      //{
      //  __int64 x = GetDumpXCoord(m_cursorPosition, m_cursor);
      //  __int64 y = GetDumpYCoord(m_cursorPosition, m_cursor);
      //  std::wstring hex = GetHex(memblock2[m_cursorPosition]);
      //  std::wstring hex1 = hex.substr(0, 1);
      //  std::wstring hex2 = hex.substr(1, 1);
      //  std::wstring hexChar = L"";
      //  if (m_cursor == CUR_ONE)
      //  {
      //    hexChar = hex1;
      //  }
      //  else if (m_cursor == CUR_TWO)
      //  {
      //    hexChar = hex2;
      //  }
      //  HasCoord(x, y, hexChar);
      //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, x, 2 + y, hexChar, FG_RED | BG_WHITE);
      //}
    }

    if (m_editingDump)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 63, 0, L"Byte:            ", FG_GREY | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 63, 0, L"Byte: " + GetHexPadded8(thestart + m_cursorPosition), FG_GREY | BG_BLACK);
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
      if (m_searchType == S_TEXT)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                               F2 case sensitive (no )  F4 search for (text   ) ", FG_GREY | BG_BLACK);
      }
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
      else if (m_saving)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L" Save modifications to disk?                                                    ", FG_GREY | BG_BLACK);
        if (m_renderCursor && HasFocus())
        {
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 29, line, L" ", FG_WHITE | BG_WHITE);
        }
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                   Yes  No  F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                   Y    N   F1       ESC        ", FG_CYAN | BG_BLACK);
        line++;
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
      else if (m_jumping)
      {
        std::wstring beforeCursor = std::wstring(L"Goto address: ") + m_typed;

        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"Goto address:                                                                   ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"Goto address: ").length(), line, m_typed, FG_CYAN | BG_BLACK);
        if (m_renderCursor && HasFocus())
        {
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), line, L"▄", FG_GREY | BG_BLACK);
        }
        else
        {
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), line, L" ", FG_GREY | BG_BLACK);
        }
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                                ", FG_GREY | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"Enter location as $ hex % percentage                ◄─┘ ok  F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                    ◄─┘     F1    p  ESC        ", FG_CYAN | BG_BLACK);
        line++;
      }
      else
      {
        int endit = 703 + 1;
        if (theend < endit)
        {
          endit = theend;
        }

        double percent = (double)(thestart + std::streampos(endit)) / (double)theend * 100.0;
        int ipercent = (int)percent;
        std::wstringstream wpercent;
        if (ipercent < 100)
        {
          wpercent << " ";
        }
        if (ipercent < 10)
        {
          wpercent << " ";
        }
        wpercent << ipercent << " %";
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"VIEW      ASCII  Dump  Edit  Find  Hex  Mask  Wordwrap  Jump                    ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"                                                                          " + wpercent.str(), FG_CYAN | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          A      D     E     F     H    M     W         J                       ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"COMMANDS  0..9 goto bookmark         F9 search             SPACE search again   ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"          0..9                       F9 s                  SPACE                ", FG_CYAN | BG_BLACK);
        line++;
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"←↑↓→ scroll  SHIFT 0..9 set ALT 0..9 clear bookmarks        F1 help  ESC cancel ", FG_GREY | BG_BLACK);
        DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, line, L"             SHIFT 0..9     ALT 0..9                        F1       ESC        ", FG_CYAN | BG_BLACK);
        line++;
      }
    }

  }

}
