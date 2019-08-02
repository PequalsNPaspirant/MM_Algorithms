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


	// ============================= Dynamic Programing Approach =============================

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack would grow with O(m + n)
	*/

	// Using vector
	double DP_KnapsackProblem_0_1_top_down_using_vector(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<vector<double>>& lookupVector)
	{
		++numberOfFunctionCalls;
		if (numItems == 0 || knapsackCapacity == 0)
			return 0;

		if (lookupVector[numItems - 1][knapsackCapacity - 1] == 0)
		{
			double exclude = DP_KnapsackProblem_0_1_top_down_using_vector(values, numItems - 1, weights, knapsackCapacity, selectedItems, lookupVector);
			double include = 0;
			if (weights[numItems - 1] <= knapsackCapacity)
				include = values[numItems - 1] + DP_KnapsackProblem_0_1_top_down_using_vector(values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems, lookupVector);
			lookupVector[numItems - 1][knapsackCapacity - 1] = std::max(exclude, include);
		}

		return lookupVector[numItems - 1][knapsackCapacity - 1];
	}
}
