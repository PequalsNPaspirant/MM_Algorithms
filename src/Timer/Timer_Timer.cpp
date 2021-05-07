//Goal
//Timer utilities

#include <stdio.h>
#include <iostream>
//#include <chrono> //C++11
//#include <ctime>
#include <sstream>
//#include <string>
#include <iomanip> //for std::put_time
#include <windows.h> //For Sleep(int milliseconds)
#include <vector>
#include <time.h>
#include <cmath>
using namespace std;

#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

#ifdef __GNUC__

#define localtime_s(type_tm, type_time_t) localtime_r(type_time_t, type_tm)
#define ctime_s(type_char, count, type_time_t) ctime_r(type_time_t, type_char)
#define asctime_s(type_char, type_tm) asctime_r(type_tm, type_char)
	//#define ssscanf_s sscanf_r
#define sscanf_s sscanf
#define gmtime_s(type_tm, type_time_t) gmtime_r(type_time_t, type_tm)
	//#define ctime_s ctime_r

#endif

	//========== Public functions ============

	Timer::Timer()
		: m_startTime(HRClock::now()),
		m_endTime(HRClock::now())
	{
	}

	Timer::~Timer()
	{
		//cout << "\nTime Duration: " << getDurationStringTillNow() << " nanoseconds";
	}

	void Timer::resetTimer()
	{
		m_startTime = HRClock::now();
	}

	void Timer::stopTimer()
	{
		m_endTime = HRClock::now();
	}

	// Section 1: duration. It uses only HRClock
	//-----------------------------

	long long Timer::getDurationTillNowInNanoSeconds()
	{
		HRClock::time_point end_time = HRClock::now();
		return getDurationInNanoSeconds(m_startTime, end_time);
	}

	string Timer::getDurationStringTillNowInNanoSeconds()
	{
		HRClock::time_point end_time = HRClock::now();
		return getDurationStringInNanoSeconds(m_startTime, end_time);
	}

	string Timer::getDurationStringTillNowInNanoSeconds2()
	{
		HRClock::time_point end_time = HRClock::now();
		return getDurationStringInNanoSeconds2(m_startTime, end_time);
	}

	long long Timer::getDurationInNanoSeconds(const HRClock::time_point& end_time)
	{
		return getDurationInNanoSeconds(m_startTime, end_time);
	}

	string Timer::getDurationStringInNanoSeconds(const HRClock::time_point& end_time)
	{
		return getDurationStringInNanoSeconds(m_startTime, end_time);
	}

	long long Timer::getDurationInNanoSeconds(const HRClock::time_point& startTime, const HRClock::time_point& end_time)
	{
		chrono::nanoseconds time_span = chrono::duration_cast<chrono::nanoseconds>(end_time - startTime);
		return time_span.count(); //return nanoseconds
	}

	long long Timer::getDurationInMicroSeconds(const HRClock::time_point& startTime, const HRClock::time_point& end_time)
	{
		chrono::microseconds time_span = chrono::duration_cast<chrono::microseconds>(end_time - startTime);
		return time_span.count() / 1000; //return microseconds
	}

	string Timer::getDurationStringInNanoSeconds(const HRClock::time_point& startTime, const HRClock::time_point& end_time)
	{
		return getDurationStringInNanoSeconds(getDurationInNanoSeconds(startTime, end_time));
	}

	string Timer::getDurationStringInNanoSeconds(long long duration)
	{
		int length = static_cast<int>(log10(duration + 1)) + 1; //Avoid log(0)
		length = length + (length - 1) / 3 + 12;
		static const string pattern("000,000,000,000,000,000,000,000,000,000,000,000,000 nanoseconds");
		string durationString(pattern.end() - length, pattern.end());
		int commaPosition = 0;
		for (int index = length - 1 - 12; duration > 0; --index, ++commaPosition)
		{
			if (durationString[index] != ',')
			{
				durationString[index] = '0' + (duration % 10);
				duration /= 10;
			}
		}

		return durationString;
	}

	string Timer::getDurationStringInNanoSeconds2(const HRClock::time_point& startTime, const HRClock::time_point& end_time)
	{
		long long duration = getDurationInNanoSeconds(startTime, end_time);
		std::stringstream ss;
		ss.imbue(std::locale(""));
		ss << std::fixed << duration;
		return ss.str() + " nanoseconds";
	}

	// Section 2: absolute time. It uses HRClock & SystemClock both.
	//-----------------------------

	const vector<int>& getDaysInYear()
	{
		static int* init = nullptr;
		static vector<int> daysInYear(200);
		if (init == nullptr)
		{
			init = new int;
			daysInYear[99] = 0; //year 1970 is at 99th index in array. The array contains years in range [1871, 2070]
			auto isLeapYear = [](const int& year) -> bool { return (year % 100 == 0 && year % 400 == 0) || (year % 4 == 0); };
			for (int i = 100; i < 200; ++i)
				daysInYear[i] = daysInYear[i - 1] + (isLeapYear(1871 + i) ? 366 : 365);
			for (int i = 98; i > 0; --i)
				daysInYear[i] = daysInYear[i + 1] - (isLeapYear(1871 + i) ? 366 : 365);
		}

		return daysInYear;
	}

	// This function uses HRClock
	string Timer::getCurrentLocalTimeInNanoSeconds()
	{
		// Reference: https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/tzset

		int daylight = 0;
		long timezone = 0;

#ifdef _MSC_VER

		_tzset();
		_get_daylight(&daylight); // Nonzero value if a daylight-saving-time zone is specified in TZ setting; otherwise, 0
		//printf("_daylight = %d\n", daylight);
		_get_timezone(&timezone); // Difference in seconds between local time and UTC
		//printf("_timezone = %ld\n", timezone);
		//size_t s;
		//char tzname[100];
		//_get_tzname(&s, tzname, sizeof(tzname), 0); 
		//printf("_tzname[0] = %s\n", tzname);

		// Output:
		// _daylight = 1
		// _timezone = 28800
		// _tzname[0] = Pacific Standard Time

#endif

		//This function takes aorund 50 to 70 microseconds to execute
		HRClock::time_point now = HRClock::now();
		long long totalDurationInNanosconds = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count();
		long long totalDurationInSeconds = totalDurationInNanosconds / (long long)1000000000 - timezone + (daylight * 60 * 60);
		long long totalDurationInMins = totalDurationInSeconds / 60;
		long long totalDurationInHrs = totalDurationInMins / 60;
		long long totalDurationInDays = totalDurationInHrs / 24;
		long long nanoseconds = totalDurationInNanosconds % (long long)1000000000;
		long long seconds = totalDurationInSeconds % 60;
		long long min = totalDurationInMins % 60;
		long long hrs = totalDurationInHrs % 24;

		auto binarySearch = [](const long long& days, const vector<int>& daysVector) -> size_t {
			size_t start = 0;
			size_t end = daysVector.size() - 1;
			MyAssert::myRunTimeAssert(days >= 0 && days <= daysVector[end], "Year out of range");

			while (end - start > 1)
			{
				size_t mid = (start + end) / 2;
				if (daysVector[mid] < days)
					start = mid;
				else
					end = mid;
			}

			return end;
		};

		int year0 = 1870;
		const vector<int>& daysInYear = getDaysInYear();
		size_t year = year0 + binarySearch(totalDurationInDays, daysInYear);

		long long remainingDaysInYear = totalDurationInDays - daysInYear[(year - 1) - year0];
		static const vector<int> daysInMonthNonLeapYear({
			0,
			31,
			31 + 28,
			31 + 28 + 31,
			31 + 28 + 31 + 30,
			31 + 28 + 31 + 30 + 31,
			31 + 28 + 31 + 30 + 31 + 30,
			31 + 28 + 31 + 30 + 31 + 30 + 31,
			31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
			31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
			31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
			31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
			31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31
		});
		static const vector<int> daysInMonthLeapYear({
			0,
			31,
			31 + 29,
			31 + 29 + 31,
			31 + 29 + 31 + 30,
			31 + 29 + 31 + 30 + 31,
			31 + 29 + 31 + 30 + 31 + 30,
			31 + 29 + 31 + 30 + 31 + 30 + 31,
			31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
			31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
			31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
			31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
			31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31
		});
		auto isLeapYear = [](size_t year) -> bool { return (year % 100 == 0 && year % 400 == 0) || (year % 4 == 0); };
		size_t month = 0;
		size_t date = 0;
		if (isLeapYear(year))
		{
			month = binarySearch(remainingDaysInYear, daysInMonthLeapYear);
			date = remainingDaysInYear - daysInMonthLeapYear[month - 1] + 1;
		}
		else
		{
			month = binarySearch(remainingDaysInYear, daysInMonthNonLeapYear);
			date = remainingDaysInYear - daysInMonthNonLeapYear[month - 1] + 1;
		}

		//duration is in nanoseconds, so it can have max 9 digits, 2 commas and 1 dot
		string timeFormat("0000-00-00_00-00-00.000,000,000");
		//timeFormat = "0000-00-00_00-00-00.000,000,000";
		int index = 3;
		for (; year > 0; --index)
		{
			timeFormat[index] = '0' + (year % 10);
			year /= 10;
		}
		for (index = 6; month > 0; --index)
		{
			timeFormat[index] = '0' + (month % 10);
			month /= 10;
		}
		for (index = 9; date > 0; --index)
		{
			timeFormat[index] = '0' + (date % 10);
			date /= 10;
		}
		for (index = 12; hrs > 0; --index)
		{
			timeFormat[index] = '0' + (hrs % 10);
			hrs /= 10;
		}
		for (index = 15; min > 0; --index)
		{
			timeFormat[index] = '0' + (min % 10);
			min /= 10;
		}
		for (index = 18; seconds > 0; --index)
		{
			timeFormat[index] = '0' + (seconds % 10);
			seconds /= 10;
		}
		for (index = 30; nanoseconds > 0; --index)
		{
			if (index == 27 || index == 23)
				continue;
			timeFormat[index] = '0' + (nanoseconds % 10);
			nanoseconds /= 10;
		}

		return std::move(timeFormat);
	}

	// This function used System Clock
	string Timer::getCurrentLocalTimeInNanoSeconds2()
	{
		//This function takes aorund 50 to 70 microseconds to execute
		SystemClock::time_point now = SystemClock::now();

		std::time_t now_t = chrono::system_clock::to_time_t(now);
		std::tm nowTm;
		localtime_s(&nowTm, &now_t);
		std::stringstream bufferStream;
		bufferStream << std::put_time(&nowTm, "%Y-%m-%d_%H-%M-%S");

		long long duration = (chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch())).count() % (long long)1000000000;

		//duration is in nanoseconds, so it can have max 9 digits, 2 commas and 1 dot
		string fractional_seconds(".000,000,000");
		for (int index = 11; duration > 0; --index)
		{
			if (index == 8 || index == 4)
				continue;
			fractional_seconds[index] = '0' + (duration % 10);
			duration /= 10;
		}

		string retVal(bufferStream.str() + fractional_seconds);
		return std::move(retVal);
	}

	/*
	string Timer::getCurrentLocalTimeInNanoSeconds()
	{
		//This function takes aorund 50 to 70 microseconds to execute

		SystemClock::time_point now = SystemClock::now();
		std::time_t now_t = chrono::system_clock::to_time_t(now);
		//std::tm now_tm = *std::localtime(&now_t);
		std::tm nowTm;
		localtime_s(&nowTm, &now_t);
		std::stringstream bufferStream;
		//bufferStream.str(std::string());
		bufferStream << std::put_time(&nowTm, "%Y-%m-%d_%H-%M-%S");

		//This block needs around 600 microseconds more
		//chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch());
		//std::chrono::seconds sec(1);
		//long long nanosecondsInOneSecond = std::chrono::nanoseconds(sec).count();
		//long long duration = ns.count() % nanosecondsInOneSecond;
		long long duration = (chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch())).count() % (long long)1000000000;

		//Following block needs around 600-700 microseconds more
		//std::stringstream ss;
		//ss.imbue(std::locale(""));
		//ss << std::fixed << duration;
		//string fractional_seconds(ss.str());

		//Instead of above block, use following block, custom version to convert long long into string with comma separated digits. It takes few microseconds
		//duration is in nanoseconds, so it can have max 9 digits, 2 commas and 1 dot
		string fractional_seconds(".000,000,000");
		for (int index = 11; duration > 0; --index)
		{
			if (index == 8 || index == 4)
				continue;
			fractional_seconds[index] = '0' + (duration % 10);
			duration /= 10;
		}

		return bufferStream.str() + fractional_seconds;
	}
	*/

	string Timer::getStartTimeInNanoSeconds()
	{
		return convertHRClockTimePointToString(m_startTime);
	}

	string Timer::getEndTimeInNanoSeconds()
	{
		return convertHRClockTimePointToString(m_endTime);
	}

	string Timer::convertHRClockTimePointToString(const HRClock::time_point& timePoint)
	{
		return convertSystemClockTimePointToString(convertToSystemClock1(timePoint));
	}

	string Timer::convertSystemClockTimePointToString(const SystemClock::time_point& now)
	{
		std::time_t now_t = chrono::system_clock::to_time_t(now);
		//std::tm now_tm = *std::localtime(&now_t);
		std::tm nowTm;
		localtime_s(&nowTm, &now_t);

		std::stringstream buffer;
		buffer << std::put_time(&nowTm, "%F %T");

		chrono::milliseconds ms = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch());
		std::size_t fractional_seconds = ms.count() % 1000000;

		return string(buffer.str() + "." + to_string(fractional_seconds));
	}


	void Timer::printCurrentLocalTimeInDifferentFormats()
	{
		//References:
		//http://en.cppreference.com/w/cpp/io/manip/put_time
		//http://www.cplusplus.com/reference/iomanip/put_time/

		int formatCounter = 1;
		string dateTimeString;
		// ======================== Common Code ========================
		SystemClock::time_point now = SystemClock::now();
		//std::time_t now_t = std::time(nullptr);
		std::time_t now_t = chrono::system_clock::to_time_t(now);
		//std::tm now_tm = *std::localtime(&now_t);
		std::tm nowTm;
		localtime_s(&nowTm, &now_t);

		std::chrono::seconds sec(1);
		//chrono::microseconds ms = chrono::duration_cast<chrono::microseconds>(now.time_since_epoch());
		//long long microsecondsInOneSecond = std::chrono::microseconds(sec).count();
		//std::size_t duration = ms.count() % microsecondsInOneSecond;

		chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch());
		long long nanosecondsInOneSecond = std::chrono::nanoseconds(sec).count();
		long long duration = ns.count() % nanosecondsInOneSecond;

		std::stringstream ss;
		ss.imbue(std::locale(""));
		ss << std::fixed << duration;
		string fractional_seconds(ss.str());

		// ======================== Date Time Format 1 ========================
		std::stringstream bufferStream;
		bufferStream << std::put_time(&nowTm, "%F %T");
		dateTimeString = string(bufferStream.str() + "." + fractional_seconds);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 2 ========================
		bufferStream.str(std::string());
		bufferStream << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S");
		dateTimeString = string(bufferStream.str() + "." + fractional_seconds);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 3 ========================
		bufferStream.str(std::string());
		bufferStream << std::put_time(&nowTm, "%a, %d %b %y - %H:%M:%S");
		dateTimeString = string(bufferStream.str() + "." + fractional_seconds);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 4 ========================
		bufferStream.str(std::string());
		bufferStream << std::put_time(&nowTm, "%A, %d %B %Y - %H:%M:%S");
		dateTimeString = string(bufferStream.str() + "." + fractional_seconds);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 5 ========================
		bufferStream.str(std::string());
		bufferStream << std::put_time(&nowTm, "%A, %d %B %Y - %I:%M:%S");
		dateTimeString = string(bufferStream.str() + "." + fractional_seconds);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 6 ========================
		bufferStream.str(std::string());
		bufferStream << std::put_time(&nowTm, "%A, %d %B %Y - %I:%M:%S %p %z %Z");
		dateTimeString = string(bufferStream.str()); // +"." + fractional_seconds);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 7 ========================
		//std::cout << std::ctime(&time) << "." << fractional_seconds << std::endl;
		char buffer[256];
		ctime_s(buffer, 256, &now_t);
		dateTimeString = string(buffer);
		dateTimeString.erase(dateTimeString.end() - 1); //Erase newline at end
		dateTimeString += " and " + fractional_seconds;
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 8 ========================
		char ch[256];
		asctime_s(ch, &nowTm);
		dateTimeString = string(ch);
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;

		// ======================== Date Time Format 9 ========================

		char date_time_format[] = "%Y.%m.%d-%H.%M.%S";
		char time_str[] = "yyyy.mm.dd.HH-MM.SS.ffffff";
		strftime(time_str, strlen(time_str), date_time_format, &nowTm);

		SystemClock::time_point now_sec = SystemClock::from_time_t(now_t);
		chrono::milliseconds millisec = chrono::duration_cast<chrono::milliseconds>(now - now_sec);

		dateTimeString = string(time_str) + "." + to_string(millisec.count());
		cout << "\nDate Time Format " << formatCounter++ << ": " << dateTimeString;
	}

	//========== Private functions ============

	SystemClock::time_point Timer::convertToSystemClock1(const HRClock::time_point& htp, bool doTally /*= false*/)
	{
		//Get the current HR clock time and System time. Assume we get it at the same time
		HRClock::time_point hnow = HRClock::now();
		SystemClock::time_point snow = SystemClock::now();

		//chrono::nanoseconds hns1 = chrono::duration_cast<chrono::nanoseconds>(hnow.time_since_epoch());
		//chrono::nanoseconds sns1 = chrono::duration_cast<chrono::nanoseconds>(snow.time_since_epoch());
		//std::cout << "\nTally now: HR clock nanoseconds = " << hns1.count() << " Sys clock nanoseconds = " << sns1.count();

		chrono::milliseconds diff = chrono::duration_cast<chrono::milliseconds>(htp - hnow);
		//std::cout << "\n Diff = " << diff.count();
		snow += diff;

		//tally
		//chrono::nanoseconds hns = chrono::duration_cast<chrono::nanoseconds>(htp.time_since_epoch());
		//chrono::nanoseconds sns = chrono::duration_cast<chrono::nanoseconds>(snow.time_since_epoch());
		//std::cout << "\nTally: HR clock nanoseconds = " << hns.count() << " Sys clock nanoseconds = " << sns.count();

		return snow;
	}

	SystemClock::time_point Timer::convertToSystemClock2(const HRClock::time_point& htp, bool doTally /*= false*/)
	{
		chrono::system_clock::duration sdu = chrono::duration_cast<chrono::system_clock::duration>(htp.time_since_epoch());
		//chrono::time_point<chrono::system_clock> stp(du); //OR use following:
		chrono::system_clock::time_point stp(sdu);

		//tally
		chrono::nanoseconds hns = chrono::duration_cast<chrono::nanoseconds>(htp.time_since_epoch());
		chrono::nanoseconds sns = chrono::duration_cast<chrono::nanoseconds>(stp.time_since_epoch());
		std::cout << "\nTally: Duration: " << sdu.count() << "HR clock nanoseconds = " << hns.count() << " Sys clock nanoseconds = " << sns.count();
		if (hns != sns)
			std::cout << "\n------TALLY FAILED------. We are losing precision.";

		return stp;
	}

	SystemClock::time_point Timer::stringToSystemClock(const string& str)
	{
		int yyyy, mm, dd, HH, MM, SS, ffffff;

		char scanf_format[] = "%4d.%2d.%2d-%2d.%2d.%2d.%3d";

		//sscanf(str.c_str(), scanf_format, &yyyy, &mm, &dd, &HH, &MM, &SS, &ffffff);
		sscanf_s(str.c_str(), scanf_format, &yyyy, &mm, &dd, &HH, &MM, &SS, &ffffff);

		tm ttm = tm();
		ttm.tm_year = yyyy - 1900; // Year since 1900
		ttm.tm_mon = mm - 1; // Month since January 
		ttm.tm_mday = dd; // Day of the month [1-31]
		ttm.tm_hour = HH; // Hour of the day [00-23]
		ttm.tm_min = MM;
		ttm.tm_sec = SS;

		time_t ttime_t = mktime(&ttm);

		SystemClock::time_point time_point_result = SystemClock::from_time_t(ttime_t);

		time_point_result += std::chrono::milliseconds(ffffff);
		return time_point_result;
	}

	//=============== Test ====================

	void Timer_sanityTest()
	{
		Timer t;
		Sleep(1234);
		cout << "\nStart Time: " << t.getStartTimeInNanoSeconds();
		cout << "\nDuration till now: " << t.getDurationStringTillNowInNanoSeconds();

		cout << "\n\nPrint current time in various different formats:";
		Timer::printCurrentLocalTimeInDifferentFormats();
	}

	void Timer_performanceTest()
	{
		Timer t;
		string currentTime, duration;
		const int repeatitions = 25;

		//-------------- Test performance difference between functions getDurationTillNowInNanoSeconds and getDurationStringTillNowInNanoSeconds for some sample floating point arithmatic --------------

		cout << "\n\nTest 1: Test performance difference between functions getDurationTillNowInNanoSeconds and getDurationStringTillNowInNanoSeconds for some sample floating point arithmatic";
		cout << "\n\nCase 1: When we use getDurationTillNowInNanoSeconds() to capture duration. Takes on an average 150,000 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			Timer tLocal;
			for (int i = 0; i < 100000; i++)
			{
				double n = 342642422.42353535;
				n /= 352525235.3467352532;
			}
			long long durationNanoSeconds = tLocal.getDurationTillNowInNanoSeconds();
			cout << "\nTime (captured by case 1:getDurationTillNowInNanoSeconds()) required for some sample floating point arithmatic = " << durationNanoSeconds << " nanoseconds";
		}

		cout << "\n\nCase 2: When we use getDurationStringTillNowInNanoSeconds() to capture duration. Takes on an average 150,000 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			Timer tLocal;
			for (int i = 0; i < 100000; i++)
			{
				double n = 342642422.42353535;
				n /= 352525235.3467352532;
			}
			string durationNanoSeconds = tLocal.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime (captured by case 2:getDurationStringTillNowInNanoSeconds()) required for some sample floating point arithmatic = " << durationNanoSeconds;
		}

		//-------------- Test performance difference between functions getDurationTillNowInNanoSeconds and getDurationStringTillNowInNanoSeconds for no operations in between --------------

		cout << "\n\nTest 2: Test performance difference between functions getDurationTillNowInNanoSeconds and getDurationStringTillNowInNanoSeconds for no operations in between";
		cout << "\n\nCase 1: When we use getDurationTillNowInNanoSeconds() to capture duration. Takes on an average 1,058 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			Timer tLocal;
			long long durationNanoSeconds = tLocal.getDurationTillNowInNanoSeconds();
			cout << "\nTime (captured by case 1:getDurationTillNowInNanoSeconds()) required for no operations in between = " << durationNanoSeconds << " nanoseconds";
		}

		cout << "\n\nCase 2: When we use getDurationStringTillNowInNanoSeconds() to capture duration. Takes on an average 705 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			Timer tLocal;
			string durationNanoSeconds = tLocal.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime (captured by case 2:getDurationStringTillNowInNanoSeconds()) required for no operations in between = " << durationNanoSeconds;
		}

		//-------------- Test performance difference between functions getDurationTillNowInNanoSeconds and getDurationStringTillNowInNanoSeconds for no operations in between when same time is reused --------------

		cout << "\n\nTest 3: Test performance difference between functions getDurationTillNowInNanoSeconds and getDurationStringTillNowInNanoSeconds for no operations in between when same time is reused";
		cout << "\n\nCase 1: When we use getDurationTillNowInNanoSeconds() to capture duration. Takes on an average 352 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			long long durationNanoSeconds = t.getDurationTillNowInNanoSeconds();
			cout << "\nTime (captured by case 1:getDurationTillNowInNanoSeconds()) required for no operations in between when same timer is reused = " << durationNanoSeconds << " nanoseconds";
		}

		cout << "\n\nCase 2: When we use getDurationStringTillNowInNanoSeconds() to capture duration. Takes on an average 352 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			string durationNanoSeconds = t.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime (captured by case 2:getDurationStringTillNowInNanoSeconds()) required for no operations in between when same timer is reused = " << durationNanoSeconds;
		}

		//-------------- Test performance of function resetTimer --------------

		cout << "\n\nTest 4: Test performance of function resetTimer. Takes on an average 352 nanoseconds";
		Timer tLocal;
		for (int i = 0; i < repeatitions; i++)
		{
			tLocal.resetTimer();
			t.resetTimer();
			string durationNanoSeconds = tLocal.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime required for execution of function resetTimer = " << durationNanoSeconds;
		}

		//-------------- Test performance of function getDurationTillNowInNanoSeconds --------------

		cout << "\n\nTest 5: Test performance of function getDurationTillNowInNanoSeconds Takes on an average 1,400 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			long long durationNanoSeconds = t.getDurationTillNowInNanoSeconds();
			duration = t.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime required for execution of function getDurationInNanoSeconds = duration string - durationNanoSeconds = " << duration << " - " << durationNanoSeconds << " nanoseconds";
		}

		//-------------- Test performance of function getDurationStringTillNowInNanoSeconds --------------

		cout << "\n\nTest 6: Test performance of function getDurationStringTillNowInNanoSeconds. Takes on an average 25,000 nanoseconds";
		string duration1, duration2;
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			duration1 = t.getDurationStringTillNowInNanoSeconds();
			duration2 = t.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime required for execution of function getDurationStringTillNowInNanoSeconds = duration string 2 - duration string 1 = " << duration2 << " - " << duration1;
		}

		//-------------- Test performance of function getDurationStringTillNowInNanoSeconds2 --------------

		cout << "\n\nTest 7: Test performance of function getDurationStringTillNowInNanoSeconds2. Takes on an average 750,000 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			duration1 = t.getDurationStringTillNowInNanoSeconds2();
			duration2 = t.getDurationStringTillNowInNanoSeconds();
			cout << "\nTime required for execution of function getDurationStringTillNowInNanoSeconds2 = duration string 2 - duration string 1 = " << duration2 << " - " << duration1;
		}

		//-------------- Test performance of function getCurrentLocalTimeInNanoSeconds --------------

		cout << "\n\nTest 8: Test performance of function getCurrentLocalTimeInNanoSeconds. Takes on an average 50,000 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			currentTime = Timer::getCurrentLocalTimeInNanoSeconds();
			duration = t.getDurationStringTillNowInNanoSeconds();
			cout << "\nCurrent Time: " << currentTime << " Time required for execution of function getCurrentLocalTimeInNanoSeconds: " << duration;
		}

		//-------------- Test performance of function getCurrentLocalTimeInNanoSeconds2 --------------

		cout << "\n\nTest 9: Test performance of function getCurrentLocalTimeInNanoSeconds2. Takes on an average 70,000 nanoseconds";
		for (int i = 0; i < repeatitions; i++)
		{
			t.resetTimer();
			currentTime = Timer::getCurrentLocalTimeInNanoSeconds2();
			duration = t.getDurationStringTillNowInNanoSeconds();
			cout << "\nCurrent Time: " << currentTime << " Time required for execution of function getCurrentLocalTimeInNanoSeconds2: " << duration;
		}
	}

	void Timer_UnitTest()
	{
		cout << "\n\n~~~~~~~~~~~~~~~~~~ Timer_UnitTest() ~~~~~~~~~~~~~~~~~~~~~~~\n";

		//Timer_sanityTest();
		Timer_performanceTest();
	}
}