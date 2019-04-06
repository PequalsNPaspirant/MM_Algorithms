//Goal
//Timer utility interface

#pragma once

//#include <iostream>
#include <chrono> //C++11
#include <ctime>
//#include <sstream>
#include <string>
//#include <iomanip> //for std::put_time
using namespace std;

/*
Clocks:

A clock consists of a starting point (or epoch) and a tick rate. For example, a clock may have an epoch of January 1, 1970
and tick every second. C++ defines three clock types:
Defined in header <chrono>
Defined in namespace std::chrono

system_clock:
wall clock time from the system-wide realtime clock

steady_clock:
monotonic clock that will never be adjusted

high_resolution_clock:
the clock with the shortest tick period available

Generally high_resolution_clock is defined as:
typedef std::chrono::steady_clock std::chrono::high_resolution_clock
*/

using namespace std;
using HRClock = std::chrono::high_resolution_clock;
using SystemClock = std::chrono::system_clock;

namespace mm {

	//============= Simple Timer class ============================

	class Timer
	{
	public:
		//Ideal set of interfaces that uses HRClock
		Timer();
		~Timer();

		void resetTimer();
		void stopTimer();

		// Section 1: duration. It uses only HRClock
		long long getDurationTillNowInNanoSeconds();
		string getDurationStringTillNowInNanoSeconds();
		string getDurationStringTillNowInNanoSeconds2();
		long long getDurationInNanoSeconds(const HRClock::time_point& end_time);
		string getDurationStringInNanoSeconds(const HRClock::time_point& end_time);
		static long long getDurationInNanoSeconds(const HRClock::time_point& startTime, const HRClock::time_point& end_time);
		static long long getDurationInMicroSeconds(const HRClock::time_point& startTime, const HRClock::time_point& end_time);
		static string getDurationStringInNanoSeconds(const HRClock::time_point& startTime, const HRClock::time_point& end_time);
		static string getDurationStringInNanoSeconds2(const HRClock::time_point& startTime, const HRClock::time_point& end_time);
		static string getDurationStringInNanoSeconds(long long duration);

		// Section 2: absolute time. It uses only SystemClock
		static string getCurrentLocalTimeInNanoSeconds();
		static string getCurrentLocalTimeInNanoSeconds2();
		string getStartTimeInNanoSeconds();
		string getEndTimeInNanoSeconds();

		static string convertHRClockTimePointToString(const HRClock::time_point& timePoint);
		static string convertSystemClockTimePointToString(const SystemClock::time_point& timePoint);

		//Test function
		static void printCurrentLocalTimeInDifferentFormats();

	private:
		HRClock::time_point m_startTime;
		HRClock::time_point m_endTime;

		static SystemClock::time_point convertToSystemClock1(const HRClock::time_point& htp, bool doTally = false);
		static SystemClock::time_point convertToSystemClock2(const HRClock::time_point& htp, bool doTally = false);
		static SystemClock::time_point stringToSystemClock(const string& str);
	};

}