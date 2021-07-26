#include "stdafx.h"
#include "OpenWith.h"
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
extern CHAR_INFO* m_bufScreen;
void RunProcess(std::wstring const& filePath, std::wstring const& arg);

#define INFO_BUFFER_SIZE 32767

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


	extern int nScreenWidth = 80;			// Console Screen Size X (columns)
	extern int nScreenHeight = 50;			// Console Screen Size Y (rows)

	Item::Item(std::wstring const& name, std::wstring const& path, std::wstring const& args) : Name(name), Path(path), Args(args)
	{
	}

	OpenWith::OpenWith() : m_activated(false), m_selected(0), m_downPressed(false), m_upPressed(false), m_escPressed(false), m_enterPessed(false)
	{
		TCHAR szPath[MAX_PATH];

		GetModuleFileName(NULL, szPath, MAX_PATH);

		std::wstringstream ss;
		ss << szPath;
		std::wstring p = ss.str();
		StrUtil::Replace(p, L"SimpleXTree.exe", L"OpenWith.txt");
		std::string srr = StrUtil::ws2s(p);

		//std::wstringstream buf;
		//buf << szPath << "\\OpenWith.txt";

		//std::string srr = StrUtil::ws2s(buf.str());
		//srr.erase(std::remove(srr.begin(), srr.end(), '\"'), srr.end());



		std::wifstream ifile;
		ifile.open(srr);
		if (ifile) {
//			std::cout << "file exists";

			OutputDebugStringA(srr.c_str());

			if (ifile.is_open())
			{
				//cplusplus.com/doc/tutorial/files/
				//https://stackoverflow.com/questions/33806620/use-wstring-get-line-read-file
				std::wstring line;
				while (std::getline(ifile, line))
				{
					//https://www.geeksforgeeks.org/tokenizing-a-string-cpp/

					// Vector of string to save tokens
					std::vector<std::wstring> tokens;

					// stringstream class check1
					std::wstringstream check1(line);

					std::wstring intermediate;

					// Tokenizing w.r.t. space ' '
					while (std::getline(check1, intermediate, L'|'))
					{
						tokens.push_back(intermediate);
					}

					Items.push_back(Item(tokens[0], tokens[1], tokens[2]));

					int c = 1;
					c++;


				}
				ifile.close();
			}
			//ss.str(L"");
		}
		else {
//			std::cout << "file doesn't exist";
			//just default to notepad
			Items.push_back(Item(L"Notepad", L"%windir%\\system32\\notepad.exe", L"%1"));
			int a = 1;
			a++;
		}
		


		//if ()
		//Items.push_back(Item(L"Notepad", L"%windir%\\system32\\notepad.exe", L"%1"));
		//Items.push_back(Item(L"Notepad++", L"C:\\Program Files\\Notepad++\\notepad++.exe", L"%1"));
		//Items.push_back(Item(L"Frhed", L"d:\\Program Files (x86)\\Frhed\\frhed.exe", L"%1"));
		//Items.push_back(Item(L"Paint", L"%windir%\\system32\\mspaint.exe", L"%1"));
	}

	OpenWith::~OpenWith()
	{
	}

	void OpenWith::Activate()
	{
		m_activated = true;
		m_selected = 0;
	}

	void OpenWith::Toggle()
	{
		m_activated = !m_activated;
		m_selected = 0;
	}

	void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F)
	{
		for (size_t i = 0; i < c.size(); i++)
		{
			m_bufScreen[y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
			m_bufScreen[y * nScreenWidth + x + i].Attributes = col;
		}
	}

	void OpenWith::Render()
	{
		if (!m_activated)
			return;

		int maxName = 0;
		for (int i = 0; i < Items.size(); ++i)
		{
			if (Items[i].Name.length() > maxName)
			{
				maxName = Items[i].Name.length()+1;
			}
		}

		int startX = 3;
		int startY = 3;

		L"┌";
		L"─";
		L"┐";
		L"│";
		L"└";
		L"┘";

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX, startY, L"┌", FG_DARK_BLUE | BG_CYAN);
		for (int i = 0; i < maxName; ++i)
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + i + 1, startY, L"─", FG_DARK_BLUE | BG_CYAN);
		}
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + maxName, startY, L"┐", FG_DARK_BLUE | BG_CYAN);

		startY++;
		for (int i = 0; i < Items.size(); ++i)
		{
			//			std::wstringstream buf;
			//			buf << L"│" << Items[i].Name;
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX, startY + i, L"│", FG_DARK_BLUE | BG_CYAN);

			if (i == m_selected)
			{
				DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + 1, startY + i, Items[i].Name, FG_BLACK | BG_GREY);
				for (int j = Items[i].Name.length() + 1; j < maxName; ++j)
				{
					DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + j, startY + i, L" ", FG_DARK_GREY | BG_GREY);
				}
			}
			else
			{
				DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + 1, startY + i, Items[i].Name, FG_DARK_BLUE | BG_CYAN);
				for (int j = Items[i].Name.length() + 1; j < maxName; ++j)
				{
					DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + j, startY + i, L" ", FG_DARK_BLUE | BG_CYAN);
				}
			}

			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + maxName, startY + i, L"│", FG_DARK_BLUE | BG_CYAN);

		}
		startY += Items.size();

		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX, startY, L"└", FG_DARK_BLUE | BG_CYAN);
		for (int i = 0; i < maxName; ++i)
		{
			DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + i + 1, startY, L"─", FG_DARK_BLUE | BG_CYAN);
		}
		DrawString(m_bufScreen, nScreenWidth, nScreenHeight, startX + maxName, startY, L"┘", FG_DARK_BLUE | BG_CYAN);


	}

	void OpenWith::CheckKeys()
	{
		if (!m_activated)
			return;

		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_DOWN))) != 0)
		{
			if (!m_downPressed)
			{
				m_downPressed = true;
				m_selected++;
				if (m_selected >= Items.size())
				{
					m_selected = Items.size() - 1;
				}
			}
		}
		else
		{
			m_downPressed = false;
		}

		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_UP))) != 0)
		{
			if (!m_upPressed)
			{
				m_upPressed = true;
				m_selected--;
				if (m_selected <= 0)
				{
					m_selected = 0;
				}
			}
		}
		else
		{
			m_upPressed = false;
		}


		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0)
		{
			if (!m_escPressed)
			{
				m_escPressed = true;
				m_activated = false;
			}
		}
		else
		{
			m_escPressed = false;
		}


		if ((0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0)
		{
			if (!m_enterPessed)
			{
				m_enterPessed = true;

				Run(Items[m_selected]);
			}
		}
		else
		{
			m_enterPessed = false;
		}

	}



	void OpenWith::Run(Item const& item)
	{
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE);
		std::wstring winDir = infoBuf;

		std::wstring args = item.Args;
		StrUtil::ReplaceAll(args, L"%1", m_path);

		std::wstring exePath = item.Path;
		StrUtil::ReplaceAll(exePath, std::wstring(L"%windir%"), winDir);

		RunProcess(exePath, args);

		m_activated = false;
		int a = 1;
		a++;
	}
}
