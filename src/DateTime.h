#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace SimpleXTree
{
	class DateTime
	{
	public:
		static std::wstring GetDateTime();
	private:
		static std::wstring GetWeekDayTwoLetter();
	};
}