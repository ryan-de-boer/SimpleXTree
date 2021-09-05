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

	Copy::Copy() : m_dirObject(NULL), m_activated(false), m_checkingForKeys(true), m_lPressed(0), m_escPressed(false), m_show(false), m_lastShown(false), m_timeSet(false), m_timePassed(0), m_renderCursor(true), m_waitForKeyLetGo(-1), m_showAvail(false)
	{
	}

	Copy::~Copy()
	{
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

//		std::experimental::filesystem::copy()
		//thread https://www.bogotobogo.com/cplusplus/multithreaded4_cplusplus11.php

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 10, L"┌─────────────────────────────────┐", FG_RED | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 11, L"│31% complete                     │", FG_RED | BG_CYAN);

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 12, L"│▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░│", FG_RED | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 13, L"│Copying 4 items from 0new to 0new│", FG_RED | BG_CYAN);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 10, 13, L"                      0new    0new│", FG_BLUE | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 14, L"│Name: fns.m4a                    │", FG_RED | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 15, L"│Time remaining: 10 seconds       │", FG_RED | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 16, L"│Items remaining: 3 (469 MB)      │", FG_RED | BG_CYAN);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 10, 17, L"└─────────────────────────────────┘", FG_RED | BG_CYAN);
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
	}

	void Copy::KeyEvent(WCHAR ch)
	{
		if (ch == '\r' || ch == '\n')
		{

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


	void Copy::CheckKeys(DirObject* dirObject, bool filesScreen)
	{
		m_dirObject = dirObject;
		if (!m_checkingForKeys || !filesScreen)
			return;

		if (!m_show)
		{
			if ((0x8000 & GetAsyncKeyState((unsigned char)(0x43/*C*/))) != 0)
			{
				m_lPressed = true;
				m_show = true;
				m_activated = true;
				m_waitForKeyLetGo = -1;
				m_typed = L"";
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
		if (enterPressed && m_typed.length()>0)
		{

			return;
		}

		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0 || (enterPressed && m_typed.length() == 0))
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

		m_lastShown = m_show;
	}
}
