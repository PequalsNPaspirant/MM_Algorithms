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
#include "Others/Others_FxSettlement_branch_and_bound_v12a.h"
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
			Not implemented (wrong logic) - Update current balances only if rmt passes and Calculate upperbound for exclude as well if include passes rmt tests.
	v9a		calculate upperbound only for exclude case, not for include
	v10a	Avoid creating exclude as a copy of current until we really need it
	v11a	Avoid all dynamic memory allocation. Code refactoring.
	v12a	What next??

	Future	Remove all elements from heap which has upper bound less than current settled amount
	*/

	void fxDecisionTreeNode_v12a::calculateAndSetUpperBound(
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
	}

	bool verifySettlement_v12a(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,		
		int memberIndex,
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
		//for (int i = 0; i < memberIndices.size(); ++i)
		{
			//int memberIndex = memberIndices[i];
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

	inline bool verifySettlement_pair_v12a(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,
		int partyId, 
		int cPartyId,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates)
	{
		bool result1 = verifySettlement_v12a(rmtPassed, updatedBalance, partyId, spl, aspl, exchangeRates);
		bool result2 = verifySettlement_v12a(rmtPassed, updatedBalance, cPartyId, spl, aspl, exchangeRates);
		return result1 && result2;
	}

	inline bool verifySettlement_range_v12a(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,
		int partyIdStart,
		int partyIdEnd,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates)
	{
		bool rmtSuccessful = true;
		for(int memberIndex = partyIdStart; memberIndex <= partyIdEnd; ++memberIndex)
			if(!verifySettlement_v12a(rmtPassed, updatedBalance, memberIndex, spl, aspl, exchangeRates))
				rmtSuccessful = false;

		return rmtSuccessful;
	}

	void debugPrint_v12a(int level, unsigned long long numberOfFunctionCalls, size_t heapSize,
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

	double doSettlement_branch_and_bound_v12a(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		vector<double>& initialBalance,
		const vector<double>& exchangeRates,
		MM_Heap<fxDecisionTreeNode_v12a*, fxDecisionTreeNodeCompare_v12a>& fxMaxHeap_v12a,
		vector<vector<fxDecisionTreeNode_v12a>>& heapObjectsGrowingPool,
		int initialHeapCapacity,
		vector< vector<double> >& cumulativeBalance,
		vector<double>& cumulativeSettledAmount)
	{
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();

		//std::sort(trades.begin(), trades.end(),
		//	[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
		//	return (lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] + lhs.sellVol_ * exchangeRates[static_cast<int>(lhs.sellCurr_)])
		//	> (rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)] + rhs.sellVol_ * exchangeRates[static_cast<int>(rhs.sellCurr_)]);
		//});

		fxDecisionTreeNode_v12a* pObj = fxMaxHeap_v12a.getNextAvailableElement();
		fxDecisionTreeNode_v12a& current = *pObj;
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

		double maxValue = doSettlement_greedy_v1(
			settleFlagsOut,
			trades,
			spl,
			aspl,
			initialBalance,
			exchangeRates
		);

		//vector< vector<double> > cumulativeBalance(trades.size(), vector<double>(numMembers * numCurrencies, 0.0));
		//vector<double> cumulativeSettledAmount(trades.size(), 0.0);
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

		current.settledAmount = 0.0;
		current.upperbound = 0.0;
		
		//std::vector<int> memberIndices(aspl.size());
		//std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
		current.rmtPassed.flip();
		verifySettlement_range_v12a(current.rmtPassed, current.currentBalance, 0, aspl.size() - 1, spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(
			cumulativeBalance[current.level + 1], 
			cumulativeSettledAmount[current.level + 1],
			spl, 
			aspl, 
			exchangeRates);
		fxMaxHeap_v12a.push(pObj);

		unsigned long long numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		/*
		We have two options:
		1. Include current item
		2. Exclude current item

		The below is the difference for both options

		------------------------------------------------------------------------------------------------------------
		Term					Include current item					Exclude current item
		------------------------------------------------------------------------------------------------------------
		upperbound				same as current							need to recalculate
		balances				need to recalculate						same as current
		settled amount			need to recalculate						same as current
		settled flag			make true for current					keep as false for current
		rmt tests				need to redo as balances change			no need to redo as balances dont change
		copy current object?	No, upperbound same, so reuse current	upperbound changes, so make a copy of current
		update max				if rmt pass								if upperboundRmtPassed
		push into heap?			N.A. (Already in heap)					push if max < upperbound
		pop from heap?			pop if upperbound < max					N.A.
		if current.level ==		consider this option					Do not consider this option because, at last level, for option include,
		trades.size() - 1												if rmt passed, we already updated max i.e. it was excluding this item
		------------------------------------------------------------------------------------------------------------

		*/

		while (!fxMaxHeap_v12a.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v12a.size())
				sizeOfHeap = fxMaxHeap_v12a.size();

			fxDecisionTreeNode_v12a* pCurrent = fxMaxHeap_v12a.top();
			fxDecisionTreeNode_v12a& current = *pCurrent;

			if ((current.upperbound - zero) <= maxValue)
				break;

			current.level += 1;

			bitset<128> excludeRmtPassed = current.rmtPassed;

			double excludeUpperbound = 0.0;
			bool excludeUpperboundRmtPassed = false;
			if (current.level < trades.size() - 1)
			{
				double currentUpperbound = current.upperbound;
				bool currentUpperboundRmtPassed = current.upperboundRmtPassed;
				//tempororily make use of current to calculate exclude upper bound before changing its balances for include option.
				//See above table of comments.
				current.calculateAndSetUpperBound(
					cumulativeBalance[current.level + 1],
					cumulativeSettledAmount[current.level + 1],
					spl,
					aspl,
					exchangeRates);

				excludeUpperbound = current.upperbound;
				excludeUpperboundRmtPassed = current.upperboundRmtPassed;

				if (excludeUpperboundRmtPassed)
				{
					if (maxValue < excludeUpperbound)
					{
						maxValue = excludeUpperbound;
						settleFlagsOut = current.settleFlags;
						settleFlagsOut[current.level] = false;
						for (int i = current.level + 1; i < current.settleFlags.size(); ++i)
							settleFlagsOut[i] = true;
					}
				}

				//Revert back the current items upperbound
				current.upperbound = currentUpperbound;
				current.upperboundRmtPassed = currentUpperboundRmtPassed;
			}

			//include this item
			fxDecisionTreeNode_v12a& include = current;
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

			verifySettlement_pair_v12a(include.rmtPassed, include.currentBalance, partyId, cPartyId, spl, aspl, exchangeRates);
			if (include.rmtPassed.all() && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				for (int i = 0; i < include.settleFlags.size(); ++i)
					settleFlagsOut[i] = include.settleFlags[i];
			}

			if (current.level < trades.size() - 1)
			{
				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if ((excludeUpperbound - zero) > maxValue)
				{
					//Grow the heap if required
					if (fxMaxHeap_v12a.capacity() == fxMaxHeap_v12a.size()) //need to grow pool
					{
						heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v12a>(initialHeapCapacity, fxDecisionTreeNode_v12a{ initialBalance.size(), trades.size() }));
						fxMaxHeap_v12a.reserve(fxMaxHeap_v12a.capacity() + initialHeapCapacity);
						int lastIndex = heapObjectsGrowingPool.size() - 1;
						for (int i = 0; i < initialHeapCapacity; ++i)
							fxMaxHeap_v12a.addToData(&heapObjectsGrowingPool[lastIndex][i]);
					}

					fxDecisionTreeNode_v12a* pExclude = fxMaxHeap_v12a.getNextAvailableElement();
					fxDecisionTreeNode_v12a& exclude = *pExclude;
					exclude = include;

					//Revert the changes for include
					exclude.rmtPassed = excludeRmtPassed;
					exclude.currentBalance[numMembers * partyId + buyCurrId] -= trades[exclude.level].buyVol_;
					exclude.currentBalance[numMembers * partyId + sellCurrId] += trades[exclude.level].sellVol_;
					exclude.currentBalance[numMembers * cPartyId + buyCurrId] += trades[exclude.level].buyVol_;
					exclude.currentBalance[numMembers * cPartyId + sellCurrId] -= trades[exclude.level].sellVol_;
					exclude.settledAmount -= (
						trades[exclude.level].buyVol_ * exchangeRates[static_cast<int>(trades[exclude.level].buyCurr_)]
						+ trades[exclude.level].sellVol_ * exchangeRates[static_cast<int>(trades[exclude.level].sellCurr_)]
						);
					exclude.settleFlags[exclude.level] = false;
					fxMaxHeap_v12a.push(pExclude);
				}
			}
			else
				fxMaxHeap_v12a.pop();
		}

		fxMaxHeap_v12a.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}