#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include <locale>
#include <random>
#include <algorithm>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {
	template<typename T>
	void branchPredictionDemo(const std::vector<T>& numbers, const T& pivot)
	{
		chrono::steady_clock::time_point start, end;
		int lessThan, moreThan, equalTo;
		lessThan = moreThan = equalTo = 0;

		start = chrono::steady_clock::now();
		for (int i = 0; i < numbers.size(); ++i)
		{
			if (numbers[i] < pivot)
				++lessThan;
			else if (numbers[i] > pivot)
				++moreThan;
			else
				++equalTo;
		}
		end = chrono::steady_clock::now();

		auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

		std::cout << "\n"
			<< " Duration: " << std::setw(15) << std::right << duration << " ns"
			<< " total: " << std::setw(12) << std::right << (lessThan + moreThan + equalTo)
			<< " lessThan: " << std::setw(12) << std::right << lessThan
			<< " moreThan: " << std::setw(12) << std::right << moreThan
			<< " equalTo: " << std::setw(5) << std::right << equalTo
			<< " pivot: " << std::setw(28) << std::right << pivot
			<< " first: " << std::setw(28) << std::right << numbers[0]
			<< " last: " << std::setw(28) << std::right << numbers[numbers.size() - 1];
	}

	template<typename T>
	void testBranchPrediction(int size)
	{
		std::random_device dev;
		std::mt19937 mt{ dev() };
		std::uniform_int_distribution<T> dist{std::numeric_limits<T>::min(), std::numeric_limits<T>::max()};
		T pivot = dist(mt);

		std::vector<T> numbers;
		numbers.reserve(size);
		for (int i = 0; i < size; ++i)
			numbers.push_back(dist(mt));

		branchPredictionDemo<T>(numbers, pivot);

		std::sort(numbers.begin(), numbers.end());
		branchPredictionDemo<T>(numbers, pivot);
	}

	MM_DECLARE_FLAG(CPU_branchPrediction);

	MM_UNIT_TEST(CPU_branchPrediction_test_1, CPU_branchPrediction)
	{
		std::cout.imbue(std::locale(""));

		std::vector<int> sizes{
			{       1'000 } //size of vector<size_t> = 8 KB
			,{      10'000 } //size of vector<size_t> = 80 KB
			,{     100'000 } //size of vector<size_t> = 800 KB
			,{    1'000'000 } //size of vector<size_t> = 8 MB
			,{   10'000'000 } //size of vector<size_t> = 80 MB
			,{  100'000'000 } //size of vector<size_t> = 800 MB
			//,{ 1'000'000'000 } //size of vector<size_t> = 8 GB
		};

		for (int i = 0; i < sizes.size(); ++i)
		{
			testBranchPrediction<unsigned long long>(sizes[i]);

			//unsigned int actualResult;
			//MM_EXPECT_TRUE((actualResult = branchPrediction(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		}		
	}
}

/*

 Duration:           4,700 ns total:        1,000 lessThan:          413 moreThan:          587 equalTo:     0 pivot:    7,742,625,649,921,801,367 first:    4,095,273,636,165,635,187 last:    9,139,226,647,284,684,866
 Duration:           1,300 ns total:        1,000 lessThan:          413 moreThan:          587 equalTo:     0 pivot:    7,742,625,649,921,801,367 first:        3,581,006,428,728,866 last:   18,442,835,100,512,329,987
 Duration:          44,200 ns total:       10,000 lessThan:        6,283 moreThan:        3,717 equalTo:     0 pivot:   11,550,430,573,747,413,155 first:   10,323,552,256,076,824,831 last:    2,741,210,718,717,158,135
 Duration:          25,200 ns total:       10,000 lessThan:        6,283 moreThan:        3,717 equalTo:     0 pivot:   11,550,430,573,747,413,155 first:        1,266,275,813,947,403 last:   18,445,832,926,144,430,563
 Duration:         429,500 ns total:      100,000 lessThan:       79,513 moreThan:       20,487 equalTo:     0 pivot:   14,722,474,188,124,331,279 first:    8,688,030,100,086,556,151 last:    4,746,500,741,964,661,690
 Duration:         140,500 ns total:      100,000 lessThan:       79,513 moreThan:       20,487 equalTo:     0 pivot:   14,722,474,188,124,331,279 first:          261,622,144,612,213 last:   18,446,091,677,631,930,348
 Duration:       5,109,300 ns total:    1,000,000 lessThan:      751,030 moreThan:      248,970 equalTo:     0 pivot:   13,862,166,760,389,318,853 first:   16,496,516,847,171,073,256 last:    8,180,363,916,545,449,881
 Duration:       1,090,200 ns total:    1,000,000 lessThan:      751,030 moreThan:      248,970 equalTo:     0 pivot:   13,862,166,760,389,318,853 first:           33,089,889,494,507 last:   18,446,724,800,213,297,217
 Duration:      57,945,600 ns total:   10,000,000 lessThan:    6,038,970 moreThan:    3,961,030 equalTo:     0 pivot:   11,140,083,055,405,791,337 first:   18,355,818,183,875,377,319 last:   16,390,094,283,912,136,019
 Duration:      23,174,100 ns total:   10,000,000 lessThan:    6,038,970 moreThan:    3,961,030 equalTo:     0 pivot:   11,140,083,055,405,791,337 first:            1,128,249,017,141 last:   18,446,743,060,935,104,461
 Duration:     361,436,000 ns total:  100,000,000 lessThan:   20,938,233 moreThan:   79,061,767 equalTo:     0 pivot:    3,861,807,840,021,309,182 first:    1,765,731,509,774,832,958 last:    5,502,297,615,453,977,286
 Duration:     290,653,500 ns total:  100,000,000 lessThan:   20,938,233 moreThan:   79,061,767 equalTo:     0 pivot:    3,861,807,840,021,309,182 first:              445,010,261,002 last:   18,446,744,049,466,312,122
 
*/