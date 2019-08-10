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

#include <vector>
#include <queue>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	/*
	What's new - v2
	Use MM_Heap<> onstead of std::priority_queue<>
	*/

	bool verifySettlement_v2(
		const vector<double>& updatedBalance,
		const vector<double>& spl, 
		const vector<double>& aspl, 
		 const vector<double>& exchangeRates)
	{
		//rmt
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();

		vector<double> currentAspl(numMembers);
		vector<double> currentNov(numMembers);
		for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			double asplTemp = 0.0;
			double novTemp = 0.0;
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				int index = numMembers * memberIndex + currencyIndex;
				if (updatedBalance[index] + zero < -spl[index])
					return false;

				double currentBalanceInDollars = updatedBalance[index] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (novTemp < -zero)
				return false;

			if (asplTemp + zero < -aspl[memberIndex])
				return false;
		}

		return true;
	}

	struct fxDecisionTreeNode_v2
	{
		int level;
		double upperbound;

		vector<double> currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bool rmtPassed;

		inline void calculateAndSetUpperBound(
			const double cumulativeSettledAmount
		)
		{
			upperbound = settledAmount + cumulativeSettledAmount;
		}
	};
	struct fxDecisionTreeNodeCompare_v2
	{
		bool operator()(const fxDecisionTreeNode_v2* lhs, const fxDecisionTreeNode_v2* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	double doSettlement_branch_and_bound_v2(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& initialBalance,
		const vector<double>& exchangeRates)
	{
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();

		std::sort(trades.begin(), trades.end(),
			[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
			return (lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] + lhs.sellVol_ * exchangeRates[static_cast<int>(lhs.sellCurr_)])
			> (rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)] + rhs.sellVol_ * exchangeRates[static_cast<int>(rhs.sellCurr_)]);
		});

		int initialHeapCapacity = 10000;
		vector<vector<fxDecisionTreeNode_v2>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v2>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v2*, fxDecisionTreeNodeCompare_v2> fxMaxHeap_v2(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v2.addToData(&heapObjectsGrowingPool[0][i]);

		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];

			cumulativeSettledAmount[i] += (
				trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]
				+ trades[i].sellVol_ * exchangeRates[static_cast<int>(trades[i].sellCurr_)]);
		}

		fxDecisionTreeNode_v2* pObj = fxMaxHeap_v2.getNextAvailableElement();
		fxDecisionTreeNode_v2& current = *pObj;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		current.rmtPassed = false;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		current.calculateAndSetUpperBound(cumulativeSettledAmount[0]);
		fxMaxHeap_v2.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v2.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v2.size())
				sizeOfHeap = fxMaxHeap_v2.size();

			fxDecisionTreeNode_v2* pCurrent = fxMaxHeap_v2.top();
			fxDecisionTreeNode_v2& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			//include this item
			if (fxMaxHeap_v2.capacity() == fxMaxHeap_v2.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v2>(initialHeapCapacity));
				fxMaxHeap_v2.reserve(fxMaxHeap_v2.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v2.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v2* pInclude = fxMaxHeap_v2.getNextAvailableElement();
			fxDecisionTreeNode_v2& include = *pInclude;
			include = current;
			// Update current balance
			include.currentBalance[numMembers * trades[include.level].partyId_  + static_cast<int>(trades[include.level].buyCurr_)] += trades[include.level].buyVol_;
			include.currentBalance[numMembers * trades[include.level].partyId_  + static_cast<int>(trades[include.level].sellCurr_)] -= trades[include.level].sellVol_;
			include.currentBalance[numMembers * trades[include.level].cPartyId_ + static_cast<int>(trades[include.level].buyCurr_)] -= trades[include.level].buyVol_;
			include.currentBalance[numMembers * trades[include.level].cPartyId_ + static_cast<int>(trades[include.level].sellCurr_)] += trades[include.level].sellVol_;
			
			// Do rmt tests and update maxValue if rmt tests are passed
			include.rmtPassed = verifySettlement_v2(include.currentBalance, spl, aspl, exchangeRates);
			include.settledAmount += (
				trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]
				+ trades[include.level].sellVol_ * exchangeRates[static_cast<int>(trades[include.level].sellCurr_)]);
			include.settleFlags[include.level] = true;
			if (include.rmtPassed && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				settleFlagsOut = include.settleFlags;
			}

			//exclude current
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if ((current.upperbound + zero) < maxValue)
				fxMaxHeap_v2.pop();

			if (current.level == trades.size() - 1)
			{
				fxMaxHeap_v2.pop();
				continue;
			}

			//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
			include.calculateAndSetUpperBound(cumulativeSettledAmount[include.level + 1]);

			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if ((include.upperbound + zero) >= maxValue)
				fxMaxHeap_v2.push(pInclude);
		}

		while (!fxMaxHeap_v2.empty())
			fxMaxHeap_v2.pop();

		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}