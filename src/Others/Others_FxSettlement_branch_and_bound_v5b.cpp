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
#include <bitset>
#include <numeric>
#include <cassert>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	/*
	Whats new - v3b
	Instead of running rmt tests every time we condsider a trade as settled, rmt test is done only at the bottom level of solution tree
	Whats new - v4b
	Use improved upper bound
	Whats new - v5b
	Remove all vector<vector<>>
	*/

	bool verifySettlement_v5b(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,		
		const vector< int >& memberIndices,
		const vector<double>& spl,
		const vector<double>& aspl, 
		 const vector<double>& exchangeRates)
	{
		// TODO: Avoid checking SPL for all currencies. 
		// It can be checked only for two currencies which are changes because of settlement of the current trade if we remember SPL results for all currencies.
		//rmt
		//int numMembers = updatedBalance.size();
		bool rmtSuccessful = true;
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();
		int startIndex = -1;
		for (int i = 0; i < memberIndices.size(); ++i)
		{
			int memberIndex = memberIndices[i];
			double asplTemp = 0.0;
			double novTemp = 0.0;
			bool splPassed = true;
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				++startIndex;
				if (updatedBalance[startIndex] + zero < -spl[startIndex])
				{
					//rmtPassed[memberIndex] = false;
					splPassed = false;
					break;
				}

				double currentBalanceInDollars = updatedBalance[startIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (!splPassed || (asplTemp + zero < -aspl[memberIndex]) || (novTemp < -zero))
			{
				rmtPassed[memberIndex] = false;
				rmtSuccessful = false;
			}
			else
				rmtPassed[memberIndex] = true;
		}
		
		return rmtSuccessful;
	}

	struct fxDecisionTreeNode_v5b
	{
		int level;
		double upperbound;

		vector<double> currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bitset<128> rmtPassed{ 0 };

		inline void calculateAndSetUpperBound(
			const vector<double>& cumulativeBalance, 
			const double cumulativeSettledAmount,
			const vector<double>& spl,
			const vector<double>& aspl,
			const vector<double>& exchangeRates
		)
		{
			double excessSettledAmountInDollars = 0.0;
			vector<double> totalBalance(currentBalance);
			int numMembers = aspl.size();
			int numCurrencies = spl.size() / aspl.size();
			int startIndex = -1;
			for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
			{
				for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
				{
					++startIndex;
					totalBalance[startIndex] += cumulativeBalance[startIndex];
					if (totalBalance[startIndex] + zero < -spl[startIndex])
						excessSettledAmountInDollars += ((-totalBalance[startIndex] - spl[startIndex]) * exchangeRates[currencyIndex]);
				}
			}

			if (cumulativeSettledAmount > excessSettledAmountInDollars)
			{
				int x = 0;
				++x;
			}
			upperbound = settledAmount + cumulativeSettledAmount - excessSettledAmountInDollars / 2.0;
		}
	};
	struct fxDecisionTreeNodeCompare_v5b
	{
		bool operator()(const fxDecisionTreeNode_v5b* lhs, const fxDecisionTreeNode_v5b* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	double doSettlement_branch_and_bound_v5b(
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
		vector<vector<fxDecisionTreeNode_v5b>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v5b>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v5b*, fxDecisionTreeNodeCompare_v5b> fxMaxHeap_v5b(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v5b.addToData(&heapObjectsGrowingPool[0][i]);

		vector< vector<double> > cumulativeBalance(trades.size(), vector<double>(numMembers * numCurrencies, 0.0));
		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
			{
				cumulativeBalance[i] = cumulativeBalance[i + 1];
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];
			}

			cumulativeBalance[i][numMembers * trades[i].partyId_ + static_cast<int>(trades[i].buyCurr_)] += trades[i].buyVol_;
			cumulativeBalance[i][numMembers * trades[i].partyId_ + static_cast<int>(trades[i].sellCurr_)] -= trades[i].sellVol_;
			cumulativeBalance[i][numMembers * trades[i].cPartyId_ + static_cast<int>(trades[i].buyCurr_)] -= trades[i].buyVol_;
			cumulativeBalance[i][numMembers * trades[i].cPartyId_ + static_cast<int>(trades[i].sellCurr_)] += trades[i].sellVol_;

			cumulativeSettledAmount[i] += (
				trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]
				+ trades[i].sellVol_ * exchangeRates[static_cast<int>(trades[i].sellCurr_)]);
		}

		fxDecisionTreeNode_v5b* pObj = fxMaxHeap_v5b.getNextAvailableElement();
		fxDecisionTreeNode_v5b& current = *pObj;
		current.level = -1;
		current.currentBalance.resize(numMembers * numCurrencies, 0.0);
		int startIndex = -1;
		for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				++startIndex;
				current.currentBalance[startIndex] = initialBalance[startIndex];
			}
		}
		current.settledAmount = 0.0;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		
		std::vector<int> memberIndices(aspl.size());
		std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
		current.rmtPassed.flip();
		verifySettlement_v5b(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(cumulativeBalance[0], cumulativeSettledAmount[0], spl, aspl, exchangeRates);
		fxMaxHeap_v5b.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v5b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v5b.size())
				sizeOfHeap = fxMaxHeap_v5b.size();

			fxDecisionTreeNode_v5b* pCurrent = fxMaxHeap_v5b.top();
			fxDecisionTreeNode_v5b& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			if (current.level == trades.size())
			{
				if (maxValue < current.settledAmount)
				{
					std::vector<int> memberIndices(aspl.size());
					std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
					verifySettlement_v5b(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
					if (current.rmtPassed.all())
					{
						maxValue = current.settledAmount;
						settleFlagsOut = current.settleFlags;

						if ((current.upperbound + zero) < maxValue)
							break;
					}
				}

				fxMaxHeap_v5b.pop();
				continue;
			}

			//include this item
			if (fxMaxHeap_v5b.capacity() == fxMaxHeap_v5b.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v5b>(initialHeapCapacity));
				fxMaxHeap_v5b.reserve(fxMaxHeap_v5b.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v5b.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v5b* pInclude = fxMaxHeap_v5b.getNextAvailableElement();
			fxDecisionTreeNode_v5b& include = *pInclude;
			include = current;
			// Update current balance
			int partyId = trades[include.level].partyId_;
			int cPartyId = trades[include.level].cPartyId_;
			int buyCurrId = static_cast<int>(trades[include.level].buyCurr_);
			int sellCurrId = static_cast<int>(trades[include.level].sellCurr_);
			include.currentBalance[numMembers * partyId + buyCurrId] += trades[include.level].buyVol_;
			include.currentBalance[numMembers * partyId + sellCurrId] -= trades[include.level].sellVol_;
			include.currentBalance[numMembers * cPartyId + buyCurrId] -= trades[include.level].buyVol_;
			include.currentBalance[numMembers * cPartyId + sellCurrId] += trades[include.level].sellVol_;
			

			include.settledAmount += (
				trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]
				+ trades[include.level].sellVol_ * exchangeRates[static_cast<int>(trades[include.level].sellCurr_)]);
			include.settleFlags[include.level] = true;

			//if (current.level == trades.size() - 1)
			//{
			//	verifySettlement_v5b(include.rmtPassed, include.currentBalance, { partyId, cPartyId }, spl, aspl, exchangeRates);
			//	if (include.rmtPassed.all() && maxValue < include.settledAmount)
			//	{
			//		maxValue = include.settledAmount;
			//		settleFlagsOut = include.settleFlags;

			//		if ((current.upperbound + zero) < maxValue)
			//			break;
			//	}
			//}

			//exclude current
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			//if ((current.upperbound + zero) < maxValue)
			//	fxMaxHeap_v5b.pop();

			if (current.level < trades.size() - 1)
			{
				//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
				include.calculateAndSetUpperBound(
					cumulativeBalance[include.level + 1], 
					cumulativeSettledAmount[include.level + 1], 
					spl, 
					aspl, 
					exchangeRates);

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				
			}
			else
			{
				include.upperbound = include.settledAmount;
			}

			if ((include.upperbound + zero) >= maxValue)
				fxMaxHeap_v5b.push(pInclude);
		}

		fxMaxHeap_v5b.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}