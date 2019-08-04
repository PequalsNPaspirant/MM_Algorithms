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


	std::hash<size_t> hasher{};

	template <class T>
	static inline void hash_combine(std::size_t& seed, const T& v)  // Similar to boost::hash_combine<T>
	{
		/*
		The magic number below is the reciprocal of the golden ratio.
		Reference:
		https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
		http://burtleburtle.net/bob/hash/doobs.html
		phi = (1 + sqrt(5)) / 2
		2^32 / phi = 0x9e3779b9
		*/
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	size_t getHash(size_t num1, size_t num2) noexcept
	{
		std::size_t seed = 0;
		hash_combine<size_t>(seed, num1);
		hash_combine<size_t>(seed, num2);
		return seed;
	}

	// Using unordered_map
	double DP_KnapsackProblem_0_1_top_down_using_hashmap(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, unordered_map<size_t, double>& lookupHashMap)
	{
		++numberOfFunctionCalls;
		if (numItems == 0 || knapsackCapacity == 0)
			return 0;

		//string key(to_string(numItems) + "_" + to_string(knapsackCapacity));
		// Instead of creating string, we can use hash-combine like utility to ensure unique hash from two numbers
		size_t key = getHash(numItems, knapsackCapacity);
		pair<unordered_map<size_t, double>::iterator, bool> result = lookupHashMap.insert(unordered_map<size_t, double>::value_type(key, 0));
		if (result.second == true) // key inserted because it was not already present
		{
			double exclude = DP_KnapsackProblem_0_1_top_down_using_hashmap(values, numItems - 1, weights, knapsackCapacity, selectedItems, lookupHashMap);
			double include = 0;
			if (weights[numItems - 1] <= knapsackCapacity)
				include = values[numItems - 1] + DP_KnapsackProblem_0_1_top_down_using_hashmap(values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems, lookupHashMap);
			result.first->second = std::max(exclude, include);
		}

		return result.first->second;
	}
}