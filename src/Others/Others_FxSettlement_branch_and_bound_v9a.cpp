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
#include <vector>
#include <queue>
#include <bitset>
#include <numeric>
#include <cassert>
#include <limits> // for std::numeric_limits<double>::digits10
#include <iomanip>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "Others/Others_FxSettlement_branch_and_bound_v9a.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	/*
	Revision History:
	v3a		Instead of running emt tests for all members every time, we are keeping a cached rmt results for every member
			and redoing rmt tests for only two members involved in the trade when it is settled
	v4a		Use improved upper bound
	v5a		Remove all vector<vector<>>
	v6a		Preallocate all memory. No dynamic memory allocation.
			Check if upperbound is slightly more than max, break the loop
	v7a		While calculating upper bound, if all remaining trades are settled, then set rmtPassed = true and break the loop if this is upperbound
			Consider ASPL and NOV as well while calculating upper bound
	v8a		Start with greedy max settled amount instead of zero.
	v9a		Update current balances only if rmt passes and Calculate upperbound for both exclude as well if include passes rmt tests.
	
	Future	Remove all elements from heap which has upper bound less than current settled amount
	*/

	void fxDecisionTreeNode_v9a::calculateAndSetUpperBound(
		const vector<double>& cumulativeBalance,
		const double cumulativeSettledAmount,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates
	)
	{
		double excessSettledAmountInDollars = 0.0;
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();
		int startIndex = -1;
		upperboundRmtPassed = true;
		for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			double asplTemp = 0.0;
			double novTemp = 0.0;
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				++startIndex;
				double totalBalance = currentBalance[startIndex] + cumulativeBalance[startIndex];
				
				if (totalBalance + zero < -spl[startIndex])
				{
					upperboundRmtPassed = false;
					excessSettledAmountInDollars += ((totalBalance - (-spl[startIndex])) * exchangeRates[currencyIndex]);
					double totalBalanceInDollars = (-spl[startIndex]) * exchangeRates[currencyIndex];
					novTemp += totalBalanceInDollars;
					asplTemp += totalBalanceInDollars;
				}
				else 
				{
					double totalBalanceInDollars = totalBalance * exchangeRates[currencyIndex];
					novTemp += totalBalanceInDollars;
					if(totalBalance < -zero)
						asplTemp += totalBalanceInDollars;
				}

			}

			if (asplTemp + zero < -aspl[memberIndex])
			{
				upperboundRmtPassed = false;
				double excessASPLInDollars = (asplTemp - (-aspl[memberIndex]));
				novTemp += excessASPLInDollars;
				excessSettledAmountInDollars += excessASPLInDollars;
			}

			if (novTemp < -zero)
			{
				upperboundRmtPassed = false;
				double excessNOVInDollars = (novTemp - (-aspl[memberIndex]));
				excessSettledAmountInDollars += excessNOVInDollars;
			}
		}

		if (cumulativeSettledAmount < -excessSettledAmountInDollars)
		{
			//We may come here sometimes, if currentBalance is already short and cumulative balance makes if go down further
			//_CrtDbgBreak();
			//int* p = nullptr;
			//*p = 10;
		}
		upperbound = settledAmount + cumulativeSettledAmount + excessSettledAmountInDollars;

		if (upperboundRmtPassed)
		{
			settledAmount = upperbound;
			for (int i = level + 1; i < settleFlags.size(); ++i)
				settleFlags[i] = true;
		}
	}

	bool verifySettlement_v9a(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,		
		const vector<int>& memberIndices,
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
		for (int i = 0; i < memberIndices.size(); ++i)
		{
			int memberIndex = memberIndices[i];
			double asplTemp = 0.0;
			double novTemp = 0.0;
			bool splPassed = true;
			
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				int index = numMembers * memberIndex + currencyIndex;
				if (updatedBalance[index] + zero < -spl[index])
				{
					//rmtPassed[memberIndex] = false;
					splPassed = false;
					break;
				}

				double currentBalanceInDollars = updatedBalance[index] * exchangeRates[currencyIndex];
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

	void debugPrint_v9a(int level, unsigned long long numberOfFunctionCalls, size_t heapSize,
		double upperbound, bool upperboundRmtPassed,
		double settledAmount, const string& key)
	{
		std::cout.imbue(std::locale(""));
		std::cout << std::fixed;
		std::cout << std::setprecision(4);
		cout << "\n"
			<< key
			<< " Level: " << level
			<< " numberOfFunctionCalls: " << numberOfFunctionCalls
			<< " Heap size: " << heapSize
			<< " upperbound: " << upperbound
			<< " upperboundRmtPassed: " << (upperboundRmtPassed ? "Yes" : "No")
			<< " settledAmount: " << settledAmount
			<< " " << key;
	}

	double doSettlement_branch_and_bound_v9a(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& initialBalance,
		const vector<double>& exchangeRates, MM_Heap<fxDecisionTreeNode_v9a*, fxDecisionTreeNodeCompare_v9a>& fxMaxHeap_v9a,
		vector<vector<fxDecisionTreeNode_v9a>>& heapObjectsGrowingPool,
		int initialHeapCapacity)
	{
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();

		//std::sort(trades.begin(), trades.end(),
		//	[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
		//	return (lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] + lhs.sellVol_ * exchangeRates[static_cast<int>(lhs.sellCurr_)])
		//	> (rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)] + rhs.sellVol_ * exchangeRates[static_cast<int>(rhs.sellCurr_)]);
		//});

		double settledAmountGreedy = doSettlement_greedy_v1(
			settleFlagsOut,
			trades,
			spl,
			aspl,
			initialBalance,
			exchangeRates
		);

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
				+ trades[i].sellVol_ * exchangeRates[static_cast<int>(trades[i].sellCurr_)]
				);
		}

		fxDecisionTreeNode_v9a* pObj = fxMaxHeap_v9a.getNextAvailableElement();
		fxDecisionTreeNode_v9a& current = *pObj;
		current.level = -1;
		current.currentBalance.resize(numMembers * numCurrencies, 0.0);
		int startIndex = -1;
		for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				current.currentBalance[++startIndex] = initialBalance[startIndex];
			}
		}
		current.settledAmount = 0.0;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		
		std::vector<int> memberIndices(aspl.size());
		std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
		current.rmtPassed.flip();
		verifySettlement_v9a(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(
			cumulativeBalance[current.level + 1], 
			cumulativeSettledAmount[current.level + 1],
			spl, 
			aspl, 
			exchangeRates);
		fxMaxHeap_v9a.push(pObj);

		double maxValue = settledAmountGreedy;
		unsigned long long numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v9a.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v9a.size())
				sizeOfHeap = fxMaxHeap_v9a.size();

			fxDecisionTreeNode_v9a* pCurrent = fxMaxHeap_v9a.top();
			fxDecisionTreeNode_v9a& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			if (current.upperboundRmtPassed && maxValue < current.settledAmount)
			{
				maxValue = current.settledAmount;
				settleFlagsOut = current.settleFlags;
				break;
			}

			//exclude current
			current.level += 1;

			//Grow the heap if required
			if (fxMaxHeap_v9a.capacity() == fxMaxHeap_v9a.size()) //need to grow pool
			{
				//Ideally the algorithm should be optimized in such a way that:
				//    We dont need to come here and in case we do, growing the pool by trades.size() is enough
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v9a>(initialHeapCapacity, fxDecisionTreeNode_v9a{ initialBalance.size() }));
				fxMaxHeap_v9a.reserve(fxMaxHeap_v9a.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v9a.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			//include this item
			fxDecisionTreeNode_v9a* pExclude = fxMaxHeap_v9a.getNextAvailableElement();
			fxDecisionTreeNode_v9a& exclude = *pExclude;
			exclude = current;

			fxDecisionTreeNode_v9a& include = current;
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
				+ trades[include.level].sellVol_ * exchangeRates[static_cast<int>(trades[include.level].sellCurr_)]
				);
			include.settleFlags[include.level] = true;

			verifySettlement_v9a(include.rmtPassed, include.currentBalance, { partyId, cPartyId }, spl, aspl, exchangeRates);
			if (include.rmtPassed.all() && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				settleFlagsOut = include.settleFlags;

				//debugPrint_v9a(include.level, numberOfFunctionCalls, fxMaxHeap_v9a.size(),
				//	include.upperbound, include.upperboundRmtPassed, 
				//	include.settledAmount, "");

				if ((current.upperbound + zero) <= maxValue)
					break;
			}

			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			//if ((current.upperbound + zero) < maxValue)
			//	fxMaxHeap_v9a.pop();

			if (current.level < trades.size() - 1)
			{
				exclude.calculateAndSetUpperBound(
					cumulativeBalance[exclude.level + 1],
					cumulativeSettledAmount[exclude.level + 1],
					spl,
					aspl,
					exchangeRates);

				if (exclude.upperboundRmtPassed)
				{
					if (maxValue < exclude.settledAmount)
					{
						maxValue = exclude.settledAmount;
						settleFlagsOut = exclude.settleFlags;
					}

					//debugPrint_v9a(include.level, numberOfFunctionCalls, fxMaxHeap_v9a.size(),
					//	include.upperbound, include.upperboundRmtPassed,
					//	include.settledAmount, "*");

					if ((current.upperbound - zero) <= maxValue)
						break;
				}

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if ((exclude.upperbound - zero) > maxValue)
					fxMaxHeap_v9a.push(pExclude);
			}
			else
				fxMaxHeap_v9a.pop();
		}

		fxMaxHeap_v9a.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}