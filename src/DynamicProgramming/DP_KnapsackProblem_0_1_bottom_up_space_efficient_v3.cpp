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

	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v3(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<double>& cache_bottom_up)
	{
		// Cache just ONE ROW instead of entire grid

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		for (unsigned long long i = 0; i <= knapsackCapacity; ++i)
			cache_bottom_up[i] = 0ULL;

		// If we have i items
		for (unsigned long long i = 0; i < values.size() - 1; ++i)
		{
			// If the knapsack capacity is j
			for (unsigned long long j = knapsackCapacity; j >= weights[i]; --j)
			{
				++numberOfFunctionCalls;
				//double include = values[i] + cache_bottom_up[j - weights[i]];
				//if(include > cache_bottom_up[j])
				//	cache_bottom_up[j] = include;	// include current item
				cache_bottom_up[j] = std::max(cache_bottom_up[j], values[i] + cache_bottom_up[j - weights[i]]);
			}
		}

		int lastIndex = values.size() - 1;
		if (knapsackCapacity < weights[lastIndex])
			return cache_bottom_up[knapsackCapacity];
		else
			return std::max(cache_bottom_up[knapsackCapacity], values[lastIndex] + cache_bottom_up[knapsackCapacity - weights[lastIndex]]);
	}
}
