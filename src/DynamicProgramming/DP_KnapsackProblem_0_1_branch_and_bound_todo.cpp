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

	// ============================= Solution using branch and bound approach v10b =============================
	/*
	Remove cumulative sum vectors. Keep endIndex and endValue in heap node to use it to calculate the new upperbound.
	*/

	// ============================= Solution using branch and bound approach v11b =============================
	/*
	Use Set in place of heap and remove elements having upper bound less than maxSoFar. This reduces space complexity.
	This is already done by starting with greedy maxvalue. This already reduces heap size by large margin. But still there are cases,
	where we need large heap size.
	*/

	// ============================= Solution using branch and bound approach v12b =============================
	/*
	Reduce numFunctionCalls i.e. optimize exploring solution tree. avoid exploring the nodes further.
	*/

	// ============================= Solution using branch and bound approach v13b =============================
	/*
	Replace heap by the vector of size numValues. Keep only one heap element in vector for each level.
	This would limit space complexity to number of values. Currently space complexity is O(2^n).
	*/

}
