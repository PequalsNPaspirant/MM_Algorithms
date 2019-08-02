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

			unsigned long long targetCumulativeWeight = cumulativeWeights[sortedIndices[currentIndex]] + remainingKnapsackCapacity;

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
				upperbound += (remainingCapacity * values[*iter] / weights[*iter]);
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

		cumulativeWeights[sortedIndices[0]] = weights[sortedIndices[0]];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[sortedIndices[i]] = cumulativeWeights[sortedIndices[i - 1]] + weights[sortedIndices[i]];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[sortedIndices[0]] = values[sortedIndices[0]];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[sortedIndices[i]] = cumulativeValues[sortedIndices[i - 1]] + values[sortedIndices[i]];

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

			current.level = prev.level + 1;
			unsigned int itemIndex = sortedIndices[current.level - 1];

			//include this item
			current.totalWeight = prev.totalWeight + weights[itemIndex];
			if (current.totalWeight <= knapsackCapacity)
			{
				current.maxValue = prev.maxValue + values[itemIndex];
				current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				if (current.maxValue > maxValue)
					maxValue = current.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if (current.level < values.size() && current.upperbound > maxValue)
					maxHeap_v2.push(current);
			}

			//exclude this item
			current.totalWeight = prev.totalWeight;
			current.maxValue = prev.maxValue;
			current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (current.level < values.size() && current.upperbound > maxValue)
				maxHeap_v2.push(current);
		}

		return maxValue;
	}
}
