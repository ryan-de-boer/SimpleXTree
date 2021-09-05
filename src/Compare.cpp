#include "stdafx.h"
#include "Compare.h"
#include <filesystem>
#include "StrUtil.h"
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <windows.h>
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
#include <vector>
#include "FreeDiskSpace.h"
extern CHAR_INFO* m_bufScreen;

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

void Log(std::wstring const& pathW);
bool CharIsPressed(int getKey);
extern std::string m_writtenKey;
void DrawStringSkipSpace(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);
bool IsDirectory(std::wstring path);

namespace SimpleXTree
{
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


	extern int nScreenWidth;			// Console Screen Size X (columns)
	extern int nScreenHeight;			// Console Screen Size Y (rows)

	Compare::Compare() : m_dirObject(NULL), m_selected(NULL), m_initial(NULL), m_activated(false), 
    m_checkingForKeys(true), m_lPressed(0), m_escPressed(false), m_show(false), m_lastShown(false), m_timeSet(false), 
    m_timePassed(0), m_renderCursor(true), m_waitForKeyLetGo(-1), m_showAvail(false), m_browse(false), m_step2(false), 
    m_identical(false), m_unique(false), m_newer(false), m_older(false), m_binary(false), m_subs(false)
	{
	}

	Compare::~Compare()
	{
	}

	void Compare::Activate()
	{
		//if (!m_activated)
		//	m_initial = m_dirObject;
		m_activated = true;
	}

	void Compare::Toggle()
	{
		//if (!m_activated)
		//	m_initial = m_dirObject;
		m_activated = !m_activated;
	}

	void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);

  std::wstring Compare::YesNo(bool value)
  {
    if (value)
    {
      return L"yes";
    }
    else
    {
      return L"no ";
    }
  }

	void Compare::Render(int start)
	{
		if (!m_activated || !m_show)
			return;

		if (m_step2)
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"COMPARE file list in:               with:                                     ", FG_GREY | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in: ").length(), start, m_initial->GetNameW(), FG_CYAN | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in:               with: ").length(), start, m_typed, FG_CYAN | BG_BLACK);

			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"  tag files that are: Identical (" + YesNo(m_identical) + L")  Unique (" + YesNo(m_unique) + L")  Newer (" + YesNo(m_newer) + L")  Older (" + YesNo(m_older) + L")  ", FG_GREY | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                      I                U             N            O          ", FG_CYAN | BG_BLACK);
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Select tag criteria   Binary (" + YesNo(m_binary) + L")  Subs (" + YesNo(m_subs) + L")      ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                      B             S               ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);


		}
		else if (m_browse)
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"COMPARE file list in:               with:                                     ", FG_GREY | BG_BLACK);
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                                                                              ", FG_GREY | BG_BLACK);
//			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Enter compare path                                  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
//			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                                    ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"←↑↓→ scroll              < > select disk  Log disk  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                         < >              L         ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);

			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in: ").length(), start, m_initial->GetNameW(), FG_CYAN | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in:               with: ").length(), start, m_typed, FG_CYAN | BG_BLACK);
		}
		else
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"COMPARE file list in:               with:                                     ", FG_GREY | BG_BLACK);
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                                                                              ", FG_GREY | BG_BLACK);
			//		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Enter compare path          F2 F4 point  ↑ history  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
			//		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                            F2 F4        ↑          ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Enter compare path             F2 point  ↑ history  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                               F2        ↑          ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);

//			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in: ").length(), start, m_dirObject->GetNameW(), FG_CYAN | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in: ").length(), start, m_initial->GetNameW(), FG_CYAN | BG_BLACK);
			DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COMPARE file list in:               with: ").length(), start, m_typed, FG_CYAN | BG_BLACK);

			//https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
			if (!m_timeSet)
			{
				auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
				m_timePassed = millisec_since_epoch;
				m_timeSet = true;
			}

			auto currentMillisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			if (currentMillisec_since_epoch - m_timePassed > 1000)
			{
				m_timePassed = currentMillisec_since_epoch;
				m_timeSet = true;
				m_renderCursor = !m_renderCursor;
			}

			std::wstringstream t;
			t << m_timePassed;
			HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
																  //		SetConsoleTitle(t.str().c_str());            // Set Buffer Size 
			std::wstring beforeCursor = std::wstring(L"COMPARE file list in:               with: ") + m_typed;
			if (m_showAvail)
			{
			}
			else if (m_renderCursor)
			{
				DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), start, L"▄", FG_GREY | BG_BLACK);
			}
			else
			{
				DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), start, L" ", FG_GREY | BG_BLACK);
			}
		}



	}

  //https://docs.microsoft.com/en-us/windows/win32/sysinfo/retrieving-the-last-write-time
  // GetLastWriteTime - Retrieves the last-write time and converts
  //                    the time to a string
  //
  // Return value - TRUE if successful, FALSE otherwise
  // hFile      - Valid file handle
  // lpszString - Pointer to buffer to receive string

  SYSTEMTIME GetLastWriteTime(HANDLE hFile)
  {
    DWORD dwSize = MAX_PATH;

    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal = {};
    DWORD dwRet;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
      return stLocal;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
//    dwRet = StringCchPrintf(lpszString, dwSize,
//      TEXT("%02d/%02d/%d  %02d:%02d"),
//      stLocal.wMonth, stLocal.wDay, stLocal.wYear,
//      stLocal.wHour, stLocal.wMinute);

    return stLocal;
  }

  FILETIME GetLastWriteTimeFILETIME(HANDLE hFile)
  {
    DWORD dwSize = MAX_PATH;

    FILETIME ftCreate, ftAccess, ftWrite = {};
    SYSTEMTIME stUTC, stLocal = {};
    DWORD dwRet;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
      ;
    return ftWrite;
  }

  //bool Check(std::wstring const& filePath, std::wstring const& filePath2)
  //{
  //  HANDLE hFile, hFile2;
  //  hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  //  hFile2 = CreateFile(filePath2.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

  //  SYSTEMTIME l1 = GetLastWriteTime(hFile);
  //  SYSTEMTIME l2 = GetLastWriteTime(hFile2);

  //  CloseHandle(hFile);
  //  CloseHandle(hFile2);

  //  return Equal(l1, l2);
  //}

  long Compare(std::wstring const& filePath, std::wstring const& filePath2)
  {
    HANDLE hFile, hFile2;
    hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    hFile2 = CreateFile(filePath2.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    FILETIME l1 = GetLastWriteTimeFILETIME(hFile);
    FILETIME l2 = GetLastWriteTimeFILETIME(hFile2);

    CloseHandle(hFile);
    CloseHandle(hFile2);

    return CompareFileTime(&l1, &l2);
  }

  bool Equal(std::wstring const& filePath, std::wstring const& filePath2)
  {
    return Compare(filePath, filePath2)==0;
  }

  bool Check(std::wstring const& filePath, std::wstring const& filePath2)
  {
    //HANDLE hFile, hFile2;
    //hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    //hFile2 = CreateFile(filePath2.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    //SYSTEMTIME l1 = GetLastWriteTime(hFile);
    //SYSTEMTIME l2 = GetLastWriteTime(hFile2);

    //CloseHandle(hFile);
    //CloseHandle(hFile2);

    //return Equal(l1, l2);

    return Equal(filePath, filePath2);
  }

  bool Newer(std::wstring const& filePath, std::wstring const& filePath2)
  {
    return Compare(filePath, filePath2)==1; //First file time is later than second file time.
  }

  bool Older(std::wstring const& filePath, std::wstring const& filePath2)
  {
    return Compare(filePath, filePath2) == -1; //First file time is earlier than second file time.
  }


  //https://codereview.stackexchange.com/questions/204478/comparing-two-large-binary-files-in-c
  // Use something more generous than 16.  Maybe even 4096 or 64k and allocate buffers
#define CMP_N 256

  // Return value:
  // 0: files compare equal in content and length, fp1 size saved as offset
  // 1: files differ, fp1 longer, fp2 size saved as offset
  // 2: files differ, fp2 longer, fp1 size saved as offset
  // 3: files differ at offset
  // -1: fp1 trouble reading.  Unspecified data in offset
  // -2: fp2 trouble reading.  Unspecified data in offset
  int compare_two_binary_files_alternate(FILE *fp1, FILE *fp2, uintmax_t *offset) {
    char tmp1[CMP_N], tmp2[CMP_N];
    size_t n1, n2;

    rewind(fp1);  // start at beginning and clear error
    rewind(fp2);
    *offset = 0;
    do {
      n1 = fread(tmp1, sizeof *tmp1, sizeof tmp1 / sizeof *tmp1, fp1);
      if (n1 == 0 && ferror(fp1)) {
        return -1;
      }
      n2 = fread(tmp2, sizeof *tmp2, sizeof tmp2 / sizeof *tmp2, fp2);
      if (n2 == 0 && ferror(fp2)) {
        return -2;
      }
      size_t n_min = n1 < n2 ? n1 : n2;
      if (memcmp(tmp1, tmp2, n_min)) {        // Quickly find if file contents differ ...
        for (size_t i = 0; i < n_min; i++) {  // Slowly find where they differ
          if (tmp1[i] != tmp2[i]) {
            *offset += i;
            return 3;
          }
        }
      }
      *offset += n_min;
      if (n1 > n_min) {
        return 1;
      }
      if (n2 > n_min) {
        return 2;
      }
    } while (n1);
    return 0;
  }

  bool BinaryEqual(std::wstring const& filePath, std::wstring const& filePath2)
  {
    std::string sFilePath = StrUtil::ws2s(filePath);
    std::string sFilePath2 = StrUtil::ws2s(filePath2);
    FILE* fp1 = fopen(sFilePath.c_str(), "rb");
    FILE* fp2 = fopen(sFilePath2.c_str(), "rb");
    uintmax_t offset;
    return compare_two_binary_files_alternate(fp1, fp2, &offset) == 0;
  }

  //void TagFile(DirObject* initial, std::wstring const& filePath)
  //{
  //  for (int i = 0; i < initial->Files.size(); ++i)
  //  {
  //    if (initial->Files[i] == filePath)
  //    {
  //      initial->Tag(i);
  //      break;
  //    }
  //  }
  //}

  void TagFile(DirObject* initial, std::wstring const& filePath)
  {
    if (!initial->IsExpanded)
    {
      initial->Expand();
    }
    for (int i = 0; i < initial->Files.size(); ++i)
    {
      if (initial->Files[i] == filePath)
      {
        initial->Tag(i);
        break;
      }
    }
    for (int i = 0; i < initial->ChildrenPaths.size(); ++i)
    {
      TagFile(&initial->ChildrenPaths[i], filePath);
    }
  }

  std::vector<std::wstring> IterateFiles(std::wstring const& path)
  {
    std::vector<std::wstring> files;
    for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
    {
      if (std::experimental::filesystem::is_directory(entry.path()))
      {
        std::wstring f = entry.path();
        std::vector<std::wstring> children = IterateFiles(f);
        for (int i = 0; i < children.size(); ++i)
        {
          files.push_back(children[i]);
        }
      }
      else
      {
        std::wstring f = entry.path();
        files.push_back(f);
      }
    }
    return files;
  }

	void Compare::CheckKeys(DirObject* dirObject, bool filesScreen)
	{
		m_dirObject = dirObject;
		if (!m_checkingForKeys || m_otherActive || filesScreen)
			return;

		if (!m_show)
		{
			if ((0x8000 & GetAsyncKeyState((unsigned char)(0x43/*C*/))) != 0)
			{
				m_lPressed = true;
				m_show = true;
				m_activated = true;
				m_initial = dirObject;
				m_waitForKeyLetGo = -1;
			}
			else
			{
				m_lPressed = false;
				return;
			}
		}
		else if ((0x8000 & GetAsyncKeyState((unsigned char)(0x43/*C*/))) == 0)
		{
			m_lPressed = false;
		}

		if (m_lPressed)
		{

		}

		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0)
		{
      if (m_step2)
      {
        // end of step 2

        std::wstring typedNoSlash = m_typed;
        if (StrUtil::EndsWith(typedNoSlash, L"\\"))
        {
          typedNoSlash = typedNoSlash.substr(0, typedNoSlash.length() - 1);
        }

        std::vector<std::wstring> folder1Files;
        std::vector<std::wstring> folder2Files;
        for (const auto & entry : std::experimental::filesystem::directory_iterator(m_initial->PathW()))
        {
          if (std::experimental::filesystem::is_directory(entry.path()))
          {
            //Subs, preforms the comparison operation into subdirectories aswell.
            if (m_subs)
            {
              std::vector<std::wstring> subFiles = IterateFiles(entry.path());
              for (int i = 0; i < subFiles.size(); ++i)
              {
                folder1Files.push_back(subFiles[i]);
              }
            }
          }
          else
          {
            std::wstring f = entry.path();
            folder1Files.push_back(f);
          }
        }
        for (const auto & entry : std::experimental::filesystem::directory_iterator(typedNoSlash))
        {
          if (std::experimental::filesystem::is_directory(entry.path()))
          {
            if (m_subs)
            {
              //Subs, preforms the comparison operation into subdirectories aswell.
              std::vector<std::wstring> subFiles = IterateFiles(entry.path());
              for (int i = 0; i < subFiles.size(); ++i)
              {
                folder2Files.push_back(subFiles[i]);
              }
            }
          }
          else
          {
            std::wstring f = entry.path();
            folder2Files.push_back(f);
          }
        }

        for (int i = 0; i < folder1Files.size(); ++i)
        {
          std::wstring file1 = folder1Files[i];
          std::wstring file1Name = file1;
          if (file1.find(L"\\") != std::wstring::npos)
          {
            file1Name = file1.substr(file1.rfind(L"\\") + 1);
          }
          std::wstring subPathFile1 = file1;
          if (file1.find(L"\\") != std::wstring::npos)
          {
            subPathFile1 = file1.substr(file1.find(m_initial->PathW()) + m_initial->PathW().length()+1);
          }

          bool filePresentInSecondList = false;
          for (int j = 0; j < folder2Files.size(); ++j)
          {
            std::wstring file2 = folder2Files[j];
            std::wstring file2Name = file2;
            if (file2.find(L"\\") != std::wstring::npos)
            {
              file2Name = file2.substr(file2.rfind(L"\\") + 1);
            }
            std::wstring subPathFile2 = file2;
            if (file2.find(L"\\") != std::wstring::npos)
            {
              subPathFile2 = file2.substr(file2.find(typedNoSlash) + typedNoSlash.length()+1);
            }

            if (file1Name == file2Name || (m_subs && StrUtil::EqualsIgnoreCase(subPathFile1, subPathFile2)))
            {
              filePresentInSecondList = true;
              if (m_identical)
              {
                //Identical, tags the files with the same name and file time.
                bool identical = Check(file1, file2);
                if (identical)
                {
                  TagFile(m_initial, file1);
                }
              }
              if (m_newer)
              {
                //Newer, tags the files with the same name and a newer file time.
                bool isNewer = Newer(file1, file2);
                if (isNewer)
                {
                  TagFile(m_initial, file1);
                }
              }
              if (m_older)
              {
                //Older, tags the files with the same name and a older file time.
                bool isOlder = Older(file1, file2);
                if (isOlder)
                {
                  TagFile(m_initial, file1);
                }
              }
              if (m_binary)
              {
                //Binary, tags the files that are the same size and contain exactly the same content.
                bool isBinaryEqual = BinaryEqual(file1, file2);
                if (isBinaryEqual)
                {
                  TagFile(m_initial, file1);
                }
              }

              int a = 1;
              a++;
            }

          }

          if (m_unique && !filePresentInSecondList)
          {
            //Unique, tags the files which are not in the second list.
            TagFile(m_initial, file1);
          }
        }

        m_escPressed = true;
        m_show = false;
        m_activated = false;
        m_showAvail = false;
        m_step2 = false;
      }
      else
      {
        m_step2 = true;
      }
		}
		else
		{
		}

		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0)
		{
			m_escPressed = true;
			m_show = false;
			m_activated = false;
			m_showAvail = false;
			m_step2 = false;
		}
		else
		{
			m_escPressed = false;
		}


		if (m_show && !m_lPressed)
		{
		}


		m_lastShown = m_show;

	}

	void Compare::KeyEvent(WCHAR ch)
	{
		if (ch == '\r' || ch == '\n' || ch=='\x1b')
		{

		}
    else if (m_step2)
    {
      if (ch == 'i')
      {
        m_identical = !m_identical;
      }
      else if (ch == 'u')
      {
        m_unique = !m_unique;
      }
      else if (ch == 'n')
      {
        m_newer = !m_newer;
      }
      else if (ch == 'o')
      {
        m_older = !m_older;
      }
      else if (ch == 'b')
      {
        m_binary = !m_binary;
      }
      else if (ch == 's')
      {
        m_subs = !m_subs;
      }
    }
		else if (ch == '\b')
		{
			if (m_typed.length() > 0)
			{
				m_typed = m_typed.substr(0, m_typed.length() - 1);
			}
		}
		else
		{
			m_typed += ch;
		}
	}

	void Compare::VK(DWORD vk)
	{
		if (vk == VK_F2)
		{
			m_browse = true;
		}
	}

	void Compare::SelectDir(DirObject* dirObject)
	{
		m_selected = dirObject;
		m_typed = dirObject->PathW();
		m_browse = false;
		m_activated = true;
		m_show = true;
	}
}
