#include "stdafx.h"
#include "StrUtil.h"
#include <codecvt>
#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <windows.h>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
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

using std::runtime_error;
using std::string;
using std::vector;
using std::wstring;

//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
namespace SimpleXTree
{
  //https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t/26914562
  int StringToWString(std::wstring &ws, const std::string &s)
  {
    std::wstring wsTmp(s.begin(), s.end());

    ws = wsTmp;

    return 0;
  }





  wstring utf8toUtf16(const string & str)
  {
    if (str.empty())
      return wstring();

    size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
      str.data(), (int)str.size(), NULL, 0);
    if (charsNeeded == 0)
      throw runtime_error("Failed converting UTF-8 string to UTF-16");

    vector<wchar_t> buffer(charsNeeded);
    int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
      str.data(), (int)str.size(), &buffer[0], buffer.size());
    if (charsConverted == 0)
      throw runtime_error("Failed converting UTF-8 string to UTF-16");

    return wstring(&buffer[0], charsConverted);
  }

  std::wstring StrUtil::s2ws(const std::string& str)
  {
    	//using convert_typeX = std::codecvt_utf8<wchar_t>;
    	//std::wstring_convert<convert_typeX, wchar_t> converterX;
    	//return converterX.from_bytes(str);

    //std::wstring ws(str.begin(), str.end());
    ////  StringToWString(ws, str);
    //return std::wstring(ws);

    ////https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t/26914562
    //std::wstring ws(str.size(), L' '); // Overestimate number of code points.
    //size_t converted;
    //ws.resize(::mbstowcs_s(&converted, &ws[0], ws.size(), str.c_str(), str.size())); // Shrink to fit.
    //return ws;

    return utf8toUtf16(str);
  }

  std::string StrUtil::ws2s(const std::wstring& wstr)
  {
    //using convert_typeX = std::codecvt_utf8<wchar_t>;
    //std::wstring_convert<convert_typeX, wchar_t> converterX;
    //return converterX.to_bytes(wstr);

    std::string s(wstr.begin(), wstr.end());
    return std::string(s);
  }

  //https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
  bool StrUtil::Replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
	  size_t start_pos = str.find(from);
	  if (start_pos == std::wstring::npos)
		  return false;
	  str.replace(start_pos, from.length(), to);
	  return true;
  }

  void StrUtil::ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
  {
	  while (Replace(str, from, to))
	  {
		  ;
	  }
  }

  std::vector<std::wstring> StrUtil::Tokenize(std::wstring const& line, std::wstring const& delimiter)
  {
	  //https://www.geeksforgeeks.org/tokenizing-a-string-cpp/

	  // Vector of string to save tokens
	  std::vector<std::wstring> tokens;

	  // stringstream class check1
	  std::wstringstream check1(line);

	  std::wstring intermediate;

	  // Tokenizing w.r.t. space ' '
	  while (std::getline(check1, intermediate, *delimiter.c_str()))
	  {
		  tokens.push_back(intermediate);
	  }
	  return tokens;
  }
}
