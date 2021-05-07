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
#include "Others/Others_FxSettlement_branch_and_bound_v13b.h"
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
	v12a	if current.upperbound < current.settledAmount then pop that node from heap and do not proceed
	v13a	upperbound calculations: remove all excessSPL + excessASPL + excessNOV from ideal settled amount
	v14a	upperbound calculations: upper bound = greedy max value + partial settlement

	Future	Remove all elements from heap which has upper bound less than current settled amount.
			Consider all values in Dollars.
	*/

	bool verifySettlement_greedy_v13b(
		bitset<128>& rmtPassedCurrent,
		const vector<double>& updatedBalance,
		int partyId,
		int cPartyId,
		int buyCurrId,
		int sellCurrId,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates)
	{
		//TODO: we can avoid checking SPL for all currencies. No need to check it for all, we can check it only for 2 currencies involved in trade.
		bool canThisTradeBeSettled = true;
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		int members[2] = { partyId, cPartyId };
		for (int i = 0; i < 2; ++i)
		{
			int memberIndex = members[i];
			double asplTemp = 0.0;
			double novTemp = 0.0;

			if (!(updatedBalance[numMembers * memberIndex + buyCurrId] + zero < -spl[numMembers * memberIndex + buyCurrId]
				&& updatedBalance[numMembers * memberIndex + buyCurrId] + zero < -spl[numMembers * memberIndex + buyCurrId]
				))
				return false;

			bool splPassed = true;
			for (size_t currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				size_t index = numMembers * memberIndex + currencyIndex;
				if (!rmtPassedCurrent[memberIndex] && updatedBalance[index] + zero < -spl[index])
				{
					splPassed = false;
				}

				double currentBalanceInDollars = updatedBalance[index] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if ((asplTemp + zero < -aspl[memberIndex]) || (novTemp < -zero))
			{
				return false;
			}

			rmtPassedCurrent[memberIndex] = splPassed;
		}

		return canThisTradeBeSettled;
	}

	void fxDecisionTreeNode_v13b::calculateAndSetUpperBound(
		vector<double>& updatedBalance,
		int level,
		const vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates
	)
	{
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		bitset<128> rmtPassedCurrent = rmtPassed;

		for (size_t i = 0; i < updatedBalance.size(); ++i)
		{
			updatedBalance[i] = currentBalance[i];
		}

		double amountSettledGreedy = 0.0;
		bool somethingSettled = true;
		size_t lastTradeSettledInLastPass = trades.size();
		bitset<128> settleFlagsGreedy{ false };
		size_t totalTrades = trades.size() - level;

		while (somethingSettled)
		{
			somethingSettled = false;
			for (size_t tradeIndex = level; tradeIndex < trades.size(); ++tradeIndex)
			{
				if (!somethingSettled && tradeIndex == lastTradeSettledInLastPass)
					break;

				if (settleFlagsGreedy[tradeIndex - level]) continue;

				// Update current balance
				int partyId = trades[tradeIndex].partyId_;
				int cPartyId = trades[tradeIndex].cPartyId_;
				int buyCurrId = static_cast<int>(trades[tradeIndex].buyCurr_);
				int sellCurrId = static_cast<int>(trades[tradeIndex].sellCurr_);
				updatedBalance[numMembers * partyId + buyCurrId] += trades[tradeIndex].buyVol_;
				updatedBalance[numMembers * partyId + sellCurrId] -= trades[tradeIndex].sellVol_;
				updatedBalance[numMembers * cPartyId + buyCurrId] -= trades[tradeIndex].buyVol_;
				updatedBalance[numMembers * cPartyId + sellCurrId] += trades[tradeIndex].sellVol_;

				if (verifySettlement_greedy_v13b(rmtPassedCurrent, updatedBalance, partyId, cPartyId, buyCurrId, sellCurrId, spl, aspl, exchangeRates))
				{
					amountSettledGreedy += (
						trades[tradeIndex].buyVol_ * exchangeRates[static_cast<int>(trades[tradeIndex].buyCurr_)]
						+ trades[tradeIndex].sellVol_ * exchangeRates[static_cast<int>(trades[tradeIndex].sellCurr_)]
						);

					settleFlagsGreedy[tradeIndex - level] = true;
					somethingSettled = true;
					lastTradeSettledInLastPass = tradeIndex;
					--totalTrades;
				}
				else
				{
					settleFlagsGreedy[tradeIndex - level] = false;
					//revert balance changes
					updatedBalance[numMembers * partyId + buyCurrId] -= trades[tradeIndex].buyVol_;
					updatedBalance[numMembers * partyId + sellCurrId] += trades[tradeIndex].sellVol_;
					updatedBalance[numMembers * cPartyId + buyCurrId] += trades[tradeIndex].buyVol_;
					updatedBalance[numMembers * cPartyId + sellCurrId] -= trades[tradeIndex].sellVol_;
				}
			}
		}

		upperboundRmtPassed = (totalTrades == 0) && rmtPassedCurrent.all();

		double predictedSettledAmount = 0.0;
		for (size_t tradeIndex = level; tradeIndex < trades.size(); ++tradeIndex)
		{
			if (settleFlagsGreedy[tradeIndex - level]) continue;

			// Update current balance
			int partyId = trades[tradeIndex].partyId_;
			int cPartyId = trades[tradeIndex].cPartyId_;
			int buyCurrId = static_cast<int>(trades[tradeIndex].buyCurr_);
			int sellCurrId = static_cast<int>(trades[tradeIndex].sellCurr_);
			updatedBalance[numMembers * partyId + buyCurrId] += trades[tradeIndex].buyVol_;
			updatedBalance[numMembers * partyId + sellCurrId] -= trades[tradeIndex].sellVol_;
			updatedBalance[numMembers * cPartyId + buyCurrId] -= trades[tradeIndex].buyVol_;
			updatedBalance[numMembers * cPartyId + sellCurrId] += trades[tradeIndex].sellVol_;
			double currentTradeSettledAmount = (
				trades[tradeIndex].buyVol_ * exchangeRates[static_cast<int>(trades[tradeIndex].buyCurr_)]
				+ trades[tradeIndex].sellVol_ * exchangeRates[static_cast<int>(trades[tradeIndex].sellCurr_)]
				);

			double excessSPL = 0.0;
			double excessASPL = 0.0;
			double excessNOV = 0.0;
			double excessSettledAmount = 0.0;

			size_t index = numMembers * partyId + sellCurrId;
			if (updatedBalance[index] + zero < -spl[index])
			{
				excessSPL = ((updatedBalance[index] - (-spl[index])) * exchangeRates[sellCurrId]);
			}

			index = numMembers * cPartyId + buyCurrId;
			if (updatedBalance[index] + zero < -spl[index])
			{
				excessSPL = std::min(excessSPL, ((updatedBalance[index] - (-spl[index])) * exchangeRates[buyCurrId]));
			}

			int memberIndices[2] = { partyId, cPartyId };
			for (int i = 0; i < 2; ++i)
			{
				int memberIndex = memberIndices[i];
				double asplTemp = 0.0;
				double novTemp = 0.0;

				for (size_t currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
				{
					size_t index = numMembers * memberIndex + currencyIndex;
					double currentBalanceInDollars = updatedBalance[index] * exchangeRates[currencyIndex];
					novTemp += currentBalanceInDollars;
					if (currentBalanceInDollars < -zero)
						asplTemp += currentBalanceInDollars;
				}

				if (asplTemp + zero < -aspl[memberIndex])
				{
					excessASPL = std::min(excessASPL, (asplTemp - (-aspl[memberIndex])));
				}

				if (novTemp < -zero)
				{
					excessNOV = std::min(excessNOV, novTemp);
				}
			}

			excessSettledAmount = 2 * std::min(std::min(excessSPL, excessASPL), excessNOV);
			if (-excessSettledAmount < currentTradeSettledAmount)
			{
				updatedBalance[numMembers * partyId + buyCurrId] += excessSettledAmount / exchangeRates[static_cast<int>(trades[tradeIndex].buyCurr_)];
				updatedBalance[numMembers * partyId + sellCurrId] -= excessSettledAmount / exchangeRates[static_cast<int>(trades[tradeIndex].sellCurr_)];
				updatedBalance[numMembers * cPartyId + buyCurrId] -= excessSettledAmount / exchangeRates[static_cast<int>(trades[tradeIndex].buyCurr_)];
				updatedBalance[numMembers * cPartyId + sellCurrId] += excessSettledAmount / exchangeRates[static_cast<int>(trades[tradeIndex].sellCurr_)];
			}
			else
				excessSettledAmount = -currentTradeSettledAmount;

			predictedSettledAmount += std::max(0.0, currentTradeSettledAmount + excessSettledAmount);
		}
		
		upperbound = settledAmount + amountSettledGreedy + predictedSettledAmount;
	}

	bool verifySettlement_v13b(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,		
		int memberIndex,
		const vector<double>& spl,
		const vector<double>& aspl, 
		 const vector<double>& exchangeRates)
	{
		// TODO: Avoid checking SPL for all currencies. 
		// It can be checked only for two currencies which are changes because of settlement of the current trade if we remember SPL results for all currencies.
		// if rmt is already passed for this member, then check SPL for only changed currencies, otherwise we have to check SPL for all
		
		//int numMembers = updatedBalance.size();
		bool rmtSuccessful = true;
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		//for (int i = 0; i < memberIndices.size(); ++i)
		{
			//int memberIndex = memberIndices[i];
			double asplTemp = 0.0;
			double novTemp = 0.0;
			bool splPassed = true;
			
			for (size_t currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				size_t index = numMembers * memberIndex + currencyIndex;
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

	inline bool verifySettlement_pair_v13b(
		bitset<128>& rmtPassed,
		const vector<double>& updatedBalance,
		int partyId, 
		int cPartyId,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates)
	{
		bool result1 = verifySettlement_v13b(rmtPassed, updatedBalance, partyId, spl, aspl, exchangeRates);
		bool result2 = verifySettlement_v13b(rmtPassed, updatedBalance, cPartyId, spl, aspl, exchangeRates);
		return result1 && result2;
	}

	inline bool verifySettlement_range_v13b(
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
			if(!verifySettlement_v13b(rmtPassed, updatedBalance, memberIndex, spl, aspl, exchangeRates))
				rmtSuccessful = false;

		return rmtSuccessful;
	}

	void debugPrint_v13b(int level, unsigned long long numberOfFunctionCalls, size_t heapSize,
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

	double doSettlement_branch_and_bound_v13b(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		vector<double>& initialBalance,
		const vector<double>& exchangeRates,
		MM_Heap<fxDecisionTreeNode_v13b*, fxDecisionTreeNodeCompare_v13b>& fxMaxHeap_v13b,
		vector<vector<fxDecisionTreeNode_v13b>>& heapObjectsGrowingPool,
		size_t initialHeapCapacity,
		vector< vector<double> >& cumulativeBalance,
		vector<double>& cumulativeSettledAmount)
	{
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();

		//std::sort(trades.begin(), trades.end(),
		//	[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
		//	return (lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] + lhs.sellVol_ * exchangeRates[static_cast<int>(lhs.sellCurr_)])
		//	> (rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)] + rhs.sellVol_ * exchangeRates[static_cast<int>(rhs.sellCurr_)]);
		//});

		fxDecisionTreeNode_v13b* pObj = fxMaxHeap_v13b.getNextAvailableElement();
		fxDecisionTreeNode_v13b& current = *pObj;
		current.level = -1;
		current.currentBalance.resize(numMembers * numCurrencies, 0.0);
		int startIndex = -1;
		for (size_t memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			for (size_t currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
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
		for (size_t s = trades.size(); s > 0; --s)
		{
			size_t i = s - 1;
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
		verifySettlement_range_v13b(current.rmtPassed, current.currentBalance, 0, static_cast<int>(aspl.size() - 1), spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(
			initialBalance,
			current.level + 1, 
			trades,
			spl, 
			aspl, 
			exchangeRates);
		fxMaxHeap_v13b.push(pObj);

		unsigned long long numberOfFunctionCalls = 0;
		size_t sizeOfHeap = 0;

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

		while (!fxMaxHeap_v13b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v13b.size())
				sizeOfHeap = fxMaxHeap_v13b.size();

			fxDecisionTreeNode_v13b* pCurrent = fxMaxHeap_v13b.top();
			fxDecisionTreeNode_v13b& current = *pCurrent;

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
					initialBalance,
					current.level + 1,
					trades,
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
			fxDecisionTreeNode_v13b& include = current;
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

			verifySettlement_pair_v13b(include.rmtPassed, include.currentBalance, partyId, cPartyId, spl, aspl, exchangeRates);
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
					if (fxMaxHeap_v13b.capacity() == fxMaxHeap_v13b.size()) //need to grow pool
					{
						heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v13b>(initialHeapCapacity, fxDecisionTreeNode_v13b{ initialBalance.size(), trades.size() }));
						fxMaxHeap_v13b.reserve(fxMaxHeap_v13b.capacity() + initialHeapCapacity);
						size_t lastIndex = heapObjectsGrowingPool.size() - 1;
						for (int i = 0; i < initialHeapCapacity; ++i)
							fxMaxHeap_v13b.addToData(&heapObjectsGrowingPool[lastIndex][i]);
					}

					fxDecisionTreeNode_v13b* pExclude = fxMaxHeap_v13b.getNextAvailableElement();
					fxDecisionTreeNode_v13b& exclude = *pExclude;
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
					fxMaxHeap_v13b.push(pExclude);
				}
			}

			if(current.upperbound < current.settledAmount)
				fxMaxHeap_v13b.pop();
			else if(current.level == trades.size() - 1)
				fxMaxHeap_v13b.pop();
		}

		fxMaxHeap_v13b.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}