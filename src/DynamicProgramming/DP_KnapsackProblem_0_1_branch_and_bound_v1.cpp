//=======================================================================================================//
//   Copyright (c) 2018 Maruti Mhetre                                                                    //
//   All rights reserved.                                                                                //
//=======================================================================================================//
//   Redistribution and use of this software in source and binary forms, with or without modification,   //
//   are permitted for personal, educational or non-commercial purposes provided that the following      //
//   conditions are met:                                                                                 //
//   1. Redistributions of source code must retain the above copyright notice, this list of conditions   //
//      and the following disclaimer.                                                                    //
//   2. Redistributions in binary form must reproduce the above copyright notice, this list of           //
//      conditions and the following disclaimer in the documentation and/or other materials provided     //
//      with the distribution.                                                                           //
//   3. Neither the name of the copyright holder nor the names of its contributors may be used to        //
//      endorse or promote products derived from this software without specific prior written            //
//      permission.                                                                                      //
//=======================================================================================================//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR      //
//   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND    //
//   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR          //
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL   //
//   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   //
//   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  //
//   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT   //
//   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     //
//=======================================================================================================//

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
			for (; i < values.size() && currentKnapsackCapacity + weights[sortedIndices[i]] < knapsackCapacity; ++i)
			{
				currentKnapsackCapacity += weights[sortedIndices[i]];
				upperbound += values[sortedIndices[i]];
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
			if (current.totalWeight <= knapsackCapacity)
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
}
