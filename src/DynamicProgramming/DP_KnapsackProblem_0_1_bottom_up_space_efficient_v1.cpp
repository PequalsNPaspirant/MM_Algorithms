#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <random>
#include <functional> //std::bind
#include <map>
#include <queue>
#include <deque>
#include <memory>
#include <numeric> //For std::iota to generate the sequence of numbers
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Utils/Utils_PrintUtils.h"
//#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	// ============================= Problem Statement: =============================
	/*
	Given a set of items, each with a weight and a value, determine the number of each item to include in a collection
	so that the total weight is less than or equal to a given limit and the total value is as large as possible.
	In short:
	maximize = sumof(ViXi)
	subject to sumof(WiXi) <= W
				Xi = 0 or 1 ...For 0-1 knapsack problem
	Vi = value of ith item
	Wi = weight of ith item
	Xi = number of items

	In case of bounded knapsack problem, 0 < Xi < Ci where Ci = number of ith items available
	In case of unbounded knapsack problem, 0 < Xi where there is no upper bound on number of ith items available
	*/
	

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m)
	The recursive call stack : NOT APPLICABLE
	*/
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up_prev, vector<double>& cache_bottom_up_current)
	{
		// Cache just two rows instead of entire grid
		//vector<unsigned long long> prev(knapsackCapacity + 1);
		//vector<unsigned long long> current(knapsackCapacity + 1);

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		for (unsigned long long i = 0; i < knapsackCapacity + 1; ++i)
			cache_bottom_up_prev[i] = 0ULL;

		//// If we have zero items, max value we have is zero...irrespective of any knapsackCapacity
		//for (unsigned long long j = 0; j < knapsackCapacity + 1; ++j)
		//	cache[0][j] = 0;

		// If we have i items
		for (unsigned long long i = 1; i < values.size() + 1; ++i)
		{
			// If the knapsack capacity is j
			for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
			{
				++numberOfFunctionCalls;
				// If current capacity of knapsack can include current item
				if (weights[i - 1] <= j)
					cache_bottom_up_current[j] = std::max(cache_bottom_up_prev[j], values[i - 1] + cache_bottom_up_prev[j - weights[i - 1]]);	// include current item
				else
					cache_bottom_up_current[j] = cache_bottom_up_prev[j];	// exclude current item as knapsack capacity is over
			}

			for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
				cache_bottom_up_prev[j] = cache_bottom_up_current[j];
		}

		return cache_bottom_up_current[knapsackCapacity];
	}
}