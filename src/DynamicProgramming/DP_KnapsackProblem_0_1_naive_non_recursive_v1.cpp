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
	Time Complexity  : 2^(m + n)
	Space Complexity : O(1)
	The recursive call stack would grow with O(m + n)
	*/

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
}
