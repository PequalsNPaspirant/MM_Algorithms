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

		cumulativeWeights[sortedIndices[0]] = weights[sortedIndices[0]];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[sortedIndices[i]] = cumulativeWeights[sortedIndices[i - 1]] + weights[sortedIndices[i]];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[sortedIndices[0]] = values[sortedIndices[0]];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[sortedIndices[i]] = cumulativeValues[sortedIndices[i - 1]] + values[sortedIndices[i]];

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

			unsigned int itemIndex = sortedIndices[top->level];

			top->level += 1;

			decisionTreeNode_v8b* prev = top;
			//unique_ptr<decisionTreeNode_v6> prev = make_unique<decisionTreeNode_v6>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if (totalWeight <= knapsackCapacity)
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

				if (top->level == values.size())
					maxHeap_v8b.pop();
			}
			else
				maxHeap_v8b.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->level < values.size() && prev->upperbound > maxValue)
				maxHeap_v8b.push(prev);
		}

		maxHeap_v8b.clear();

		return maxValue;
	}
}