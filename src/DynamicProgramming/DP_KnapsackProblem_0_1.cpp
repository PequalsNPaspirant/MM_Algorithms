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

//#include "Assert/MyAssert.h"
#include "Others/PrintUtils.h"
//#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Assert/MyAssert.h"

namespace mm {

	// ============================= Naive Approach =============================
	/*
	Problem Statement:
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
	Time Complexity  : 2^(m + n)
	Space Complexity : O(1)
	The recursive call stack would grow with O(m + n)
	*/	

	double DP_KnapsackProblem_0_1_naive_v1(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems)
	{
		++numberOfFunctionCalls;
		if (numItems == 0 || knapsackCapacity == 0)
			return 0;

		double exclude = DP_KnapsackProblem_0_1_naive_v1(values, numItems - 1, weights, knapsackCapacity, selectedItems);
		double include = 0;
		if (weights[numItems - 1] <= knapsackCapacity)
			include = values[numItems - 1] + DP_KnapsackProblem_0_1_naive_v1(values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems);
		return std::max(exclude, include);
	}

	double DP_KnapsackProblem_0_1_naive_v2(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems)
	{
		++numberOfFunctionCalls;
		if (knapsackCapacity == 0)
			return 0;
		if (numItems == 1)
		{
			if (weights[numItems - 1] <= knapsackCapacity)
				return values[numItems - 1];
			else
				return 0;
		}

		double exclude = DP_KnapsackProblem_0_1_naive_v2(values, numItems - 1, weights, knapsackCapacity, selectedItems);
		double include = 0;
		if (weights[numItems - 1] <= knapsackCapacity)
			include = values[numItems - 1] + DP_KnapsackProblem_0_1_naive_v2(values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems);
		return std::max(exclude, include);
	}

	double DP_KnapsackProblem_0_1_naive_tail_recursive(double retValue, const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems)
	{
		/*
		// This is not tail-recursive but another implementation of above algo. Run time is similar.
		++numberOfFunctionCalls;
		if (numItems == 0 || knapsackCapacity == 0)
			return 0;
		
		if (weights[numItems - 1] <= knapsackCapacity)
		{
			double include = values[numItems - 1] + DP_KnapsackProblem_0_1_naive(values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems);
			double exclude = DP_KnapsackProblem_0_1_naive(values, numItems - 1, weights, knapsackCapacity, selectedItems);
			return std::max(exclude, include);
		}
		else
			return DP_KnapsackProblem_0_1_naive(values, numItems - 1, weights, knapsackCapacity, selectedItems);
		*/

		//double exclude = DP_KnapsackProblem_0_1_naive(values, numItems - 1, weights, knapsackCapacity, selectedItems);
		double include = 0;
		if (weights[numItems - 1] <= knapsackCapacity)
			include = values[numItems - 1] + DP_KnapsackProblem_0_1_naive_tail_recursive(retValue, values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems);
		
		//return std::max(exclude, include);
		return DP_KnapsackProblem_0_1_naive_tail_recursive(std::max(retValue, include), values, numItems - 1, weights, knapsackCapacity, selectedItems);
	}

	double DP_KnapsackProblem_0_1_naive_non_recursive_v1(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<StackMember>& functionStack)
	{
		//++numberOfFunctionCalls;
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
				if (stackIndex == -1) 
					break;
			}

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

			*functionStack[stackIndex].returnValuePlaceHolder = std::max(functionStack[stackIndex].exclude, functionStack[stackIndex].include);
			--stackIndex;
		}

		return returnValue;
	}

	double DP_KnapsackProblem_0_1_naive_non_recursive_v2(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<StackMember>& functionStack)
	{
		//++numberOfFunctionCalls;
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
			if (functionStack[stackIndex].knapsackCapacity == 0)
			{
				// While returning from a function, copy the return value and decrement the stack index
				*functionStack[stackIndex].returnValuePlaceHolder = 0;
				--stackIndex;
			}
			else if (functionStack[stackIndex].numItems == 1)
			{
				// While returning from a function, copy the return value and decrement the stack index
				if (weights[functionStack[stackIndex].numItems - 1] <= functionStack[stackIndex].knapsackCapacity)
					*functionStack[stackIndex].returnValuePlaceHolder = values[functionStack[stackIndex].numItems - 1];
				else
					*functionStack[stackIndex].returnValuePlaceHolder = 0;
				--stackIndex;
			}

			if (stackIndex == -1) break;
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

			*functionStack[stackIndex].returnValuePlaceHolder = std::max(functionStack[stackIndex].exclude, functionStack[stackIndex].include);
			--stackIndex;
		}

		return returnValue;
	}

	double DP_KnapsackProblem_0_1_naive_non_recursive_v3(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<StackMember>& functionStack)
	{
		//++numberOfFunctionCalls;
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
			//recursive function call means, build a new stack element and increment the stack index
			//Make sure you carry the information of where to return i.e. which value of earlier function stack will get the result from this call/stack element
			
			// If first recusrive call is not yet made i.e. exclude value is not yet calculated
			if (functionStack[stackIndex].exclude == numeric_limits<double>::max())
			{
				//if (functionStack[stackIndex].numItems == 1)
				//{
				//	// While returning from a function, copy the return value and decrement the stack index
				//	//*functionStack[stackIndex].returnValuePlaceHolder = values[functionStack[stackIndex].numItems - 1];
				//	//--stackIndex;
				//	functionStack[stackIndex].exclude = values[functionStack[stackIndex].numItems - 1];
				//}
				//else
				{
					if (functionStack[stackIndex].numItems > 1)
					{
						functionStack[stackIndex + 1].numItems = functionStack[stackIndex].numItems - 1;
						functionStack[stackIndex + 1].knapsackCapacity = functionStack[stackIndex].knapsackCapacity;
						functionStack[stackIndex + 1].returnValuePlaceHolder = &functionStack[stackIndex].exclude;
						functionStack[stackIndex + 1].include = numeric_limits<double>::max();
						functionStack[stackIndex + 1].exclude = numeric_limits<double>::max();
						++stackIndex;
						++numberOfFunctionCalls;
					}

					if (functionStack[stackIndex].numItems == 1)
					{
						// While returning from a function, copy the return value and decrement the stack index
						if (weights[functionStack[stackIndex].numItems - 1] <= functionStack[stackIndex].knapsackCapacity)
							*functionStack[stackIndex].returnValuePlaceHolder = values[functionStack[stackIndex].numItems - 1];
						else
							*functionStack[stackIndex].returnValuePlaceHolder = 0;
						--stackIndex;
					}
					else
						continue;
				}
			}
			if (stackIndex == -1) break;

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

					if (functionStack[stackIndex].knapsackCapacity == 0)
					{
						// While returning from a function, copy the return value and decrement the stack index
						*functionStack[stackIndex].returnValuePlaceHolder = 0;
						--stackIndex;
					}
					else if (functionStack[stackIndex].numItems == 1)
					{
						// While returning from a function, copy the return value and decrement the stack index
						if (weights[functionStack[stackIndex].numItems - 1] <= functionStack[stackIndex].knapsackCapacity)
							*functionStack[stackIndex].returnValuePlaceHolder = values[functionStack[stackIndex].numItems - 1];
						else
							*functionStack[stackIndex].returnValuePlaceHolder = 0;
						--stackIndex;
					}
					else
						continue;
				}

				functionStack[stackIndex].include += values[functionStack[stackIndex].numItems - 1];
			}
			else
				functionStack[stackIndex].include = 0;

			*functionStack[stackIndex].returnValuePlaceHolder = std::max(functionStack[stackIndex].exclude, functionStack[stackIndex].include);
			--stackIndex;
		}

		return returnValue;
	}

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

	// top-down tail recursive
	double DP_KnapsackProblem_0_1_top_down_using_map_tail_recursive(const vector<double>& values, unsigned long long numItems, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, unordered_map<size_t, double>& lookup)
	{
		++numberOfFunctionCalls;
		if (numItems == 0 || knapsackCapacity == 0)
			return 0;

		//string key(to_string(numItems) + "_" + to_string(knapsackCapacity));
		// Instead of creating string, we can use hash-combine like utility to ensure unique hash from two numbers
		size_t key = getHash(numItems, knapsackCapacity);
		pair<unordered_map<size_t, double>::iterator, bool> result = lookup.insert(unordered_map<size_t, double>::value_type(key, 0));
		if (result.second == true) // key inserted because it was not already present
		{
			double exclude = DP_KnapsackProblem_0_1_top_down_using_map_tail_recursive(values, numItems - 1, weights, knapsackCapacity, selectedItems, lookup);
			double include = 0;
			if (weights[numItems - 1] <= knapsackCapacity)
				include = values[numItems - 1] + DP_KnapsackProblem_0_1_top_down_using_map_tail_recursive(values, numItems - 1, weights, knapsackCapacity - weights[numItems - 1], selectedItems, lookup);
			result.first->second = std::max(exclude, include);
		}

		return result.first->second;
	}

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

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack : NOT APPLICABLE

	Example:
	item index = { 1, 2,  3,  4,  5,  6}
	values     = {20, 5, 10, 40, 15, 25}
	weights    = { 1, 2,  3,  8,  7,  4}
	knapsack capacity = 10

	bottom up approach:

	knapsack capacity --->  0	 1	 2	 3	 4	 5	 6	 7	 8	 9	10
			num_items   0	0	 0	 0	 0	 0	 0	 0	 0	 0	 0	 0
				||      1	0	20	20	20	20	20	20	20	20	20	20
				\/      2	0	20	20	25	25	25	25	25	25	25	25
						3	0	20	20	25	30	30	35	35	35	35	35
						4	0	20	20	25	30	30	35	35	40	60	60
						5	0	20	20	25	30	30	35	35	40	60	60
						6	0	20	20	25	30	45	45	50	55	60	60

	*/

	double DP_KnapsackProblem_0_1_bottom_up_v1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<vector<double>>& cache_bottom_up_2d)
	{
		//vector<vector<unsigned long long>> cache(values.size() + 1, vector<unsigned long long>(knapsackCapacity + 1));

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		// zero'th column in above comments
		for (unsigned long long i = 0; i < values.size() + 1; ++i)
			cache_bottom_up_2d[i][0] = 0;

		// If we have zero items, max value we have is zero...irrespective of any knapsackCapacity
		// zero'th row in above comments
		for (unsigned long long j = 0; j < knapsackCapacity + 1; ++j)
			cache_bottom_up_2d[0][j] = 0;

		// If we have i items
		for (unsigned long long i = 1; i < values.size() + 1; ++i)
		{
			// If the knapsack capacity is j
			for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
			{
				++numberOfFunctionCalls;
				// If current capacity of knapsack can include current item
				if (weights[i - 1] <= j)
					cache_bottom_up_2d[i][j] = std::max(cache_bottom_up_2d[i - 1][j], values[i - 1] + cache_bottom_up_2d[i - 1][j - weights[i - 1]]); // include current item
				else
					cache_bottom_up_2d[i][j] = cache_bottom_up_2d[i - 1][j];       // exclude current item
			}
		}

		return cache_bottom_up_2d[values.size()][knapsackCapacity];
	}

	double DP_KnapsackProblem_0_1_bottom_up_v2(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<double>& cache_bottom_up)
	{
		//vector<vector<unsigned long long>> cache(values.size() + 1, vector<unsigned long long>(knapsackCapacity + 1));

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		// zero'th column in above comments
		int rowSize = knapsackCapacity + 1;
		for (unsigned long long i = 0; i < values.size() + 1; ++i)
			cache_bottom_up[i * rowSize + 0] = 0;

		// If we have zero items, max value we have is zero...irrespective of any knapsackCapacity
		// zero'th row in above comments
		for (unsigned long long j = 0; j < knapsackCapacity + 1; ++j)
			cache_bottom_up[0 * rowSize + j] = 0;

		// If we have i items
		for (int i = 1; i < values.size() + 1; ++i)
		{
			// If the knapsack capacity is j
			for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
			{
				++numberOfFunctionCalls;
				// If current capacity of knapsack can include current item
				if (weights[i - 1] <= j)
					cache_bottom_up[i * rowSize + j] = std::max(cache_bottom_up[(i - 1) * rowSize + j], values[i - 1] + cache_bottom_up[(i - 1) * rowSize + (j - weights[i - 1])]); // include current item
				else
					cache_bottom_up[i * rowSize + j] = cache_bottom_up[(i - 1) * rowSize + j];       // exclude current item
			}
		}

		return cache_bottom_up[values.size() * rowSize + knapsackCapacity];
	}

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


	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m)
	The recursive call stack : NOT APPLICABLE
	*/
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v1_1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up_prev, vector<double>& cache_bottom_up_current)
	{
		// Cache just two rows instead of entire grid
		//vector<unsigned long long> prev(knapsackCapacity + 1);
		//vector<unsigned long long> current(knapsackCapacity + 1);

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		for (unsigned long long i = 0; i < knapsackCapacity + 1; ++i)
			cache_bottom_up_current[i] = 0ULL;

		//// If we have zero items, max value we have is zero...irrespective of any knapsackCapacity
		//for (unsigned long long j = 0; j < knapsackCapacity + 1; ++j)
		//	cache[0][j] = 0;

		vector<double>* pPrev = &cache_bottom_up_prev;
		vector<double>* pCurrent = &cache_bottom_up_current;

		// If we have i items
		for (unsigned long long i = 1; i < values.size() + 1; ++i)
		{
			vector<double>& prev = *pPrev;
			vector<double>& current = *pCurrent;
			// If the knapsack capacity is j
			for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
			{
				++numberOfFunctionCalls;
				// If current capacity of knapsack can include current item
				if (weights[i - 1] <= j)
					current[j] = std::max(prev[j], values[i - 1] + prev[j - weights[i - 1]]);	// include current item
				else
					current[j] = prev[j];	// exclude current item as knapsack capacity is over
			}

			// Avoid copying these arrays
			//for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
			//	cache_bottom_up_prev[j] = cache_bottom_up_current[j];
			std::swap(pPrev, pCurrent);
		}

		return cache_bottom_up_current[knapsackCapacity];
	}

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m)
	The recursive call stack : NOT APPLICABLE
	*/
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v2(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<double>& cache_bottom_up)
	{
		// Cache just ONE ROW instead of entire grid

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		for (unsigned long long i = 0; i <= knapsackCapacity; ++i)
			cache_bottom_up[i] = 0ULL;

		// If we have i items
		for (unsigned long long i = 0; i < values.size(); ++i)
		{
			// If the knapsack capacity is j
			for (unsigned long long j = knapsackCapacity; j >= weights[i]; --j)
			{
				++numberOfFunctionCalls;
				cache_bottom_up[j] = std::max(cache_bottom_up[j], values[i] + cache_bottom_up[j - weights[i]]);	// include current item
			}
		}

		return cache_bottom_up[knapsackCapacity];
	}

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

	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v4(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<double>& cache_bottom_up)
	{
		// Cache just ONE ROW instead of entire grid

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		for (unsigned long long i = 0; i <= knapsackCapacity; ++i)
			cache_bottom_up[i] = 0ULL;

		// If we have i items
		for (unsigned long long i = 0; i < values.size() - 1; ++i)
		{
			// If the knapsack capacity is j
			for (unsigned long long j = knapsackCapacity; j > weights[i]; --j)
			{
				++numberOfFunctionCalls;
				//double include = values[i] + cache_bottom_up[j - weights[i]];
				//if(include > cache_bottom_up[j])
				//	cache_bottom_up[j] = include;	// include current item
				cache_bottom_up[j] = std::max(cache_bottom_up[j], values[i] + cache_bottom_up[j - weights[i]]);
			}
			//when j == weights[i], cache_bottom_up[0] = 0
			cache_bottom_up[weights[i]] = std::max(cache_bottom_up[weights[i]], values[i]);
		}

		int lastIndex = values.size() - 1;
		return std::max(cache_bottom_up[knapsackCapacity], values[lastIndex] + cache_bottom_up[knapsackCapacity - weights[lastIndex]]);
	}

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack : NOT APPLICABLE

	Example:
	item index = { 1, 2,  3,  4,  5,  6}
	values     = {20, 5, 10, 40, 15, 25}
	weights    = { 1, 2,  3,  8,  7,  4}
	knapsack capacity = 10

	bottom up approach:

	knapsack capacity --->  0	 1	 2	 3	 4	 5	 6	 7	 8	 9	10
			num_items   0	0	 0	 0	 0	 0	 0	 0	 0	 0	 0	 0
				||      1	0	20	20	20	20	20	20	20	20	20	20
				\/      2	0	20	20	25	25	25	25	25	25	25	25
						3	0	20	20	25	30	30	35	35	35	35	35
						4	0	20	20	25	30	30	35	35	40	60	60
						5	0	20	20	25	30	30	35	35	40	60	60
						6	0	20	20	25	30	45	45	50	55	60	60

	*/
	double DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_using_set(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems)
	{
		// Cache just ONE ROW instead of entire grid

		//struct MapValue
		//{
		//	MapValue()
		//		: value(0),
		//		step(0)
		//	{}

		//	MapValue(unsigned long long valueIn, unsigned long stepIn)
		//		: value(valueIn),
		//		step(stepIn)
		//	{}
		//	unsigned long long value;
		//	unsigned long step;
		//};

		//Keep map of <capacity, max-value>
		std::map<unsigned long long, double>* pPrev = new std::map<unsigned long long, double>;
		std::map<unsigned long long, double>* pCurrent = new std::map<unsigned long long, double>;
		// If we have i items
		(*pPrev)[weights[0]] = values[0];
		for (unsigned long long i = 1; i < values.size(); ++i)
		{
			std::map<unsigned long long, double>& prev = *pPrev;
			std::map<unsigned long long, double>& current = *pCurrent;
			current.clear();
			for (auto iterator = prev.begin(); iterator != prev.end(); ++iterator)
			{
				unsigned long long currentCapacity = iterator->first;
				//Recalculate it
				if(currentCapacity > weights[i])
				{
					if (currentCapacity - weights[i] > prev.begin()->first)
					{
						auto it = prev.upper_bound(currentCapacity - weights[i]);
						--it;
						current[currentCapacity] = max(iterator->second, values[i] + it->second);
					}
					else
						current[currentCapacity] = max(iterator->second, values[i] + 0);
				}

				if((currentCapacity + weights[i]) <= knapsackCapacity)
				{
					auto it = prev.upper_bound(currentCapacity + weights[i]); //it will not be equal to prev.begin()
					--it;
					current[currentCapacity + weights[i]] = max(it->second, values[i] + iterator->second);
				}
			}

			//The current element should also be added
			if (weights[i] < prev.begin()->first) //No FIX required for: when weights[i] == prev.begin()->first
			{
				current[weights[i]] = values[i];
			}
			else
			{
				auto it = prev.upper_bound(weights[i]);
				--it;
				current[weights[i]] = max(it->second, values[i] + 0); // knapsack value of capacity zero is 0
			}

			std::swap(pPrev, pCurrent);
		}

		int lastIndex = values.size() - 1;
		auto it1 = pPrev->upper_bound(knapsackCapacity); // it1 is pPrev->end()
		--it1;
		auto it2 = pPrev->upper_bound(knapsackCapacity - weights[lastIndex]);
		--it2;
		return std::max(it1->second, it2->second);
	}

	// This approach is not different than the above approach. In above approach we go on increasing the number of items.
	// In this approach the attempt is made to increase the capacity, but it needs all previous results of subproblems and ends up in same approach as above.
	double DP_KnapsackProblem_0_1_bottom_up_capacity_independent(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, vector<vector<double>>& cache_bottom_up_2d)
	{
		// Note: There is no way we can implement space efficient version similar to this considering only two columns from above grid. 
		// Reason: We may need to refer results from any of previous columns, and not just from previous column.

		//vector<vector<unsigned long long>> cache(values.size() + 1, vector<unsigned long long>(knapsackCapacity + 1));

		// If we have knapsack with capacity zero, for any number of items, max price will be zero as we can not select any item
		for (unsigned long long i = 0; i < values.size() + 1; ++i)
			cache_bottom_up_2d[i][0] = 0;

		// If we have zero items, max value we have is zero...irrespective of any knapsackCapacity
		for (unsigned long long j = 0; j < knapsackCapacity + 1; ++j)
			cache_bottom_up_2d[0][j] = 0;

		// If the knapsack capacity is j
		for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
		{
			// If we have i items
			for (unsigned long long i = 1; i < values.size() + 1; ++i)
			{
				++numberOfFunctionCalls;
				// If current capacity of knapsack can include current item
				if (weights[i - 1] <= j)
					cache_bottom_up_2d[i][j] = std::max(cache_bottom_up_2d[i - 1][j], values[i - 1] + cache_bottom_up_2d[i - 1][j - weights[i - 1]]); // include current item
				else
					cache_bottom_up_2d[i][j] = cache_bottom_up_2d[i - 1][j];   // exclude current item
			}
		}

		return cache_bottom_up_2d[values.size()][knapsackCapacity];
	}

	// This approach is not possible. See comments in above approach.
	double DP_KnapsackProblem_0_1_bottom_up_capacity_independent_space_efficient(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& cache_bottom_up_prev, vector<double>& cache_bottom_up_current)
	{
		//vector<unsigned long long> prev(values.size() + 1);
		//vector<unsigned long long> current(values.size() + 1);

		// If we have knapsack with capacity zero
		for (unsigned long long i = 0; i < values.size() + 1; ++i)
			cache_bottom_up_prev[i] = 0ULL;

		//// If we have zero items, max value we have is zero...irrespective of any knapsackCapacity
		//for (int j = 0; j < knapsackCapacity + 1; ++j)
		//	cache[0][j] = 0;

		// If the knapsack capacity is j
		for (unsigned long long j = 1; j < knapsackCapacity + 1; ++j)
		{
			// If we have i items
			for (unsigned long long i = 1; i < values.size() + 1; ++i)
			{
				++numberOfFunctionCalls;
				// If current capacity of knapsack can include current item
				if (weights[i - 1] <= j)
					cache_bottom_up_current[i] = std::max(cache_bottom_up_prev[i - 1], values[i - 1] + cache_bottom_up_prev[i - 1]);	// include current item
				else
					cache_bottom_up_current[i] = cache_bottom_up_prev[i - 1];	// exclude current item as knapsack capacity is over
			}

			for (unsigned long long i = 0; i < values.size() + 1; ++i)
				cache_bottom_up_prev[i] = cache_bottom_up_current[i];
		}

		return cache_bottom_up_current[values.size()];
	}

	// ============================= Solution using branch and bound approach v1 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version: NOT APPLICABLE
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
						+ O(n)     //to store sortedIndices
						+ O(n)     //to store valueToWeightsRatio used for sorting
						+ O(n)     //to store values
						+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v1
	{
		unsigned int level;
		double maxValue;
		unsigned long long totalWeight;
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& sortedIndices)
		{
			upperbound = maxValue;
			unsigned long long currentKnapsackCapacity = totalWeight;
			int i = level;
			for (; i < values.size() && currentKnapsackCapacity + weights[ sortedIndices[i] ] < knapsackCapacity; ++i)
			{
				currentKnapsackCapacity += weights[ sortedIndices[i] ];
				upperbound += values[ sortedIndices[i] ];
			}

			if (i < values.size() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[sortedIndices[i]] / weights[sortedIndices[i]]);
			}
		}
	};
	struct decisionTreeNodeCompare_v1
	{
		bool operator()(const decisionTreeNode_v1& lhs, const decisionTreeNode_v1& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};
	std::priority_queue<decisionTreeNode_v1, vector<decisionTreeNode_v1>, decisionTreeNodeCompare_v1> maxHeap_v1;

	double DP_KnapsackProblem_0_1_branch_and_bound_v1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		decisionTreeNode_v1 prev, current;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		prev.upperbound = 0;
		maxHeap_v1.push(prev);

		double maxValue = 0.0;

		while (!maxHeap_v1.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v1.size())
				sizeOfHeap = maxHeap_v1.size();

			prev = maxHeap_v1.top();
			maxHeap_v1.pop();

			if (prev.level == values.size())
				continue;

			current.level = prev.level + 1;
			unsigned int itemIndex = sortedIndices[current.level - 1];

			//include this item
			current.totalWeight = prev.totalWeight + weights[itemIndex];
			if(current.totalWeight <= knapsackCapacity)
			{
				current.maxValue = prev.maxValue + values[itemIndex];
				current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices);
				if (current.maxValue > maxValue)
					maxValue = current.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if (current.upperbound > maxValue)
					maxHeap_v1.push(current);
			}

			//exclude this item
			current.totalWeight = prev.totalWeight;
			current.maxValue = prev.maxValue;
			current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (current.upperbound > maxValue)
				maxHeap_v1.push(current);			
		}

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v2 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v1: 
		Instead of linearly calculating UpperBound in O(n) in calculateAndSetUpperBound(), keep the cumulative sum of
		weights and values after sorting, so that the upper bound can be searched in O(log n)
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
						+ O(n)     //to store sortedIndices
						+ O(n)     //to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)     //to store cumulativeWeights
						+ O(n)     //to store values
						+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v2
	{
		unsigned int level;
		double maxValue;
		unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned int currentIndex = level - 1; // level > 0 is Guaranteed 
			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeights[ sortedIndices[currentIndex] ] + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + currentIndex + 1, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + currentIndex + 1)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeights[sortedIndices[currentIndex]]);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValues[sortedIndices[currentIndex]]);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}

		friend bool operator<(const decisionTreeNode_v2& lhs, const decisionTreeNode_v2& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v2, vector<decisionTreeNode_v2>, std::less<decisionTreeNode_v2>> maxHeap_v2;

	double DP_KnapsackProblem_0_1_branch_and_bound_v2(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v2 prev, current;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		prev.upperbound = 0;
		maxHeap_v2.push(prev);

		double maxValue = 0.0;

		while (!maxHeap_v2.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v2.size())
				sizeOfHeap = maxHeap_v2.size();

			prev = maxHeap_v2.top();
			maxHeap_v2.pop();

			if (prev.level == values.size())
				continue;

			current.level = prev.level + 1;
			unsigned int itemIndex = sortedIndices[current.level - 1];

			//include this item
			current.totalWeight = prev.totalWeight + weights[itemIndex];
			if(current.totalWeight <= knapsackCapacity)
			{
				current.maxValue = prev.maxValue + values[itemIndex];
				current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				if (current.maxValue > maxValue)
					maxValue = current.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if (current.upperbound > maxValue)
					maxHeap_v2.push(current);
			}

			//exclude this item
			current.totalWeight = prev.totalWeight;
			current.maxValue = prev.maxValue;
			current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (current.upperbound > maxValue)
				maxHeap_v2.push(current);			
		}

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v3 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v2: 
		When we refer the top element in heap in while loop, Instead of removing the top item from heap and re-inserting it, 
		just change the level, maxValue and totalWeight. The upperbound remains the same, so its position in heap is unchanged. 
		Also no need to recalculate the upperbound for this.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
						+ O(n)     //to store sortedIndices
						+ O(n)     //to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)     //to store cumulativeWeights
						+ O(n)     //to store values
						+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v3
	{
		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}

		friend bool operator<(const decisionTreeNode_v3& lhs, const decisionTreeNode_v3& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v3, vector<decisionTreeNode_v3>, std::less<decisionTreeNode_v3>> maxHeap_v3;

	double DP_KnapsackProblem_0_1_branch_and_bound_v3(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v3 prev;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		//prev.upperbound = 0;
		prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v3.push(prev);

		double maxValue = 0.0;

		while (!maxHeap_v3.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v3.size())
				sizeOfHeap = maxHeap_v3.size();

			const decisionTreeNode_v3& top = maxHeap_v3.top();

			if (top.upperbound <= maxValue)
				break;

			if (top.level == values.size())
				continue;

			unsigned int itemIndex = sortedIndices[top.level];

			top.level += 1;

			prev = top;
			//include this item
			unsigned long long totalWeight = top.totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				top.totalWeight = totalWeight;
				top.maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top.maxValue > maxValue)
					maxValue = top.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top.upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v3.pop();

			//exclude this item
			prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev.upperbound > maxValue)
				maxHeap_v3.push(prev);			
		}

		while(!maxHeap_v3.empty())
			maxHeap_v3.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v3a =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v3: 
		Instead of starting with maxValue = 0, start with maxValue = greedy like max value.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
						+ O(n)     //to store sortedIndices
						+ O(n)     //to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)     //to store cumulativeWeights
						+ O(n)     //to store values
						+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v3a
	{
		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}

		friend bool operator<(const decisionTreeNode_v3a& lhs, const decisionTreeNode_v3a& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v3a, vector<decisionTreeNode_v3a>, std::less<decisionTreeNode_v3a>> maxHeap_v3a;

	double DP_KnapsackProblem_0_1_branch_and_bound_v3a(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v3a prev;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		//prev.upperbound = 0;
		prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v3a.push(prev);

		double maxValue = 0.0;
		vector<unsigned int>::iterator iter = std::upper_bound(sortedIndices.begin(), sortedIndices.end(), knapsackCapacity,
			[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });
		if (iter != sortedIndices.begin())
		{
			--iter;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
			maxValue = cumulativeValues[*iter];
		}

		while (!maxHeap_v3a.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v3a.size())
				sizeOfHeap = maxHeap_v3a.size();

			const decisionTreeNode_v3a& top = maxHeap_v3a.top();

			if (top.upperbound <= maxValue)
				break;

			if (top.level == values.size())
				continue;

			unsigned int itemIndex = sortedIndices[top.level];

			top.level += 1;

			prev = top;
			//include this item
			unsigned long long totalWeight = top.totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				top.totalWeight = totalWeight;
				top.maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top.maxValue > maxValue)
					maxValue = top.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3a.push(current);
				if (top.upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v3a.pop();

			//exclude this item
			prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev.upperbound > maxValue)
				maxHeap_v3a.push(prev);			
		}

		while(!maxHeap_v3a.empty())
			maxHeap_v3a.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v3b =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v3a: 
		Instead of starting with maxValue = 0, start with maxValue = exact greedy max value.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
						+ O(n)     //to store sortedIndices
						+ O(n)     //to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)     //to store cumulativeWeights
						+ O(n)     //to store values
						+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v3b
	{
		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}

		friend bool operator<(const decisionTreeNode_v3b& lhs, const decisionTreeNode_v3b& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v3b, vector<decisionTreeNode_v3b>, std::less<decisionTreeNode_v3b>> maxHeap_v3b;

	double DP_KnapsackProblem_0_1_branch_and_bound_v3b(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
	{
		unsigned long long smallestWeight = weights[0];
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
		{
			valueToWeightsRatio[i] = values[i] / weights[i];
			if (smallestWeight > weights[i])
				smallestWeight = weights[i];
		}

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v3b prev;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		//prev.upperbound = 0;
		prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v3b.push(prev);

		double maxValue = 0.0;
		vector<unsigned int>::iterator iter = std::upper_bound(sortedIndices.begin(), sortedIndices.end(), knapsackCapacity,
			[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });
		if (iter != sortedIndices.begin())
		{
			--iter;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
			maxValue = cumulativeValues[*iter];
			unsigned long long remainingKnapsackCapacity = knapsackCapacity - cumulativeWeights[*iter];
			for (++iter; iter != sortedIndices.end() && remainingKnapsackCapacity >= smallestWeight; ++iter)
			{
				if (remainingKnapsackCapacity >= weights[*iter])
				{
					maxValue += values[*iter];
					remainingKnapsackCapacity -= weights[*iter];
				}
			}
		}

		while (!maxHeap_v3b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v3b.size())
				sizeOfHeap = maxHeap_v3b.size();

			const decisionTreeNode_v3b& top = maxHeap_v3b.top();

			if (top.upperbound <= maxValue)
				break;

			if (top.level == values.size())
				continue;

			unsigned int itemIndex = sortedIndices[top.level];

			top.level += 1;

			prev = top;
			//include this item
			unsigned long long totalWeight = top.totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				top.totalWeight = totalWeight;
				top.maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top.maxValue > maxValue)
					maxValue = top.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3b.push(current);
				if (top.upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v3b.pop();

			//exclude this item
			prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev.upperbound > maxValue)
				maxHeap_v3b.push(prev);			
		}

		while(!maxHeap_v3b.empty())
			maxHeap_v3b.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v4 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v3:
		Use deque instead of vector in priority_queue.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
						+ O(n)     //to store sortedIndices
						+ O(n)     //to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)     //to store cumulativeWeights
						+ O(n)     //to store values
						+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v4
	{
		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}

		friend bool operator<(const decisionTreeNode_v4& lhs, const decisionTreeNode_v4& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v4, deque<decisionTreeNode_v4>, std::less<decisionTreeNode_v4>> maxHeap_v4;

	double DP_KnapsackProblem_0_1_branch_and_bound_v4(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v4 prev;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		//prev.upperbound = 0;
		prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v4.push(prev);

		double maxValue = 0.0;

		while (!maxHeap_v4.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v4.size())
				sizeOfHeap = maxHeap_v4.size();

			const decisionTreeNode_v4& top = maxHeap_v4.top();

			if (top.upperbound <= maxValue)
				break;

			if (top.level == values.size())
				continue;

			unsigned int itemIndex = sortedIndices[top.level];

			top.level += 1;

			prev = top;
			//include this item
			unsigned long long totalWeight = top.totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				top.totalWeight = totalWeight;
				top.maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top.maxValue > maxValue)
					maxValue = top.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top.upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v4.pop();

			//exclude this item
			prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev.upperbound > maxValue)
				maxHeap_v4.push(prev);			
		}

		while(!maxHeap_v4.empty())
			maxHeap_v4.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v5 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v4: 
		Use vector in priority_queue and use unique_ptr of heap object so that 
		swapping of objects is easy when rearranging heap elements.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
						+ O(2^n * 4)	//The heap object size is 4
						+ O(n)			//to store sortedIndices
						+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)			//to store cumulativeWeights
						+ O(n)			//to store values
						+ O(n)			//to store weights
	The recursive call stack : NOT APPLICABLE
	*/

	struct decisionTreeNode_v5
	{
		//decisionTreeNode_v5()
		//	: level(0),
		//	maxValue(0.0),
		//	totalWeight(0),
		//	upperbound(0.0)
		//{
		//}

		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, 
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[ *iter ] / weights[ *iter ]);
			}
		}
	};

	struct Comparator_v5
	{
		bool operator()(const unique_ptr<decisionTreeNode_v5>& lhs, const unique_ptr<decisionTreeNode_v5>& rhs)
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	std::priority_queue<unique_ptr<decisionTreeNode_v5>, vector<unique_ptr<decisionTreeNode_v5>>, Comparator_v5> maxHeap_v5;

	double DP_KnapsackProblem_0_1_branch_and_bound_v5(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
		//deque<decisionTreeNode_v4>& heapObjects)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		unique_ptr<decisionTreeNode_v5> obj = make_unique<decisionTreeNode_v5>();
		obj->level = 0;
		obj->maxValue = 0.0;
		obj->totalWeight = 0;
		//prev.upperbound = 0;
		obj->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v5.push(std::move(obj));

		double maxValue = 0.0;

		while (!maxHeap_v5.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v5.size())
				sizeOfHeap = maxHeap_v5.size();

			decisionTreeNode_v5* top = maxHeap_v5.top().get();
			
			if (top->upperbound <= maxValue)
				break;

			if (top->level == values.size())
			{
				maxHeap_v5.pop();
				continue;
			}

			unsigned int itemIndex = sortedIndices[top->level];
			
			top->level += 1;
			unique_ptr<decisionTreeNode_v5> prev;
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				prev = make_unique<decisionTreeNode_v5>(*top);

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;
			}
			else
			{
				const unique_ptr<decisionTreeNode_v5>& temp = maxHeap_v5.top();
				prev = std::move(const_cast<unique_ptr<decisionTreeNode_v5>&>(temp));
				maxHeap_v5.pop();
			}

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->upperbound > maxValue)
				maxHeap_v5.push(std::move(prev));
		}

		while(!maxHeap_v5.empty())
			maxHeap_v5.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v6 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v5: 
		Instead of unique_ptr use vector of objects (memory pool) and keep pointer to these objects in heap.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
						+ O(2^n * 4)	//growing memory pool - the heap object size is 4
						+ O(n)			//to store next available index of pool
						+ O(n)			//to store sortedIndices
						+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)			//to store cumulativeWeights
						+ O(n)			//to store values
						+ O(n)			//to store weights
	The recursive call stack : NOT APPLICABLE
	*/

	struct Comparator_v6
	{
		bool operator()(const decisionTreeNode_v6* lhs, const decisionTreeNode_v6* rhs)
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v6*, vector<decisionTreeNode_v6*>, Comparator_v6> maxHeap_v6;

	double DP_KnapsackProblem_0_1_branch_and_bound_v6(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		vector<vector<decisionTreeNode_v6>>& heapObjectsGrowingPool, queue<decisionTreeNode_v6*, deque<decisionTreeNode_v6*>>& poolIndices)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v6* pObj = poolIndices.front();
		poolIndices.pop();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v6.push(pObj);

		double maxValue = 0.0; // set max value by greedy

		while (!maxHeap_v6.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v6.size())
				sizeOfHeap = maxHeap_v6.size();

			decisionTreeNode_v6* top = maxHeap_v6.top();
			
			if (top->upperbound <= maxValue)
				break;

			if (top->level == values.size())
			{
				maxHeap_v6.pop();
				continue;
			}

			unsigned int itemIndex = sortedIndices[top->level];
			
			top->level += 1;

			decisionTreeNode_v6* prev = top;
			//unique_ptr<decisionTreeNode_v6> prev = make_unique<decisionTreeNode_v6>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				if (poolIndices.empty()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v6>(values.size()));
					int lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						poolIndices.push(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = poolIndices.front();
				poolIndices.pop();
				*prev = *top;

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v6.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->upperbound > maxValue)
				maxHeap_v6.push(prev);
			else
				poolIndices.push(prev);
		}

		while(!maxHeap_v6.empty())
			maxHeap_v6.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v7 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v6: 
		Instead of std::queue use mm::MM_Queue.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
						+ O(2^n * 4)	//growing memory pool - the heap object size is 4
						+ O(n)			//to store next available index of pool
						+ O(n)			//to store sortedIndices
						+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)			//to store cumulativeWeights
						+ O(n)			//to store values
						+ O(n)			//to store weights
	The recursive call stack : NOT APPLICABLE
	*/

	struct Comparator_v7
	{
		bool operator()(const decisionTreeNode_v7* lhs, const decisionTreeNode_v7* rhs)
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v7*, vector<decisionTreeNode_v7*>, Comparator_v7> maxHeap_v7;

	double DP_KnapsackProblem_0_1_branch_and_bound_v7(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		vector<vector<decisionTreeNode_v7>>& heapObjectsGrowingPool, MM_Queue<decisionTreeNode_v7*>& poolIndices)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		decisionTreeNode_v7* pObj = poolIndices.front();
		poolIndices.pop();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v7.push(pObj);

		double maxValue = 0.0; // set max value by greedy

		while (!maxHeap_v7.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v7.size())
				sizeOfHeap = maxHeap_v7.size();

			decisionTreeNode_v7* top = maxHeap_v7.top();
			
			if (top->upperbound <= maxValue)
				break;

			if (top->level == values.size())
			{
				maxHeap_v7.pop();
				continue;
			}

			unsigned int itemIndex = sortedIndices[top->level];
			
			top->level += 1;

			decisionTreeNode_v7* prev = top;
			//unique_ptr<decisionTreeNode_v7> prev = make_unique<decisionTreeNode_v7>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				if (poolIndices.empty()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v7>(values.size()));
					int lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						poolIndices.push(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = poolIndices.front();
				poolIndices.pop();
				*prev = *top;

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v7.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->upperbound > maxValue)
				maxHeap_v7.push(prev);
			else
				poolIndices.push(prev);
		}

		while(!maxHeap_v7.empty())
			maxHeap_v7.pop();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v8 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v7: 
		Instead of using priority_queue, use MM_Heap to work with memory pool in efficient way.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
						+ O(2^n * 4)	//growing memory pool - the heap object size is 4
						//+ O(n)			//to store next available index of pool -- WE DON'T NEED THIS NOW
						+ O(n)			//to store sortedIndices
						+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)			//to store cumulativeWeights
						+ O(n)			//to store values
						+ O(n)			//to store weights
	The recursive call stack : NOT APPLICABLE
	*/

	double DP_KnapsackProblem_0_1_branch_and_bound_v8(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v8*, Comparator_v8>& maxHeap_v8, vector<vector<decisionTreeNode_v8>>& heapObjectsGrowingPool)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		MyAssert::myRunTimeAssert(maxHeap_v8.capacity() > maxHeap_v8.size());
		decisionTreeNode_v8* pObj = maxHeap_v8.getNextAvailableElement();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v8.push(pObj);

		double maxValue = 0.0; // set max value by greedy

		while (!maxHeap_v8.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v8.size())
				sizeOfHeap = maxHeap_v8.size();

			decisionTreeNode_v8* top = maxHeap_v8.top();
			
			if (top->upperbound <= maxValue)
				break;

			if (top->level == values.size())
			{
				maxHeap_v8.pop();
				continue;
			}

			unsigned int itemIndex = sortedIndices[top->level];
			
			top->level += 1;

			decisionTreeNode_v8* prev = top;
			//unique_ptr<decisionTreeNode_v6> prev = make_unique<decisionTreeNode_v6>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				if (maxHeap_v8.capacity() == maxHeap_v8.size()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v8>(values.size()));
					maxHeap_v8.reserve(maxHeap_v8.capacity() + values.size());
					int lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						maxHeap_v8.addToData(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = maxHeap_v8.getNextAvailableElement();
				*prev = *top;

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v8.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->upperbound > maxValue)
				maxHeap_v8.push(prev);
		}

		//while(!maxHeap_v7.empty())
		//	maxHeap_v7.pop();
		maxHeap_v8.clear();

		return maxValue;
	}

	// ============================= Solution using branch and bound approach v8b =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v8: 
		Instead of starting with maxValue = 0, start with maxValue = exact greedy max value.
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
						+ O(2^n * 4)	//growing memory pool - the heap object size is 4
						+ O(n)			//to store sortedIndices
						+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)			//to store cumulativeWeights
						+ O(n)			//to store values
						+ O(n)			//to store weights
	The recursive call stack : NOT APPLICABLE
	*/

	double DP_KnapsackProblem_0_1_branch_and_bound_v8b(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v8b*, Comparator_v8b>& maxHeap_v8b, vector<vector<decisionTreeNode_v8b>>& heapObjectsGrowingPool)
	{
		unsigned long long smallestWeight = weights[0];
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
		{
			valueToWeightsRatio[i] = values[i] / weights[i];
			if (smallestWeight > weights[i])
				smallestWeight = weights[i];
		}

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[ sortedIndices[0] ] = weights[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[ sortedIndices[i] ] = cumulativeWeights[ sortedIndices[i - 1] ] + weights[ sortedIndices[i] ];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[ sortedIndices[0] ] = values[ sortedIndices[0] ];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[ sortedIndices[i] ] = cumulativeValues[ sortedIndices[i - 1] ] + values[ sortedIndices[i] ];

		MyAssert::myRunTimeAssert(maxHeap_v8b.capacity() > maxHeap_v8b.size());
		decisionTreeNode_v8b* pObj = maxHeap_v8b.getNextAvailableElement();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v8b.push(pObj);

		double maxValue = 0.0; // set max value by greedy
		vector<unsigned int>::iterator iter = std::upper_bound(sortedIndices.begin(), sortedIndices.end(), knapsackCapacity,
			[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });
		if (iter != sortedIndices.begin())
		{
			--iter;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
			maxValue = cumulativeValues[*iter];
			unsigned long long remainingKnapsackCapacity = knapsackCapacity - cumulativeWeights[*iter];
			for (++iter; iter != sortedIndices.end() && remainingKnapsackCapacity >= smallestWeight; ++iter)
			{
				if (remainingKnapsackCapacity >= weights[*iter])
				{
					maxValue += values[*iter];
					remainingKnapsackCapacity -= weights[*iter];
				}
			}
		}

		while (!maxHeap_v8b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v8b.size())
				sizeOfHeap = maxHeap_v8b.size();

			decisionTreeNode_v8b* top = maxHeap_v8b.top();
			
			if (top->upperbound <= maxValue)
				break;

			if (top->level == values.size())
			{
				maxHeap_v8b.pop();
				continue;
			}

			unsigned int itemIndex = sortedIndices[top->level];
			
			top->level += 1;

			decisionTreeNode_v8b* prev = top;
			//unique_ptr<decisionTreeNode_v6> prev = make_unique<decisionTreeNode_v6>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				if (maxHeap_v8b.capacity() == maxHeap_v8b.size()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v8b>(values.size()));
					maxHeap_v8b.reserve(maxHeap_v8b.capacity() + values.size());
					int lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						maxHeap_v8b.addToData(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = maxHeap_v8b.getNextAvailableElement();
				*prev = *top;

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v8b.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->upperbound > maxValue)
				maxHeap_v8b.push(prev);
		}

		maxHeap_v8b.clear();

		return maxValue;
	}


	// ============================= Solution using branch and bound approach v9b =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v8b: 
		Use in place sorting and discard sortedIndices
	n = number of objects
	Time Complexity  :  worst case: O(2^n) 
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
						+ O(2^n * 4)	//growing memory pool - the heap object size is 4
						+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
						+ O(n)			//to store cumulativeWeights and also to store sortedIndices
						+ O(n)			//to store input values
						+ O(n)			//to store input weights
	The recursive call stack : NOT APPLICABLE
	*/

	double DP_KnapsackProblem_0_1_branch_and_bound_v9b(vector<double>& values, vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, 
		vector<double>& valueToWeightsRatio, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v9b*, Comparator_v9b>& maxHeap_v9b, vector<vector<decisionTreeNode_v9b>>& heapObjectsGrowingPool)
	{
		unsigned long long smallestWeight = weights[0];
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
		{
			valueToWeightsRatio[i] = values[i] / weights[i];
			if (smallestWeight > weights[i])
				smallestWeight = weights[i];
		}

		//initialize the indices
		vector<unsigned long long>&sortedIndices = cumulativeWeights;
		std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
		//for (int i = 0; i < values.size(); ++i)
		//	sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Testing
		//vector<double> oldValues = values;
		//vector<unsigned long long> oldWeights = weights;
		//vector<unsigned long long> oldSortedIndices = sortedIndices;

		//Apply sortedIndices to weights and values
		for (unsigned int i = 0; i < sortedIndices.size(); ++i)
		{
			unsigned long long curr_i = i;
			unsigned long long sourceIndex = sortedIndices[curr_i];

			if (curr_i == sourceIndex || sourceIndex == numeric_limits<unsigned long long>::max())
				continue;

			double value = values[curr_i]; //back up
			unsigned long long weight = weights[curr_i]; //back up
			sortedIndices[curr_i] = numeric_limits<unsigned long long>::max();
			while (sortedIndices[sourceIndex] != numeric_limits<unsigned long long>::max()) //value at sourceIndex is NOT yet moved to its right sorted position
			{
				values[curr_i] = values[ sourceIndex ];
				weights[curr_i] = weights[ sourceIndex ];
				unsigned long long oldSourceIndex = sourceIndex;
				curr_i = sourceIndex;
				sourceIndex = sortedIndices[curr_i];
				sortedIndices[oldSourceIndex] = numeric_limits<unsigned long long>::max(); //value at sourceIndex is NOW moved to its right sorted position
			}
			values[curr_i] = value;
			weights[curr_i] = weight;			
		}

		//Testing
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	MyAssert::myRunTimeAssert(values[i] == oldValues[ oldSortedIndices[i] ]);
		//	MyAssert::myRunTimeAssert(weights[i] == oldWeights[ oldSortedIndices[i] ]);
		//}

		//Store cumulative weights
		cumulativeWeights[0] = weights[0];
		for (int i = 1; i < weights.size(); ++i)
			cumulativeWeights[i] = cumulativeWeights[i - 1] + weights[i];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[0] = values[0];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[i] = cumulativeValues[i - 1] + values[i];

		MyAssert::myRunTimeAssert(maxHeap_v9b.capacity() > maxHeap_v9b.size());
		decisionTreeNode_v9b* pObj = maxHeap_v9b.getNextAvailableElement();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, cumulativeWeights, cumulativeValues);
		maxHeap_v9b.push(pObj);

		double maxValue = 0.0; // set max value by greedy
		vector<unsigned long long>::iterator iter = std::upper_bound(cumulativeWeights.begin(), cumulativeWeights.end(), knapsackCapacity);
			//[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned long long currentCumulativeWeight) { return targetCumulativeWeightIn < currentCumulativeWeight; });
		unsigned int index = iter - cumulativeWeights.begin();
		if (index != 0)
		{
			--index;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
			maxValue = cumulativeValues[index];
			unsigned long long remainingKnapsackCapacity = knapsackCapacity - cumulativeWeights[index];
			for (++index; index < values.size() && remainingKnapsackCapacity >= smallestWeight; ++index)
			{
				if (remainingKnapsackCapacity >= weights[index])
				{
					maxValue += values[index];
					remainingKnapsackCapacity -= weights[index];
				}
			}
		}

		while (!maxHeap_v9b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v9b.size())
				sizeOfHeap = maxHeap_v9b.size();

			decisionTreeNode_v9b* top = maxHeap_v9b.top();
			
			if (top->upperbound <= maxValue)
				break;

			if (top->level == values.size())
			{
				maxHeap_v9b.pop();
				continue;
			}

			unsigned int itemIndex = top->level;
			
			top->level += 1;

			decisionTreeNode_v9b* prev = top;
			//unique_ptr<decisionTreeNode_v6> prev = make_unique<decisionTreeNode_v6>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if(totalWeight <= knapsackCapacity)
			{
				if (maxHeap_v9b.capacity() == maxHeap_v9b.size()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v9b>(values.size()));
					maxHeap_v9b.reserve(maxHeap_v9b.capacity() + values.size());
					int lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						maxHeap_v9b.addToData(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = maxHeap_v9b.getNextAvailableElement();
				*prev = *top;

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;
			}
			else
				maxHeap_v9b.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->upperbound > maxValue)
				maxHeap_v9b.push(prev);
		}

		maxHeap_v9b.clear();

		return maxValue;
	}

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

		std::sort(indices.begin(), indices.end(), [values, weights](unsigned long long a, unsigned long long b) -> bool { return (values[a] / weights[a]) > (values[b] / weights[b]);  });
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
