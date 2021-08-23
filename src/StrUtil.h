#pragma once
#include <string>
#include <vector>

//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string

namespace SimpleXTree
{
  class StrUtil
  {
  public:
    static std::wstring s2ws(const std::string& str);
    static std::string ws2s(const std::wstring& wstr);
	static bool Replace(std::wstring& str, const std::wstring& from, const std::wstring& to);
	static void ReplaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to);
	static std::vector<std::wstring> Tokenize(std::wstring const& line, std::wstring const& delimiter);
	static bool StartsWith(const std::wstring& str, const std::wstring& prefix);
	static bool EndsWith(const std::wstring& str, const std::wstring& suffix);
  static bool EqualsIgnoreCase(std::wstring const& str1, std::wstring const& str2);
  };
}