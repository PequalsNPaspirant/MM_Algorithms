#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

#include "Others_h/Others_h_DistinctNumbersGenerated.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	class FreqStack {
	public:
		FreqStack() {
			index_ = 0;
		}

		void push(int x) {
			auto itPair = hash.insert({ x, data(x) });
			auto it = itPair.first;
				
			//data& val = hash[x];
			if (it->second.count > 0)
			{
				sorted.erase(sorted.find(it));
			}
			it->second.count += 1;
			it->second.index = (++index_);
			sorted.insert(it);
		}

		int pop() {
			auto itSet = sorted.begin();
			if (itSet != sorted.end())
			{
				auto itUMap = *itSet;
				itUMap->second.count -= 1;
				if (itUMap->second.count == 0)
					sorted.erase(itSet);

				return itUMap->second.num;
			}

			return 0;
		}

	private:
		struct data
		{
			data(int n)
			{
				num = n;
				count = 0;
				index = 0;
			}

			int num;
			int count;
			int index;
		};


		unordered_map<int, data> hash;
		//vector<data> heap;
		struct comp
		{
			bool operator()(const unordered_map<int, data>::iterator& lhs,
				const unordered_map<int, data>::iterator& rhs) const
			{
				if (lhs->second.count == rhs->second.count)
					return lhs->second.index > rhs->second.index;
				else
					return lhs->second.count > rhs->second.count;
			}
		};

		set<unordered_map<int, data>::iterator, comp> sorted;
		int index_;
	};

#define MM_TIME2(msg, statement, time) \
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now(); \
	statement; \
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now(); \
	time = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count(); \
	int columnWidth = 25; \
	cout << "\n" << setw(columnWidth) << std::right << msg; \
	cout << setw(columnWidth) << std::right << getCommaSeparatedTimeDuration(time);

#define MM_TIME(msg, statement) \
	{ \
	unsigned long long time; \
	MM_TIME2(msg, statement, time) \
	}

	

	//===============================================================================

	MM_DECLARE_FLAG(Others_L_MaxFrequencyStack);

	MM_UNIT_TEST(Others_L_MaxFrequencyStack_sanity, Others_L_MaxFrequencyStack)
	{
		// Test Date creation

		struct testInputStruct
		{
			testInputStruct(unsigned long n, unsigned long s, unsigned long p, unsigned long q, unsigned long result)
				: n_(n),
				s_(s),
				p_(p),
				q_(q),
				result_(result)
			{
			}

			unsigned long n_;
			unsigned long s_;
			unsigned long p_;
			unsigned long q_;
			unsigned int result_;
		};

		vector<testInputStruct> testData = {
			{ 3, 1, 1, 1, 3 },
			{ 10, 2, 3, 4, 10 },
			{ 12345678, 524867, 4865215, 48976163, 12345678 },
			{ 100'000'000, 1, 3, 1, 100'000'000 },
			{ 100'000'000, 2'065'188'356, 657'733'125, 1'636'844'041, 100'000'000 }
		};

		//Generate random test data
		testData.reserve(testData.size() + 10'000);
		constexpr unsigned two_to_exponent = 1u << 31;
		for (int i = 0; i < 10'000; ++i)
		{
			unsigned long n = rand() % (two_to_exponent - 100'000'000) + 100'000'000; //range 100'000'000 to 2^31
			unsigned long s = rand();
			unsigned long p = rand();
			unsigned long q = rand();
			unsigned int result = 0;

			testData.push_back({ n, s, p, q, result });
		}

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualNum;
			//cout << endl;
			//MM_TIME("findDistrinctNumbersGenerated_v1: ", actualNum = findDistrinctNumbersGenerated_v1(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			//MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			//cout << endl;
			//MM_TIME("findDistrinctNumbersGenerated_v2: ", actualNum = findDistrinctNumbersGenerated_v2(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			//MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			//cout << endl;
			//MM_TIME("findDistrinctNumbersGenerated_v3: ", actualNum = findDistrinctNumbersGenerated_v3(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			//MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			//cout << endl;
			//MM_TIME("findDistrinctNumbersGenerated_v4: ", actualNum = findDistrinctNumbersGenerated_v4(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			//MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			//cout << endl;
			//MM_TIME("findDistrinctNumbersGenerated_v5: ", actualNum = findDistrinctNumbersGenerated_v5(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			//MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
		}

		int n;
		FreqStack s;
		s.push(5);
		s.push(7);
		s.push(5);
		s.push(7);
		s.push(4);
		s.push(5);
		n = s.pop();
		n = s.pop();
		n = s.pop();
		n = s.pop();
		s.push(1);
		s.push(1);
		n = s.pop();
		s.push(7);
		s.push(1);
		n = s.pop();
		n = s.pop();
		n = s.pop();
		s.push(7);
		s.push(10);
		n = s.pop();
		n = s.pop();
	}

}