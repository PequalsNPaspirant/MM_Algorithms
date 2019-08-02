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


	// top-down non recursive
	double DP_KnapsackProblem_0_1_top_down_using_map_non_recursive(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, unordered_map<size_t, double>& lookup, vector<StackMember>& functionStack)
	{
		int stackIndex = -1;
		double returnValue = 0;
		functionStack[++stackIndex].numItems = numItems;
		functionStack[stackIndex].knapsackCapacity = knapsackCapacity;
		functionStack[stackIndex].returnValuePlaceHolder = &returnValue;
		functionStack[stackIndex].include = numeric_limits<double>::max();
		functionStack[stackIndex].exclude = numeric_limits<double>::max();
		++numberOfFunctionCalls;

		while (stackIndex > -1)
		{
			if (functionStack[stackIndex].numItems == 0 || functionStack[stackIndex].knapsackCapacity == 0)
			{
				// While returning from a function, copy the return value and decrement the stack index
				*functionStack[stackIndex].returnValuePlaceHolder = 0;
				--stackIndex;
			}

			//string key(to_string(numItems) + "_" + to_string(knapsackCapacity));
			// Instead of creating string, we can use hash-combine like utility to ensure unique hash from two numbers
			size_t key = getHash(functionStack[stackIndex].numItems, functionStack[stackIndex].knapsackCapacity);
			pair<unordered_map<size_t, double>::iterator, bool> result = lookup.insert(unordered_map<size_t, double>::value_type(key, numeric_limits<double>::max()));
			// key may be inserted because it may not be already present. But the value is max() which ensures it is not calculated yet.
			if (result.first->second == numeric_limits<double>::max())
			{
				//recursive function call means, build a new stack element and increment the stack index
				//Make sure you carry the information of where to return i.e. which value of earlier function stack will get the result from this call/stack element

				// If first recusrive call is not yet made i.e. exclude value is not yet calculated
				if (functionStack[stackIndex].exclude == numeric_limits<double>::max())
				{
					functionStack[stackIndex + 1].numItems = functionStack[stackIndex].numItems - 1;
					functionStack[stackIndex + 1].knapsackCapacity = functionStack[stackIndex].knapsackCapacity;
					functionStack[stackIndex + 1].returnValuePlaceHolder = &functionStack[stackIndex].exclude;
					functionStack[stackIndex + 1].include = numeric_limits<double>::max();
					functionStack[stackIndex + 1].exclude = numeric_limits<double>::max();
					++stackIndex;
					++numberOfFunctionCalls;
					continue;
				}

				if (weights[functionStack[stackIndex].numItems - 1] <= functionStack[stackIndex].knapsackCapacity)
				{
					// If second recusrive call is not yet made i.e. include value is not yet calculated
					if (functionStack[stackIndex].include == numeric_limits<double>::max())
					{
						functionStack[stackIndex + 1].numItems = functionStack[stackIndex].numItems - 1;
						functionStack[stackIndex + 1].knapsackCapacity = functionStack[stackIndex].knapsackCapacity - weights[functionStack[stackIndex].numItems - 1];
						functionStack[stackIndex + 1].returnValuePlaceHolder = &functionStack[stackIndex].include;
						functionStack[stackIndex + 1].include = numeric_limits<double>::max();
						functionStack[stackIndex + 1].exclude = numeric_limits<double>::max();
						++stackIndex;
						++numberOfFunctionCalls;
						continue;
					}

					functionStack[stackIndex].include += values[functionStack[stackIndex].numItems - 1];
				}
				else
					functionStack[stackIndex].include = 0;

				result.first->second = std::max(functionStack[stackIndex].exclude, functionStack[stackIndex].include);
			}

			*functionStack[stackIndex].returnValuePlaceHolder = result.first->second;
			--stackIndex;
		}

		return returnValue;
	}
}
