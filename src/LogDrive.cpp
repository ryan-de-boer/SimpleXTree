#include "stdafx.h"
#include "LogDrive.h"
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

	LogDrive::LogDrive() : m_activated(false), m_checkingForKeys(true), m_lPressed(0), m_escPressed(false), m_show(false), m_lastShown(false), m_timeSet(false), m_timePassed(0), m_renderCursor(true), m_waitForKeyLetGo(-1)
	{
	}

	LogDrive::~LogDrive()
	{
	}

	void LogDrive::Activate()
	{
		m_activated = true;
	}

	void LogDrive::Toggle()
	{
		m_activated = !m_activated;
	}

	void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);

	void LogDrive::Render(int start)
	{
		if (!m_activated || !m_show)
			return;
		/*
		LOG DISK DRIVE (Logged drives: CD)
		↑ list
		C D E                                                      F1 help  ESC cancel
		*/

		//https://stackoverflow.com/questions/2284110/get-a-list-of-the-available-drives-and-their-sizes/36428737
		std::wstringstream drive;
		drive << L" ";
		DWORD d = GetLogicalDrives();
		int i;
		TCHAR Drive[] = _T("A:\\");
		for (i = 0; i<26; i++)
		{
			if (d & (1 << i))
			{
				std::wstringstream test;
				test << (TCHAR)(_T('A') + i) << ":\\";
				if (IsDirectory(test.str()))
				{
					Drive[0] = _T('A') + i;
					drive << (TCHAR)(_T('A') + i) << L" ";
				}
			}
		}

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"LOG DISK DRIVE                                                                  ", FG_GREY | BG_BLACK);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                                                                         ↑ list", FG_GREY | BG_BLACK);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                                                                         ↑", FG_CYAN | BG_BLACK);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                                    ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                                    ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);

		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, drive.str(), FG_CYAN | BG_BLACK);


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

		if (m_renderCursor)
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"LOG DISK DRIVE ").length(), start, L" ", FG_WHITE | BG_WHITE);
		}
		else
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"LOG DISK DRIVE ").length(), start, L" ", FG_BLACK | BG_BLACK);
		}
	}

	void LogDrive::CheckKeys()
	{
		if (!m_checkingForKeys)
			return;

		if (m_waitForKeyLetGo != -1)
		{
			if ((0x8000 & GetAsyncKeyState((unsigned char)(m_waitForKeyLetGo))) != 0)
			{
				//waiting for key to let go before deactivated
			}
			else// if (m_lastShown && !m_show)
			{
				m_activated = false;
				m_lastShown = false;
				m_show = false;
				m_waitForKeyLetGo = -1;
				return;
			}
			return;
		}

		if (!m_show)
		{
			if ((0x8000 & GetAsyncKeyState((unsigned char)(0x4C/*L*/))) != 0)
//			if ((0x8000 & GetAsyncKeyState((unsigned char)(0x43/*C*/))) != 0)
			{
				m_lPressed = true;
				m_show = true;
				m_activated = true;
				m_waitForKeyLetGo = -1;
			}
			else
			{
				m_lPressed = false;
				return;
			}
		}
//		else if ((0x8000 & GetAsyncKeyState((unsigned char)(0x43/*C*/))) == 0)
		else if ((0x8000 & GetAsyncKeyState((unsigned char)(0x4C/*L*/))) == 0)
		{
			m_lPressed = false;
		}

		if (m_lPressed)
		{

		}


		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0)
		{
			m_escPressed = true;
			m_show = false;
			m_activated = false;
		}
		else
		{
			m_escPressed = false;
		}


		if (m_show && !m_lPressed)
		{
			for (int i = 0x41; i < 0x5A; ++i)
			{
				if ((0x8000 & GetAsyncKeyState((unsigned char)(i))) != 0 && CharIsPressed(i))
				{
					std::string key = m_writtenKey;
					std::transform(key.begin(), key.end(), key.begin(),
						[](unsigned char c) { return std::toupper(c); });
					std::wstringstream buf;
					buf << StrUtil::s2ws(key) << ":\\";

					if (IsDirectory(buf.str()))
					{
						Log(buf.str());
					}
					m_show = false;
					//m_activated = false;
					m_waitForKeyLetGo = i;
				}
			}
		}


		m_lastShown = m_show;

	}
}
