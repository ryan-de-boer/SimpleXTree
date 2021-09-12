/*
┌────────────────────┐
│31% complete
 XXXXXXXXXXXXXXXXXX
│Copying 4 items from 0new to 0new
 Name: fns.m4a
 Time remaining: 10 seconds
 Items remaining: 3 (469 MB)
└────────────────────┘
*/

#include "stdafx.h"
#include "Copy.h"
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
extern CHAR_INFO* m_bufScreen;

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

void Log(std::wstring const& pathW);
bool CharIsPressed(int getKey);
bool CharOrNumIsPressed(int getKey);
extern std::string m_writtenKey;
void DrawStringSkipSpace(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);
bool IsDirectory(std::wstring path);
void Refresh();

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

  void CopyItem::DoCopy()
  {
    Sleep(1000);
  }

	Copy::Copy() : m_dirObject(NULL), m_activated(false), m_checkingForKeys(true), m_lPressed(0), m_escPressed(false), m_show(false), m_lastShown(false), m_timeSet(false), m_timePassed(0), m_renderCursor(true), m_waitForKeyLetGo(-1), m_showAvail(false), m_percent(0.0), m_exitThread(false), m_threadReadyToCopy(false), m_currentName(L""), m_timeSecondsLeft(0.0), m_calculating(true), m_numLeft(0), m_numItems(0), m_bytesLeft(0), m_selectStep(false), m_fileSpec(L""), m_toStep(false), m_destinationFolder(L""), m_createDirStep(false), m_copyStep(false)
	{
    //https://softwareengineering.stackexchange.com/questions/382195/is-it-okay-to-start-a-thread-from-within-a-constructor-of-a-class
    m_member_thread = std::thread(&Copy::ThreadFn, this);
	}

	Copy::~Copy()
	{
    m_exitThread = true;
    m_member_thread.join();
  }

	void Copy::Activate()
	{
		m_activated = true;
	}

	void Copy::Toggle()
	{
		m_activated = !m_activated;
	}

	void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);

	void Copy::Render(int start)
	{
		if (!m_activated || !m_show)
			return;

    if (m_selectStep)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"COPY all tagged files as:                                                         ", FG_GREY | BG_BLACK);
//      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COPY all tagged files as: ").length(), start, m_fileSpec, FG_CYAN | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COPY all tagged files as: ").length(), start, m_typed, FG_CYAN | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                                                                              ", FG_GREY | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Enter file spec or strike enter          ↑ history  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                         ↑          ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);


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
      std::wstring beforeCursor = std::wstring(L"COPY all tagged files as: ") + m_typed;
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


      return;
    }
    else if (m_toStep)
    {
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"COPY all tagged files as:                                                         ", FG_GREY | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COPY all tagged files as: ").length(), start, m_typed, FG_CYAN | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"       to:                                                                     ", FG_GREY | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"       to: ").length(), start +1, m_typed2, FG_CYAN | BG_BLACK);
      DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Enter file spec or strike enter          ↑ history  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
      DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                         ↑          ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);

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
//      std::wstring beforeCursor = std::wstring(L"       to: ") + m_typed;
      std::wstring beforeCursor = std::wstring(L"       to: ") + m_typed2;
      if (m_showAvail)
      {
      }
      else if (m_renderCursor)
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), start+1, L"▄", FG_GREY | BG_BLACK);
      }
      else
      {
        DrawString(m_bufScreen, nScreenWidth, nScreenHeight, beforeCursor.length(), start+1, L" ", FG_GREY | BG_BLACK);
      }

      return;
    }
	else if (m_createDirStep)
	{
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"COPY all tagged files as:                                                         ", FG_GREY | BG_BLACK);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"COPY all tagged files as: ").length(), start, m_typed, FG_CYAN | BG_BLACK);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"       to:                                                                     ", FG_GREY | BG_BLACK);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"       to: ").length(), start + 1, m_typed2, FG_CYAN | BG_BLACK);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Directory does not exist.  Make new path?          Yes  No  F1 help  ESC cancel", FG_GREY | BG_BLACK);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                                   Y    N   F1       ESC       ", FG_CYAN | BG_BLACK);


		return;
	}
	else if (m_copyStep)
	{

		//		std::experimental::filesystem::copy()
				//thread https://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11.php

		//    double percentComplete = 32.5;
		//    double percentComplete = 50;
		double percentComplete = m_percent;
		int iPercentComplete = (int)percentComplete;
		std::wstringstream wPercentComplete;
		wPercentComplete << L"│" << iPercentComplete << "% complete";
		int num = wPercentComplete.str().length();
		for (int i = num; i < 51; ++i)
		{
			wPercentComplete << L" ";
		}
		wPercentComplete << L"│";

		double totalNumBricks = 50.0;
		int iTotalNumBricks = (int)totalNumBricks;
		double numBricks = (percentComplete / 100.0) * totalNumBricks;
		int iNumBricks = (int)numBricks;
		std::wstringstream wBricks;
		wBricks << L"│";
		for (int i = 0; i < iNumBricks; ++i)
		{
			wBricks << L"▓";
		}
		for (int i = iNumBricks; i < iTotalNumBricks; ++i)
		{
			wBricks << L"░";
		}
		wBricks << L"│";

		std::wstringstream wItems;
		wItems << L"│Copying " << m_numItems << L" items from:";
		num = wItems.str().length();
		for (int i = num; i < 51; ++i)
		{
			wItems << L" ";
		}
		wItems << L"│";

		std::wstring from = m_from;
		std::wstring to = m_to;
		std::wstring fromc = from;
		std::wstring toc = to;
		std::wstringstream wFrom;
		std::wstringstream wFromInner;
		bool fromt = false;
		if (fromc.length() > 20)
		{
			fromc = fromc.substr(0, 20);
			fromt = true;
		}
		bool tot = false;
		if (toc.length() > 20)
		{
			toc = toc.substr(0, 20);
			tot = true;
		}
		wFromInner << fromc << (fromt ? L"…" : L"") << L" to " << toc << (tot ? L"…" : L"");
		wFrom << L"│" << wFromInner.str();
		num = wFrom.str().length();
		for (int i = num; i < 51; ++i)
		{
			wFrom << L" ";
		}
		wFrom << L"│";
		std::wstringstream wFrom2;
		wFrom2 << L" " << fromc << (fromt ? L"…" : L"") << L"    " << toc << (tot ? L"…" : L"");

		//    std::wstring name = L"fns.m4a";
		std::wstring name = m_currentName;
		std::wstringstream wName;
		wName << L"│Name: " << name;
		num = wName.str().length();
		for (int i = num; i < 51; ++i)
		{
			wName << L" ";
		}
		wName << L"│";

		//    std::wstring time = L"11 seconds";
		std::wstringstream wTime;
		if (m_calculating)
		{
			wTime << L"│Time remaining: calculating...";
		}
		else
		{
			wTime << L"│Time remaining: " << (int)m_timeSecondsLeft << L" seconds";
		}
		num = wTime.str().length();
		for (int i = num; i < 51; ++i)
		{
			wTime << L" ";
		}
		wTime << L"│";

		//    std::wstring itemsRemaining = L"4 (468 MB)";
		std::wstringstream wItemsRemaining;
		std::wstringstream wSize;
		if (m_bytesLeft >= 1000000000)
		{
			double gigs = ((double)m_bytesLeft) / 1000000000.0;
			wSize << L" (" << gigs << L" GB)";
		}
		else if (m_bytesLeft >= 1000000)
		{
			double mb = ((double)m_bytesLeft) / 1000000;
			wSize << L" (" << mb << L" MB)";
		}
		else if (m_bytesLeft >= 1000)
		{
			double kb = ((double)m_bytesLeft) / 1000;
			wSize << L" (" << kb << L" KB)";
		}
		else
		{
			wSize << L" (" << m_bytesLeft << L" bytes)";
		}
		//    wItemsRemaining << L"│Items remaining: " << m_numLeft << L" (468 MB)";
		wItemsRemaining << L"│Items remaining: " << m_numLeft << wSize.str();
		num = wItemsRemaining.str().length();
		for (int i = num; i < 51; ++i)
		{
			wItemsRemaining << L" ";
		}
		wItemsRemaining << L"│";

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 10, L"┌──────────────────────────────────────────────────┐", FG_BLACK | BG_CYAN);
		//		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 11, L"│31% complete                                      │", FG_BLACK | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 11, wPercentComplete.str().c_str(), FG_BLACK | BG_CYAN);

		//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 12, L"│▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│", FG_BLACK | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 12, wBricks.str().c_str(), FG_BLACK | BG_CYAN);
		//DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 13, L"│Copying 4 items from 0new to 0new│", FG_RED | BG_CYAN);
	//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 13, L"│Copying 4 items from research_new2 to research_new2│", FG_RED | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 13, wItems.str().c_str(), FG_BLACK | BG_CYAN);
		//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 13, L"│Copying 4 items from:                             │", FG_BLACK | BG_CYAN);
		//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 14, L"│research_new2_123456… to research_new2_123456…    │", FG_BLACK | BG_CYAN);
		//    DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 13, 14, L" research_new2_123456… to research_new2_123456…", FG_DARK_BLUE | BG_CYAN);

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 14, wFrom.str().c_str(), FG_BLACK | BG_CYAN);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 13, 14, wFrom2.str().c_str(), FG_DARK_BLUE | BG_CYAN);

		//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 15, L"│Name: fns.m4a                                     │", FG_BLACK | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 15, wName.str().c_str(), FG_BLACK | BG_CYAN);
		//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 16, L"│Time remaining: 10 seconds                        │", FG_BLACK | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 16, wTime.str().c_str(), FG_BLACK | BG_CYAN);
		//    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 17, L"│Items remaining: 3 (469 MB)                       │", FG_BLACK | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 17, wItemsRemaining.str().c_str(), FG_BLACK | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 13, 18, L"└──────────────────────────────────────────────────┘", FG_BLACK | BG_CYAN);
		/*
		┌────────────────────┐
		│31% complete

			▓▓▓▓▓▓░░░░░░░░░░░░░░░░░
			│Copying 4 items from 0new to 0new
			Name: fns.m4a
			Time remaining : 10 seconds
			Items remaining : 3 (469 MB)
			└────────────────────┘
			*/
			//…
			//https://www.fileformat.info/info/charset/UTF-8/list.htm
	   }
	}

	void Copy::KeyEvent(WCHAR ch)
	{
		if (ch == '\r' || ch == '\n')
		{
      if (m_selectStep && m_typed==L"")
      {
        m_typed = L"*.*";
        m_selectStep = false;
        m_toStep = true;
      }
	  else if (m_toStep)
	  {
		  m_destinationFolder = m_typed2;
		  m_toStep = false;
		  if (!std::experimental::filesystem::exists(m_destinationFolder))
		  {
			  m_createDirStep = true;
		  }
	  }
		}
		else if (ch == '\b')
		{
      if (m_selectStep)
      {
        if (m_typed.length() > 0)
        {
          m_typed = m_typed.substr(0, m_typed.length() - 1);
        }
      }
      else if (m_toStep)
      {
        if (m_typed2.length() > 0)
        {
          m_typed2 = m_typed2.substr(0, m_typed2.length() - 1);
        }
      }
		}
    else if (m_toStep)
    {
      m_typed2 += ch;
    }
	else if (m_createDirStep && ch == 'y')
	{
		std::wstring createPath = m_destinationFolder;
		_mkdir(StrUtil::ws2s(createPath).c_str());
		//https://stackoverflow.com/questions/30937227/create-directory-in-c

		m_createDirStep = false;
		m_copyStep = true;
		StartCopy();
	}
	else if (m_createDirStep && ch == 'n')
	{
		m_escPressed = true;
		m_show = false;
		m_activated = false;
		m_showAvail = false;
		m_timePressed.clear();
	}
    else
		{
			m_typed += ch;
		}
	}

  void Copy::ThreadFn()
  {
    if (m_exitThread)
      return;

    while (!m_exitThread)
    {
      if (m_show)
      {
        if (m_threadReadyToCopy)
        {
          long long initialMillisecSinceEpoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
          m_calculating = true;
          m_numLeft = m_copyItems.size();
          m_numItems = m_copyItems.size();
          long long bytesLeft = 0;
          for (int i = 0; i < m_copyItems.size(); ++i)
          {
            bytesLeft += m_copyItems[i].SizeInBytes;
          }
          m_bytesLeft = bytesLeft;
          for (int i = 0; i < m_copyItems.size(); ++i)
          {
            m_currentName = m_copyItems[i].Name;
            m_copyItems[i].DoCopy();

            long long currentMillisecSinceEpoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            long long millisecSoFar = currentMillisecSinceEpoch - initialMillisecSinceEpoch;

            double v1 = (double)i + 1.0;
            double v2 = (double)m_copyItems.size();
            double v3 = v1 / v2;
            double v4 = 100.0;
            m_percent = v3 * v4;
//            m_percent = ((double)i+1.0 / (double)m_copyItems.size()) *100.0;

            long long millisecPerItem = millisecSoFar / (i + 1);
            m_numLeft = m_copyItems.size() - i - 1;
            long long millisecLeft = millisecPerItem * m_numLeft;
            m_timeSecondsLeft = ((double)millisecLeft) / 1000.0;

            m_bytesLeft -= m_copyItems[i].SizeInBytes;

            m_calculating = false;
          }
          m_copyItems.clear();
          m_threadReadyToCopy = false;

		  //m_escPressed = true;
		  //m_show = false;
		  //m_activated = false;
		  //m_showAvail = false;
		  //m_timePressed.clear();
        }
        //while (m_percent < 100.0)
        //{
        //  if (m_exitThread)
        //    return;
        //  m_percent += 1.0;
        //  Sleep(500);
        //}
      }
    }
  }

  void Copy::StartCopy()
  {
    m_from = L"research_new2_12345678";
    m_to = L"research_new2_12345678";
    m_percent = 0.0;

    for (int i = 0; i < 10; ++i)
    {
      m_copyItems.push_back(CopyItem());
    }
    for (int i = 0; i < 10; ++i)
    {
      std::wstringstream buf;
      buf << L"the_file_" << i;
      m_copyItems[i].Name = buf.str();
      m_copyItems[i].SizeInBytes = 100000000 * (10 - i);
    }
    m_threadReadyToCopy = true;
  }

	void Copy::CheckKeys(DirObject* dirObject, bool filesScreen)
	{
		m_dirObject = dirObject;
		if (!m_checkingForKeys || !filesScreen)
			return;

		if (!m_show)
		{
      bool control = (0x8000 & GetAsyncKeyState((unsigned char)(VK_CONTROL))) != 0;
			if ((0x8000 & GetAsyncKeyState((unsigned char)(0x43/*C*/))) != 0)
			{
				m_lPressed = true;
				m_show = true;
				m_activated = true;
				m_waitForKeyLetGo = -1;
				m_typed = L"";
        m_typed2 = L"";
        if (control)
        {
          m_selectStep = true;
        }
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

		bool enterPressed = (0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0;
		if (enterPressed && m_copyStep && m_percent>=100.0)
		{
			m_copyStep = false;

			m_escPressed = true;
			m_show = false;
			m_activated = false;
			m_showAvail = false;
			m_timePressed.clear();
			return;
		}

//    if (m_percent >= 100.0)
    {
//      if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0 || enterPressed)
      if (!m_copyStep && (0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0)
      {
        m_escPressed = true;
        m_show = false;
        m_activated = false;
        m_showAvail = false;
        m_timePressed.clear();
      }
      else
      {
        m_escPressed = false;
      }
    }


		m_lastShown = m_show;
	}
}
