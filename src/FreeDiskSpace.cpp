#include "stdafx.h"
#include "FreeDiskSpace.h"
#include <Windows.h>
#include <sstream>
//#include <locale>
#include <iostream>
//#include <iomanip>

namespace SimpleXTree
{
	//class comma_numpunct : public std::numpunct<char>
	//{
	//protected:
	//	virtual char do_thousands_sep() const
	//	{
	//		return ',';
	//	}

	//	virtual std::string do_grouping() const
	//	{
	//		return "\03";
	//	}
	//};

	std::wstring FreeDiskSpace::GetFreeDiskSpace(std::wstring const& drive)
	{
		//https://stackoverflow.com/questions/11917946/how-do-i-get-available-disk-space-from-windows-using-c
		BOOL  fResult;
		unsigned __int64 i64FreeBytesToCaller,
			i64TotalBytes,
			i64FreeBytes;
		fResult = GetDiskFreeSpaceEx(drive.c_str(),
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		if (fResult)
		{
			printf("\n\nGetDiskFreeSpaceEx reports\n\n");
			printf("Available space to caller = %I64u MB\n",
				i64FreeBytesToCaller / (1024 * 1024));
			printf("Total space               = %I64u MB\n",
				i64TotalBytes / (1024 * 1024));
			printf("Free space on drive       = %I64u MB\n",
				i64FreeBytes / (1024 * 1024));
		}

		//// this creates a new locale based on the current application default
		//// (which is either the one given on startup, but can be overriden with
		//// std::locale::global) - then extends it with an extra facet that 
		//// controls numeric output.
		//std::locale comma_locale(std::locale(), new comma_numpunct());

		//// tell cout to use our new locale.
		//std::cout.imbue(comma_locale);

		//std::cout << std::setprecision(2) << std::fixed << 1000000.1234;

		//https://stackoverflow.com/questions/7276826/c-format-number-with-commas
		auto s = std::to_wstring(i64FreeBytes);
		int n = s.length() - 3;
		while (n > 0) {
			s.insert(n, L",");
			n -= 3;
		}

		std::wstringstream ws;
		ws << s;
		return ws.str();
	}
}
