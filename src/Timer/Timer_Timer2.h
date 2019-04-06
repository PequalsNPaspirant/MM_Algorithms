#pragma once
//Goal
//Timer utility interface

//#include <iostream>
#include <stdio.h>
#include <chrono> //C++11
#include <ctime>
//#include <sstream>
#include <string>
//#include <iomanip> //for std::put_time

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

#ifdef __GNUC__

#define localtime_s(type_tm, type_time_t) localtime_r(type_time_t, type_tm)
#define ctime_s(type_char, count, type_time_t) ctime_r(type_time_t, type_char)
#define asctime_s(type_char, type_tm) asctime_r(type_tm, type_char)
//#define sscanf_s sscanf_r
#define sscanf_s sscanf
#define gmtime_s(type_tm, type_time_t) gmtime_r(type_time_t, type_tm)
//#define ctime_s ctime_r

#endif

	//============= More generic templetized Clock Utility class ============================

	template <typename clockType>
	class BasicClockUtilities
	{
	public:
		void captureStartTime();
		void captureEndTime();
		typename clockType::time_point getStartTime();
		typename clockType::time_point getEndTime();

		typename clockType::time_point getNow();
		unsigned long long getNanoSecondsSinceEpoch(typename clockType::time_point now);

		double getDiffInSeconds();
		unsigned long long getDiffInNanoSeconds();
		unsigned long long getDiffInNanoSeconds2();

	protected:
		typename clockType::time_point start_time;
		typename clockType::time_point end_time;
	};

	template <typename clockType>
	class ClockUtilities : public BasicClockUtilities<clockType>
	{
	};

	template <>
	class ClockUtilities<SystemClock> : public BasicClockUtilities<SystemClock>
	{
	public:
		void print(SystemClock::time_point now) { print1(now);  }
		void print1(SystemClock::time_point now);
		void print2(SystemClock::time_point now);
		SystemClock::time_point string_to_time_point(const std::string &str);
		std::string time_point_to_string(SystemClock::time_point &tp);
	};

#if _MSC_VER == 1900 || _MSC_VER > 1900

	template <>
	class ClockUtilities<HRClock> : public BasicClockUtilities<HRClock>
	{
	public:
		SystemClock::time_point convertToSystemClock1(HRClock::time_point htp);
		SystemClock::time_point convertToSystemClock2(HRClock::time_point htp);
		void print(HRClock::time_point now);
	};

#endif

	//============= class BasicClockUtilities - function definitions ============================

	template <typename clockType>
	void BasicClockUtilities<clockType>::captureStartTime()
	{
		start_time = clockType::now();
	}

	template <typename clockType>
	typename clockType::time_point BasicClockUtilities<clockType>::getStartTime()
	{
		return start_time;
	}

	template <typename clockType>
	void BasicClockUtilities<clockType>::captureEndTime()
	{
		end_time = clockType::now();
	}

	template <typename clockType>
	typename clockType::time_point BasicClockUtilities<clockType>::getEndTime()
	{
		return end_time;
	}

	template <typename clockType>
	typename clockType::time_point BasicClockUtilities<clockType>::getNow()
	{
		return clockType::now();
	}

	template <typename clockType>
	unsigned long long BasicClockUtilities<clockType>::getNanoSecondsSinceEpoch(typename clockType::time_point now)
	{
		unsigned long long nanosecondsSinceEpoch = now.time_since_epoch().count();
		return nanosecondsSinceEpoch;
	}

	template <typename clockType>
	double BasicClockUtilities<clockType>::getDiffInSeconds()
	{
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
		return time_span.count();
	}

	template <typename clockType>
	unsigned long long BasicClockUtilities<clockType>::getDiffInNanoSeconds()
	{
		return getNanoSecondsSinceEpoch(end_time) - getNanoSecondsSinceEpoch(start_time);
	}

	template <typename clockType>
	unsigned long long BasicClockUtilities<clockType>::getDiffInNanoSeconds2()
	{
		std::chrono::nanoseconds time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
		return time_span.count();
	}

	void systemClockUtils();
	void HRClockUtils();
	void useOfCtime();
	void useOfSteadyClock();
	void useOfSystemClock();
	void useOfHighResolutionClock();
	void printingTimeInVariousStyles();

}