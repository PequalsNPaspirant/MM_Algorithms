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

	double doSettlement_naive_v1(
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
		vector<double> currentBalanceExclude{ currentBalanceOut };
		double exclude = doSettlement_naive_v1(currentTradeIndex + 1, settleFlagsExclude, currentBalanceExclude, trades, spl, aspl, exchangeRates);

		//Try to settle this trade
		vector<bool> settleFlagsInclude{ settleFlagsOut };
		int partyIndex = trades[currentTradeIndex].partyId_;
		int cPartyIndex = trades[currentTradeIndex].cPartyId_;
		int buyCurrIndex = static_cast<int>(trades[currentTradeIndex].buyCurr_);
		int sellCurrIndex = static_cast<int>(trades[currentTradeIndex].sellCurr_);
		int numMembers = aspl.size();
		int numCurrencies = spl.size() / aspl.size();
		
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

		double include = currentSettledAmount + doSettlement_naive_v1(currentTradeIndex + 1, settleFlagsInclude, currentBalanceInclude,
			trades, spl, aspl, exchangeRates);

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
				if (currentBalanceInclude[numMembers * memberIndex + currencyIndex] + zero < -spl[numMembers * memberIndex + currencyIndex])
				{
					rmtTestResult = false;
					break;
				}

				double currentBalanceInDollars = currentBalanceInclude[numMembers * memberIndex + currencyIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (novTemp < -zero)
				rmtTestResult = false;

			if (asplTemp + zero < -aspl[memberIndex])
				rmtTestResult = false;
		}

		//return std::max(exclude, include);
		if (include > exclude && rmtTestResult)
		{
			settleFlagsOut = settleFlagsInclude;
			currentBalanceOut = currentBalanceInclude;
			return include;
		}
		else
		{
			settleFlagsOut = settleFlagsExclude;
			currentBalanceOut = currentBalanceExclude;
			return exclude;
		}
	}

	double doSettlement_naive_v1(
		vector<bool>& settleFlagsOut, 
		const vector<Trade>& trades, 
		const vector<double>& spl, 
		const vector<double>& aspl, 
		const vector<double>& initialBalance, 
		const vector<double>& exchangeRates)
	{
		vector<double> currentBalance{ initialBalance };
		return doSettlement_naive_v1(0, settleFlagsOut, currentBalance, trades, spl, aspl, exchangeRates);
	}
}