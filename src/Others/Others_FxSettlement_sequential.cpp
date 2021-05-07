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

	bool verifySettlement_sequential(
		const vector<double>& updatedBalance,
		const vector<int>& memberIndices,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& exchangeRates)
	{
		//TODO: we can avoid checking SPL for all currencies. No need to check it for all, we can check it only for 2 currencies involved in trade.
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		for (int tradeIndex = 0; tradeIndex < memberIndices.size(); ++tradeIndex)
		{
			int memberIndex = memberIndices[tradeIndex];
			double asplTemp = 0.0;
			double novTemp = 0.0;

			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				size_t index = numMembers * memberIndex + currencyIndex;
				if (updatedBalance[index] + zero < -spl[index])
				{
					return false;
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
		}

		return true;
	}

	double doSettlement_sequential(
		vector<bool>& settleFlagsOut,
		const vector<Trade>& trades,
		const vector<double>& spl,
		const vector<double>& aspl,
		const vector<double>& initialBalance,
		const vector<double>& exchangeRates)
	{
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		double amountSettled = 0.0;
		vector<double> currentBalance{ initialBalance };
		bool somethingSettled = true;
		size_t lastTradeSettledInLastPass = trades.size();
		while (somethingSettled)
		{
			somethingSettled = false;
			for (int tradeIndex = 0; tradeIndex < trades.size(); ++tradeIndex)
			{
				if (!somethingSettled && tradeIndex == lastTradeSettledInLastPass)
					break;

				++(TestStats::currentTestStats.numberOfFunctionCalls);

				if (settleFlagsOut[tradeIndex]) continue;

				// Update current balance
				int partyId = trades[tradeIndex].partyId_;
				int cPartyId = trades[tradeIndex].cPartyId_;
				int buyCurrId = static_cast<int>(trades[tradeIndex].buyCurr_);
				int sellCurrId = static_cast<int>(trades[tradeIndex].sellCurr_);
				currentBalance[numMembers * partyId + buyCurrId] += trades[tradeIndex].buyVol_;
				currentBalance[numMembers * partyId + sellCurrId] -= trades[tradeIndex].sellVol_;
				currentBalance[numMembers * cPartyId + buyCurrId] -= trades[tradeIndex].buyVol_;
				currentBalance[numMembers * cPartyId + sellCurrId] += trades[tradeIndex].sellVol_;

				if (verifySettlement_sequential(currentBalance, { partyId, cPartyId }, spl, aspl, exchangeRates))
				{
					amountSettled += (
						trades[tradeIndex].buyVol_ * exchangeRates[static_cast<int>(trades[tradeIndex].buyCurr_)]
						+ trades[tradeIndex].sellVol_ * exchangeRates[static_cast<int>(trades[tradeIndex].sellCurr_)]
						);
					settleFlagsOut[tradeIndex] = true;
					somethingSettled = true;
					lastTradeSettledInLastPass = tradeIndex;
				}
				else
				{
					settleFlagsOut[tradeIndex] = false;
					//revert balance changes
					currentBalance[numMembers * partyId + buyCurrId] -= trades[tradeIndex].buyVol_;
					currentBalance[numMembers * partyId + sellCurrId] += trades[tradeIndex].sellVol_;
					currentBalance[numMembers * cPartyId + buyCurrId] += trades[tradeIndex].buyVol_;
					currentBalance[numMembers * cPartyId + sellCurrId] -= trades[tradeIndex].sellVol_;
				}
			}
		}

		return amountSettled;
	}
}