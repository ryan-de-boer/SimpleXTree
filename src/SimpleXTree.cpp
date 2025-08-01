// OneLoneCoder_Tetris.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <locale> 
#include <codecvt>

//#include <fcntl.h>
//#include <io.h>
//#include <stdio.h>

#include <chrono>  // for high_resolution_clock
#include "Stopwatch.h"
#include "RandomAccessBufferedReader.h"
#include "Stopwatch.h"
#include <algorithm>
#include <iterator>

#include <string>
#include <iostream>
#include <filesystem>

#include <algorithm>
#include <cctype>
#include <string>
#include <iomanip>      // std::setprecision

#include "StrUtil.h"
#include "DateTime.h"
#include "FreeDiskSpace.h"
#include "Search.h"
/*
OneLoneCoder.com - Command Line Tetris
"Put Your Money Where Your Mouth Is" - @Javidx9

License
~~~~~~~
Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.
Original works located at:
https://www.github.com/onelonecoder
https://www.onelonecoder.com
https://www.youtube.com/javidx9

GNU GPLv3
https://github.com/OneLoneCoder/videos/blob/master/LICENSE

From Javidx9 :)
~~~~~~~~~~~~~~~
Hello! Ultimately I don't care what you use this for. It's intended to be
educational, and perhaps to the oddly minded - a little bit of fun.
Please hack this, change it and use it in any way you see fit. You acknowledge
that I am not responsible for anything bad that happens as a result of
your actions. However this code is protected by GNU GPLv3, see the license in the
github repo. This means you must attribute me if you use it. You can view this
license here: https://github.com/OneLoneCoder/videos/blob/master/LICENSE
Cheers!

Background
~~~~~~~~~~
I made a video "8-Bits of advice for new programmers" (https://youtu.be/vVRCJ52g5m4)
and suggested that building a tetris clone instead of Dark Sould IV might be a better
approach to learning to code. Tetris is nice as it makes you think about algorithms.

Controls are Arrow keys Left, Right & Down. Use Z to rotate the piece.
You score 25pts per tetronimo, and 2^(number of lines)*100 when you get lines.

Future Modifications
~~~~~~~~~~~~~~~~~~~~
1) Show next block and line counter

Author
~~~~~~
Twitter: @javidx9
Blog: www.onelonecoder.com

Video:
~~~~~~
https://youtu.be/8OK8_tHeCIA

Last Updated: 30/03/2017
*/

#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#include "DirObject.h"
#include "OpenWith.h"
#include "Bookmarks.h"
#include "LogDrive.h"
#include "Avail.h"
#include "Make.h"
#include "Compare.h"
#include "Delete.h"
#include "FileSpec.h"
#include "Copy.h"
using namespace SimpleXTree;

#include <stdio.h>
#include <Windows.h>
#include <sstream>

int nScreenWidth = 80;			// Console Screen Size X (columns)
int nScreenHeight = 50;			// Console Screen Size Y (rows)
wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;
SimpleXTree::Search searchObj;

std::streampos thestart = 0;
std::streampos theend = 0;
std::streampos rtheend = 0;
std::streampos theSearchPos = 0;
std::streampos theAfterSearchPos = 0;
bool endOfSearch = false;
void ReadFile();


BOOL __stdcall InstallHook(void);

int Rotate(int px, int py, int r)
{
  int pi = 0;
  switch (r % 4)
  {
  case 0: // 0 degrees			// 0  1  2  3
    pi = py * 4 + px;			// 4  5  6  7
    break;						// 8  9 10 11
                      //12 13 14 15

  case 1: // 90 degrees			//12  8  4  0
    pi = 12 + py - (px * 4);	//13  9  5  1
    break;						//14 10  6  2
                      //15 11  7  3

  case 2: // 180 degrees			//15 14 13 12
    pi = 15 - (py * 4) - px;	//11 10  9  8
    break;						// 7  6  5  4
                      // 3  2  1  0

  case 3: // 270 degrees			// 3  7 11 15
    pi = 3 - py + (px * 4);		// 2  6 10 14
    break;						// 1  5  9 13
  }								// 0  4  8 12

  return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
  // All Field cells >0 are occupied
  for (int px = 0; px < 4; px++)
    for (int py = 0; py < 4; py++)
    {
      // Get index into piece
      int pi = Rotate(px, py, nRotation);

      // Get index into field
      int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

      // Check that test is in bounds. Note out of bounds does
      // not necessarily mean a fail, as the long vertical piece
      // can have cells that lie outside the boundary, so we'll
      // just ignore them
      if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
      {
        if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
        {
          // In Bounds so do collision check
          if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
            return false; // fail on first hit
        }
      }
    }

  return true;
}

//https://stackoverflow.com/questions/12900713/reducing-console-size
void SetWindow(int Width, int Height)
{
  _COORD coord;
  coord.X = Width;
  coord.Y = Height;

  _SMALL_RECT Rect;
  Rect.Top = 0;
  Rect.Left = 0;
  Rect.Bottom = Height - 1;
  Rect.Right = Width - 1;

  HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
  SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
  SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
}

//https://stackoverflow.com/questions/56911976/set-console-screen-size-and-screen-buffer-by-rows-and-columns
void resizeConBufAndWindow(HANDLE hConsole, SHORT xSize, SHORT ySize)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* hold current console buffer info */
	BOOL bSuccess;
	SMALL_RECT srWindowRect; /* hold the new console size */
	COORD coordScreen;

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
//	PERR(bSuccess, L"GetConsoleScreenBufferInfo");
	/* get the largest size we can size the console window to */
	coordScreen = GetLargestConsoleWindowSize(hConsole);
//	PERR(coordScreen.X | coordScreen.Y, L"GetLargestConsoleWindowSize");
	/* define the new console window size and scroll position */
	srWindowRect.Right = (SHORT)(min(xSize, coordScreen.X) - 1);
	srWindowRect.Bottom = (SHORT)(min(ySize, coordScreen.Y) - 1);
	srWindowRect.Left = srWindowRect.Top = (SHORT)0;
	/* define the new console buffer size */
	coordScreen.X = xSize;
	coordScreen.Y = ySize;
	/* if the current buffer is larger than what we want, resize the */
	/* console window first, then the buffer */
	if ((DWORD)csbi.dwSize.X * csbi.dwSize.Y > (DWORD)xSize * ySize)
	{
		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect);
//		PERR(bSuccess, L"SetConsoleWindowInfo");
		bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen);
//		PERR(bSuccess, L"SetConsoleScreenBufferSize");
	}
	/* if the current buffer is smaller than what we want, resize the */
	/* buffer first, then the console window */
	if ((DWORD)csbi.dwSize.X * csbi.dwSize.Y < (DWORD)xSize * ySize)
	{
		bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen);
    DWORD er = ::GetLastError();
//		PERR(bSuccess, L"SetConsoleScreenBufferSize");
		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect);
    DWORD er2 = ::GetLastError();
//		PERR(bSuccess, L"SetConsoleWindowInfo");
	}
	/* if the current buffer *is* the size we want, don't do anything! */
	return;
}

std::wstring m_lastC;
int m_lastI;

void DrawString(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F)
{
  m_lastC = c;
	for (size_t i = 0; i < c.size(); i++)
	{
		m_bufScreen[y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
		m_bufScreen[y * nScreenWidth + x + i].Attributes = col;
	}
}

void DrawStringSkipSpace(CHAR_INFO *m_bufScreen, int nScreenWidth, int nScreenHeight, int x, int y, std::wstring c, short col = 0x000F)
{
	for (size_t i = 0; i < c.size(); i++)
	{
		if (c[i] != ' ')
		{
			m_bufScreen[y * nScreenWidth + x + i].Char.UnicodeChar = c[i];
			m_bufScreen[y * nScreenWidth + x + i].Attributes = col;
		}
	}
}

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

void print(std::wstring const& str)
{
  OutputDebugString(str.c_str());
}

COORD click_coord;
bool leftButtonPress = false;
bool isLeftDown = false;

char const *up_down[] = { "up", "down" };

extern int g_itemsp;
extern int g_itemsleft;
extern int g_leftSpace;
extern int g_rightSpace;
bool g_mouseDownOnIt = false;
SHORT g_prevMx = 0;

bool scrolledUp = false;

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
  print(L"Mouse event: ");

  switch (mer.dwEventFlags)
  {
  case 0:

    if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
      print(L"left button press \n");
      leftButtonPress = true;
      isLeftDown = true;
      click_coord.X = mer.dwMousePosition.X;
      click_coord.Y = mer.dwMousePosition.Y;

      std::wstringstream s;
      s << click_coord.X << "," << click_coord.Y << std::endl;
      print(s.str().c_str());

      if (click_coord.Y == 46)
      {
        if (click_coord.X < g_leftSpace)
        {
          //PageUp
          if (thestart < std::streampos(688 + 16))
          {
            thestart = 0;
          }
          else
          {
            thestart -= std::streampos(688 + 16);
          }
          ReadFile();
        }
        else if (click_coord.X <= g_leftSpace+ g_itemsp)
        {
          // on handle
          int a = 1;
          a++;
          g_mouseDownOnIt = true;
        }
        else
        {
          //PageDown
          for (int i = 0; i < 44; ++i) //44 lines in a page
          {
            bool inc = true;
            if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
              inc = false;
            if (inc)
            {
              thestart += 16;
            }
          }

          ReadFile();
        }
      }
    }
    else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
    {
      print(L"right button press \n");
    }
    else if (mer.dwButtonState == 0 && isLeftDown)
    {
      isLeftDown = false;
      print(L"left button released \n");
      g_mouseDownOnIt = false;
      g_prevMx = 0;
      ReadFile();
    }
    else
    {
      print(L"button press\n");

      std::wstringstream s;
      s << "bs:" << mer.dwButtonState << std::endl;
      print(s.str().c_str());
    }
    break;
  case DOUBLE_CLICK:
    print(L"double click\n");
    break;
  case MOUSE_HWHEELED:
    print(L"horizontal mouse wheel\n");
    break;
  case MOUSE_MOVED:
    print(L"mouse moved\n");
    if (g_mouseDownOnIt)
    {
      SHORT mx = mer.dwMousePosition.X;
      SHORT my = mer.dwMousePosition.Y;
      // map mx to thestart?

      std::wstringstream s;
      s << "mx:" << mx << std::endl;
      print(s.str().c_str());

      bool g = false;
      //if (g_prevMx >= mx)
      //{
      //  g = true;
      //}
      //g_prevMx = mx;

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
      int leftSpace = mx+1;
      if (mx == 0 || g)
        leftSpace = 0;
      int itemsleft = 80 - itemsp;
      double percents = ((double)leftSpace - 0.5) / (double)itemsleft;
      if (itemsleft == 0)
      {
        percents = 1.0;
      }
      if (leftSpace == 0)
      {
        percents = 0.0;
      }
      thestart = percents*(double)theend;
      thestart = thestart - thestart % 16; //issue non 16 aligned
      //if (thestart + std::streampos(688) >= theend)
      //{
      //  thestart = theend - std::streampos(688);
      //}
      if (thestart + std::streampos(703 + 1) >= theend)
      {
        thestart = theend - std::streampos(703 + 1);
        thestart = thestart - thestart % 16 + std::streampos(16); //issue non 16 aligned
      }

      //double percents = (double)(thestart) / (double)theend;
      //if (percents >= 1.0)
      //{
      //  percents = 1.0;
      //}
      //double thepercent = (double)(0 + std::streampos(703 + 1)) / (double)theend;
      //int itemsp = (int)(thepercent * 80.0);
      //itemsp *= 2;
      //if (itemsp  >80)
      //{
      //  itemsp = 80;
      //}
      //if (itemsp < 1)
      //{
      //  itemsp = 1;
      //}
      //int itemsleft = 80 - itemsp;

      //int leftSpace = (int)((percents*(double)itemsleft) + 0.5);
      //int rightSpace = (int)(((1.0 - percents)*(double)itemsleft) + 0.5);

      //warning try not to do too much io when mousemove, currently we are buffering io
      ReadFile();

      int a = 1;
      a++;
    }

    break;
  case MOUSE_WHEELED:
  {
  print(L"vertical mouse wheel\n");
  //https://github.com/murarth/mortal/blob/master/src/windows/terminal.rs
  // The high word of `dwButtonState` indicates wheel direction
  //int direction = (int)(mer.dwButtonState >> 16);
  //if (direction > 0)
  //{
  //  print(L"UP vertical mouse wheel\n");
  //}
  //else
  //{
  //  print(L"DOWN vertical mouse wheel\n");
  //}

  //if (HIWORD(mer.dwButtonState) > 0)
  //{
  //  print(L"UP vertical mouse wheel\n");
  //}
  //else
  //{
  //  print(L"DOWN vertical mouse wheel\n");
  //}

  //https://stackoverflow.com/questions/53466611/c-identifying-x-buttons-scroll-wheel-directions
  bool down = static_cast<std::make_signed_t<WORD>>(HIWORD(mer.dwButtonState)) < 0;
  std::cout << "Mouse wheel scrolled " << up_down[down] << '\n';

  if (!down)
  {
    scrolledUp = true;
    int a = 1;
    a++;
    if (searchObj.IsActivated())
    {
//      searchObj.VK(VK_UP);
//      searchObj.VK(VK_UP);
    }
    int offset = 16;
    if (searchObj.m_renderDump)
    {
      offset = 64;
    }

    if (thestart < offset)
    {
      thestart = 0;
    }
    else
    {
      thestart -= offset;
    }
    ReadFile();

    if (thestart < offset)
    {
      thestart = 0;
    }
    else
    {
      thestart -= offset;
    }
    ReadFile();

    if (thestart < offset)
    {
      thestart = 0;
    }
    else
    {
      thestart -= offset;
    }
    ReadFile();
  }
  else
  {
    int a = 1;
    a++;
    if (searchObj.IsActivated())
    {
//      searchObj.VK(VK_DOWN);
//      searchObj.VK(VK_DOWN);

      int offset1 = 16;
      int offset2 = 43*16;
      if (searchObj.m_renderDump)
      {
        offset1 = 64;
        offset2 = 43*64;
      }

      bool inc = true;
      if ((thestart + std::streampos(offset2) + std::streampos(offset1)) >= (theend))
        inc = false;

      if (inc)
      {
        thestart += offset1;
        ReadFile();
      }

      inc = true;
      if ((thestart + std::streampos(offset2) + std::streampos(offset1)) >= (theend))
        inc = false;

      if (inc)
      {
        thestart += offset1;
        ReadFile();
      }

      inc = true;
      if ((thestart + std::streampos(offset2) + std::streampos(offset1)) >= (theend))
        inc = false;

      if (inc)
      {
        thestart += offset1;
        ReadFile();
      }
    }
    
  }

  }
    break;
  default:
    print(L"unknown\n");
    scrolledUp = false;
    break;
  }
}

HWND g_createdWindow = NULL;

//https://gist.github.com/mmozeiko/5b1700c558d37cb2bb2f00328c2982f6
HWND hwnd;
bool isRunning;
WNDCLASSEX windowClass;

//LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
//{
//  if (uMsg == WM_DROPFILES) {
//    MessageBox(hwnd, "Dragged!", "Title", MB_OK | MB_ICONINFORMATION);
//  }
//  return DefSubclassProc(hwnd, uMsg, wParam, lParam);
//}

INPUT_RECORD g_ir;
WPARAM g_lastKeywParam;

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  DWORD dwTmp = 0;
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

	switch (uMsg)
	{
	case WM_CREATE:
	{
		isRunning = true;
    DragAcceptFiles(g_createdWindow, TRUE);
	}
	break;
  case WM_CHAR:

    //https://stackoverflow.com/questions/5880173/how-to-send-return-enter-character-to-the-console-using-writeconsoleinput-writec
    g_ir.EventType = KEY_EVENT;
    g_ir.Event.KeyEvent.bKeyDown = TRUE;
    g_ir.Event.KeyEvent.dwControlKeyState = 0;
    g_ir.Event.KeyEvent.uChar.AsciiChar = wParam;
    g_ir.Event.KeyEvent.uChar.UnicodeChar = wParam;
    g_ir.Event.KeyEvent.wRepeatCount = 1;
    g_ir.Event.KeyEvent.wVirtualKeyCode = g_lastKeywParam;
    g_ir.Event.KeyEvent.wVirtualScanCode = MapVirtualKey(g_lastKeywParam, MAPVK_VK_TO_VSC);
    WriteConsoleInput(hin, &g_ir, 1, &dwTmp);

    SetForegroundWindow(GetConsoleWindow());
    SetFocus(GetConsoleWindow());

    break;
  case WM_KEYDOWN:
    g_lastKeywParam = wParam;
//  case WM_KEYUP:
////    SetForegroundWindow(GetConsoleWindow());
//    SendMessage(GetConsoleWindow(), uMsg, wParam, lParam);
//    DWORD dwTmp = 0;
//    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

    ////https://stackoverflow.com/questions/5880173/how-to-send-return-enter-character-to-the-console-using-writeconsoleinput-writec
    //g_ir.EventType = KEY_EVENT;
    //g_ir.Event.KeyEvent.bKeyDown = TRUE;
    //g_ir.Event.KeyEvent.dwControlKeyState = 0;
    //g_ir.Event.KeyEvent.uChar.AsciiChar = 'A';
    //g_ir.Event.KeyEvent.uChar.UnicodeChar = 'A';
    //g_ir.Event.KeyEvent.wRepeatCount = 1;
    //g_ir.Event.KeyEvent.wVirtualKeyCode = 0x41;
    //g_ir.Event.KeyEvent.wVirtualScanCode = MapVirtualKey('A', MAPVK_VK_TO_VSC);
    //WriteConsoleInput(hin, &g_ir, 1, &dwTmp);

    //SetForegroundWindow(GetConsoleWindow());
    //SetFocus(GetConsoleWindow());

    break;
	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		isRunning = false;
		PostQuitMessage(0);
		return 0;
		break;
	case WM_DROPFILES:
	{
		HDROP drop = (HDROP)wParam;
		UINT filePathesCount = DragQueryFileW(drop, 0xFFFFFFFF, NULL, 512);//If "0xFFFFFFFF" as the second parameter: return the count of files dropped
		wchar_t* fileName = NULL;
		UINT longestFileNameLength = 0;
		for (UINT i = 0; i < filePathesCount; ++i)
		{
			//If NULL as the third parameter: return the length of the path, not counting the trailing '0'
			UINT fileNameLength = DragQueryFileW(drop, i, NULL, 512) + 1;
			if (fileNameLength > longestFileNameLength)
			{
				longestFileNameLength = fileNameLength;
				fileName = (wchar_t*)realloc(fileName, longestFileNameLength * sizeof(*fileName));
			}
			DragQueryFileW(drop, i, fileName, fileNameLength);

			std::wcout << fileName << std::endl;
		}
		free(fileName);
		DragFinish(drop);
		break;
	}

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


bool createWindow(HINSTANCE hInstance, int x, int y, int width, int height, int bpp)
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
windowClass.style = CS_HREDRAW | CS_VREDRAW;
windowClass.lpfnWndProc = wndProc;
windowClass.cbClsExtra = 0;
windowClass.cbWndExtra = 0;
windowClass.hInstance = hInstance;
windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
windowClass.hbrBackground = NULL;
windowClass.lpszMenuName = NULL;
windowClass.lpszClassName = L"DragnDropClass";
windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

if (!RegisterClassEx(&windowClass))
{
	std::cout << "Couldn't register window class" << std::endl;
	return false;
}

DWORD      dwExStyle;
DWORD      dwStyle;
dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
dwStyle = WS_OVERLAPPEDWINDOW;

RECT windowRect;
windowRect.left = x;// 0;
windowRect.right = width;
windowRect.top = y;// 0;
windowRect.bottom = height;
if (AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle) == 0)
{
	std::cout << "Couldn't set the size of the window rectangle" << std::endl;
	return false;
}

//https://social.msdn.microsoft.com/Forums/vstudio/en-US/5a756e83-959c-49c0-9b38-11d541ba670b/how-to-hide-the-window-hwnd-his-shortcut-from-the-taskbar?forum=vcgeneral
///WS_EX_TOOLWINDOW

hwnd = CreateWindowEx(WS_EX_TOOLWINDOW ,//0,
	windowClass.lpszClassName,
	L"Minimal Drag and Drop Application for Windows",
	WS_POPUP | WS_EX_ACCEPTFILES ,
//	dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_EX_ACCEPTFILES,
	//		dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
//	CW_USEDEFAULT,
//	CW_USEDEFAULT,
x,y,
	width,
	height,
	NULL,
	NULL,
	hInstance,
	NULL);
g_createdWindow = hwnd;
//	hwnd = GetConsoleWindow();

//SetWindowSubclass(hwnd, &StaticWndProc, 0, 0);

if (!hwnd)
{
	std::cout << "Couldn't create window" << std::endl;
	return false;
}

DragAcceptFiles(hwnd, TRUE);

return true;
}

bool processEvents()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//if (msg.message == WM_WINDOWPOSCHANGED)
		//{
		//	if (IsWindowVisible(g_createdWindow) == true)
		//		SetWindowPos(g_createdWindow, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

		//}

    //if (msg.message == WM_KEYDOWN)
    //{
    //  SendMessage(GetConsoleWindow(), msg.message, msg.wParam, msg.lParam);
    //}

		if (msg.message == WM_DROPFILES)
		{
			int a = 1;
			a++;

			//https://stackoverflow.com/questions/43823771/passing-wparam-into-dragqueryfile-not-compatible

			auto const drop_handle{ reinterpret_cast<::HDROP>(msg.wParam) };
			auto const dropped_files_count
			{
				::DragQueryFileW(drop_handle, 0xFFFFFFFF, nullptr, 0)
			};
			::std::vector< wchar_t > buffer;
			for (::UINT dropped_file_index{ 0 }; dropped_files_count != dropped_file_index; ++dropped_file_index)
			{
				auto const file_path_symbols_count_excluding_terminating_null
				{
					::DragQueryFileW(drop_handle, dropped_file_index, nullptr, 0)
				};
				if (0 < file_path_symbols_count_excluding_terminating_null)
				{
					auto const buffer_size{ file_path_symbols_count_excluding_terminating_null + 1 };
					buffer.resize(buffer_size);
					auto const copied_symbols_count_excluding_terminating_null
					{
						::DragQueryFileW(drop_handle, dropped_file_index, buffer.data(), buffer_size)
					};
					if (copied_symbols_count_excluding_terminating_null == file_path_symbols_count_excluding_terminating_null)
					{
						buffer.back() = L'\0'; // just in case....
											   // buffer now contains file path...

						std::wstringstream buf;
						for (int i = 0; i < buffer.size(); ++i)
						{
							buf << buffer[i];
						}
						std::wstring thePath = buf.str();
						OutputDebugString(L"WM_DROPFILES\n");
						OutputDebugString(thePath.c_str());
						OutputDebugString(L"\n");
					}
				}
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return isRunning;
}

void OpenFilesFromDataObject(IDataObject *pdto)
{
//	if (!g_hwndChild) { /* need to create a new main window */ }
	FORMATETC fmte = { CF_HDROP, NULL, DVASPECT_CONTENT,
		-1, TYMED_HGLOBAL };
	STGMEDIUM stgm;
	if (SUCCEEDED(pdto->GetData(&fmte, &stgm))) {
		HDROP hdrop = reinterpret_cast<HDROP>(stgm.hGlobal);
		UINT cFiles = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);
		for (UINT i = 0; i < cFiles; i++) {
			TCHAR szFile[MAX_PATH];
			UINT cch = DragQueryFile(hdrop, i, szFile, MAX_PATH);
			if (cch > 0 && cch < MAX_PATH) {
//				ListBox_AddString(g_hwndChild, szFile);
				int a = 1;
			}
		}
		ReleaseStgMedium(&stgm);
	}
}

class SimpleDropTarget : public IDropTarget
{
public:
	SimpleDropTarget() : m_cRef(1) { /*g_ppr->AddRef();*/ }
	~SimpleDropTarget() { /*g_ppr->Release();*/ }
	// *** IUnknown ***
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
	{
		if (riid == IID_IUnknown || riid == IID_IDropTarget) {
			*ppv = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}
	STDMETHODIMP_(ULONG) Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0) delete this;
		return cRef;
	}
//	Next come the methods of IDropTarget, none of which are particularly interesting.We just say that we are going to copy the data.

		// *** IDropTarget ***
		STDMETHODIMP DragEnter(IDataObject *pdto,
			DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect)
	{
		*pdwEffect &= DROPEFFECT_COPY;
		return S_OK;
	}
	STDMETHODIMP DragOver(DWORD grfKeyState,
		POINTL ptl, DWORD *pdwEffect)
	{
		*pdwEffect &= DROPEFFECT_COPY;
		return S_OK;
	}
	STDMETHODIMP DragLeave()
	{
		return S_OK;
	}
	STDMETHODIMP Drop(IDataObject *pdto, DWORD grfKeyState,
		POINTL ptl, DWORD *pdwEffect)
	{
		OpenFilesFromDataObject(pdto);
		*pdwEffect &= DROPEFFECT_COPY;
		return S_OK;
	}
private:
	LONG m_cRef;
};

bool StdinOpen() {
	static HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD bytes_left;
	PeekNamedPipe(handle, NULL, 0, NULL, &bytes_left, NULL);
	return bytes_left;
}

inline bool exists_test0(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

bool IsDirectory(std::wstring path)
{
	DWORD attrib = GetFileAttributes(path.c_str());

	if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY) != 0)
		return true;

	return false;
}

//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string

//https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t/26914562
int StringToWString(std::wstring &ws, const std::string &s)
{
  std::wstring wsTmp(s.begin(), s.end());

  ws = wsTmp;

  return 0;
}

std::wstring s2ws(const std::string& str)
{
  return StrUtil::s2ws(str);
////	using convert_typeX = std::codecvt_utf8<wchar_t>;
////	std::wstring_convert<convert_typeX, wchar_t> converterX;
//
////	return converterX.from_bytes(str);
//  std::wstring ws(str.begin(), str.end());
////  StringToWString(ws, str);
//  return std::wstring(ws);
}

std::string ws2s(const std::wstring& wstr)
{
  return StrUtil::ws2s(wstr);
  ////using convert_typeX = std::codecvt_utf8<wchar_t>;
  ////std::wstring_convert<convert_typeX, wchar_t> converterX;

  ////return converterX.to_bytes(wstr);

  //std::string s(wstr.begin(), wstr.end());
  //return std::string(s);
}

//HINSTANCE g__hInstance;
int mainX();

//https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
// generic solution
template <class T>
int numDigits(T number)
{
	int digits = 0;
	if (number < 0) digits = 1; // remove this line if '-' counts as a digit
	while (number) {
		number /= 10;
		digits++;
	}
	return digits;
}

std::wstring GetPaddedString(std::wstring const& str, int num)
{
	std::wstring start = str.substr(0, str.length() - numDigits(num));
	std::wstringstream buf;
	buf << start << num;
	return buf.str();
}

bool GetParentA(int num, DirObject* dirObject)
{
  DirObject* parent = dirObject;
  for (int i = 0; i < num; ++i)
  {
    if (parent->Parent != NULL)
    {
      parent = parent->Parent;
    }
  }
  if (parent != dirObject)
  {
    return parent->IsLastOfParent();
  }
  return true;
}

void ParseParents(int c, int numSlash, std::wstringstream& buf3, DirObject* dirObject)
{
  bool handled = false;
  if (c == numSlash - 1)
  {
    if (dirObject->IsLastOfParent())
    {
      buf3 << L"└──";
    }
    else
    {
      buf3 << L"├──";
    }
    handled = true;
  }
  for (int count = 2; count < numSlash; ++count)
  {
    if (c == numSlash - count) //2
    {
      if (GetParentA(count, dirObject))
      {
        buf3 << L"   ";
      }
      else
      {
        buf3 << L"│  ";
      }
    }
    handled = true;
  }

  if (!handled)
  {
    if (dirObject->Parent!=NULL && dirObject->Parent->IsLastOfParent())
    {
      buf3 << L"   ";
    }
    else if (dirObject->IsLastOfParent())
    {
      buf3 << L"│  ";
    }
    else
    {
      buf3 << L"│  ";
    }
    handled = true;
  }

}


void startup(LPCSTR lpApplicationName, LPSTR args)
{
  // additional information
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;

  // set the size of the structures
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  // start the program up
  CreateProcessA
  (
    lpApplicationName,   // the path
    args,                // Command line
    NULL,                   // Process handle not inheritable
    NULL,                   // Thread handle not inheritable
    FALSE,                  // Set handle inheritance to FALSE
    CREATE_NEW_CONSOLE,     // Opens file in a separate console
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi           // Pointer to PROCESS_INFORMATION structure
  );
  // Close process and thread handles. 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void startupHide(LPCSTR lpApplicationName, LPSTR args)
{
  // additional information
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;

  // set the size of the structures
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  // start the program up
  CreateProcessA
  (
    lpApplicationName,   // the path
    args,                // Command line
    NULL,                   // Process handle not inheritable
    NULL,                   // Thread handle not inheritable
    FALSE,                  // Set handle inheritance to FALSE
    CREATE_NO_WINDOW,     // hides
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi           // Pointer to PROCESS_INFORMATION structure
  );
  WaitForSingleObject(pi.hProcess, INFINITE);
  // Close process and thread handles. 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void RunProcess(std::wstring const& filePath, std::wstring const& arg)
{
	std::wstringstream wstr;
	wstr << L" " << arg;

	std::wstring ws = wstr.str();
	std::string str = ws2s(ws);
	LPSTR s = const_cast<char *>(str.c_str());

	std::string process = ws2s(filePath);

	startup(process.c_str(), s);
}

//waits for proccess to exit
void RunProcessHide(std::wstring const& filePath, std::wstring const& arg)
{
  std::wstringstream wstr;
  wstr << L" " << arg;

  std::wstring ws = wstr.str();
  std::string str = ws2s(ws);
  LPSTR s = const_cast<char *>(str.c_str());

  std::string process = ws2s(filePath);

  startupHide(process.c_str(), s);
}

std::string m_writtenKey = "";
void WriteKey(char* key)
{
  m_writtenKey = key;
}

//https://stackoverflow.com/questions/35054849/c-getting-from-a-to-z-from-virtual-keys
bool CharIsPressed(int getKey)
{
  char buf[] = " ";
  if ('A' <= getKey && getKey <= 'Z') {
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) buf[0] = getKey;
    else buf[0] = getKey + ('a' - 'A');
    WriteKey(buf);
    return true;
  }
  return false;
}

bool NumIsPressed(int getKey)
{
	char buf[] = " ";
	if ('0' <= getKey && getKey <= '9') {
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) buf[0] = getKey;
		else buf[0] = getKey;
		WriteKey(buf);
		return true;
	}
	return false;
}

bool CharOrNumIsPressed(int getKey)
{
	return CharIsPressed(getKey) || NumIsPressed(getKey);
}

//https://stackoverflow.com/questions/14762456/getclipboarddatacf-text
std::wstring GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		return L"";
	}

    // Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{
		return L"";
	}

	// Lock the handle to get the actual text pointer
	char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		return L"";
	}

	// Save text in a string class instance
	std::wstring text(s2ws(pszText));

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

std::vector<std::string> dPaths;
int selectedPath = 0;
int viewStart = 0;
int maxViewEnd = 34;
int viewEnd = 34;

int fSelectedPath = 0;
int fViewStart = 0;
int fMaxViewEnd = 43;
int fViewEnd = 43;
int fMaxSelPath = 76;

//  int maxSelPath = 50;
int maxSelPath = 76;

bool filesScreen = false;
bool qDown = false;

//  std::vector<DirObject> dirObjects;

std::vector<std::string> dFiles;
std::string path = "D:\\";
DirObject root(path, NULL);
OpenWith openWith;
Bookmarks bookmarks;
LogDrive logDrive;
Avail avail;
Make make;
Compare compare;
Delete deleteObj;
FileSpec fileSpec;
Copy copyObj;


void GoTo(std::wstring const& path)
{
	filesScreen = false;
  fileSpec.m_typed = L"*.*";

	std::vector<std::wstring> tokens = StrUtil::Tokenize(path, L"\\");
	std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(),
		[](unsigned char c) { return std::toupper(c); });
	int a = 1;
	a++;

	if (root.PathW().find(tokens[0]) != 0)
	{
		selectedPath = 0;
		viewStart = 0;
		maxViewEnd = 34;
		viewEnd = 34;

		std::vector<std::string> dFiles;
		std::wstringstream buf;
		buf << tokens[0] << L"\\";
		std::string path = ws2s(buf.str());
		root = DirObject(path, NULL);
		dPaths.clear();
		dFiles.clear();
		for (const auto & entry : std::filesystem::directory_iterator(path))
		{
			if (std::filesystem::is_directory(entry.path()))
			{
				std::cout << entry.path() << std::endl;
				std::wstring f = entry.path();
				OutputDebugString(f.c_str());

				std::wstring sub = f.substr(path.length());
				dPaths.push_back(ws2s(sub));
			}
			else
			{
				std::wstring f = entry.path();
				std::wstring sub = f.substr(path.length());
				dFiles.push_back(ws2s(sub));
			}
		}
		root.Expand();
		root.Compile();
		maxSelPath = root.AllPaths.size();
//		fMaxSelPath = root.AllPaths[selectedPath]->Files.size();
		fMaxSelPath = root.AllPaths[selectedPath]->FileSpecFiles.size();
	}

	int newSelectedPath = selectedPath;
	std::vector<DirObject>* currentChildrenPaths = &root.ChildrenPaths;
	for (int i = 1; i < tokens.size(); ++i)
	{
		for (int j = 0; j < currentChildrenPaths->size(); ++j)
		{
			if ((*currentChildrenPaths)[j].GetNameW() == tokens[i])
			{
				(*currentChildrenPaths)[j].Expand();

				root.Compile();
        maxSelPath = root.AllPaths.size();

				for (int k = 1; k < root.AllPaths.size(); ++k)
				{
					if (root.AllPaths[k] == &(*currentChildrenPaths)[j])
					{
						newSelectedPath = k;
						break;
					}
				}

				currentChildrenPaths = &((*currentChildrenPaths)[j].ChildrenPaths);

				break;
			}
		}
	}

	if (newSelectedPath < selectedPath)
	{
		for (int i = selectedPath; i > newSelectedPath; --i)
		{
			selectedPath--;
			if (selectedPath < 0)
			{
				selectedPath = 0;
				viewStart = 0;
				viewEnd = maxViewEnd;
			}
			else if (selectedPath >= viewStart + 1)
			{
				;
			}
			else if (selectedPath >= 1)
			{
				viewEnd--;
				if (viewEnd < maxViewEnd)
				{
					viewEnd = maxViewEnd;
				}
				viewStart--;
				if (viewStart < 0)
				{
					viewStart = 0;
				}
			}
		}
	}
	else
	{
		for (int i = selectedPath; i < newSelectedPath; ++i)
		{
			selectedPath++;
			if (selectedPath >= maxSelPath)
			{
				selectedPath--;
			}
			else if (selectedPath <= viewEnd)
			{
				;
			}
			else if (selectedPath >= maxViewEnd)
			{
				viewEnd++;
				viewStart++;
			}
		}
	}


	if (!IsDirectory(path))
	{
		filesScreen = true;
		std::wstring fileToFind = tokens[tokens.size() - 1];
		std::transform(fileToFind.begin(), fileToFind.end(), fileToFind.begin(),
			[](unsigned char c) { return std::toupper(c); });

		fSelectedPath = 0; //start from top
//		fMaxSelPath = root.AllPaths[selectedPath]->Files.size();
		fMaxSelPath = root.AllPaths[selectedPath]->FileSpecFiles.size();
		for (int i = 0; i<fMaxSelPath; ++i)
		{
//			if (root.AllPaths[selectedPath]->Files.size() <= i)
			if (root.AllPaths[selectedPath]->FileSpecFiles.size() <= i)
			{
				// Not found
				fSelectedPath = 0;
				break;
			}

//			std::wstring ws = root.AllPaths[selectedPath]->GetFileNameW(i);
			std::wstring ws = root.AllPaths[selectedPath]->GetFileNameFileSpecW(i);
			std::transform(ws.begin(), ws.end(), ws.begin(),
				[](unsigned char c) { return std::toupper(c); });
			if (ws == fileToFind)
			{
				break;
			}

			fSelectedPath++;
			if (fSelectedPath >= fMaxSelPath)
			{
				fSelectedPath--;
			}
			else if (fSelectedPath <= fViewEnd - 1)
			{
				;
			}
			else if (fSelectedPath >= fMaxViewEnd)
			{
				fViewEnd++;
				fViewStart++;
			}
		}


		int b = 1;
		b++;
	}
}

void Log(std::wstring const& pathW)
{
	selectedPath = 0;
	viewStart = 0;
	maxViewEnd = 34;
	viewEnd = 34;

	std::vector<std::string> dFiles;
	std::string path = StrUtil::ws2s(pathW);
	root = DirObject(path, NULL);
	dPaths.clear();
	dFiles.clear();
	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
		if (std::filesystem::is_directory(entry.path()))
		{
			std::cout << entry.path() << std::endl;
			std::wstring f = entry.path();
			OutputDebugString(f.c_str());

			std::wstring sub = f.substr(path.length());
			dPaths.push_back(ws2s(sub));
		}
		else
		{
			std::wstring f = entry.path();
			std::wstring sub = f.substr(path.length());
			dFiles.push_back(ws2s(sub));
		}
	}
	root.Expand();
	root.Compile();
	maxSelPath = root.AllPaths.size();
}

CHAR_INFO *m_bufScreen;

bool HasFocus()
{
//	HWND find = FindWindow(NULL, L"SimpleXTree");

	//https://stackoverflow.com/questions/18034988/checking-if-a-window-is-active
	return(GetConsoleWindow() == GetForegroundWindow());
}

std::wstring GetExePath()
{
	TCHAR szPath[MAX_PATH];

	GetModuleFileName(NULL, szPath, MAX_PATH);

	std::wstringstream ss;
	ss << szPath;
	return ss.str();
}

std::wstring GetAppPath()
{
	std::wstring appPath = GetExePath();
	appPath = appPath.substr(0, appPath.rfind(L"\\"));
	return appPath;
}

void DrawTime()
{
	std::wstring dt = DateTime::GetDateTime();
//	std::wstringstream ws;
	//	  ws << L" Path:                                                  " << dt << L" 18-07-21 12:54:09 pm";
//	ws << L" Path:                                                  " << dt;
//	DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L" Path:                                                  ", FG_GREY | BG_BLACK);
	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 56, 0, dt, FG_GREY | BG_BLACK);
}

std::wstring DirSizeToGigs(unsigned long long dirSize)
{
	unsigned long long bytesInGB = 1000000000;
	double theSize = (double)dirSize / (double)bytesInGB;
	std::wstringstream bufFS;
	bufFS << std::setprecision(10) << theSize;
	return bufFS.str();
}

std::wstring DirSizeToBytes(unsigned long long dirSize)
{
	//https://stackoverflow.com/questions/7276826/c-format-number-with-commas
	auto s = std::to_wstring(dirSize);
	int n = s.length() - 3;
	while (n > 0) {
		s.insert(n, L",");
		n -= 3;
	}

	std::wstringstream ws;
	ws << s;
	return ws.str();
}

void Refresh()
{
	root.Compile();
}

void SetComponentOtherActive(std::vector<Component*>& components, Component* component)
{
	component->m_otherActive = false;
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i] != component && components[i]->IsOtherActivated())
		{
			component->m_otherActive = true;
			break;
		}
	}
}

void DisableCtrlC()
{
  //https://stackoverflow.com/questions/18291284/handle-ctrlc-on-win32
  DWORD dwMode = 0x0;
  GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &dwMode);
  // Remove ENABLE_PROCESSED_INPUT flag
  dwMode &= ~ENABLE_PROCESSED_INPUT;
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), dwMode);
}

std::wstring GetHex(char o)
{
	int theInt8 = (int)o;
	std::wstringstream ss;
	ss << std::hex << theInt8;
	std::wstring hexy = ss.str();

	if (hexy.length() < 2)
	{
		ss.str(L"");
		ss << std::hex << L"0" << theInt8;
		hexy = ss.str();
	}

	std::wstring subh = hexy.substr(hexy.length() - 2, 2);
	std::transform(subh.begin(), subh.end(), subh.begin(), ::toupper);
	return subh;
}

const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

//std::wstring stringToWstring(const char* utf8Bytes)
//{
//	//setup converter
//	using convert_type = std::codecvt_utf8<typename std::wstring::value_type>;
//	std::wstring_convert<convert_type, typename std::wstring::value_type> converter;
//
//	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
////	return converter.from_bytes(utf8Bytes);
//	return converter.from_bytes(*utf8Bytes);
//}

//std::wstring stringToWstring(const char* utf8Bytes, const size_t numBytes)
//{
//	//setup converter
//	using convert_type = std::codecvt_utf8<typename std::wstring::value_type>;
//	std::wstring_convert<convert_type, typename std::wstring::value_type> converter;
//
//	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
//	return converter.from_bytes(utf8Bytes, utf8Bytes + numBytes);
//}

std::wstring stringToWstring(const char* utf8Bytes, const size_t numBytes)
{
	if (utf8Bytes == nullptr || numBytes == 0)
		return L"";

	int wideLength = MultiByteToWideChar(
		CP_UTF8, 0,
		utf8Bytes, static_cast<int>(numBytes),
		nullptr, 0);

	if (wideLength == 0)
		return L"";

	std::wstring wstr(wideLength, 0);

	MultiByteToWideChar(
		CP_UTF8, 0,
		utf8Bytes, static_cast<int>(numBytes),
		&wstr[0], wideLength);

	return wstr;
}

wchar_t* chars = new wchar_t[255];

//---------------------------------------------------------------------------
// Convert an hex string to a number

int HexToInt2(std::wstring str)
{
  if (str.size() == 0)return 0;
  std::wistringstream iss(str);
  int result = 0;
  if (!(iss >> std::hex >> result))throw exception("Invalid argument");
  return result;
}

std::wstring GetChar(char o)
{
//	const wchar_t * wt = GetWC(&o);
	//	char t = L"┌";
//	std::wstring tmp_string2(1, *wt);
//	return tmp_string2;

	//std::wstring wc(1, o);
	//mbstowcs(&wc[0], &o, 1);
	//return wc;

//	int utf32 = 1570;
	//int utf32 = (unsigned int)o;

	//const int mask = (1 << 10) - 1;
	//std::wstring str;
	//if (utf32 < 0xFFFF)
	//{
	//	str.push_back((wchar_t)utf32);
	//}
	//else
	//{
	//	utf32 -= 0x10000;
	//	int hi = (utf32 >> 10) & mask;
	//	int lo = utf32 & mask;

	//	hi += 0xD800;
	//	lo += 0xDC00;

	//	str.push_back((wchar_t)hi);
	//	str.push_back((wchar_t)lo);
	//}
	//return str;

//	setlocale(LC_ALL, "en_US.UTF-8");
//	wprintf(L"%lc", o);
//	wscanf_s(L"%lc", o);

	//std::wstring test0 = L"┌";
 // std::wstring test01 = L"\u250C";
 // unsigned int test = 0xDA;
 //   int test2 = 0xDA;
	//char test3 = 0xDA;
	//wchar_t test4 = (wchar_t)test;
	//std::wstring str;
	//str.push_back(test4);

 // wchar_t c;

 // setlocale(LC_ALL, "");
 // for (c = 128; c < 256; c++) {
 //   wprintf(L"%lc\n", c);
 // }

  //	char* two = new char[2];
//	two[0] = o;
//	two[1] = '\0';
//	std::wstring test5 = stringToWstring(two, 2);

//	wstring ws1 = L"Infinity: \uDA";

//	return str;

  const std::wstring th = GetHex(o);
  int i = HexToInt2(th);

  if (i == 255) // ASCII code 255 = nbsp(Non - breaking space or no - break space). This is the only one we don't have a mapping for.
  {
//    std::wstring tmp_string(1, o);
//    return tmp_string;
    return L" ";
  }

  wchar_t theChar = chars[i];

  std::wstringstream ret;
  ret << theChar;
  std::wstring retw = ret.str();
  return retw;

	//switch (o)
	//{
	//case 0x0D:
	//	return L"♪";
	//case 0x0A:
	//	return L"◙";
	//case -38:
	//	return L"┌";
	//case -60:
	//	return L"─";
	//case -77:
	//	return L"│";
	//case -65:
	//	return L"┐";
	//case -61:
	//	return L"├";
	//case -76:
	//	return L"┤";
	//case -2:
	//	return L"■";
	//}

	//std::wstring tmp_string(1, o);
	//return tmp_string;
}

std::wstring GetChar2(char o)
{
  switch (o)
  {
  case 0x0D:
    return L"♪";
  case 0x0A:
    return L"◙";
  case -38:
    return L"┌";
  case -60:
    return L"─";
  case -77:
    return L"│";
  case -65:
    return L"┐";
  case -61:
    return L"├";
  case -76:
    return L"┤";
  case -2:
    return L"■";
  }

  std::wstring tmp_string(1, o);
  return tmp_string;
}



std::wstring GetHexPadded8(int num)
{
	std::wstringstream ss;
	ss << std::hex << num;
	std::wstring hexy = ss.str();

	std::wstringstream padded;
	for (int i = hexy.length(); i < 8; ++i)
	{
		padded << L"0";
	}
	padded << hexy;

	std::wstring subh = padded.str();
	std::transform(subh.begin(), subh.end(), subh.begin(), ::toupper);
	return subh;
}

inline bool exists_test3(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

char* memblock2 = NULL;
int numMemBlock2 = 0;

char* rmemblock2 = NULL;

int rnumMemBlock2 = 0;

bool testReadFile = false;
bool viewFile = false;
bool searchMode = false;
std::wstring searchHex = L"";
int numFound = 0;

bool wasEditing = false;
bool wasJumping = false;

std::string pathToFile = "d:\\tmp\\file_id.diz";
std::string theFile = "file_id.diz";
//std::string pathToFile = "D:\\tmp\\LargeFile\\mod Rocksmith Remastered 2014 Edition (Windows PC).iso";
//std::string theFile = "mod Rocksmith Remastered 2014 Edition (Windows PC).iso";
//std::string pathToFile = "d:\\tmp\\61.txt";
//std::string theFile = "61.txt";

//void ReadFile()
//{
//  std::ifstream source;
//  source.open(pathToFile, std::ios::in | std::ios::binary);
//  source.seekg(std::streampos(0), std::ios::end);
//  theend = source.tellg();
//
//  if (theend < 704)
//  {
//    numMemBlock2 = theend;
//  }
//  else
//  {
//    numMemBlock2 = 704;
//  }
//
//  memblock2 = new char[numMemBlock2];
//  source.seekg(std::streampos(thestart), std::ios::beg);
//  source.read(memblock2, numMemBlock2);
//
//  source.close();
//}

RandomAccessBufferedReader file;

void ClearFileCache()
{
  file.Close();
  file.Open(StrUtil::s2ws(pathToFile));
}

void ReadFile()
{
  file.Open(StrUtil::s2ws(pathToFile));

  theend = file.GetEnd();

  if (theend == std::streampos(-1))
  {
    int a = 2;
    a++;
  }
  if (theend < 704)
  {
    numMemBlock2 = theend;
  }
  else
  {
    numMemBlock2 = 704;
  }
  
  if (searchObj.m_renderAscii)
  {
    std::streampos maxPageSize = std::streampos(44/*lines*/ * 1024/*maxCharPerLine*/);
    if (theend < maxPageSize)
    {
      numMemBlock2 = theend;
    }
    else
    {
      numMemBlock2 = maxPageSize;
    }
  }
  if (searchObj.m_renderDump)
  {
    std::streampos maxPageSize = std::streampos(44/*lines*/ * 65/*maxCharPerLine*/);
    if (theend < maxPageSize)
    {
      numMemBlock2 = theend;
    }
    else
    {
      numMemBlock2 = maxPageSize;
    }
  }

  if (memblock2 != NULL)
  {
    delete[] memblock2;
  }

  if (thestart + std::streampos(numMemBlock2) > theend)
  {
    numMemBlock2 = theend - thestart;
  }

  memblock2 = new char[numMemBlock2];
  file.SeekAndRead(std::streampos(thestart), memblock2, numMemBlock2);
}

void rReadFile()
{
  std::ifstream source;
  source.open(pathToFile, std::ios::in | std::ios::binary);
  source.seekg(std::streampos(0), std::ios::end);
  rtheend = source.tellg();
  rnumMemBlock2 = rtheend;
  if (rmemblock2 != NULL)
  {
    delete[] rmemblock2;
  }
  rmemblock2 = new char[rnumMemBlock2];
  source.seekg(0, std::ios::beg);
  source.read(rmemblock2, rnumMemBlock2);
}


const char * begin(const char* str) { return str; }
const char * end(const char* str) { return str + strlen(str); }

const std::byte * begin(const std::byte* str) { return str; }
const std::byte * end(const std::byte* str, int numBytes) { return str + numBytes; }

void Search2(std::wstring theSearchHex)
{
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
  std::byte* aSearch = (std::byte*)&chSearch[0];

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
//  numMemBlock2 = 2;

  //if (theend < 50)
  //{
  //  numMemBlock2 = theend;
  //}
  //else
  //{
  //  numMemBlock2 = 50; //100 meg try
  //}


  std::byte* memblock3 = new std::byte[numMemBlock2];
//  memblock2 = new char[numMemBlock2];


  source.seekg(std::streampos(thestart), ios::beg);
  source.read((char*)memblock3, numMemBlock2);

  int currentFound = -1;
  bool foundEmAll = false;
  endOfSearch = false;

//  char a[] = { 0, 0, 0, 0xFC };
 // char Buffer[20000] = { 0, 0, 0, 0xFC };

  bool atTheEnd = false;
  while (!atTheEnd)
  {
    auto it = std::search(::begin(memblock3), ::end(memblock3, numMemBlock2), ::begin(aSearch), ::end(aSearch, chSearch.size()));

    if (it == ::end(memblock3, numMemBlock2))
    {
      // not found
      int b = 1;
      b++;
    }
    else
    {
      // subrange found at std::distance(std::begin(Buffer), it)
      int index = std::distance(::begin(memblock3), it);
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




//  for (int i = 0; i < numMemBlock2; ++i)
//  {
//    char ch = memblock2[i];
//
//    for (int j = 0; j < chSearch.size(); ++j)
//    {
//      if (i+j<numMemBlock2 && chSearch[j] != memblock2[i+j])
//      {
//        numFound = 0;
//        break;
//      }
//      else
//      {
//        numFound++;
//      }
//    }
//
////    if (currentFound == -1)
////    {
////      if (ch == chSearch[0])
////      {
////        currentFound++;
////        chFound[0] = true;
////        numFound++;
////      }
////    }
////    else
////    {
////      if (currentFound + 1 >= chSearch.size())
////      {
//////        //found em all
//////        foundEmAll = true;
//////        break;
////      }
////      else if (currentFound < chSearch.size() && chFound[currentFound] && ch == chSearch[currentFound + 1])
////      {
//////        currentFound++;
//////        chFound[currentFound] = true;
//////        numFound++;
//////
//////        if (numFound >= chSearch.size())
//////        {
//////          //found em all
//////          foundEmAll = true;
//////          break;
//////        }
////      }
////      else if (currentFound < chSearch.size() && ch != chSearch[currentFound + 1])
////      {
//////        for (int j = 0; j < chSearch.size(); j++)
//////        {
//////          chFound[j] = false;
//////        }
////////        std::fill(chFound.begin(), chFound.end(), false);
//////        currentFound = -1;
//////        numFound = 0;
////      }
////    }
//  }

  starta.Stop();
  double seconds = starta.ElapsedSeconds();


  thestart = searchPos;
  theSearchPos = searchPos;
  theAfterSearchPos = searchPos + std::streampos(chSearch.size());
  ReadFile();


  int a = 1;
  a++;
}

void Search(std::wstring theSearchHex)
{
  //char* chSearch = new char[2];
  //chSearch[0] = 0x65;
  //chSearch[1] = 0x6D;
  //bool* chFound = new bool[2];
  //chFound[0] = false;
  //chFound[1] = false;
  //int arraySize = 2;

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




  //set prior to call 0 for first, theAfterSearchPos after
//  thestart = std::streampos(0);


  std::streampos searchPos = thestart;

  ifstream source;
  source.open(pathToFile, ios::in | ios::binary);
  bool fo = source.is_open();
  source.seekg(std::streampos(0), ios::end);
  theend = source.tellg();
  source.seekg(std::streampos(0), ios::beg);

  char* memblock = new char[1];
  source.seekg(std::streampos(theend), ios::beg);
  source.seekg(std::streampos(-1), ios::cur);
  source.read(memblock, 1);

  bool ok = source.eof();
  char o = memblock[0];

  std::wstring lastW = GetHex(o);

  if (theend < 100000000)
  {
    numMemBlock2 = theend;
  }
  else
  {
    numMemBlock2 = 100000000; //100 meg try
  }
//    numMemBlock2 = 3;
//  numMemBlock2 = 50;
//  numMemBlock2 = 100000000; //100 meg try

  memblock2 = new char[numMemBlock2];
  source.seekg(std::streampos(thestart), ios::beg);
  source.read(memblock2, numMemBlock2);

  int currentFound = -1;
  numFound = 0;
  bool foundEmAll = false;
  endOfSearch = false;
//  std::streampos savedI = 0;
//  std::streampos savedI = thestart;
  __int64 savedI = thestart;
  std::streampos lastI = std::streampos(0);
  std::vector<char> currentMatches;
  std::vector<std::streampos> currentStreamPos;
  bool readCurrent = false;
  while (source.tellg() <= theend && !foundEmAll && !endOfSearch)
  {
    // Record start time
// auto starta = std::chrono::high_resolution_clock::now();
    //https://www.pluralsight.com/blog/software-development/how-to-measure-execution-time-intervals-in-c--
    win32::Stopwatch starta;
    starta.Start();

    double oneSec = 0.0;
    double twoSec = 0.0;
    double threeSec = 0.0;
    double sec4 = 0.0;
    for (int i = 0; i < numMemBlock2; i = i + 1)
    {
      //auto start1 = std::chrono::high_resolution_clock::now();
//      win32::Stopwatch start1;
 //     start1.Start();
      bool setSavedI = false;
	  char b = memblock2[i];
      if (memblock2[i] == 0x73)
      {
        int a = 1;
        a++;
      }
     // auto finish1 = std::chrono::high_resolution_clock::now();
      //std::chrono::duration<double> elapsed = finish1 - start1;
      //oneSec += elapsed.count();
//      start1.Stop();
//      oneSec += start1.ElapsedSeconds();

//      auto start2 = std::chrono::high_resolution_clock::now();
//      win32::Stopwatch start2;
//      start2.Start();
      if (readCurrent)
      {
        for (int k = 0; k < currentMatches.size(); ++k)
        {
          if (currentFound == -1)
          {
            if (currentMatches[k] == chSearch[0])
            {
              currentFound++;
              chFound[0] = true;
              numFound++;

              searchPos = currentStreamPos[k];
              currentMatches.clear();
              currentMatches.push_back(memblock2[i]);
            }
          }
          else
          {
            if (currentFound + 1 >= chSearch.size())
            {
              //found em all
              foundEmAll = true;
//              savedI = savedI + std::streampos(k);
              break;
            }
            else if (currentFound < chSearch.size() && chFound[currentFound] && currentMatches[k] == chSearch[currentFound + 1])
            {
              currentFound++;
              chFound[currentFound] = true;
              numFound++;

              if (numFound >= chSearch.size())
              {
                //found em all
                foundEmAll = true;
//                savedI = savedI + std::streampos(k);
                break;
              }
            }
            else if (currentFound < chSearch.size() && currentMatches[k] != chSearch[currentFound + 1])
            {
              for (int j = 0; j < chSearch.size(); j++)
              {
                chFound[j] = false;
              }
              currentFound = -1;
              numFound = 0;
              //          i = searchPos;  //rewind back to search pos as there may be other duplicates
//              setSavedI = true;
//              savedI = searchPos;
              currentMatches.erase(currentMatches.begin());
              currentStreamPos.erase(currentStreamPos.begin());
            }
          }
        }
      }
      readCurrent = false;
//      auto finish2 = std::chrono::high_resolution_clock::now();
//      elapsed = finish2 - start2;
//      twoSec += elapsed.count();
 //     start2.Stop();
//      twoSec += start2.ElapsedSeconds();

//      auto start3 = std::chrono::high_resolution_clock::now();
//      win32::Stopwatch start3;
//      start3.Start();
      if (currentFound == -1)
      {
        if (memblock2[i] == chSearch[0])
        {
          currentFound++;
          chFound[0] = true;
          numFound++;
//          searchPos = thestart + i;
//          searchPos = savedI + i;
          searchPos = savedI;
          currentMatches.clear();
          currentMatches.push_back(memblock2[i]);
          currentStreamPos.clear();
          currentStreamPos.push_back(searchPos);
        }
      }
      else
      {
        if (currentFound + 1 >= chSearch.size())
        {
          //found em all
          foundEmAll = true;
//          savedI = thestart+i;
          savedI = savedI + std::streampos(i);
          break;
        }
        else if (currentFound < chSearch.size() && chFound[currentFound] && memblock2[i] == chSearch[currentFound + 1])
        {
          currentFound++;
          chFound[currentFound] = true;
          numFound++;

          if (numFound >= chSearch.size())
          {
            //found em all
            foundEmAll = true;
            searchPos = savedI - std::streampos(numFound-1);
            savedI = savedI + std::streampos(i);
            break;
          }

          //        break;
        }
        else if (currentFound < chSearch.size() && memblock2[i] != chSearch[currentFound + 1])
        {
          for (int j = 0; j < chSearch.size(); j++)
          {
            chFound[j] = false;
          }
          currentFound = -1;
          numFound = 0;
//          i = searchPos;  //rewind back to search pos as there may be other duplicates
          readCurrent = true;
          setSavedI = true;
//          savedI = searchPos;
          currentMatches.erase(currentMatches.begin());
          currentStreamPos.erase(currentStreamPos.begin());
        }
      }

//      savedI = thestart + i;// +std::streampos(1);
//      if (!setSavedI)
      {
 //       win32::Stopwatch start4;
 //       start4.Start();
        savedI = savedI + 1;
   //     start4.Stop();
   //     sec4 += start4.ElapsedSeconds();
      }
      lastI = i;

//      auto finish3 = std::chrono::high_resolution_clock::now();
//      elapsed = finish3 - start3;
//      threeSec += elapsed.count();
 //     start3.Stop();
 //     threeSec += start3.ElapsedSeconds();
  }
  // Record end time
//  auto finisha = std::chrono::high_resolution_clock::now();
//  std::chrono::duration<double> elapsed = finisha - starta;
//  std::cout << "Elapsed time: " << elapsed.count() << " s\n";
//  double seconds = elapsed.count();
  starta.Stop();
  double seconds = starta.ElapsedSeconds();
  //65.594510661000001, 51.487741905368992, 30.110781244757835
  //216.73067200400001 with timers
  //152.84096568141936 with stopwatches
  //125.75763149718401
  //sec4 20.875836750213196.. try __int64 ? 5.5913810444866181

//    savedI = savedI + numMemBlock2;

    if (savedI + std::streampos(numMemBlock2) <= theend)
    {
//      thestart = savedI;
      source.seekg(savedI, ios::beg);
      source.read(memblock2, numMemBlock2);
    }
    else
    {
      if (savedI >= theend)
      {
        endOfSearch = true;
        break;
      }
      numMemBlock2 = theend - savedI;
      source.seekg(savedI, ios::beg);
      source.read(memblock2, numMemBlock2);
    }
//    savedI += numMemBlock2;

    //if (!chFound[currentFound+1] && memblock2[i] == chSearch[currentFound+1])
    //{
    //  chFound[currentFound + 1] = true;
    //  searchPos = thestart + i;
    //}
    //else if (chFound[0] && memblock2[i] == chSearch[1])
    //{
    //  chFound[1] = true;
    //  numFound = 2;
    //  break;
    //}

    //if (!chFound[0] && memblock2[i] == chSearch[0])
    //{
    //  chFound[0] = true;
    //  searchPos = thestart + i;
    //}
    //else if (chFound[0] && memblock2[i] == chSearch[1])
    //{
    //  chFound[1] = true;
    //  numFound = 2;
    //  break;
    //}
    //else
    //{
    //  chFound[0] = false;
    //  chFound[1] = false;
    //}
//    savedI = i;
  }
  source.close();


  if (!foundEmAll && savedI >= theend- std::streampos(numMemBlock2))
  {
    endOfSearch = true;
  }

  thestart = searchPos;
  theSearchPos = searchPos;
  theAfterSearchPos = searchPos + std::streampos(chSearch.size());
  ReadFile();


//  delete[] chSearch;
//  delete[] chFound;








}

HANDLE ghConsole = NULL;



//int main()
int WINAPI WinMain(HINSTANCE hInstance2, HINSTANCE hPrevInstance, LPSTR     lpCmdLine, int       nShowCmd)
{
  std::wstring args = StrUtil::s2ws(lpCmdLine);
  std::transform(args.begin(), args.end(), args.begin(),
    [](unsigned char c) { return std::tolower(c); });

  if (StrUtil::StartsWith(args, L"test"))
  {
    SimpleXTree::Search searchObjTest;
    searchObjTest.Test(StrUtil::s2ws(lpCmdLine));

    return 0;
  }

//  ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

//  AllocConsole();

  TCHAR szPath[MAX_PATH];

  GetModuleFileName(NULL, szPath, MAX_PATH);

  std::wstringstream ss2;
  ss2 << szPath;
  std::wstring p = ss2.str();
  StrUtil::Replace(p, L"SimpleXTree.exe", L"ExtendedAciiKey.txt");
  std::string srr = StrUtil::ws2s(p);


  if (!exists_test3(srr))
  {
    ofstream wf(srr, ios::out | ios::binary);
    if (!wf) {
      cout << "Cannot open file!" << endl;
      return 1;
    }
    for (int i = 0; i <= 255; ++i)
    {
      char c = (char)i;
      wf.write(&c, sizeof(c));
    }
    wf.close();
  }


  ss2.str(L"");
  ss2 << szPath;
  p = ss2.str();
  StrUtil::Replace(p, L"SimpleXTree.exe", L"ExtendedAciiValue.txt");
  std::string srr2 = StrUtil::ws2s(p);

  if (exists_test3(srr2))
  {
    ifstream rf(srr2, ios::out | ios::binary);
    if (!rf) {
      cout << "Cannot open file!" << endl;
      return 1;
    }
    wchar_t  c;
    rf.read((char *)&c, 2); // skip FF FE header
    for (int i = 0; i <= 254; i+=1)
    {
      rf.read((char *)&c, 2);
      chars[i] = c;
//      std::wstring ws = GetChar(c);
      int a = 1;
    }
    rf.close();
  }

  //wchar_t c = 218;
  //setlocale(LC_ALL, "en-UK");
  //wprintf(L"%lc\n", c);
  ////for (c = 128; c < 256; c++) {
  ////  wprintf(L"%lc\n", c);
  ////}
  //_setmode(_fileno(stdout), _O_U8TEXT);
  //std::wcout << "\u00DA" << std::endl;



	char lastChar = 0;

//  ReadFile();

//	if (testReadFile)
//	{
////		std::wstring file = L"D:\webpages\hex open source\biew-610\file_id2.diz.txt";
////		std::wstring file = L"D:\webpages\hex open source\biew-610\file_id2.diz.txt";
//		//PS: off64_t is defined in stdio.h as "long long"..a 64 bit integer!
//
////		unsigned long p;
//		ifstream source;
////		source.open("d:\\tmp\\a.txt", ios::in | ios::binary);
////		source.open("D:\\tmp\\LargeFile\\mod Rocksmith Remastered 2014 Edition (Windows PC).iso", ios::in | ios::binary);
//		source.open("d:\\tmp\\file_id.diz", ios::in | ios::binary);
//		bool fo = source.is_open();
//		source.seekg(std::streampos(0), ios::end);
//		std::streampos end = source.tellg();
//		source.seekg(std::streampos(0), ios::beg);
//		//		streampos pp(p);
////		source.seekg(pp);
////		char * memblock;
////		source.seekg(std::streampos(0), ios::cur);
////		source.seekg(std::streampos(100000), ios::cur);
//
//		char* memblock = new char[1];
//		//while (!source.eof())
//		//{
//		//	source.read(memblock, 1);
//		//}
//		source.seekg(std::streampos(end), ios::beg);
//		source.seekg(std::streampos(-1), ios::cur);
//		source.read(memblock, 1);
//
//		bool ok = source.eof();
//		char o = memblock[0];
//
//		std::wstring lastW = GetHex(o);
//
//		//int theInt8 = (int)o;
//		//std::wstringstream ss;
//		//ss << std::hex << theInt8;
//		//std::wstring hexy = ss.str();
//
//		if (end < 100000)
//		{
//			numMemBlock2 = end;
//
//		}
//		else
//		{
//			numMemBlock2 = 100000;
//		}
//		memblock2 = new char[numMemBlock2];
//		source.seekg(std::streampos(0), ios::beg);
//		source.read(memblock2, numMemBlock2);
//
//		lastChar = o;
//	}

  std::string testString = "KurtHugoSchneider - Hope ya like our cover of Kina Grannis and I playing 'Stay' by Zedd ft. Alessia Cara 18075806_1019614248173476_3751693137517477888_n [High quality].mp3";
  std::wstring out = s2ws(testString);

  std::wstring appPath = GetAppPath();

  TCHAR d = appPath[0];
  std::wstringstream db;
  db << d << L":\\";
  std::string pathu = ws2s(db.str());
  root = DirObject(pathu, NULL);

 // SetWindow(80, 50);
 // if (!testReadFile)
	//SetConsoleTitle(L"Simple XTree");
//  SetConsoleCtrlHandler(NULL, TRUE);


//	ofstream myfile;
//	myfile.open("example.txt");

//	DragAcceptFiles(GetConsoleWindow(), TRUE);


  for (const auto & entry : std::filesystem::directory_iterator(path))
	{
		if (std::filesystem::is_directory(entry.path()))
		{
//			std::cout << entry.path() << std::endl;
			std::wstring f = entry.path();
//			OutputDebugString(f.c_str());

			std::wstring sub = f.substr(path.length());
			dPaths.push_back(ws2s(sub));

//      dirObjects.push_back(DirObject(ws2s(sub)));

//			myfile << ws2s(f) << std::endl;
		}
		else
		{
			std::wstring f = entry.path();
			std::wstring sub = f.substr(path.length());
			dFiles.push_back(ws2s(sub));
		}
	}
  root.Expand();
//  maxSelPath = root.ChildrenPaths.size();
  root.Compile();
  maxSelPath = root.AllPaths.size();
//	myfile.close();
	 


  AllocConsole();

  SetWindow(80, 50);
  if (!testReadFile)
    SetConsoleTitle(L"Simple XTree");

	//	mainX();
//	g__hInstance = hInstance;
  // Create Screen Buffer
  wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
  for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
  HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
  
  ghConsole = hConsole;

  CONSOLE_CURSOR_INFO     cursorInfo;
  GetConsoleCursorInfo(hConsole, &cursorInfo);
  cursorInfo.bVisible = false; // set the cursor visibility
  SetConsoleCursorInfo(hConsole, &cursorInfo);

  resizeConBufAndWindow(hConsole, nScreenWidth, nScreenHeight);

  SetConsoleActiveScreenBuffer(hConsole);
  DWORD dwBytesWritten = 0;


  m_bufScreen = new CHAR_INFO[nScreenWidth*nScreenHeight];
  memset(m_bufScreen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);



  //https://stackoverflow.com/questions/18028808/remove-blinking-underscore-on-console-cmd-prompt
//  CONSOLE_CURSOR_INFO     cursorInfo;

  /*
  GetConsoleCursorInfo(hConsole, &cursorInfo);
  cursorInfo.bVisible = false; // set the cursor visibility
  SetConsoleCursorInfo(hConsole, &cursorInfo);
  */

  //SetConsoleOutputCP(65001);
 // _setmode(_fileno(stdout), _O_U16TEXT);


  tetromino[0].append(L"..X...X...X...X."); // Tetronimos 4x4
  tetromino[1].append(L"..X..XX...X.....");
  tetromino[2].append(L".....XX..XX.....");
  tetromino[3].append(L"..X..XX..X......");
  tetromino[4].append(L".X...XX...X.....");
  tetromino[5].append(L".X...X...XX.....");
  tetromino[6].append(L"..X...X..XX.....");

  pField = new unsigned char[nFieldWidth*nFieldHeight]; // Create play field buffer
  for (int x = 0; x < nFieldWidth; x++) // Board Boundary
    for (int y = 0; y < nFieldHeight; y++)
      pField[y*nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

  // Game Logic
  bool bKey[4];
  int nCurrentPiece = 0;
  int nCurrentRotation = 0;
  int nCurrentX = nFieldWidth / 2;
  int nCurrentY = 0;
  int nSpeed = 20;
  int nSpeedCount = 0;
  bool bForceDown = false;
  bool bRotateHold = true;
  int nPieceCount = 0;
  int nScore = 0;
  vector<int> vLines;
  bool bGameOver = false;

  bool firstGo = true;


  //https://stackoverflow.com/questions/6285270/how-can-i-get-the-mouse-position-in-a-console-program
  //https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events?redirectedfrom=MSDN
  INPUT_RECORD InputRecord[128];
  DWORD Events;

  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

  /*
  CONSOLE_CURSOR_INFO cci;
  cci.dwSize = 25;
  cci.bVisible = FALSE;
  SetConsoleCursorInfo(hConsole, &cci);
  */
  SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

  //https://stackoverflow.com/questions/46567248/how-to-disable-user-selection-in-windows-console
  DWORD prev_mode;
  GetConsoleMode(hin, &prev_mode);
  SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS |
    (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

  DisableCtrlC();

  InstallHook();

  // https://devblogs.microsoft.com/oldnewthing/20100503-00/?p=14183
  SimpleDropTarget target;

  RECT rect;
  GetWindowRect(GetConsoleWindow(), &rect);

  HINSTANCE hInstance = GetModuleHandle(NULL);
//  if (!createWindow(hInstance, 800, 600, 32))
//  if (!createWindow(hInstance, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 32))
  if (!createWindow(hInstance, rect.left+474, rect.top+427, 177, 200, 32))
//  if (!createWindow(hInstance, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 32))
  {
	  return 1;
  }

  //SetForegroundWindow(GetConsoleWindow());
  //SetFocus(GetConsoleWindow());

  RegisterDragDrop(g_createdWindow, &target);

 // RegisterDragDrop(GetConsoleWindow(), &target);
  
//  DragAcceptFiles(GetConsoleWindow(), TRUE);


  std::wstringstream ss;

  bool lDown = false;
  bool enterDown = false;
  bool vDown = false;
  bool pdDown = false;
  bool puDown = false;
  bool oDown = false;
  bool tDown = false;
  bool uDown = false;
  bool spaceDown = false;
  bool spaceDownDir = false;
  bool ctrlVDown = false;
  bool escapeFilesDown = false;
  


  SMALL_RECT writeRegion;

  COORD coord;
  coord.X = 25;
  coord.Y = 48;
  SetConsoleCursorPosition(hConsole, coord);

  int tweak = 480;
  //  while (!bGameOver) // Main Loop

  while (true) // Main Loop
  {
	  writeRegion.Left = 0;
	  writeRegion.Top = 0;
	  writeRegion.Right = nScreenWidth * sizeof(CHAR_INFO);
	  writeRegion.Bottom = nScreenHeight * sizeof(CHAR_INFO);

	  //cci.bVisible = FALSE;
	  if (make.IsActivated())
	  {
//		  cci.bVisible = TRUE;
	  }
	//  SetConsoleCursorInfo(hConsole, &cci);

    //CONSOLE_CURSOR_INFO     cursorInfo;
    //GetConsoleCursorInfo(ghConsole, &cursorInfo);
    //cursorInfo.bVisible = true; 
    //SetConsoleCursorInfo(ghConsole, &cursorInfo);
    //COORD coord;
    //coord.X = 0 + 0;
    //coord.Y = 2;
    //SetConsoleCursorPosition(ghConsole, coord);
    searchObj.RenderCursor();

	  RECT rect;
	  GetWindowRect(GetConsoleWindow(), &rect);
	  RECT rect2;
	  GetWindowRect(g_createdWindow, &rect2);

	  //SetWindowPos(g_createdWindow, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
//	  SetWindowPos(g_createdWindow, HWND_TOP, 0, 0, 642, 832, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	  //	  SetWindowPos(g_createdWindow, HWND_TOP, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE);

//	  SetWindowPos(g_createdWindow, HWND_TOP, rect.left + 474, rect.top + 427, rect2.right - rect2.left, rect2.bottom - rect2.top, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);


	  SetWindowPos(g_createdWindow, HWND_TOP, rect.left + 474, rect.top + 427, rect2.right - rect2.left, rect2.bottom - rect2.top, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE|SWP_NOACTIVATE);
	  MoveWindow(g_createdWindow, rect.left + 474, rect.top + 427, rect2.right - rect2.left, rect2.bottom - rect2.top, TRUE);
	  //SetWindowPos(g_createdWindow, HWND_TOP, rect.left, rect.top, rect2.right - rect2.left, rect2.bottom - rect2.top, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	  //MoveWindow(g_createdWindow, rect.left, rect.top, rect2.right - rect2.left, rect2.bottom - rect2.top, TRUE);



	  	  processEvents();

			//fd_set readfds;
			//FD_ZERO(&readfds);
			//FD_SET(STDIN_FILENO, &readfds);
			//fd_set savefds = readfds;

			//struct timeval timeout;
			//timeout.tv_sec = 0;
			//timeout.tv_usec = 0;

			//int chr;

			//int sel_rv = select(1, &readfds, NULL, NULL, &timeout);
			//if (sel_rv > 0) {
			   // puts("Input:");
			   // while ((chr = getchar()) != EOF) putchar(chr);
			//}
			//else if (sel_rv == -1) {
			   // perror("select failed");
			//}

			//readfds = savefds;


	  //	  if (ioctl(0, I_NREAD, &n) == 0 && n > 0)
			//if (StdinOpen())
			//{
			   // std::wstring mystr;
			   // wcin >> mystr;
			//}
			//std::getline(std::wcin, mystr);

	  //	  std::wstringstream s;
	  //	  bool foundFirstQuote = false;
	  //	  bool foundSecondQuote = false;
	  //	  while (StdinOpen())
	  //	  {
	  //		  char ch = std::getchar();
	  //		  s << ch;
	  //		  std::wstring r1 = s.str();
	  //		  if (ch == '\"' && !foundFirstQuote)
	  //		  {
	  //			  foundFirstQuote = true;
	  //		  }
	  //		  else if (ch == '\"' && !foundSecondQuote)
	  //		  {
	  //			  foundSecondQuote = true;
	  //		  }
	  //
	  //		  std::string str(r1.begin(), r1.end());
	  //
	  ////		  if (exists_test0(str))
	  ////		  if (foundSecondQuote)
	  //
	  //
	  //		  ifstream ifile;
	  //		  ifile.open(str);
	  //		  if (ifile) {
	  //			  cout << "file exists";
	  //
	  //			  OutputDebugString(r1.c_str());
	  //			  s.str(L"");
	  //		  }
	  //		  else {
	  //			  cout << "file doesn't exist";
	  //		  }
	  //
	  //
	  //	  }
	  //	  std::wstring r = s.str();
	  if (!HasFocus())
	  {
		  if (!viewFile)
		  {
			  //		  this_thread::sleep_for(400ms); // Delay a bit makes conhost.exe use less cpu
			  this_thread::sleep_for(50ms); // Delay a bit makes conhost.exe use less cpu

			  if ((0x8000 & GetAsyncKeyState((unsigned char)(0x51/*Q*/))) != 0)
			  {
				  if (!qDown)
				  {
					  qDown = true;
					  // don't want to quit if q down and just gets focus
				  }
			  }
			  else
			  {
				  qDown = false;
			  }

			  copyObj.Render(47);
			  DrawTime();
        WriteConsoleOutput(hConsole, m_bufScreen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &writeRegion);
      }
      else
      {
        searchObj.Render();
        WriteConsoleOutput(hConsole, m_bufScreen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &writeRegion);
      }
		  continue;
	  }
    searchObj.RenderCursor();

	  //ReadConsoleInput blocks but we want to show a blinking cursor
	  //https://stackoverflow.com/questions/23164492/how-can-i-detect-if-there-is-input-waiting-on-stdin-on-windows/23168235
	  //https://docs.microsoft.com/en-us/windows/console/getnumberofconsoleinputevents?redirectedfrom=MSDN

	  DWORD numEvents = 0;
	  GetNumberOfConsoleInputEvents(hin, &numEvents);

	  if (numEvents > 0)
	  {
		  ReadConsoleInput(hin, InputRecord, 128, &Events);
	  }
	  else
	  {
		  Events = 0;
	  }
//    if (numEvents > 0 || (viewFile && !searchObj.Searching()))
    if (numEvents > 0)
    {
      int mnumDown = 0;
      int mDR = 0;
      int mnumUp = 0;
      int mUR = 0;
      int mnumPgDown = 0;
      int mPgDR = 0;
      int mnumPgUp = 0;
      int mPgUpR = 0;
      int mnumEnd = 0;
      int mEndR = 0;
      int mnumHome = 0;
      int mHomeR = 0;
      int mnumEnter = 0;
      int mnumESC = 0;
      int mnumQ = 0;
      int mnumS = 0;
      int numF = 0;
      int numF9 = 0;
      int msp = 0;
      int mspR = 0;

      searchObj.m_activated = viewFile;
      if (viewFile)
		  {
        for (int i = 0; i < Events; ++i)
        {
          MouseEventProc(InputRecord[i].Event.MouseEvent);

          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.AsciiChar != 0)
          {
            WCHAR ch = InputRecord[i].Event.KeyEvent.uChar.UnicodeChar;

            if (searchObj.IsActivated())
            {
              searchObj.KeyEvent(InputRecord[i].Event.KeyEvent.uChar.UnicodeChar);
            }

            if (searchMode)
            {
              if (ch == '\r' || ch == '\n' || ch == '\x1b')
              {

              }
              else if (ch == '\b')
              {
                if (searchHex.length() > 0)
                {
                  searchHex = searchHex.substr(0, searchHex.length() - 1);
                }
              }
              else
              {
                searchHex += ch;
              }
            }
          }

          if (!InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode==VK_ESCAPE)
          {
            if (searchObj.IsActivated())
            {
              wasEditing = searchObj.m_editing;
              wasJumping = searchObj.m_jumping;
              searchObj.VKUp(InputRecord[i].Event.KeyEvent.wVirtualKeyCode);
            }
          }

          if (InputRecord[i].Event.KeyEvent.bKeyDown)
          {
            if (searchObj.IsActivated())
            {
              wasEditing = searchObj.m_editing;
              wasJumping = searchObj.m_jumping;
              searchObj.VK(InputRecord[i].Event.KeyEvent.wVirtualKeyCode);
            }
          }

          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
          {
            mnumDown++;
            mDR = InputRecord[i].Event.KeyEvent.wRepeatCount;
            int r = InputRecord[i].Event.KeyEvent.wRepeatCount;
            int a = 1;
            a++;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_UP)
          {
            mnumUp++;
            mUR = InputRecord[i].Event.KeyEvent.wRepeatCount;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_END)
          {
            mnumEnd++;
            mEndR = InputRecord[i].Event.KeyEvent.wRepeatCount;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_HOME)
          {
            mnumHome++;
            mHomeR = InputRecord[i].Event.KeyEvent.wRepeatCount;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_NEXT)
          {
            mnumPgDown++;
            mPgDR = InputRecord[i].Event.KeyEvent.wRepeatCount;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_PRIOR)
          {
            mnumPgUp++;
            mPgUpR = InputRecord[i].Event.KeyEvent.wRepeatCount;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_SPACE)
          {
            msp++;
            mspR = InputRecord[i].Event.KeyEvent.wRepeatCount;
          }

          if (!searchMode)
          {
            if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
            {
              mnumEnter++;
            }
            if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
            {
              mnumESC++;
            }
            if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.UnicodeChar == 'q')
            {
              mnumQ++;
            }
          }
          else
          {
            if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
            {
              if (searchHex == L"")
              {
                numFound = 0;
              }
              else
              {
                thestart = std::streampos(0);
                //Search2(searchHex);
                searchObj.StartSearch(searchHex);
                searchMode = false;
              }
            }
            if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
            {
              searchMode = false;
            }
          }

          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.UnicodeChar == 's')
          {
            mnumS++;
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_F9)
          {
            if (!scrolledUp)
            {
              numF9++;
            }
          }
          if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.UnicodeChar == 'f')
          {
            numF++;
          }
        }

        if (testReadFile && (mnumEnter > 0 || mnumESC>0 || mnumQ>0) && searchHex != L"" && endOfSearch)
        {
          endOfSearch = false;
          searchHex == L"";
        }
        else if (testReadFile && !wasEditing && !wasJumping && (mnumEnter > 0 || mnumESC>0 || mnumQ>0))
        {
          testReadFile = false;
          viewFile = false;
          searchMode = false;
          searchHex == L"";
          numFound = 0;
        }
        if (testReadFile && !searchMode && (mnumS > 0 || numF9>0 || numF>0) && !searchObj.m_editing && !searchObj.m_editingAscii)
        {
          searchMode = true;
          searchHex = L"";
        }
        if (testReadFile && mnumDown>0 && !searchObj.m_editing)
        {
          for (int i = 0; i < mDR; ++i)
          {
            if (searchObj.m_renderDump)
            {
              bool inc = true;
              if ((thestart + std::streampos(2624 - 64) + std::streampos(64)) >= (theend))
                inc = false;

              if (inc)
              {
                thestart += 64;
                ReadFile();
              }
            }
            else if (!searchObj.m_renderAscii)
            {
              bool inc = true;
              //            if ((thestart + std::streampos(688) + std::streampos(16)) >(numMemBlock2))
              if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
                inc = false;

              if (inc)
              {
                thestart += 16;
                ReadFile();
              }
            }
            else
            {
              searchObj.Down();
            }
          }
        }
        if (testReadFile && mnumPgDown>0/* && !searchObj.m_editing*/)
        {
          for (int i = 0; i < mPgDR; ++i)
          {
            if (searchObj.m_renderDump)
            {
              //start + 2752;
              for (int i = 0; i < 44; ++i) //44 lines in a page
              {
                bool inc = true;
                if ((thestart + std::streampos(2624-64) + std::streampos(64)) >= (theend))
                  inc = false;
                if (inc)
                {
                  thestart += 64;
                }
              }

              ReadFile();
            }
            else if (!searchObj.m_renderAscii)
            {
              for (int i = 0; i < 44; ++i) //44 lines in a page
              {
                bool inc = true;
                if ((thestart + std::streampos(688) + std::streampos(16)) >= (theend))
                  inc = false;
                if (inc)
                {
                  thestart += 16;
                }
              }

              ReadFile();
            }
            else
            {
              //int numChars = searchObj.GetNumChars();
              //bool inc = true;
              //if ((thestart + std::streampos(numChars)) >= (theend))
              //  inc = false;
              //if (inc)
              //{
              //  thestart += numChars;
              //}
              //ReadFile();
              //if (searchObj.GetNumLines() == -1)
              //{
              //  thestart = theend - std::streampos(numChars);
              //  ReadFile();
              //}

              for (int i = 0; i < 44; ++i) //44 lines in a page
              {
                if (searchObj.Down() == -1)
                  break;
              }


            }
          }
        }
        if (testReadFile && mnumUp>0 && !searchObj.m_editing)
        {
          for (int i = 0; i < mUR; ++i)
          {
            if (searchObj.m_renderDump)
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
            else if (!searchObj.m_renderAscii)
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
            else
            {
              searchObj.Up();
            }
          }
        }
        if (testReadFile && mnumPgUp>0/* && !searchObj.m_editing*/)
        {
          for (int i = 0; i < mPgUpR; ++i)
          {
            if (searchObj.m_renderDump)
            {
              //start + 2752;
              if (thestart < std::streampos(2752 + 64))
              {
                thestart = 0;
              }
              else
              {
                thestart -= std::streampos(2752 + 64);
              }
              ReadFile();
            }
            else if (!searchObj.m_renderAscii)
            {
              if (thestart < std::streampos(688 + 16))
              {
                thestart = 0;
              }
              else
              {
                thestart -= std::streampos(688 + 16);
              }
              ReadFile();
            }
            else
            {
              for (int i = 0; i < 44; ++i) //44 lines in a page
              {
                searchObj.Up();
                if (thestart == std::streampos(0))
                  break;
              }
            }
          }
        }
        if (testReadFile && mnumEnd>0)
        {
          if (searchObj.m_renderDump)
          {
            //start + 2752;

            thestart = theend;
            if (theend >= std::streampos(2752))
            {
              thestart = theend - std::streampos(2752);
            }
            if (thestart >= thestart % 64)
            {
              thestart = thestart - thestart % 64;


              if ((thestart + std::streampos(2752) + std::streampos(64)) >= (theend))
              {
                if (thestart >= 64)
                {
                  thestart -= 64;
                }
              }


              bool inc = true;
              if ((thestart + std::streampos(2752) + std::streampos(64)) >= (theend))
                inc = false;
              if (inc)
              {
                thestart += 64;
              }

            }
            else
            {
              thestart = 0;
            }

            ReadFile();

          }
          else if (!searchObj.m_renderAscii)
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
            else
            {
              thestart = 0;
            }

            ReadFile();
          }
          else
          {
            //while (searchObj.Down() != -1)
            //{
            //  int a = 1;
            //  a++;
            //}

            //thestart = theend;
            //for (int i = 0; i < 44; ++i) //44 lines in a page
            //{
            //  searchObj.Up();
            //  if (thestart == std::streampos(0))
            //    break;
            //}
            /*
            int numChars = searchObj.GetNumChars();
            if (numChars == 0)
            {
              numChars = 80 * 44; //44 lines of 80 approx
            }
            thestart = theend-std::streampos(numChars);
            ReadFile();
            */
            //searchObj.Down();
            //while (searchObj.Down() != -1)
            //{
            //  int a = 1;
            //  a++;
            //}


//            thestart = theend - std::streampos(688);
//            thestart = theend - std::streampos(42*80);

            /*

            //get near where we want
            int numChars = searchObj.GetNumChars();
            if (numChars == 0)
            {
              numChars = 80 * 44; //44 lines of 80 approx
            }
            thestart = theend - std::streampos(numChars);

            //now go to end
            searchObj.Down();
            while (searchObj.Down() != -1)
            {
              int a = 1;
              a++;
            }


            ReadFile();
            */

            searchObj.End();
          }
        }
        if (testReadFile && mnumHome>0)
        {
          thestart = 0;
          ReadFile();
        }
        if (testReadFile && !searchMode && msp > 0)
        {
          if (searchHex != L"")
          {
            __int64 startBeforeSearch = thestart;
            if (thestart == theSearchPos)
            {
              thestart = theAfterSearchPos;
            }
//            Search2(searchHex);
//            ReadFile();
            searchObj.StartSearch(searchHex, startBeforeSearch);
          }
        }

        searchObj.Render();
        WriteConsoleOutput(hConsole, m_bufScreen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &writeRegion);
			  continue;
		  }
      searchObj.RenderCursor();

		  //if (!HasFocus())
		  //{
			 // DrawTime();
			 // continue;
		  //}

      int numDown = 0;
      int numUp = 0;

		  int numT = 0;
		  int numU = 0;
	  for (int i = 0; i < Events; ++i)
	  {
		  //      std::wstringstream s;
		  //      s << InputRecord[i].Event.MouseEvent.dwMousePosition.X << "," << InputRecord[i].Event.MouseEvent.dwMousePosition.Y << "|" << InputRecord[i].Event.MouseEvent.dwButtonState << std::endl;
		  //      OutputDebugString(s.str().c_str());

		 // MouseEventProc(InputRecord[i].Event.MouseEvent);

		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_LEFT)
		  {
			  int a = 1;
			  a++;
		  }

		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
		  {
			  int a = 1;
			  a++;
			  numDown++;
		  }
		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_RIGHT)
		  {
			  int a = 1;
			  a++;
			  numDown++;
		  }
		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.wVirtualKeyCode == VK_UP)
		  {
			  int a = 1;
			  a++;
			  numUp++;
		  }

		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.UnicodeChar == 't')
		  {
			  int a = 1;
			  a++;
			  numT++;
		  }
		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.UnicodeChar == 'u')
		  {
			  int a = 1;
			  a++;
			  numU++;
		  }

		  if (InputRecord[i].Event.KeyEvent.bKeyDown)
		  {
			  if (compare.IsActivated())
			  {
				  compare.VK(InputRecord[i].Event.KeyEvent.wVirtualKeyCode);
			  }
        if (deleteObj.IsActivated())
        {
          deleteObj.VK(InputRecord[i].Event.KeyEvent.wVirtualKeyCode);
        }
		if (fileSpec.IsActivated())
		{
			fileSpec.VK(InputRecord[i].Event.KeyEvent.wVirtualKeyCode);
		}
		if (copyObj.IsActivated())
		{
			copyObj.VK(InputRecord[i].Event.KeyEvent.wVirtualKeyCode);
			if (copyObj.m_browse)
			{
				filesScreen = false;
			}
		}

		  }

		  if (InputRecord[i].Event.KeyEvent.bKeyDown && InputRecord[i].Event.KeyEvent.uChar.AsciiChar != 0)
		  {
			  //		  UINT cha = MapVirtualKeyA(InputRecord[i].Event.KeyEvent.wVirtualKeyCode, MAPVK_VK_TO_CHAR);
			  //		  ss << cha;
			  ss << InputRecord[i].Event.KeyEvent.uChar.AsciiChar;

			  if (make.IsActivated())
			  {
				  make.KeyEvent(InputRecord[i].Event.KeyEvent.uChar.UnicodeChar);
			  }
			  if (compare.IsActivated())
			  {
				  compare.KeyEvent(InputRecord[i].Event.KeyEvent.uChar.UnicodeChar);
			  }
			  if (deleteObj.IsActivated())
			  {
				  deleteObj.KeyEvent(InputRecord[i].Event.KeyEvent.uChar.UnicodeChar);
			  }
			  if (fileSpec.IsActivated())
			  {
				  fileSpec.KeyEvent(InputRecord[i].Event.KeyEvent.uChar.UnicodeChar);
			  }
			  if (copyObj.IsActivated())
			  {
				  copyObj.KeyEvent(InputRecord[i].Event.KeyEvent.uChar.UnicodeChar);
			  }
		  }
	  }
    searchObj.RenderCursor();
	  std::wstring rr = ss.str();

	  if (IsDirectory(rr))
	  {
		  OutputDebugString(rr.c_str());
		  ss.str(L"");
	  }


	  std::string srr = ws2s(rr);
	  srr.erase(std::remove(srr.begin(), srr.end(), '\"'), srr.end());
	  ifstream ifile;
	  ifile.open(srr);
	  if (ifile) {
		  cout << "file exists";

		  OutputDebugStringA(srr.c_str());
		  ss.str(L"");
	  }
	  else {
		  cout << "file doesn't exist";
	  }


	  // Timing =======================
  //    this_thread::sleep_for(50ms); // Small Step = 1 Game Tick
	  this_thread::sleep_for(16ms); // Small Step = 1 Game Tick
//	  this_thread::sleep_for(150ms); //try to slow down keys a bit nope too slow
	  nSpeedCount++;
	  bForceDown = (nSpeedCount == nSpeed);

	  // Input ========================
	  for (int k = 0; k < 4; k++)								// R   L   D Z
		  bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

	  std::vector<Component*> components;
	  components.push_back(&logDrive);
	  components.push_back(&avail);
	  components.push_back(&make);
	  components.push_back(&compare);
	  components.push_back(&deleteObj);
	  components.push_back(&fileSpec);
	  components.push_back(&bookmarks);
	  components.push_back(&openWith);
	  components.push_back(&copyObj);

//	  bool otherActive = logDrive.IsActivated() || avail.IsActivated() || make.IsActivated() || (compare.IsActivated() && !compare.m_browse) || deleteObj.IsActivated() || bookmarks.IsActivated() || openWith.IsActivated();
//    compare.m_otherActive = logDrive.IsActivated() || avail.IsActivated() || make.IsActivated() || deleteObj.IsActivated() || bookmarks.IsActivated() || openWith.IsActivated();
//    deleteObj.m_otherActive = logDrive.IsActivated() || avail.IsActivated() || make.IsActivated() || compare.IsActivated() || bookmarks.IsActivated() || openWith.IsActivated();

    searchObj.RenderCursor();
	  bool otherActive = false;
	  for (int i = 0; i < components.size(); ++i)
	  {
		  if (components[i]->IsOtherActivatedAndNotBrowse())
		  {
			  otherActive = true;
			  break;
		  }
	  }
	  for (int i = 0; i < components.size(); ++i)
	  {
		  SetComponentOtherActive(components, components[i]);
	  }

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
//	  bool keyTestUp = false;
//	  bool keyTestDown = false;
    pdDown = false;
    puDown = false;
   // if (!otherActive && (0x8000 & GetAsyncKeyState((unsigned char)(VK_UP))) != 0)
	  //{
		 // keyTestUp = true;
	  //}
	  //if (!otherActive && (0x8000 & GetAsyncKeyState((unsigned char)(VK_DOWN))) != 0 || (0x8000 & GetAsyncKeyState((unsigned char)(VK_RIGHT))) != 0)
	  //{
		 // keyTestDown = true;
	  //}

    //begin
   // if (!copyObj.IsActivated())
   // {
   //   openWith.CheckKeys();
   //   bookmarks.CheckKeys();
   //   logDrive.CheckKeys();
   //   avail.CheckKeys();
   //   make.CheckKeys(root.AllPaths[selectedPath]);
   //   compare.CheckKeys(root.AllPaths[selectedPath], filesScreen);
   //   deleteObj.CheckKeys(root.AllPaths[selectedPath]);
   //   fileSpec.CheckKeys(root.AllPaths[selectedPath]);
   // }
	  //copyObj.CheckKeys(root.AllPaths[selectedPath], filesScreen);
    //end

	bool shift = (0x8000 & GetAsyncKeyState((unsigned char)(VK_SHIFT))) != 0;
	bool control = (0x8000 & GetAsyncKeyState((unsigned char)(VK_CONTROL))) != 0;

    // if a component is active, only parse keys for it.
    if (otherActive)
    {
      for (int i = 0; i < components.size(); ++i)
      {
        if (components[i]->IsOtherActivatedAndNotBrowse())
        {
          components[i]->CheckKeysDO(root.AllPaths[selectedPath], filesScreen);
          break;
        }
      }
    }
    else
    {
      //if no components active, parse keys for them all

      for (int i = 0; i < components.size(); ++i)
      {
          if (!shift)
		  {
			  components[i]->CheckKeysDO(root.AllPaths[selectedPath], filesScreen);
		  }
      }
    }

    if (copyObj.m_refreshDest)
    {
//      root.AllPaths[selectedPath]->Expand();      
      GoTo(copyObj.m_destinationFolder);
      copyObj.m_refreshDest = false;
    }


    if (!otherActive && (0x8000 & GetAsyncKeyState((unsigned char)(VK_NEXT))) != 0)
    {
      pdDown = true;
    }
    if (!otherActive && (0x8000 & GetAsyncKeyState((unsigned char)(VK_PRIOR))) != 0)
    {
      puDown = true;
    }

  //'O' ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", wstr2.str().c_str(), NULL, SW_HIDE);
	if (!otherActive && !shift && (0x8000 & GetAsyncKeyState((unsigned char)(0x4F/*O*/))) != 0)
	{
		if (!oDown)
		{
			oDown = true;
			if (filesScreen)
			{
				//        std::wstringstream buf;
				//        buf << "\"" << root.AllPaths[selectedPath]->Files[fSelectedPath] << "\"";
				//::system(ws2s(buf.str()).c_str());

				std::wstringstream wstr;
				//        wstr << L" " << "\"" << root.AllPaths[selectedPath]->Files[fSelectedPath] << "\"";
//				wstr << L" " << "\"" << root.AllPaths[selectedPath]->Files[fSelectedPath] << "\"";
				wstr << L" " << "\"" << root.AllPaths[selectedPath]->FileSpecFiles[fSelectedPath] << "\"";

				std::wstring ws = wstr.str();
				LPCWSTR x = ws.c_str();
				std::string str = ws2s(ws);
				LPSTR s = const_cast<char *>(str.c_str());
				LPSTR s2 = const_cast<char *>("");
				//        startup("", s);
				//startup(s, s2);

				//https://stackoverflow.com/questions/9115574/how-can-you-open-a-file-with-the-program-associated-with-its-file-extension
				//        ShellExecute(0, 0, x, 0, 0, SW_SHOW);
				// suppose, strFileName is a CString variable  containing your file name
				//        HINSTANCE h = ShellExecute(NULL, _T("open"), root.AllPaths[selectedPath]->Files[fSelectedPath].c_str(), NULL, NULL, SW_SHOWNORMAL);
				//        SHELLEXECUTEINFOW execInfo = {};
				//        execInfo.lpFile = root.AllPaths[selectedPath]->Files[fSelectedPath].c_str();
				//        ShellExecuteEx(&execInfo);

				std::wstringstream wstr2;
//				wstr2 << " /c " << "\"" << root.AllPaths[selectedPath]->Files[fSelectedPath] << "\"";
				wstr2 << " /c " << "\"" << root.AllPaths[selectedPath]->FileSpecFiles[fSelectedPath] << "\"";

//				std::wstring fileUpper = root.AllPaths[selectedPath]->Files[fSelectedPath];
				std::wstring fileUpper = root.AllPaths[selectedPath]->FileSpecFiles[fSelectedPath];
				std::transform(fileUpper.begin(), fileUpper.end(), fileUpper.begin(),
					[](unsigned char c) { return std::toupper(c); });
//				if (fileUpper.find(L".EXE") != std::wstring::npos)
//			else if (fileUpper.find(L".BAT") != std::wstring::npos)
				if (StrUtil::EndsWith(fileUpper, L".EXE"))
				{
//					RunProcess(root.AllPaths[selectedPath]->Files[fSelectedPath], L"");
					RunProcess(root.AllPaths[selectedPath]->FileSpecFiles[fSelectedPath], L"");
				}
				else if (StrUtil::EndsWith(fileUpper, L".BAT"))
				{
					std::wstringstream bat;
//					bat << L"/c " << "\"" << root.AllPaths[selectedPath]->Files[fSelectedPath] << "\"";
					bat << L"/c " << "\"" << root.AllPaths[selectedPath]->FileSpecFiles[fSelectedPath] << "\"";
					RunProcess(L"C:\\Windows\\System32\\CMD.exe", bat.str().c_str());
				}
				else
				{
					//        ShellExecute Me.hwnd, "Open", "C:\Windows\System32\CMD.exe", " /c ipconfig >> c:\Myip.txt", "c:\", SW_SHOWNORMAL
					//        ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", wstr2.str().c_str(), NULL, SW_SHOWNORMAL);
					//https://forums.codeguru.com/showthread.php?458706-RESOLVED-ShellExecute-not-opening-file-but-no-errors
					ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", wstr2.str().c_str(), NULL, SW_HIDE);
				}
			}
			else
			{
				//open file explorer
				RunProcess(L"c:\\windows\\explorer.exe", root.AllPaths[selectedPath]->PathW());
				//ShellExecute(NULL, L"open", L"C:\\Windows\\System32\\CMD.exe", root.AllPaths[selectedPath]->PathW().c_str(), NULL, SW_HIDE);
			}

		}
	}
	else
	{
		oDown = false;
	}
  searchObj.RenderCursor();

	if (!otherActive && !shift && (0x8000 & GetAsyncKeyState((unsigned char)(0x54/*T*/))) != 0)
	{
		if (!tDown)
		{
			tDown = true;
			if (filesScreen)
			{
				if (control)
				{
					root.AllPaths[selectedPath]->Tag();
				}
				else
				{
					root.AllPaths[selectedPath]->Tags[fSelectedPath] = true;
//					keyTestDown = true;
					numDown += 1;
				}
			}
			else
			{
				if (control)
				{
					root.TagAndChildren();
				}
				else
				{
					root.AllPaths[selectedPath]->Tag();
//					keyTestDown = true;
					numDown += 1;
				}
			}
		}
	}
	else
	{
		tDown = false;
	}
  searchObj.RenderCursor();

	if (!otherActive && !shift && (0x8000 & GetAsyncKeyState((unsigned char)(0x55/*U*/))) != 0)
	{
		if (!uDown)
		{
			uDown = true;
			if (filesScreen)
			{
				if (control)
				{
					root.AllPaths[selectedPath]->Untag();
				}
				else
				{
					root.AllPaths[selectedPath]->Tags[fSelectedPath] = false;
//					keyTestDown = true;
					numDown += 1;
				}
			}
			else
			{
				if (control)
				{
					root.UntagAndChildren();
				}
				else
				{
					root.AllPaths[selectedPath]->Untag();
//					keyTestDown = true;
					numDown += 1;
				}
			}
		}
	}
	else
	{
		uDown = false;
	}
  searchObj.RenderCursor();

	//if (!logDriveActive && !bookmarksActive && !openWithActive && (0x8000 & GetAsyncKeyState((unsigned char)((0x51/*Q*/))) != 0)
	//{
	//	qDown = true;
	//}

	//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	if (!otherActive && !shift && (0x8000 & GetAsyncKeyState((unsigned char)(0x51/*Q*/))) != 0)
	{
		if (!qDown)
		{
			qDown = true;
			//quit
			return 0;
		}
	}

	if (!otherActive && control && (0x8000 & GetAsyncKeyState((unsigned char)(0x56/*V*/))) != 0)
	{
		if (!ctrlVDown)
		{
			ctrlVDown = true;
			std::wstring clipboard = GetClipboardText();

			if (IsDirectory(clipboard))
			{
				GoTo(clipboard);
			}

			int d = 0;
			d++;
		}
	}
	else
	{
		ctrlVDown = false;
	}

	if (!otherActive && (0x8000 & GetAsyncKeyState((unsigned char)(VK_ESCAPE))) != 0 && filesScreen)
	{
		if (!escapeFilesDown)
		{
			escapeFilesDown = true;
			filesScreen = false;
		}
	}
	else
	{
		escapeFilesDown = false;
	}

  searchObj.RenderCursor();
  if (copyObj.m_activated && copyObj.m_escPressed)
  {
    copyObj.m_activated = false;
  }

    if (!otherActive && !openWith.m_enterPessed && !bookmarks.m_enterPessed && !copyObj.m_enterPressed && ((0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0 || (0x8000 & GetAsyncKeyState((unsigned char)(VK_ADD))) != 0 || (0x8000 & GetAsyncKeyState((unsigned char)(VK_OEM_PLUS))) != 0))
    {
      bool justEnter = ((0x8000 & GetAsyncKeyState((unsigned char)(VK_RETURN))) != 0 && (0x8000 & GetAsyncKeyState((unsigned char)(VK_ADD))) == 0 && (0x8000 & GetAsyncKeyState((unsigned char)(VK_OEM_PLUS))) == 0);

		if (!enterDown)
		{
			enterDown = true;
			if (filesScreen)
			{
				filesScreen = false;
			}
			else
			{
				if (compare.m_browse && justEnter)
				{
					compare.SelectDir(root.AllPaths[selectedPath]);
				}
				else if (copyObj.m_browse && justEnter)
				{
					copyObj.SelectDir(root.AllPaths[selectedPath]);
//					filesScreen = true;
				}
        else if (root.AllPaths[selectedPath]->IsExpanded && root.AllPaths[selectedPath]->FileSpecFiles.size() == 0)
        {
          // don't enter if empty or filespec empty
        }
				else
				{
					int d = 1;
					d++;
					root.Compile();
					if (root.AllPaths[selectedPath]->IsExpanded)
					{
						filesScreen = true;

						fSelectedPath = 0;
						fViewStart = 0;
						fMaxViewEnd = 43;
						fViewEnd = 43;
					}
					else
					{
						root.AllPaths[selectedPath]->Expand();
						root.Compile();
					}

					maxSelPath = root.AllPaths.size();
//					fMaxSelPath = root.AllPaths[selectedPath]->Files.size();
					fMaxSelPath = root.AllPaths[selectedPath]->FileSpecFiles.size();

				}

			}
		}
    }
	else
	{
		enterDown = false;
	}

	if (fileSpec.m_escPressed)
	{
		fileSpec.m_activated = false;
	}
	if (!copyObj.m_copyStep && copyObj.m_enterPressed)
	{
		copyObj.m_enterPressed = false;
	}
  searchObj.RenderCursor();

    if (!otherActive && filesScreen)
    {
      if (!shift && (0x8000 & GetAsyncKeyState((unsigned char)(0x56/*V*/))) != 0)
      {
        if (!vDown && !ctrlVDown)
        {
          vDown = true;

          std::wstringstream wstr;
          wstr << L" " << root.AllPaths[selectedPath]->Files[fSelectedPath];

          std::wstring ws = wstr.str();
          std::string str = ws2s(ws);
          LPSTR s = const_cast<char *>(str.c_str());
//          startup("c:\\windows\\notepad.exe", s);

          pathToFile = ws2s(root.AllPaths[selectedPath]->Files[fSelectedPath]);
          theFile = pathToFile.substr(pathToFile.rfind("\\")+1);
          if (StrUtil::EndsWith(StrUtil::s2ws(theFile), L".diz") || StrUtil::EndsWith(StrUtil::s2ws(theFile), L".txt") || StrUtil::EndsWith(StrUtil::s2ws(theFile), L".cs") || StrUtil::EndsWith(StrUtil::s2ws(theFile), L".csx") || StrUtil::EndsWith(StrUtil::s2ws(theFile), L".log") || StrUtil::EndsWith(StrUtil::s2ws(theFile), L".bat"))
          {
            searchObj.m_renderAscii = true;
            searchObj.m_wordwrap = true;

            // Force reloading gap buffer.
            searchObj.SetupTest(StrUtil::s2ws(pathToFile));
          }
          else
          {
            searchObj.m_renderAscii = false;
          }
          
          thestart = 0;
          testReadFile = true;
          viewFile = true;
		  rmemblock2 = NULL;
          ReadFile();

          int a = 1;
          a++;
        }
      }
      else
      {
        vDown = false;
      }
    }

    

    if (!otherActive && ((0x8000 & GetAsyncKeyState((unsigned char)(VK_SUBTRACT))) != 0  || (0x8000 & GetAsyncKeyState((unsigned char)(VK_OEM_MINUS))) != 0))
    {
		if (!filesScreen)
		{
			root.Compile();
			root.AllPaths[selectedPath]->Collapse();
			root.Compile();

			maxSelPath = root.AllPaths.size();
		}
    }

	if (!otherActive && shift && ((0x8000 & GetAsyncKeyState((unsigned char)(0x38/*8*/))) != 0 || (0x8000 & GetAsyncKeyState((unsigned char)(VK_MULTIPLY))) != 0))
	{
		if (!filesScreen)
		{
			root.Compile();
			root.AllPaths[selectedPath]->ExpandAll();
			root.Compile();

			maxSelPath = root.AllPaths.size();
		}
	}

    if (!otherActive && (0x8000 & GetAsyncKeyState((unsigned char)(VK_LEFT))) != 0)
    {
		if (!filesScreen)
		{
      int newSelectedPath = selectedPath;
			root.Compile();
			if (root.AllPaths[selectedPath]->Parent != NULL)
			{
				DirObject* parent = root.AllPaths[selectedPath]->Parent;
				for (int i = 0; i < root.AllPaths.size(); ++i)
				{
					if (root.AllPaths[i] == parent)
					{
            newSelectedPath = i;
						break;
					}
				}
			}
			root.Compile();

			maxSelPath = root.AllPaths.size();


      while (selectedPath != newSelectedPath)
      {
        selectedPath--;
        if (selectedPath < 0)
        {
          selectedPath = 0;
          viewStart = 0;
          viewEnd = maxViewEnd;
        }
        else if (selectedPath >= viewStart + 1)
        {
          ;
        }
        else if (selectedPath >= 1)
        {
          viewEnd--;
          if (viewEnd < maxViewEnd)
          {
            viewEnd = maxViewEnd;
          }
          viewStart--;
          if (viewStart < 0)
          {
            viewStart = 0;
          }
        }
      }




		}

    }

    searchObj.RenderCursor();
	if (!otherActive && !shift && (0x8000 & GetAsyncKeyState((unsigned char)(0x57/*W*/))) != 0)
	{
		root.WriteToFile();
	}


//	if (!otherActive &&keyTestUp && !filesScreen)
		if (!otherActive &&numUp>0 && !filesScreen)
		{
			for (int i = 0; i < numUp; ++i)
			{
				selectedPath--;
				if (selectedPath < 0)
				{
					selectedPath = 0;
					viewStart = 0;
					viewEnd = maxViewEnd;
				}
				else if (selectedPath >= viewStart + 1)
				{
					;
				}
				else if (selectedPath >= 1)
				{
					viewEnd--;
					if (viewEnd < maxViewEnd)
					{
						viewEnd = maxViewEnd;
					}
					viewStart--;
					if (viewStart < 0)
					{
						viewStart = 0;
					}
				}
			}
	  }
	  //if (!otherActive &&keyTestDown && !filesScreen)
		  if (!otherActive &&numDown>0 && !filesScreen)
		  {
			  for (int i = 0; i < numDown; ++i)
			  {
				  selectedPath++;
				  if (selectedPath >= maxSelPath)
				  {
					  //			  selectedPath = maxSelPath;
					  selectedPath--;
				  }
				  else if (selectedPath <= viewEnd)
				  {
					  ;
				  }
				  else if (selectedPath >= maxViewEnd)
				  {
					  viewEnd++;
					  viewStart++;
				  }
			}


    }

    if (!otherActive && pdDown && !filesScreen)
    {
      for (int i=0;i<34;++i)
      {
        selectedPath++;
        if (selectedPath >= maxSelPath)
        {
          //			  selectedPath = maxSelPath;
          selectedPath--;
        }
        else if (selectedPath <= viewEnd)
        {
          ;
        }
        else if (selectedPath >= maxViewEnd)
        {
          viewEnd++;
          viewStart++;
        }
      }
    }
    if (!otherActive && puDown && !filesScreen)
    {
      for (int i = 0; i < 34; ++i)
      {
        selectedPath--;
        if (selectedPath < 0)
        {
          selectedPath = 0;
          viewStart = 0;
          viewEnd = maxViewEnd;
        }
        else if (selectedPath >= viewStart + 1)
        {
          ;
        }
        else if (selectedPath >= 1)
        {
          viewEnd--;
          if (viewEnd < maxViewEnd)
          {
            viewEnd = maxViewEnd;
          }
          viewStart--;
          if (viewStart < 0)
          {
            viewStart = 0;
          }
        }
      }
    }

	fMaxSelPath = root.AllPaths[selectedPath]->FileSpecFiles.size();
	if (fSelectedPath >= fMaxSelPath)
	{
		fSelectedPath = fMaxSelPath - 1;
	}

	//if (!otherActive &&keyTestUp && filesScreen)
	if (!otherActive &&numUp > 0 && filesScreen)
	{
		for (int i = 0; i < numUp; ++i)
		{
			fSelectedPath--;
			if (fSelectedPath < 0)
			{
				fSelectedPath = 0;
				fViewStart = 0;
				fViewEnd = fMaxViewEnd;
			}
			else if (fSelectedPath >= fViewStart + 1)
			{
				;
			}
			else if (fSelectedPath >= 1)
			{
				fViewEnd--;
				if (fViewEnd < fMaxViewEnd)
				{
					fViewEnd = fMaxViewEnd;
				}
				fViewStart--;
				if (fViewStart < 0)
				{
					fViewStart = 0;
				}
			}
		}
	}

//	if (!otherActive && keyTestDown && filesScreen)
	      if (!otherActive && numDown>0 && filesScreen)
{
			  for (int i = 0; i < numDown; ++i)
			  {
				  fSelectedPath++;
				  if (fSelectedPath >= fMaxSelPath)
				  {
					  fSelectedPath--;
				  }
				  else if (fSelectedPath <= fViewEnd - 1)
				  {
					  ;
				  }
				  else if (fSelectedPath >= fMaxViewEnd)
				  {
					  fViewEnd++;
					  fViewStart++;
				  }
			  }
      }



      if (!otherActive && pdDown && filesScreen)
      {
        for (int i = 0; i<43; ++i)
        {
          fSelectedPath++;
          if (fSelectedPath >= fMaxSelPath)
          {
            fSelectedPath--;
          }
          else if (fSelectedPath <= fViewEnd - 1)
          {
            ;
          }
          else if (fSelectedPath >= fMaxViewEnd)
          {
            fViewEnd++;
            fViewStart++;
          }
        }
      }
      if (!otherActive && puDown && filesScreen)
      {
        for (int i = 0; i < 43; ++i)
        {
          fSelectedPath--;
          if (fSelectedPath < 0)
          {
            fSelectedPath = 0;
            fViewStart = 0;
            fViewEnd = fMaxViewEnd;
          }
          else if (fSelectedPath >= fViewStart + 1)
          {
            ;
          }
          else if (fSelectedPath >= 1)
          {
            fViewEnd--;
            if (fViewEnd < fMaxViewEnd)
            {
              fViewEnd = fMaxViewEnd;
            }
            fViewStart--;
            if (fViewStart < 0)
            {
              fViewStart = 0;
            }
          }
        }
      }

      if (!otherActive && !filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_SHIFT))) != 0)
      {
        for (int i = 0x41; i < 0x5A; ++i)
        {
          if ((0x8000 & GetAsyncKeyState((unsigned char)(i))) != 0 && CharIsPressed(i))
          {
            std::string key = m_writtenKey;
            int c = 1;
            c++;

            bool first = true;
            while (selectedPath < maxSelPath)
            {

              //https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a
              //https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case

              std::string s = root.AllPaths[selectedPath]->GetName();

              std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return std::toupper(c); });

              if (!first && s.rfind(key, 0) == 0)
              { 
                // pos=0 limits the search to the prefix
                // s starts with prefix
                
                // found it
                break;
              }
              else
              {
                first = false;
                selectedPath++;
                if (selectedPath >= maxSelPath)
                {
                  selectedPath--;
                  break;
                }
                else if (selectedPath <= viewEnd)
                {
                  ;
                }
                else if (selectedPath >= maxViewEnd)
                {
                  viewEnd++;
                  viewStart++;
                }
              }

            }
            
          }
        }
      }

      if (!otherActive && !filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_HOME))) != 0)
      {
        while (selectedPath > 0)
        {
          selectedPath--;
          if (selectedPath < 0)
          {
            selectedPath = 0;
            viewStart = 0;
            viewEnd = maxViewEnd;
          }
          else if (selectedPath >= viewStart + 1)
          {
            ;
          }
          else if (selectedPath >= 1)
          {
            viewEnd--;
            if (viewEnd < maxViewEnd)
            {
              viewEnd = maxViewEnd;
            }
            viewStart--;
            if (viewStart < 0)
            {
              viewStart = 0;
            }
          }
        }
      }

      if (!otherActive && !filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_END))) != 0)
      {
        while (selectedPath < maxSelPath)
        {
          selectedPath++;
          if (selectedPath >= maxSelPath)
          {
            selectedPath--;
            break;
          }
          else if (selectedPath <= viewEnd)
          {
            ;
          }
          else if (selectedPath >= maxViewEnd)
          {
            viewEnd++;
            viewStart++;
          }
        }
      }


      if (!otherActive &&filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_SHIFT))) != 0)
      {
        for (int i = 0x41; i < 0x5A; ++i)
        {
          if ((0x8000 & GetAsyncKeyState((unsigned char)(i))) != 0 && CharIsPressed(i))
          {
            std::wstring key = s2ws(m_writtenKey);
            int c = 1;
            c++;

            bool first = true;
            while (fSelectedPath < fMaxSelPath)
            {

              //https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a
              //https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case

              std::wstring s = root.AllPaths[selectedPath]->GetFileNameW(fSelectedPath);

              std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return std::toupper(c); });

              if (!first && s.rfind(key, 0) == 0)
              {
                // pos=0 limits the search to the prefix
                // s starts with prefix

                // found it
                break;
              }
              else
              {
                first = false;

                fSelectedPath++;
                if (fSelectedPath >= fMaxSelPath)
                {
                  fSelectedPath--;
                  break;
                }
                else if (fSelectedPath <= fViewEnd - 1)
                {
                  ;
                }
                else if (fSelectedPath >= fMaxViewEnd)
                {
                  fViewEnd++;
                  fViewStart++;
                }

              }

            }

          }
        }
      }


      if (!otherActive &&filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_HOME))) != 0)
      {
        while (fSelectedPath > 0)
        {

          fSelectedPath--;
          if (fSelectedPath < 0)
          {
            fSelectedPath = 0;
            fViewStart = 0;
            fViewEnd = fMaxViewEnd;
          }
          else if (fSelectedPath >= fViewStart + 1)
          {
            ;
          }
          else if (fSelectedPath >= 1)
          {
            fViewEnd--;
            if (fViewEnd < fMaxViewEnd)
            {
              fViewEnd = fMaxViewEnd;
            }
            fViewStart--;
            if (fViewStart < 0)
            {
              fViewStart = 0;
            }
          }

        }
      }

      if (!otherActive && filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_END))) != 0)
      {
        while (fSelectedPath < fMaxSelPath)
        {
          fSelectedPath++;
          if (fSelectedPath >= fMaxSelPath)
          {
            fSelectedPath--;
            break;
          }
          else if (fSelectedPath <= fViewEnd - 1)
          {
            ;
          }
          else if (fSelectedPath >= fMaxViewEnd)
          {
            fViewEnd++;
            fViewStart++;
          }
        }
      }
	  
	  if (!otherActive && filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_SPACE))) != 0)
	  {
		  if (!spaceDown)
		  {
			  spaceDown = true;
//			  openWith.SetPath(root.AllPaths[selectedPath]->Files[fSelectedPath]);
			  openWith.SetPath(root.AllPaths[selectedPath]->FileSpecFiles[fSelectedPath]);
			  openWith.Toggle();
		  }
	  }
	  else
	  {
		  spaceDown = false;
	  }

	  if (!otherActive && !filesScreen && (0x8000 & GetAsyncKeyState((unsigned char)(VK_SPACE))) != 0)
	  {
		  if (!spaceDownDir)
		  {
			  spaceDownDir = true;
			  bookmarks.Toggle();
		  }
	  }
	  else
	  {
		  spaceDownDir = false;
	  }
    searchObj.RenderCursor();
	  

	  // Game Logic ===================

	  // Handle player movement
	  nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
	  nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
	  nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

	  // Rotate, but latch to stop wild spinning
	  if (bKey[3])
	  {
		  nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
		  bRotateHold = false;
	  }
	  else
		  bRotateHold = true;

	  // Force the piece down the playfield if it's time
	  if (bForceDown)
	  {
		  // Update difficulty every 50 pieces
		  nSpeedCount = 0;
		  nPieceCount++;
		  if (nPieceCount % 50 == 0)
			  if (nSpeed >= 10) nSpeed--;

		  // Test if piece can be moved down
		  if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			  nCurrentY++; // It can, so do it!
		  else
		  {
			  // It can't! Lock the piece in place
			  for (int px = 0; px < 4; px++)
				  for (int py = 0; py < 4; py++)
					  if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
						  pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

			  // Check for lines
			  for (int py = 0; py < 4; py++)
				  if (nCurrentY + py < nFieldHeight - 1)
				  {
					  bool bLine = true;
					  for (int px = 1; px < nFieldWidth - 1; px++)
						  bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

					  if (bLine)
					  {
						  // Remove Line, set to =
						  for (int px = 1; px < nFieldWidth - 1; px++)
							  pField[(nCurrentY + py) * nFieldWidth + px] = 8;
						  vLines.push_back(nCurrentY + py);
					  }
				  }

			  nScore += 25;
			  if (!vLines.empty())	nScore += (1 << vLines.size()) * 100;

			  // Pick New Piece
			  nCurrentX = nFieldWidth / 2;
			  nCurrentY = 0;
			  nCurrentRotation = 0;
			  nCurrentPiece = rand() % 7;

			  // If piece does not fit straight away, game over!
			  bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
		  }
	  }
    searchObj.RenderCursor();
	  // Display ======================

	  // Draw Field
	  for (int x = 0; x < nFieldWidth; x++)
		  for (int y = 0; y < nFieldHeight; y++)
			  screen[(y + 2)*nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y*nFieldWidth + x]];

	  const wchar_t* lines = L"┌─────────────────────────────────────────────────────────┬────────────────────┐";
	  for (int x = 0; x < wcslen(lines); x++)
	  {
		  //		screen[x] = lines[x];
	  }

	  // Draw Current Piece
	  for (int px = 0; px < 4; px++)
		  for (int py = 0; py < 4; py++)
			  if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
				  screen[(nCurrentY + py + 2)*nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

	  // Draw Score
	  swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

	  // Animate Line Completion
	  if (!vLines.empty())
	  {
		  // Display Frame (cheekily to draw lines)
		  WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		  this_thread::sleep_for(400ms); // Delay a bit

		  for (auto &v : vLines)
			  for (int px = 1; px < nFieldWidth - 1; px++)
			  {
				  for (int py = v; py > 0; py--)
					  pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
				  pField[px] = 0;
			  }

		  vLines.clear();
	  }

	  // Display Frame
  //    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    searchObj.RenderCursor();

	  //https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
	  writeRegion;
	  writeRegion.Left = 0;
	  writeRegion.Top = 0;
	  writeRegion.Right = nScreenWidth * sizeof(CHAR_INFO);
	  writeRegion.Bottom = nScreenHeight * sizeof(CHAR_INFO);

	  //ReadConsoleOutput(
	  //	hConsole,        // screen buffer to read from
	  //	chiBuffer,      // buffer to copy into
	  //	coordBufSize,   // col-row size of chiBuffer
	  //	coordBufCoord,  // top left dest. cell in chiBuffer
	  //	&srctReadRect); // screen buffer source rectangle


  //	//DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 15, L"A", FG_RED);
  ////	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"┌─────────────────────────────────────────────────────────┬────────────────────┐A┌─────────────────────────────────────────────────────────┬────────────────────B┌─────────────────────────────────────────────────────────┬────────────────────", FG_RED| BG_CYAN);
  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L"┌─────────────────────────────────────────────────────────┬────────────────────┐", FG_RED | BG_CYAN);
  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"A┌─────────────────────────────────────────────────────────┬────────────────────", FG_RED| BG_CYAN);
  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, L"B┌─────────────────────────────────────────────────────────┬────────────────────", FG_RED | BG_CYAN);


//	  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L" Path: D:\\Program Files\\EXTREME64                       Su 18-07-21 12:54:09 pm", FG_GREY | BG_BLACK);
//	  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L" Path:                                                  Su 18-07-21 12:54:09 pm", FG_GREY | BG_BLACK);


		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 0, L" Path:                                                  ", FG_GREY | BG_BLACK);
		  DrawTime();

		  std::wstring p = root.AllPaths[selectedPath]->PathW();
		  std::wstringstream buf;
		  if (p.length() > 47)
		  {
			  buf << p.substr(0, 47) << L"→";
		  }
		  else
		  {
			  buf << p;
		  }

		  if (filesScreen)
		  {
			  buf.str(L"");
			  if (p.length() > 47)
			  {
				  buf << p.substr(0, 24) << L"→" << p.substr(p.length() - 23, 23);
			  }
			  else
			  {
				  buf << p;
			  }
		  }

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 7, 0, buf.str(), FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 1, L"┌─────────────────────────────────────────────────────────┬────────────────────┐", FG_GREY | BG_BLACK);

		  bool isSelected = selectedPath == 0;
		  int rootStart = 0;
		  //	  if (viewStart <= 0 && viewEnd >= 0) //in view
		  if (true)
		  {
			  std::wstring fs = L"*.*";
			  if (fileSpec.m_typed != L"")
			  {
				  fs = fileSpec.m_typed;
			  }

			  if (!isSelected)
			  {
				  std::wstringstream buf;
				  buf << L"│ " << s2ws(root.Path) << L"                                                     │FILE  *.*           │";
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, buf.str(), FG_GREY | BG_BLACK);
				  buf.str(L"");
				  buf << s2ws(root.Path);
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, 2, buf.str(), FG_CYAN | BG_BLACK);
				  //			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, L"│ D:\\                                                     │FILE  *.*           │", FG_GREY | BG_BLACK);
				  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, 2, L"D:\\", FG_CYAN | BG_BLACK);
	//			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 2, L"*.*", FG_CYAN | BG_BLACK);
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 2, fs, FG_CYAN | BG_BLACK);
			  }
			  else
			  {
				  std::wstringstream buf;
				  buf << L"│ " << s2ws(root.Path) << L"                                                     │FILE  *.*           │";
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, buf.str(), FG_GREY | BG_BLACK);

				  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 2, L"│ D:\\                                                     │FILE  *.*           │", FG_GREY | BG_BLACK);
	//			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 2, L"*.*", FG_CYAN | BG_BLACK);
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 2, fs, FG_CYAN | BG_BLACK);

				  buf.str(L"");
				  buf << " " << s2ws(root.Path) << L"                                                     ";
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, 2, buf.str(), FG_DARK_GREY | BG_GREY);

				  //			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, 2, L" D:\\                                                     ", FG_DARK_GREY | BG_GREY);
			  }
		  }
		  else
		  {
			  rootStart++;
		  }



		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 3, L"│+├──$RECYCLE.BIN                                         ├────────────────────┤", FG_GREY | BG_BLACK);
		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, 3, L"+├──$RECYCLE.BIN", FG_CYAN | BG_BLACK);

		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 4, L"│+├──4K Videos                                            │DISK  D:            │", FG_GREY | BG_BLACK);
		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, 4, L"+├──4K Videos", FG_CYAN | BG_BLACK);
		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 4, L"D:", FG_CYAN | BG_BLACK);

		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, 5, L"│+├──angular workshops                                    │ Available Bytes    │", FG_GREY | BG_BLACK);
		  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, 5, L"+├──angular workshops", FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 3, L"├────────────────────┤", FG_GREY | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 4, L"│DISK                │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 4, root.PathW().substr(0, 2), FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 5, L"│ Available Bytes    │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 6, L"│                    │", FG_GREY | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 61, 6, L"1,152,957,759,488", FG_CYAN | BG_BLACK);
		  std::wstring freeDisk = FreeDiskSpace::GetFreeDiskSpace(root.PathW());
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 59 + (19 - freeDisk.length()), 6, freeDisk, FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 7, L"├────────────────────┤", FG_GREY | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 8, L"│                    │", FG_GREY | BG_BLACK);
		  if (!filesScreen)
		  {
			  std::wstring padded = GetPaddedString(L"DISK               ", selectedPath + 1);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 59, 8, padded, FG_CYAN | BG_BLACK);
		  }
		  else
		  {
			  std::wstring padded = GetPaddedString(L"DIRECTORY          ", fSelectedPath + 1);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 59, 8, padded, FG_CYAN | BG_BLACK);
		  }

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 9, L"│ Total              │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 10, L"│  Files             │", FG_GREY | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 76, 10, L"19", FG_CYAN | BG_BLACK);

		  std::wstring numFiles = root.NumFilesRecursiveW();
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 59 + 8 + (11 - numFiles.length()), 10, numFiles, FG_CYAN | BG_BLACK);



		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 11, L"│  Gigas             │", FG_GREY | BG_BLACK);
		  ////	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 11, L"25.81351073", FG_CYAN | BG_BLACK);
		  //
		  unsigned long long fileSize = root.GetAllFilesSize();
		  //	
		  //	unsigned long long bytesInGB = 1000000000;
		  //	double theSize = (double)fileSize / (double)bytesInGB;
		  //	//int iSize = (int)theSize;
		  //	//std::wstringstream bufI;
		  //	//bufI << iSize;
		  //	//int len = bufI.str().length();
		  //
		  //	std::wstringstream bufFS;
		  //	bufFS << std::setprecision(10) << theSize;
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 11, bufFS.str(), FG_CYAN | BG_BLACK);

		  if (fileSize >= 1000000000)
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 11, L"│  Gigas             │", FG_GREY | BG_BLACK);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 11, DirSizeToGigs(fileSize), FG_CYAN | BG_BLACK);
		  }
		  else
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 11, L"│  Bytes             │", FG_GREY | BG_BLACK);
			  std::wstring dirSizeBytes = DirSizeToBytes(fileSize);
			  int numSpaces = 0;
			  if (dirSizeBytes.length() < 11)
			  {
				  numSpaces = 11 - dirSizeBytes.length();
			  }
			  std::wstringstream bytesBuf;
			  for (int i = 0; i < numSpaces; ++i)
			  {
				  bytesBuf << L" ";
			  }
			  bytesBuf << dirSizeBytes;
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 11, bytesBuf.str(), FG_CYAN | BG_BLACK);
		  }

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 12, L"│ Matching           │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 13, L"│  Files             │", FG_GREY | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 76, 13, L"19", FG_CYAN | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 59 + 8 + (11 - numFiles.length()), 13, numFiles, FG_CYAN | BG_BLACK);


		  //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 14, L"│  Gigas             │", FG_GREY | BG_BLACK);
		  ////	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 14, L"25.81351073", FG_CYAN | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 14, bufFS.str(), FG_CYAN | BG_BLACK);

		  if (fileSize >= 1000000000)
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 14, L"│  Gigas             │", FG_GREY | BG_BLACK);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 14, DirSizeToGigs(fileSize), FG_CYAN | BG_BLACK);
		  }
		  else
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 14, L"│  Bytes             │", FG_GREY | BG_BLACK);
			  std::wstring dirSizeBytes = DirSizeToBytes(fileSize);
			  int numSpaces = 0;
			  if (dirSizeBytes.length() < 11)
			  {
				  numSpaces = 11 - dirSizeBytes.length();
			  }
			  std::wstringstream bytesBuf;
			  for (int i = 0; i < numSpaces; ++i)
			  {
				  bytesBuf << L" ";
			  }
			  bytesBuf << dirSizeBytes;
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 14, bytesBuf.str(), FG_CYAN | BG_BLACK);
		  }

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 15, L"│ Tagged             │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 16, L"│  Files             │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 77, 16, L"0", FG_CYAN | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 17, L"│  Bytes             │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 77, 17, L"0", FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 18, L"│ Current Directory  │", FG_GREY | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 19, L"│                    │", FG_GREY | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 61, 19, L"D:\\", FG_CYAN | BG_BLACK);
		  std::wstring currentDir = root.AllPaths[selectedPath]->PathW();
		  if (selectedPath != 0)
		  {
			  if (currentDir.rfind(L"\\") != std::wstring::npos)
			  {
				  currentDir = currentDir.substr(currentDir.rfind(L"\\") + 1, currentDir.length() - currentDir.rfind(L"\\"));
			  }
			  if (currentDir.length() > 17)
			  {
				  currentDir = currentDir.substr(0, 17);
			  }
		  }
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 61, 19, currentDir, FG_CYAN | BG_BLACK);
		  unsigned long long dirSize = root.AllPaths[selectedPath]->DirSize;
		  if (dirSize >= 1000000000)
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 20, L"│  Gigas             │", FG_GREY | BG_BLACK);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 20, DirSizeToGigs(dirSize), FG_CYAN | BG_BLACK);
		  }
		  else
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 20, L"│  Bytes             │", FG_GREY | BG_BLACK);
			  std::wstring dirSizeBytes = DirSizeToBytes(dirSize);
			  int numSpaces = 0;
			  if (dirSizeBytes.length() < 11)
			  {
				  numSpaces = 11 - dirSizeBytes.length();
			  }
			  std::wstringstream bytesBuf;
			  for (int i = 0; i < numSpaces; ++i)
			  {
				  bytesBuf << L" ";
			  }
			  bytesBuf << dirSizeBytes;
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 20, bytesBuf.str(), FG_CYAN | BG_BLACK);
		  }

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 21, L"│  Attr              │", FG_GREY | BG_BLACK);

		  std::wstring attrib = root.AllPaths[selectedPath]->GetAttributes();
		  int numSpaces = 0;
		  if (attrib.length() < 13)
		  {
			  numSpaces = 13 - attrib.length();
		  }
		  std::wstringstream attribBuf;
		  for (int i = 0; i < numSpaces; ++i)
		  {
			  attribBuf << L" ";
		  }
		  attribBuf << attrib;

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 65, 21, attribBuf.str(), FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 22, L"│  Create            │", FG_GREY | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 68, 22, L"06-02-4341", FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 68, 22, root.AllPaths[selectedPath]->GetCreatedDate(), FG_CYAN | BG_BLACK);

		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 23, L"│  Time              │", FG_GREY | BG_BLACK);
		  //	DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 68, 23, L"7:29:22 am", FG_CYAN | BG_BLACK);
		  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 67, 23, root.AllPaths[selectedPath]->GetCreatedTime(), FG_CYAN | BG_BLACK);

		  int start = 24;
		  for (int i = 0; i < 13; ++i)
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, start + i, L"│                    │", FG_GREY | BG_BLACK);
		  }
		  int bottomPanelY = 0;
		  int scrollY = 0;
		  if (!filesScreen)
		  {
			  bottomPanelY = 13 + start;

			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, bottomPanelY, L"├─────────────────────────────────────────────────────────┴────────────────────┤", FG_GREY | BG_BLACK);

			  start = bottomPanelY + 1;
			  for (int i = 0; i < 8; ++i)
			  {
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + i, L"│                                                                              │", FG_GREY | BG_BLACK);
			  }
			  scrollY = 8 + start;

			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, scrollY, L"└▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┘", FG_GREY | BG_BLACK);

		  }
		  else
		  {
			  bottomPanelY = 13 + start;
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, bottomPanelY, L"│                                                                              │", FG_GREY | BG_BLACK);

			  for (int i = 0; i < 9; ++i)
			  {
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 58, 13 + start + i, L"│                    │", FG_GREY | BG_BLACK);
			  }
			  int bottomY = 9 + 13 + start;
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, bottomY, L"└▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┴────────────────────┘", FG_GREY | BG_BLACK);

        for (int i = 2; i <= 45; ++i)
        {
          // some reason this needs to be redrawn after exiting hex view
          DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, i, L"│", FG_GREY | BG_BLACK);
        }
		  }

		  /*
			//	for (int i = 6; i < dPaths.size() + 6; ++i)
			int numDirs = dPaths.size();
		//	if (numDirs > 34)
		//		numDirs = 34;
			int countDir = 0;
			int displayInt = 2;
			for (int i = 0; i < numDirs; ++i)
			{
				countDir++;
				if (countDir >= viewStart+1 && countDir <= viewEnd)
				{
					displayInt++;
					DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"                                                         ", FG_CYAN | BG_BLACK);
					std::wstringstream buf;
					buf << L"│+├──" << s2ws(dPaths[i]);
					DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, displayInt, buf.str(), FG_GREY | BG_BLACK);
					std::wstringstream buf2;
					buf2 << L"+├──" << s2ws(dPaths[i]);
					DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_CYAN | BG_BLACK);

					if (countDir == selectedPath)
					{
						DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"                                                         ", FG_DARK_GREY | BG_GREY);

						DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_DARK_GREY | BG_GREY);
					}
				}
			}
		  */


		  //  int numDirs = root.ChildrenPaths.size();

			//don't know about how to preserve tags here:
		  //	root.Compile();

		  int numDirs = root.AllPaths.size();
		  int countDir = 0;
		  int displayInt = 2;
		  for (int i = 1; i < numDirs; ++i)
		  {
			  countDir++;
			  if (countDir >= viewStart + 1 && countDir <= viewEnd)
			  {
				  displayInt++;
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"                                                         ", FG_CYAN | BG_BLACK);

				  std::wstringstream buf;
				  std::wstringstream buf2;

				  std::wstringstream buf3;
				  std::wstringstream buf4l;
				  //│ ├──

				  //│ │  ├──

				  //│+│  │  ├──


				  int numSlash = 0;
				  for (int c = 0; c < root.AllPaths[i]->Path.length(); ++c)
				  {
					  if (root.AllPaths[i]->Path[c] == '\\')
					  {
						  numSlash++;
					  }
				  }
				  int nextSlash = 0;
				  if (root.AllPaths.size() >= i + 2)
				  {
					  for (int c = 0; c < root.AllPaths[i + 1]->Path.length(); ++c)
					  {
						  if (root.AllPaths[i + 1]->Path[c] == '\\')
						  {
							  nextSlash++;
						  }
					  }
				  }

				  //bool lastOfParent = false;
				  //if (root.AllPaths[i]->Parent != NULL)
				  //{
				  //  if (root.AllPaths[i] == &root.AllPaths[i]->Parent->ChildrenPaths[root.AllPaths[i]->Parent->ChildrenPaths.size() - 1])
				  //  {
				  //    lastOfParent = true;
				  //    int a = 1;
				  //    a++;
				  //  }
				  //}

				  bool lastOfParent = root.AllPaths[i]->IsLastOfParent();
				  bool parentLastOfParent = false;
				  if (root.AllPaths[i]->Parent != NULL)
				  {
					  parentLastOfParent = root.AllPaths[i]->Parent->IsLastOfParent();
				  }

				  bool parentLastOfParent2 = false;
				  if (root.AllPaths[i]->Parent != NULL && root.AllPaths[i]->Parent->Parent != NULL)
				  {
					  parentLastOfParent2 = root.AllPaths[i]->Parent->Parent->IsLastOfParent();
				  }

				  //      buf3 << L"│";
				  if (root.AllPaths[i]->IsExpanded)
				  {
					  buf3 << L" ";
				  }
				  else
				  {
					  buf3 << L"+";
				  }

				  std::vector<bool> showDown;
				  DirObject* current = root.AllPaths[i];
				  while (current != &root)
				  {
					  current = current->Parent;
					  showDown.push_back(current->IsLastOfParent());
				  }
				  for (int c = showDown.size() - 2; c >= 0; --c)
				  {
					  if (showDown[c])
					  {
						  buf3 << L"   ";
					  }
					  else
					  {
						  buf3 << L"│  ";
					  }
				  }
				  if (lastOfParent)
				  {
					  buf3 << L"└──";
				  }
				  else
				  {
					  buf3 << L"├──";
				  }

				  for (int c = 0; c < numSlash; ++c)
				  {
					  //        ParseParents(c, numSlash, buf3, root.AllPaths[i]);



							  //if (c == numSlash - 1)
							  //{
							  //  if (lastOfParent)
							  //  {
							  //    buf3 << L"└──";
							  //  }
							  //  else
							  //  {
							  //    buf3 << L"├──";
							  //  }
							  //}
							  //else if (c == numSlash - 2)
							  //{
							  //  if (parentLastOfParent)
							  //  {
							  //    buf3 << L"   ";
							  //  }
							  //  else
							  //  {
							  //    buf3 << L"│  ";
							  //  }
							  //}
							  //else if (c == numSlash - 3)
							  //{
							  //  if (parentLastOfParent2)
							  //  {
							  //    buf3 << L"   ";
							  //  }
							  //  else
							  //  {
							  //    buf3 << L"│  ";
							  //  }
							  //}
							  //else
							  //{
							  //  if (parentLastOfParent)
							  //  {
							  //    buf3 << L"   ";
							  //  }
							  //  else if (lastOfParent)
							  //  {
							  //    buf3 << L"│  ";
							  //  }
							  //  else
							  //  {
							  //    buf3 << L"│  ";
							  //  }
							  //}
				  }

				  if (root.AllPaths[i]->IsExpanded)
				  {
					  buf4l << L" ";
				  }
				  else
				  {
					  buf4l << L"+";
				  }
				  for (int c = 0; c < numSlash; ++c)
				  {
					  if (c == numSlash - 1)
					  {
						  buf4l << L"└──";
					  }
					  else
					  {
						  buf4l << L"   ";
					  }
				  }

				  if (!filesScreen)
				  {
					  std::wstring name = root.AllPaths[i]->GetNameW();
					  if (name.length() > 57 - buf3.str().length())
					  {
						  name = name.substr(0, 57 - buf3.str().length());
					  }

					  if (i == numDirs - 1)
					  {
						  //        buf << L"│" << buf4l.str() << s2ws(root.AllPaths[i]->Path);
						  //		  buf << L"│" << buf4l.str() << s2ws(root.AllPaths[i]->GetName());
						  buf << L"│" << buf4l.str() << name;
						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, displayInt, buf.str(), FG_GREY | BG_BLACK);
						  //        buf2 << buf4l.str() << s2ws(root.AllPaths[i]->Path);
						  buf2 << buf4l.str() << s2ws(root.AllPaths[i]->GetName());
						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_CYAN | BG_BLACK);
					  }
					  else
					  {
						  //        buf << L"│" << buf3.str() << s2ws(root.AllPaths[i]->Path);
						  //		  buf << L"│" << buf3.str() << s2ws(root.AllPaths[i]->GetName());
						  buf << L"│" << buf3.str() << name;
						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, displayInt, buf.str(), FG_GREY | BG_BLACK);
						  //        buf2 << buf3.str() << s2ws(root.AllPaths[i]->Path);
						  //		  buf2 << buf3.str() << s2ws(root.AllPaths[i]->GetName());
						  buf2 << buf3.str() << name;
						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_CYAN | BG_BLACK);
					  }
				  }

				  //if (i == numDirs - 1)
				  //{
				  //  buf << L"│+└──" << s2ws(root.AllPaths[i]->Path);
				  //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, displayInt, buf.str(), FG_GREY | BG_BLACK);
				  //  buf2 << L"+└──" << s2ws(root.AllPaths[i]->Path);
				  //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_CYAN | BG_BLACK);
				  //}
				  //else
				  //{
				  //  buf << L"│+├──" << s2ws(root.AllPaths[i]->Path);
				  //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, displayInt, buf.str(), FG_GREY | BG_BLACK);
				  //  buf2 << L"+├──" << s2ws(root.AllPaths[i]->Path);
				  //  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_CYAN | BG_BLACK);
				  //}

				  //std::wstringstream buf;
				  //buf << L"│+├──" << s2ws(root.GetIndexValue(0, i));
				  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, displayInt, buf.str(), FG_GREY | BG_BLACK);
				  //std::wstringstream buf2;
				  //buf2 << L"+├──" << s2ws(root.GetIndexValue(0, i));
				  //DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_CYAN | BG_BLACK);

				  if (!filesScreen)
				  {
					  if (countDir == selectedPath)
					  {
						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"                                                         ", FG_DARK_GREY | BG_GREY);

						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, buf2.str(), FG_DARK_GREY | BG_GREY);
					  }
				  }


			  }
		  }

		  if (!filesScreen)
		  {
			  for (int i = displayInt + 1; i < bottomPanelY; ++i)
			  {
				  //previous drive may have overdraw, so black it out
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, i, L"                                                         ", FG_GREY | BG_BLACK);
			  }

			  start = bottomPanelY + 1;

			  if (root.AllPaths[selectedPath]->IsExpanded)
			  {
				  if (root.AllPaths[selectedPath]->Files.size() == 0)
				  {
					  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, start, L"Dir empty", FG_CYAN | BG_BLACK);
				  }
				  else if (root.AllPaths[selectedPath]->FileSpecFiles.size() == 0)
				  {
					  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, start, L"No Files Match", FG_CYAN | BG_BLACK);
				  }
			  }

			  for (int i = 0; i < 8; ++i)
			  {
				  if (root.AllPaths[selectedPath]->IsExpanded)
				  {
					  //			  if (i < root.AllPaths[selectedPath]->Files.size())
					  if (i < root.AllPaths[selectedPath]->FileSpecFiles.size())
					  {
						  std::wstringstream buf2;
						  //				  buf2 << s2ws(root.AllPaths[selectedPath]->GetFileName(i));
						  buf2 << root.AllPaths[selectedPath]->GetFileNameFileSpecW(i);
						  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, start + i, buf2.str(), FG_CYAN | BG_BLACK);
						  if (root.AllPaths[selectedPath]->GetTag(i))
						  {
							  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, start + i, L"♦", FG_CYAN | BG_BLACK);
						  }
						  else
						  {
							  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, start + i, L" ", FG_CYAN | BG_BLACK);
						  }
					  }

				  }
				  else
				  {
					  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, start + i, L"Dir Not Logged", FG_CYAN | BG_BLACK);
					  break;
				  }
			  }
		  }
		  else
		  {
			  for (int i = displayInt + 1; i < 46; ++i)
			  {
				  //previous drive may have overdraw, so black it out
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, i, L"                                                         ", FG_GREY | BG_BLACK);
			  }

			  int start = 2;
			  std::wstringstream buf5;
			  std::wstring ws;
			  int countDir = 0;
			  int displayInt = 1;
			  //    int numFiles = root.AllPaths[selectedPath]->Files.size();
			  int numFiles = root.AllPaths[selectedPath]->FileSpecFiles.size();

			  //    for (int i = 0; i < 44; ++i)
			  for (int i = 0; i < numFiles; ++i)
			  {
				  countDir++;
				  if (countDir >= fViewStart + 1 && countDir <= fViewEnd + 1)
				  {
					  displayInt++;
					  //if (i < root.AllPaths[selectedPath]->Files.size())
					  if (i < root.AllPaths[selectedPath]->FileSpecFiles.size())
					  {
						  //	std::wstring ws = root.AllPaths[selectedPath]->GetFileNameW(i);
						  std::wstring ws = root.AllPaths[selectedPath]->GetFileNameFileSpecW(i);
						  if (ws.length() > 56)
						  {
							  ws = ws.substr(0, 56);
						  }

						  if (root.AllPaths[selectedPath]->GetTag(i))
						  {
							  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"♦", FG_CYAN | BG_BLACK);
							  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, displayInt, ws, FG_CYAN | BG_BLACK);
						  }
						  else
						  {
							  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, displayInt, ws, FG_CYAN | BG_BLACK);
						  }

						  if (i == fSelectedPath)
						  {
							  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"                                                         ", FG_DARK_GREY | BG_GREY);

							  if (root.AllPaths[selectedPath]->GetTag(i))
							  {
								  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 1, displayInt, L"♦", FG_DARK_GREY | BG_GREY);
								  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, displayInt, ws, FG_DARK_GREY | BG_GREY);
							  }
							  else
							  {
								  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 2, displayInt, ws, FG_DARK_GREY | BG_GREY);
							  }

						  }

					  }
				  }
			  }

			  scrollY = 46;
		  }

		  start = scrollY + 1;
		  if (!filesScreen)
		  {
			  if (!compare.m_browse && !copyObj.m_browse)
			  {
				  //    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"DIR       Avail  Branch  Compare  Delete  Filespec  Global  Invert  Log  Make", FG_GREY | BG_BLACK);
				  //    DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"          A      B       C        D       F         G       I       L    M   ", FG_CYAN | BG_BLACK);
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"DIR       Avail  Compare  Delete  Filespec  Global  Invert  Log  Make ", FG_GREY | BG_BLACK);
				  DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"          A      C        D       F         G       I       L    M   ", FG_CYAN | BG_BLACK);
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"COMMANDS  Open  Pick  Rename  Showall  Tag  Untag  Volume  eXecute  Quit       ", FG_GREY | BG_BLACK);
				  DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"          O     P     R       S        T    U      V        X       Q   ", FG_CYAN | BG_BLACK);
				  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"◄─┘ file  F5 rescan  F6 newview  F8 split    F11 clipboard  F1 help  < > select", FG_GREY | BG_BLACK);
				  DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"◄─┘       F5         F6          F8          F11            F1       < >       ", FG_CYAN | BG_BLACK);

			  }
		  }
		  else
		  {
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"FILE      Attrib  Copy  Delete  Edit  Filespec  Invert  JFC  Log  Move         ", FG_GREY | BG_BLACK);
			  DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start, L"          A       C     D       E     F         I       J    L    M   ", FG_CYAN | BG_BLACK);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"COMMANDS  New date  Open  Pick  Rename  Tag  Untag  View  eXecute  Quit       ", FG_GREY | BG_BLACK);
			  DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 1, L"          N         O     P     R       T    U      V      X       Q   ", FG_CYAN | BG_BLACK);
			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"◄─┘ tree  F5 rescan  F6 newview  F9 menu     F11 clipboard  F1 help  ESC cancel", FG_GREY | BG_BLACK);
			  DrawStringSkipSpace(m_bufScreen, nScreenWidth, nScreenHeight, 0, start + 2, L"◄─┘       F5         F6          F9          F11            F1       ESC       ", FG_CYAN | BG_BLACK);
		  }

		  if (filesScreen)
		  {
			  openWith.Render();
		  }
		  else
		  {
			  bookmarks.Render();
		  }
		  logDrive.Render(start);
		  avail.Render(start);
		  make.Render(start);
		  if (compare.m_browse)
		  {
			  //	  compare.m_selected = root.AllPaths[selectedPath];
			  compare.m_typed = root.AllPaths[selectedPath]->PathW();
		  }
		  if (copyObj.m_browse)
		  {
			  copyObj.m_typed2 = root.AllPaths[selectedPath]->PathW();
		  }
		  compare.Render(start);
		  deleteObj.Render(start);
		  fileSpec.Render(start);
		  copyObj.Render(start);

		  //https://stackoverflow.com/questions/6285270/how-can-i-get-the-mouse-position-in-a-console-program
		  //COORD coord;
		  //switch (InputRecord.EventType)
		  //{
		  //case MOUSE_EVENT: // mouse input 
		  //  if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		  //  {
		  //    coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
		  //    coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;

		  //    DrawString(m_bufScreen, nScreenWidth, nScreenHeight, coord.X, coord.Y, L"X", FG_BLUE | BG_WHITE);
		  //  }
		  //  break;
		  //}

		  if (leftButtonPress)
		  {
			  //    leftButtonPress = false;

			  DrawString(m_bufScreen, nScreenWidth, nScreenHeight, click_coord.X, click_coord.Y, L"X", FG_BLUE | BG_WHITE);
		  }


	}
	  else
	  {
		  //do blinking cursors with no input here:

		  if (!viewFile)
		  {
			  logDrive.Render(47);
			  avail.Render(47);
			  make.Render(47);
			  compare.Render(47);
			  deleteObj.Render(47);
			  fileSpec.Render(47);
			  copyObj.Render(47);
			  DrawTime();
        searchObj.RenderCursor();
		  }
      else
      {
        searchObj.Render();
      }

	  }

//	if (firstGo)
	{

		WriteConsoleOutput(hConsole, m_bufScreen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &writeRegion);
		firstGo = false;
	}

  }

  // Oh Dear
  CloseHandle(hConsole);
  cout << "Game Over!! Score:" << nScore << endl;
  system("pause");
  return 0;
}

