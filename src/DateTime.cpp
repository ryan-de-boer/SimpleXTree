#include "stdafx.h"
#include "DateTime.h"
#include <iostream>
#include <chrono>

#include <iostream>       // std::cout, std::ios
#include <sstream>        // std::istringstream
#include <ctime>          // std::tm
#include <locale>         // std::locale, std::time_get, std::use_facet

#include "date.h"
#include <iostream>
#include <cstdint>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace std::chrono;

namespace SimpleXTree
{

	std::wstring DateTime::GetDateTime()
	{
//		auto t = system_clock::now();
////		weekday wd{ t };
////		weekday wd{ Wednesday };
//
//
//		std::locale loc;        // classic "C" locale
//
//								// get time_get facet:
//		const std::time_get<char>& tmget = std::use_facet <std::time_get<char> >(loc);
//
//		std::ios::iostate state;
//		std::istringstream iss("Friday");
//		std::tm when;
//
//		tmget.get_weekday(iss, std::time_get<char>::iter_type(), iss, state, &when);
//
//		std::cout << "weekday: " << when.tm_wday << '\n';

		//https://stackoverflow.com/questions/57569940/how-do-i-get-the-current-day-of-week-in-c-using-date-h
		auto now = std::chrono::system_clock::now();
		date::sys_days now_in_days{ std::chrono::time_point_cast<date::days>(now) };
		date::weekday weekday{ now_in_days };
		std::cout << weekday << '\n';

		std::wstringstream ws;
		ws << weekday;

		std::wstring twoLetterDay = ws.str().substr(0, 2);
		return twoLetterDay;
	}
}
