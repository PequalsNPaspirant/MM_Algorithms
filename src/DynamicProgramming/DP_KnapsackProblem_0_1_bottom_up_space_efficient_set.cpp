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
				if (currentCapacity > weights[i])
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

				if ((currentCapacity + weights[i]) <= knapsackCapacity)
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

		size_t lastIndex = values.size() - 1;
		auto it1 = pPrev->upper_bound(knapsackCapacity); // it1 is pPrev->end()
		--it1;
		auto it2 = pPrev->upper_bound(knapsackCapacity - weights[lastIndex]);
		--it2;
		return std::max(it1->second, it2->second);
	}
}
