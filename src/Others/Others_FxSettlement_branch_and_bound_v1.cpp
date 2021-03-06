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
#include <cmath>
using namespace std;

#include "Others/Others_FxSettlement.h"

namespace mm {

	/*
	struct Result
	{
		double settledAmount;

	};

	double doSettlement_branch_and_bound_v1(
		int currentTradeIndex, 
		vector<bool>& settleFlagsOut, 
		vector<double>& currentBalanceOut,
		const vector<Trade>& trades, 
		const vector<double>& spl, 
		const vector<double>& aspl, 
		const vector<double>& exchangeRates)
	{
		if (currentTradeIndex == trades.size())
			return 0.0;

		//Do not settle this trade
		vector<bool> settleFlagsExclude{ settleFlagsOut };
		//vector<double> currentNOVExclude{ currentNOV };
		//vector<double> currentSPExclude{ currentSP };
		//vector<double> currentASPExclude{ currentASP };
		vector<double> currentBalanceExclude{ currentBalanceOut };
		double exclude = doSettlement_naive_v2(currentTradeIndex + 1, settleFlagsExclude, currentBalanceExclude, trades, spl, aspl, exchangeRates);

		//Try to settle this trade
		vector<bool> settleFlagsInclude{ settleFlagsOut };
		int partyIndex = trades[currentTradeIndex].partyId_;
		int cPartyIndex = trades[currentTradeIndex].cPartyId_;
		int buyCurrIndex = static_cast<int>(trades[currentTradeIndex].buyCurr_);
		int sellCurrIndex = static_cast<int>(trades[currentTradeIndex].sellCurr_);
		int numCurrencies = currentBalanceOut[0].size();
		
		vector<double> currentBalanceInclude{ currentBalanceOut };
		double buyVolInDollars = trades[currentTradeIndex].buyVol_ * exchangeRates[buyCurrIndex];
		double sellVolInDollars = trades[currentTradeIndex].sellVol_ * exchangeRates[sellCurrIndex];
		if (fabs(buyVolInDollars - sellVolInDollars) > zero)
		{
			//Need to correct fx rates
			int i = 0;
		}
		double currentSettledAmount = buyVolInDollars;
		//Update Balances for current trade
		currentBalanceInclude[partyIndex][buyCurrIndex] += trades[currentTradeIndex].buyVol_;
		currentBalanceInclude[partyIndex][sellCurrIndex] -= trades[currentTradeIndex].sellVol_;
		currentBalanceInclude[cPartyIndex][buyCurrIndex] -= trades[currentTradeIndex].buyVol_;
		currentBalanceInclude[cPartyIndex][sellCurrIndex] += trades[currentTradeIndex].sellVol_;
		settleFlagsInclude[currentTradeIndex] = true;

		double include = currentSettledAmount + doSettlement_naive_v2(currentTradeIndex + 1, settleFlagsInclude, currentBalanceInclude,
			trades, spl, aspl, exchangeRates);

		//return std::max(exclude, include);
		if (include > exclude)
		{
			//Do rmt tests
			bool rmtTestResult = true;
			int memberIndices[2] = { partyIndex , cPartyIndex };

			for (int i = 0; rmtTestResult && i < 2; ++i)
			{
				int memberIndex = memberIndices[i];
				double asplTemp = 0.0;
				double novTemp = 0.0;
				for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
				{
					if (currentBalanceInclude[memberIndex][currencyIndex] + zero < -spl[memberIndex][currencyIndex])
					{
						rmtTestResult = false;
						break;
					}

					double currentBalanceInDollars = currentBalanceInclude[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
					novTemp += currentBalanceInDollars;
					if (currentBalanceInDollars < -zero)
						asplTemp += currentBalanceInDollars;
				}

				if (novTemp < -zero)
					rmtTestResult = false;

				if (asplTemp + zero < -aspl[memberIndex])
					rmtTestResult = false;
			}

			if (rmtTestResult)
			{
				settleFlagsOut = settleFlagsInclude;
				//currentNOV = currentNOVInclude;
				//currentSP = currentSPInclude;
				//currentASP = currentASPInclude;
				currentBalanceOut = currentBalanceInclude;
				return include;
			}
		}

		settleFlagsOut = settleFlagsExclude;
		//currentNOV = currentNOVExclude;
		//currentSP = currentSPExclude;
		//currentASP = currentASPExclude;
		currentBalanceOut = currentBalanceExclude;
		return exclude;
		
		//vector<double> currentNOVInclude{ currentNOV };
		//vector<double> currentSPInclude{ currentSP };
		//vector<double> currentASPInclude{ currentASP };

		//Back up all previous NOV, SPL and ASPL to revert later for backtracking
		//double prevPartyBuySP = currentSPInclude[partyIndex][buyCurrIndex];
		//double prevPartySellSP = currentSPInclude[partyIndex][sellCurrIndex];
		//double prevCPartyBuySP = currentSPInclude[cPartyIndex][buyCurrIndex];
		//double prevCPartySellSP = currentSPInclude[cPartyIndex][sellCurrIndex];
		//double prevPartyASP = currentASPInclude[partyIndex];
		//double prevCPartyASP = currentASPInclude[cPartyIndex];
		//double prevPartyNOV = currentNOVInclude[partyIndex];
		//double prevCPartyNOV = currentNOVInclude[cPartyIndex];

		//double prevPartyBuySPLInDollars = currentSPInclude[partyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		//double prevPartySellSPLInDollars = currentSPInclude[partyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		//double prevCPartyBuySPLInDollars = currentSPInclude[cPartyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		//double prevCPartySellSPLInDollars = currentSPInclude[cPartyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		//currentSPInclude[partyIndex][buyCurrIndex] += trades[currentTradeIndex].buyVol_;
		//currentSPInclude[partyIndex][sellCurrIndex] -= trades[currentTradeIndex].sellVol_;
		//currentSPInclude[cPartyIndex][buyCurrIndex] -= trades[currentTradeIndex].buyVol_;
		//currentSPInclude[cPartyIndex][sellCurrIndex] += trades[currentTradeIndex].sellVol_;
		//double currPartyBuySPLInDollars = currentSPInclude[partyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		//double currPartySellSPLInDollars = currentSPInclude[partyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		//double currCPartyBuySPLInDollars = currentSPInclude[cPartyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		//double currCPartySellSPLInDollars = currentSPInclude[cPartyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		
		//if (prevPartyBuySPLInDollars < -zero)
		//	currentASPInclude[partyIndex] -= prevPartyBuySPLInDollars;
		//if(currPartyBuySPLInDollars < -zero)
		//	currentASPInclude[partyIndex] += currPartyBuySPLInDollars;
		//if (prevPartySellSPLInDollars < -zero)
		//	currentASPInclude[partyIndex] -= prevPartySellSPLInDollars;
		//if (currPartySellSPLInDollars < -zero)
		//	currentASPInclude[partyIndex] += currPartySellSPLInDollars;

		//if (prevCPartyBuySPLInDollars < -zero)
		//	currentASPInclude[cPartyIndex] -= prevCPartyBuySPLInDollars;
		//if (currCPartyBuySPLInDollars < -zero)
		//	currentASPInclude[cPartyIndex] += currCPartyBuySPLInDollars;
		//if (prevCPartySellSPLInDollars < -zero)
		//	currentASPInclude[cPartyIndex] -= prevCPartySellSPLInDollars;
		//if (currCPartySellSPLInDollars < -zero)
		//	currentASPInclude[cPartyIndex] += currCPartySellSPLInDollars;

		//double buyVolInDollars = trades[currentTradeIndex].buyVol_ * exchangeRates[buyCurrIndex];
		//double sellVolInDollars = trades[currentTradeIndex].sellVol_ * exchangeRates[sellCurrIndex];
		//currentNOVInclude[partyIndex] += buyVolInDollars;
		//currentNOVInclude[partyIndex] -= sellVolInDollars;
		//currentNOVInclude[cPartyIndex] -= buyVolInDollars;
		//currentNOVInclude[cPartyIndex] += sellVolInDollars;

		//bool rmtFailed = (currentSPInclude[partyIndex][buyCurrIndex] < -spl[partyIndex][buyCurrIndex]
		//	|| currentSPInclude[partyIndex][sellCurrIndex] < -spl[partyIndex][sellCurrIndex]
		//	|| currentSPInclude[cPartyIndex][buyCurrIndex] < -spl[cPartyIndex][buyCurrIndex]
		//	|| currentSPInclude[cPartyIndex][sellCurrIndex] < -spl[cPartyIndex][sellCurrIndex]
		//	|| currentNOVInclude[partyIndex] < -zero
		//	|| currentNOVInclude[cPartyIndex] < -zero
		//	|| currentASPInclude[partyIndex] < -aspl[partyIndex]
		//	|| currentASPInclude[cPartyIndex] < -aspl[cPartyIndex]
		//	);
		//if(!rmtFailed)
		//{
		//	
		//}

		//Revert back changes to currentNOV, currentSPL, currentASPL to backtrack
		//currentSP[partyIndex][buyCurrIndex] = prevPartyBuySP;
		//currentSP[partyIndex][sellCurrIndex] = prevPartySellSP;
		//currentSP[cPartyIndex][buyCurrIndex] = prevCPartyBuySP;
		//currentSP[cPartyIndex][sellCurrIndex] = prevCPartySellSP;
		//currentASP[partyIndex] = prevPartyASP;
		//currentASP[cPartyIndex] = prevCPartyASP;
		//currentNOV[partyIndex] = prevPartyNOV;
		//currentNOV[cPartyIndex] = prevCPartyNOV;

	}

	double doSettlement_naive_v2(
		vector<bool>& settleFlagsOut, 
		const vector<Trade>& trades, 
		const vector<double>& spl, 
		const vector<double>& aspl, 
		const vector<double>& initialBalance, 
		const vector<double>& exchangeRates)
	{
		//vector<double> currentNOV(aspl.size(), 0.0);
		//vector<double> currentSP{ initialBalance };
		//vector<double> currentASP(aspl.size(), 0.0);

		//int numMembers = initialBalance.size();
		//int numCurrencies = initialBalance[0].size();
		//for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		//{
		//	double asplTemp = 0.0;
		//	double novTemp = 0.0;
		//	for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
		//	{
		//		double currentBalanceInDollars = initialBalance[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
		//		novTemp += currentBalanceInDollars;
		//		if (currentBalanceInDollars < -zero)
		//			asplTemp += currentBalanceInDollars;
		//	}

		//	currentNOV[memberIndex] = novTemp;
		//	currentASP[memberIndex] = asplTemp;
		//}
		vector<double> currentBalance{ initialBalance };
		return doSettlement_naive_v2(0, settleFlagsOut, currentBalance, trades, spl, aspl, exchangeRates);
	}
	*/




	bool verifySettlement_v1(
		const vector<double>& updatedBalance,
		const vector<double>& spl, 
		const vector<double>& aspl, 
		 const vector<double>& exchangeRates)
	{
		//rmt
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		vector<double> currentAspl(numMembers);
		vector<double> currentNov(numMembers);
		for (size_t memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			double asplTemp = 0.0;
			double novTemp = 0.0;
			for (size_t currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				size_t index = numMembers * memberIndex + currencyIndex;
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

	struct fxDecisionTreeNode_v1
	{
		int level;
		double upperbound;

		vector<double> currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bool rmtPassed;

		void calculateAndSetUpperBound(
			const vector<double>& cumulativeBalance,
			const double cumulativeSettledAmount,
			const vector<double>& spl, 
			const vector<double>& aspl, 
			const vector<double>& exchangeRates)
		{
			//double invalidAmountInDollars = 0.0;
			//vector<double> totalBalance(currentBalance);
			//for (int memberIndex = 0; memberIndex < totalBalance.size(); ++memberIndex)
			//{
			//	for (int currencyIndex = 0; currencyIndex < totalBalance.size(); ++currencyIndex)
			//	{
			//		totalBalance[memberIndex][currencyIndex] += cumulativeBalance[memberIndex][currencyIndex];
			//		//if (totalBalance[memberIndex][currencyIndex] < -spl[memberIndex][currencyIndex])
			//		//	invalidAmountInDollars += ((-totalBalance[memberIndex][currencyIndex] - spl[memberIndex][currencyIndex]) * exchangeRates[currencyIndex]);
			//	}
			//}

			upperbound = settledAmount + cumulativeSettledAmount;
			//upperbound -= (invalidAmountInDollars / 2.0);
			//if (upperbound < 0.0)
			//	upperbound = 0.0;
		}
	};
	struct fxDecisionTreeNodeCompare_v1
	{
		bool operator()(const fxDecisionTreeNode_v1& lhs, const fxDecisionTreeNode_v1& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	double doSettlement_branch_and_bound_v1(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& initialBalance,
		const vector<double>& exchangeRates)
	{
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();

		//calculate value to weight ratio
		//for (int i = 0; i < values.size(); ++i)
		//	valueToWeightsRatio[i] = values[i] / weights[i];

		////initialize the indices
		//for (int i = 0; i < values.size(); ++i)
		//	sortedIndices[i] = i;

		std::sort(trades.begin(), trades.end(),
			[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
			return (lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] + lhs.sellVol_ * exchangeRates[static_cast<int>(lhs.sellCurr_)])
			> (rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)] + rhs.sellVol_ * exchangeRates[static_cast<int>(rhs.sellCurr_)]);
		});

		vector< vector<double> > cumulativeBalance(trades.size(), vector<double>(initialBalance.size(), 0.0));
		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
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
				+ trades[i].sellVol_ * exchangeRates[static_cast<int>(trades[i].sellCurr_)]);
		}

		std::priority_queue<fxDecisionTreeNode_v1, vector<fxDecisionTreeNode_v1>, fxDecisionTreeNodeCompare_v1> fxMaxHeap_v1;

		fxDecisionTreeNode_v1 current;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		//vector<bool> settleFlags;
		current.rmtPassed = false;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		current.calculateAndSetUpperBound(cumulativeBalance[0], cumulativeSettledAmount[0], spl, aspl, exchangeRates);
		fxMaxHeap_v1.push(current);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		size_t sizeOfHeap = 0;

		while (!fxMaxHeap_v1.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v1.size())
				sizeOfHeap = fxMaxHeap_v1.size();

			current = fxMaxHeap_v1.top();
			fxMaxHeap_v1.pop();

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			//include this item
			fxDecisionTreeNode_v1 include = current;
			// Update current balance
			include.currentBalance[numMembers * trades[include.level].partyId_ + static_cast<int>(trades[include.level].buyCurr_)] += trades[include.level].buyVol_;
			include.currentBalance[numMembers * trades[include.level].partyId_ + static_cast<int>(trades[include.level].sellCurr_)] -= trades[include.level].sellVol_;
			include.currentBalance[numMembers * trades[include.level].cPartyId_ + static_cast<int>(trades[include.level].buyCurr_)] -= trades[include.level].buyVol_;
			include.currentBalance[numMembers * trades[include.level].cPartyId_ + static_cast<int>(trades[include.level].sellCurr_)] += trades[include.level].sellVol_;
			
			// Do rmt tests and update maxValue if rmt tests are passed
			include.rmtPassed = verifySettlement_v1(include.currentBalance, spl, aspl, exchangeRates);
			include.settledAmount += (
				trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]
				+ trades[include.level].sellVol_ * exchangeRates[static_cast<int>(trades[include.level].sellCurr_)]);
			include.settleFlags[include.level] = true;
			if (include.rmtPassed && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				settleFlagsOut = include.settleFlags;
			}

			if (current.level == trades.size() - 1)
				continue;

			include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);

			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if ((include.upperbound + zero) >= maxValue)
				fxMaxHeap_v1.push(include);

			//exclude this item
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if ((current.upperbound + zero) >= maxValue)
				fxMaxHeap_v1.push(current);
		}

		while (!fxMaxHeap_v1.empty())
			fxMaxHeap_v1.pop();

		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}