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

	// ============================= Solution using greedy approach =============================
	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m + n)
	Space Complexity : O( )
	The recursive call stack : NOT APPLICABLE
	*/

	double DP_KnapsackProblem_0_1_greedy(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems)
	{
		// Arrange the input in descending order of value/weight ratio
		vector<unsigned long long> indices(values.size());
		for (unsigned long long i = 0; i < indices.size(); ++i)
			indices[i] = i;

		std::sort(indices.begin(), indices.end(), [values, weights](unsigned long long a, unsigned long long b) -> bool { return (values[a] / weights[a]) >(values[b] / weights[b]);  });
		double maxValue = 0.0;
		auto iter = min_element(weights.begin(), weights.end());
		unsigned long long smallestWeight = *iter;
		for (unsigned long long i = 0; i < indices.size() && knapsackCapacity >= smallestWeight; ++i)
		{
			++numberOfFunctionCalls;
			if (knapsackCapacity >= weights[indices[i]])
			{
				maxValue += values[indices[i]];
				knapsackCapacity -= weights[indices[i]];
			}
		}

		return maxValue;
	}
}
