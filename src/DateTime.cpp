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
		//http://cplusplus.com/forum/beginner/32329/
		//http://www.cplusplus.com/reference/ctime/tm/
		time_t t = time(NULL);
		tm* timePtr = localtime(&t);

		std::wstring weekday = L"Su";
		switch (timePtr->tm_wday)
		{
		case 0:
			weekday = L"Su";
			break;
		case 1:
			weekday = L"Mo";
			break;
		case 2:
			weekday = L"Tu";
			break;
		case 3:
			weekday = L"We";
			break;
		case 4:
			weekday = L"Th";
			break;
		case 5:
			weekday = L"Fr";
			break;
		case 6:
			weekday = L"Sa";
			break;
		}

		std::wstringstream yearWs;
		yearWs << (timePtr->tm_year + 1900);
		std::wstring yearTwoLetter = yearWs.str().substr(2, 2);

		std::wstringstream dayWs;
		if (timePtr->tm_mday < 10)
		{
			dayWs << "0";
		}
		dayWs << timePtr->tm_mday;
		std::wstring day = dayWs.str();

		std::wstringstream monthWs;
		if ((timePtr->tm_mon+1) < 10)
		{
			monthWs << "0";
		}
		monthWs << (timePtr->tm_mon+1);
		std::wstring month = monthWs.str();

		std::wstringstream hourWs;
		int iHour = timePtr->tm_hour;
		if (iHour > 12)
		{
			iHour = iHour - 12;
		}
		if (iHour < 10)
		{
			hourWs << " "; // not 0
		}
		if (iHour == 0)
		{
			iHour = 12; // 12 am
		}
		hourWs << iHour;
		std::wstring hour = hourWs.str();

		std::wstringstream minWs;
		if (timePtr->tm_min < 10)
		{
			minWs << "0";
		}
		minWs << timePtr->tm_min;
		std::wstring min = minWs.str();
		
		std::wstringstream secWs;
		if (timePtr->tm_sec < 10)
		{
			secWs << "0";
		}
		secWs << timePtr->tm_sec;
		std::wstring sec = secWs.str();

		std::wstringstream ws;
		ws << weekday << L" " << day << L"-" << month << L"-" << yearTwoLetter << L" " << hour << L":" << min << L":" << sec << L" ";
		if (timePtr->tm_hour < 12)
		{
			ws << "am";
		}
		else
		{
			ws << "pm";
		}

		return ws.str();
	}

	//gives incorrect weekday for wednesday
    std::wstring DateTime::GetWeekDayTwoLetter()
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
