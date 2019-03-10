#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <queue>
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

	/*
	FreqStack_v1:
	Runtime: 308 ms, faster than 38.33% of C++ online submissions for Maximum Frequency Stack.
	Memory Usage: 70.4 MB, less than 81.69% of C++ online submissions for Maximum Frequency Stack.
	FreqStack_v2: Changes: Different implementation of FreqStack_v2::push()
	Runtime: 268 ms, faster than 79.72% of C++ online submissions for Maximum Frequency Stack.
	Memory Usage: 68.1 MB, less than 85.91% of C++ online submissions for Maximum Frequency Stack.
	FreqStack_v3: Changes: store frequency and index in one variable
	Runtime: 264 ms, faster than 82.22% of C++ online submissions for Maximum Frequency Stack.
	Memory Usage: 68.1 MB, less than 85.91% of C++ online submissions for Maximum Frequency Stack.
	FreqStack_v4: 

	*/

	class FreqStack_v1 {
	public:
		FreqStack_v1()
		{
			index_ = 0;
		}

		void push(int x) {
			//Insert it into hash table
			auto itPair = hash_.insert({ x, 0 });
			auto it = itPair.first;
			it->second += 1;
			max_heap_.push(data{x, it->second, ++index_});		
		}

		int pop() {
			data d = max_heap_.top();
			max_heap_.pop();
			--hash_[d.num];

			return d.num;
		}

	private:
		struct data
		{
			data(int n, unsigned int freq, int ind)
			{
				num = n;
				frequency = freq;
				index = ind;
			}

			int num;
			unsigned int frequency; //it is always non-negative.
			unsigned long index; // it is always incremental, it never decreases
		};

		int index_;
		unordered_map<int, unsigned int> hash_; //Map of num -> frequency

		struct comp
		{
			bool operator()(const data& lhs,
				const data& rhs) const
			{
				if (lhs.frequency == rhs.frequency)
					return lhs.index < rhs.index; //max index would be at top
				else
					return lhs.frequency < rhs.frequency; //max frequency would be at top
			}
		};

		priority_queue<data, vector<data>, comp> max_heap_;
	};


	//Changes: Different implementation of FreqStack_v2::push()
	class FreqStack_v2 {
	public:
		FreqStack_v2()
		{
			index_ = 0;
		}

		void push(int x) {
			unsigned int& freq = hash_[x];
			++freq;
			max_heap_.push(data{ x, freq, ++index_ });
		}

		int pop() {
			data d = max_heap_.top();
			max_heap_.pop();
			--hash_[d.num];

			return d.num;
		}

	private:
		struct data
		{
			data(int n, unsigned int freq, int ind)
			{
				num = n;
				frequency = freq;
				index = ind;
			}

			int num;
			unsigned int frequency; //it is always non-negative.
			unsigned long index; // it is always incremental, it never decreases
		};

		int index_;
		unordered_map<int, unsigned int> hash_; //Map of num -> frequency

		struct comp
		{
			bool operator()(const data& lhs,
				const data& rhs) const
			{
				if (lhs.frequency == rhs.frequency)
					return lhs.index < rhs.index; //max index would be at top
				else
					return lhs.frequency < rhs.frequency; //max frequency would be at top
			}
		};

		priority_queue<data, vector<data>, comp> max_heap_;
	};

	//Changes: store frequency and index in one variable
	class FreqStack_v3 {
	public:
		FreqStack_v3()
		{
			index_ = 0;
		}

		void push(int x) {

			unsigned int& freq = hash_[x];
			++freq;
			max_heap_.push(data{ x, freq, ++index_ });
		}

		int pop() {
			data d = max_heap_.top();
			max_heap_.pop();
			--hash_[d.num];

			return d.num;
		}

	private:
		struct data
		{
			data(int n, unsigned int freq, unsigned int ind)
				: num(n), frequency_index(((unsigned long)freq << 32) | ind)
			{
			}

			int num;
			unsigned long frequency_index; // combined frequency and index
		};

		unsigned int index_;
		unordered_map<int, unsigned int> hash_; //Map of num -> frequency

		struct comp
		{
			bool operator()(const data& lhs,
				const data& rhs) const
			{
				return lhs.frequency_index < rhs.frequency_index; //max frequency_index would be at top
			}
		};

		priority_queue<data, vector<data>, comp> max_heap_;
	};

	//Changes: 
	class FreqStack_v4 {
	public:
		FreqStack_v4()
			: freq_stacks_(1)
		{
		}

		void push(int x) {

			unsigned int& freq = hash_[x];
			++freq;
			if (freq == freq_stacks_.size())
			{
				//if (freq_stacks_.capacity() <= freq)
				//	freq_stacks_.reserve(freq_stacks_.capacity() + 100);
				freq_stacks_.push_back({});
			}
			freq_stacks_[freq].push_back(x);
		}

		int pop() {
			int num = *freq_stacks_.rbegin()->rbegin();
			freq_stacks_.rbegin()->pop_back();
			if (freq_stacks_.rbegin()->size() == 0)
				freq_stacks_.pop_back();

			--hash_[num];

			return num;
		}

	private:
		unordered_map<int, unsigned int> hash_; //Map of num -> frequency
		vector< vector<int> > freq_stacks_; //freq_stacks_[freq] is a stack of num having frequency = freq
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
		FreqStack_v4 s;
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