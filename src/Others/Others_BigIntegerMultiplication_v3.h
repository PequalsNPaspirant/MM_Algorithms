#pragma once

#include <string>
using namespace std;

namespace mm {

	string longMultiplicationInBaseMAXINT_v1(const string& str1, const string& str2);
	string longMultiplicationInBaseMAXINT_v2(const string& str1, const string& str2);
	string longMultiplicationInBaseMAXINT_v3(const string& str1, const string& str2);

	extern vector<unsigned long long> durations;
}