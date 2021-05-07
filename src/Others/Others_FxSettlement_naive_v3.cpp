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
using namespace std;

#include "Others/Others_FxSettlement.h"

namespace mm {

	/*
	Whats new - v3
	Remove all vector<vector<>>
	*/

	double doSettlement_naive_v3(
		int currentTradeIndex, 
		vector<bool>& settleFlagsOut, 
		vector<double>& currentBalanceOut,
		const vector<Trade>& trades, 
		const vector<double>& spl, 
		const vector<double>& aspl, 
		const vector<double>& exchangeRates)
	{
		++(TestStats::currentTestStats.numberOfFunctionCalls);

		if (currentTradeIndex == trades.size())
			return 0.0;

		//Do not settle this trade
		vector<bool> settleFlagsExclude{ settleFlagsOut };
		//vector<double> currentNOVExclude{ currentNOV };
		//vector<double> currentSPExclude{ currentSP };
		//vector<double> currentASPExclude{ currentASP };
		vector<double> currentBalanceExclude{ currentBalanceOut };
		double exclude = doSettlement_naive_v3(currentTradeIndex + 1, settleFlagsExclude, currentBalanceExclude, trades, spl, aspl, exchangeRates);

		//Try to settle this trade
		vector<bool> settleFlagsInclude{ settleFlagsOut };
		int partyIndex = trades[currentTradeIndex].partyId_;
		int cPartyIndex = trades[currentTradeIndex].cPartyId_;
		int buyCurrIndex = static_cast<int>(trades[currentTradeIndex].buyCurr_);
		int sellCurrIndex = static_cast<int>(trades[currentTradeIndex].sellCurr_);
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		
		vector<double> currentBalanceInclude{ currentBalanceOut };
		double buyVolInDollars = trades[currentTradeIndex].buyVol_ * exchangeRates[buyCurrIndex];
		double sellVolInDollars = trades[currentTradeIndex].sellVol_ * exchangeRates[sellCurrIndex];
		if (fabs(buyVolInDollars - sellVolInDollars) > zero)
		{
			//Need to correct fx rates
			int i = 0;
		}
		double currentSettledAmount = buyVolInDollars + sellVolInDollars;
		//Update Balances for current trade
		currentBalanceInclude[numMembers * partyIndex + buyCurrIndex] += trades[currentTradeIndex].buyVol_;
		currentBalanceInclude[numMembers * partyIndex + sellCurrIndex] -= trades[currentTradeIndex].sellVol_;
		currentBalanceInclude[numMembers * cPartyIndex + buyCurrIndex] -= trades[currentTradeIndex].buyVol_;
		currentBalanceInclude[numMembers * cPartyIndex + sellCurrIndex] += trades[currentTradeIndex].sellVol_;
		settleFlagsInclude[currentTradeIndex] = true;

		double include = currentSettledAmount + doSettlement_naive_v3(currentTradeIndex + 1, settleFlagsInclude, currentBalanceInclude,
			trades, spl, aspl, exchangeRates);

		//return std::max(exclude, include);
		if (include > exclude)
		{
			//Do rmt tests
			bool rmtTestResult = true;
			int memberIndices[2] = { partyIndex , cPartyIndex };

			for (int memberIndex = 0; rmtTestResult && memberIndex < numMembers; ++memberIndex)
			{
				//int memberIndex = memberIndices[i];
				double asplTemp = 0.0;
				double novTemp = 0.0;
				for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
				{
					size_t index = numMembers * memberIndex + currencyIndex;
					if (currentBalanceInclude[index] + zero < -spl[index])
					{
						rmtTestResult = false;
						break;
					}

					double currentBalanceInDollars = currentBalanceInclude[index] * exchangeRates[currencyIndex];
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

	double doSettlement_naive_v3(
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
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		vector<double> currentBalance{ initialBalance };
		//for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		//{
		//	for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
		//	{
		//		int i
		//		currentBalance[numMembers * memberIndex + currencyIndex] = initialBalance[memberIndex][currencyIndex];
		//	}
		//}
		return doSettlement_naive_v3(0, settleFlagsOut, currentBalance, trades, spl, aspl, exchangeRates);
	}
}