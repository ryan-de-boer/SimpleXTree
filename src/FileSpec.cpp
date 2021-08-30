#include "stdafx.h"
#include "FileSpec.h"
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
#include <experimental/filesystem>
#include "FreeDiskSpace.h"
#include <Ktmw32.h>
#include <shlwapi.h>
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

	FileSpec::FileSpec() : m_dirObject(NULL), m_selected(NULL), m_initial(NULL), m_activated(false),
		m_checkingForKeys(true), m_lPressed(0), m_escPressed(false), m_show(false), m_lastShown(false), m_timeSet(false),
		m_timePassed(0), m_renderCursor(true), m_waitForKeyLetGo(-1), m_showAvail(false), m_yesNo(false),
		m_identical(false), m_unique(false), m_newer(false), m_older(false), m_binary(false), m_subs(false), m_useRecycleBin(true), m_typed(L""), m_typedInProg(L"")
	{
	}

	FileSpec::~FileSpec()
	{
	}

	void FileSpec::Activate()
	{
		m_activated = true;
	}

	void FileSpec::Toggle()
	{
		m_activated = !m_activated;
	}

	void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F);


	void FileSpec::Render(int start)
	{
		if (!m_activated || !m_show)
			return;


		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"Filespec:                                                                     ", FG_GREY | BG_BLACK);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"                                                                              ", FG_GREY | BG_BLACK);
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"Enter file specification                 ↑ history  ◄─┘ ok  F1 help  ESC cancel", FG_GREY | BG_BLACK);
		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"                                         ↑          ◄─┘     F1       ESC       ", FG_CYAN | BG_BLACK);

		DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, std::wstring(L"Filespec: ").length(), start, m_typedInProg, FG_CYAN | BG_BLACK);

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
		HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

		std::wstring beforeCursor = std::wstring(L"Filespec: ") + m_typedInProg;
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



	void FileSpec::CheckKeys(DirObject* dirObject)
	{
		m_dirObject = dirObject;
		if (!m_checkingForKeys || m_otherActive)
			return;

		if (!m_show)
		{
			if ((0x8000 & GetAsyncKeyState((unsigned char)(0x46/*F*/))) != 0)
			{
				m_typedInProg = L"";
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
		else if ((0x8000 & GetAsyncKeyState((unsigned char)(0x46/*F*/))) == 0)
		{
			m_lPressed = false;
		}

		if (m_lPressed)
		{

		}

//		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0)
//		{
//			if (m_typedInProg != L"")
//			{
//				m_typed = m_typedInProg;
//
//				DirObject* root = m_dirObject;
//				while (root->Parent != NULL)
//				{
//					root = root->Parent;
//				}
//				for (int i = 0; i < root->AllPaths.size(); ++i)
//				{
//					if (root->AllPaths[i]->IsExpanded)
//					{
//						root->AllPaths[i]->ApplyFileSpec(m_typed);
//					}
//				}
//				root->UntagAndChildren(); //Tags refer to wrong index so clear them.
//			
//			}
//			
//
////			m_dirObject->ApplyFileSpec(m_typed);
//
//				m_escPressed = true;
//				m_show = false;
//				m_activated = false;
//				m_showAvail = false;
//				m_yesNo = false;
//
//		}
//		else
//		{
//		}

		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0)
		{
			m_escPressed = true;
			m_show = false;
			m_activated = false;
			m_showAvail = false;
			m_yesNo = false;
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

	void FileSpec::KeyEvent(WCHAR ch)
	{
		if (ch == '\r' || ch == '\n' || ch == '\x1b')
		{

		}
		else if (ch == '\b')
		{
			if (m_typedInProg.length() > 0)
			{
				m_typedInProg = m_typedInProg.substr(0, m_typedInProg.length() - 1);
			}
		}
		else
		{
			m_typedInProg += ch;
		}
	}

	void FileSpec::VK(DWORD vk)
	{
		if (vk == VK_RETURN)
		{
//			m_useRecycleBin = !m_useRecycleBin;

//			if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0)
			{
				if (m_typedInProg != L"")
				{
					m_typed = m_typedInProg;

					DirObject* root = m_dirObject;
					while (root->Parent != NULL)
					{
						root = root->Parent;
					}
					for (int i = 0; i < root->AllPaths.size(); ++i)
					{
						if (root->AllPaths[i]->IsExpanded)
						{
							root->AllPaths[i]->ApplyFileSpec(m_typed);
						}
					}
					root->UntagAndChildren(); //Tags refer to wrong index so clear them.

				}


				//			m_dirObject->ApplyFileSpec(m_typed);

				m_escPressed = true;
				m_show = false;
//				m_activated = false;
				m_showAvail = false;
				m_yesNo = false;

			}


		}
	}

	void FileSpec::SelectDir(DirObject* dirObject)
	{
		m_selected = dirObject;
		m_typed = dirObject->PathW();
		m_activated = true;
		m_show = true;
	}
}
