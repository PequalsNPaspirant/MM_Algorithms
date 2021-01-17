#pragma once

#include <iostream>
#include <string>
using namespace std;

namespace mm {

	//#define MM_TIME(msg, statement) \
	//	unsigned long long time; \
	//	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now(); \
	//	statement; \
	//	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now(); \
	//	time = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count(); \
	//	int columnWidth = 25; \
	//	cout.imbue(std::locale("")); \
	//	cout << "\n" << setw(columnWidth) << std::right << msg; \
	//	cout << setw(columnWidth) << std::right << time;

	//template<typename FunctionType, typename... Args>
	//unsigned int mm_time_1(const string& msg, FunctionType functionObj, Args... args)
	//{
	//	unsigned long long time;
	//	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	//	unsigned int retVal = functionObj(args);
	//	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	//	time = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
	//	int columnWidth = 25;
	//	cout.imbue(std::locale(""));
	//	cout << "\n" << setw(columnWidth) << std::right << msg;
	//	cout << setw(columnWidth) << std::right << time;
	//	return retVal;
	//}

	//// below function does not work unless we call it like:
	//// unsigned int actualNum = mm_time_2<decltype(actualNum), decltype(findDistrinctNumbersGenerated_v1), unsigned long, unsigned long, unsigned long, unsigned long>("\nfindDistrinctNumbersGenerated_v2: ", findDistrinctNumbersGenerated_v2, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
	//template<typename ReturnType, typename FunctionType, typename... Args>
	//ReturnType mm_time_2(const string& msg, FunctionType functionObj, Args... args)
	//{
	//	unsigned long long time;
	//	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
	//	ReturnType retVal = functionObj(args);
	//	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
	//	time = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
	//	int columnWidth = 25;
	//	cout.imbue(std::locale(""));
	//	cout << "\n" << setw(columnWidth) << std::right << msg;
	//	cout << setw(columnWidth) << std::right << time;
	//	return retVal;
	//}

	//Usage:
	//unsigned int actualNum = mm_time<decltype(actualNum)>{}("findDistrinctNumbersGenerated_v1: ", findDistrinctNumbersGenerated_v1, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
	template<typename ReturnType>
	class mm_time
	{
	public:
		template<typename FunctionType, typename... Args>
		ReturnType operator()(const string& msg, FunctionType functionObj, Args... args)
		{
			unsigned long long time;
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
			ReturnType retVal = functionObj(args...);
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			time = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
			int columnWidth = 25;
			cout << "\n";
			cout << setw(columnWidth) << std::right << msg;
			cout << setw(columnWidth) << std::right << time << " ns";
			return retVal;
		}
	};


}