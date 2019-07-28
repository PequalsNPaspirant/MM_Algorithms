#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
#include <fstream>
#include <limits> // for std::numeric_limits<double>::digits10
#include <chrono>
#include <unordered_map>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "Utils/Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	double doSettlement_naive_v1(int currentTradeIndex, vector<double>& currentNOV, vector< vector<double> >& currentSP, vector<double>& currentASP,
		vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		if (currentTradeIndex == trades.size())
			return 0.0;

		//Do not settle this trade
		double exclude = doSettlement_naive_v1(currentTradeIndex + 1, currentNOV, currentSP, currentASP,
			trades, spl, aspl, initialBalance, exchangeRates);

		//Try to settle this trade
		double include = 0.0;

		int partyIndex = trades[currentTradeIndex].partyId_;
		int cPartyIndex = trades[currentTradeIndex].cPartyId_;
		int buyCurrIndex = static_cast<int>(trades[currentTradeIndex].buyCurr_);
		int sellCurrIndex = static_cast<int>(trades[currentTradeIndex].sellCurr_);
		//Back up all previous NOV, SPL and ASPL to revert later for backtracking
		double prevPartyBuySP = currentSP[partyIndex][buyCurrIndex];
		double prevPartySellSP = currentSP[partyIndex][sellCurrIndex];
		double prevCPartyBuySP = currentSP[cPartyIndex][buyCurrIndex];
		double prevCPartySellSP = currentSP[cPartyIndex][sellCurrIndex];
		double prevPartyASP = currentASP[partyIndex];
		double prevCPartyASP = currentASP[cPartyIndex];
		double prevPartyNOV = currentNOV[partyIndex];
		double prevCPartyNOV = currentNOV[cPartyIndex];

		double prevPartyBuySPLInDollars = currentSP[partyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		double prevPartySellSPLInDollars = currentSP[partyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		double prevCPartyBuySPLInDollars = currentSP[cPartyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		double prevCPartySellSPLInDollars = currentSP[cPartyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		currentSP[partyIndex][buyCurrIndex] += trades[currentTradeIndex].buyVol_;
		currentSP[partyIndex][sellCurrIndex] -= trades[currentTradeIndex].sellVol_;
		currentSP[cPartyIndex][buyCurrIndex] -= trades[currentTradeIndex].buyVol_;
		currentSP[cPartyIndex][sellCurrIndex] += trades[currentTradeIndex].sellVol_;
		double currPartyBuySPLInDollars = currentSP[partyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		double currPartySellSPLInDollars = currentSP[partyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];
		double currCPartyBuySPLInDollars = currentSP[cPartyIndex][buyCurrIndex] * exchangeRates[buyCurrIndex];
		double currCPartySellSPLInDollars = currentSP[cPartyIndex][sellCurrIndex] * exchangeRates[sellCurrIndex];

		
		if (prevPartyBuySPLInDollars < -zero)
			currentASP[partyIndex] -= prevPartyBuySPLInDollars;
		if(currPartyBuySPLInDollars < -zero)
			currentASP[partyIndex] += currPartyBuySPLInDollars;
		if (prevPartySellSPLInDollars < -zero)
			currentASP[partyIndex] -= prevPartySellSPLInDollars;
		if (currPartySellSPLInDollars < -zero)
			currentASP[partyIndex] += currPartySellSPLInDollars;

		if (prevCPartyBuySPLInDollars < -zero)
			currentASP[cPartyIndex] -= prevCPartyBuySPLInDollars;
		if (currCPartyBuySPLInDollars < -zero)
			currentASP[cPartyIndex] += currCPartyBuySPLInDollars;
		if (prevCPartySellSPLInDollars < -zero)
			currentASP[cPartyIndex] -= prevCPartySellSPLInDollars;
		if (currCPartySellSPLInDollars < -zero)
			currentASP[cPartyIndex] += currCPartySellSPLInDollars;

		double buyVolInDollars = trades[currentTradeIndex].buyVol_ * exchangeRates[buyCurrIndex];
		double sellVolInDollars = trades[currentTradeIndex].sellVol_ * exchangeRates[sellCurrIndex];
		currentNOV[partyIndex] += buyVolInDollars;
		currentNOV[partyIndex] -= sellVolInDollars;
		currentNOV[cPartyIndex] -= buyVolInDollars;
		currentNOV[cPartyIndex] += sellVolInDollars;

		if (!(currentSP[partyIndex][buyCurrIndex] < -spl[partyIndex][buyCurrIndex]
			&& currentSP[partyIndex][sellCurrIndex] < -spl[partyIndex][sellCurrIndex]
			&& currentSP[cPartyIndex][buyCurrIndex] < -spl[cPartyIndex][buyCurrIndex]
			&& currentSP[cPartyIndex][sellCurrIndex] < -spl[cPartyIndex][sellCurrIndex]
			&& currentNOV[partyIndex] < -zero
			&& currentNOV[cPartyIndex] < -zero
			&& currentASP[partyIndex] < -aspl[partyIndex]
			&& currentASP[cPartyIndex] < -aspl[cPartyIndex]
			))
		{
			trades[currentTradeIndex].isSettled_ = true;
			double currentSettledAmount = trades[currentTradeIndex].buyVol_ * exchangeRates[buyCurrIndex];
			include = currentSettledAmount + doSettlement_naive_v1(currentTradeIndex + 1, currentNOV, currentSP, currentASP,
				trades, spl, aspl, initialBalance, exchangeRates);
		}

		//Revert back changes to currentNOV, currentSPL, currentASPL to backtrack
		currentSP[partyIndex][buyCurrIndex] = prevPartyBuySP;
		currentSP[partyIndex][sellCurrIndex] = prevPartySellSP;
		currentSP[cPartyIndex][buyCurrIndex] = prevCPartyBuySP;
		currentSP[cPartyIndex][sellCurrIndex] = prevCPartySellSP;
		currentASP[partyIndex] = prevPartyASP;
		currentASP[cPartyIndex] = prevCPartyASP;
		currentNOV[partyIndex] = prevPartyNOV;
		currentNOV[cPartyIndex] = prevCPartyNOV;

		//return std::max(exclude, include);
		if (exclude > include)
		{
			trades[currentTradeIndex].isSettled_ = false;
			return exclude;
		}
		
		return include;
	}

	double doSettlement_naive_v1(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		vector<double> currentNOV(aspl.size(), 0.0);
		vector< vector<double> > currentSP = initialBalance;
		vector<double> currentASP(aspl.size(), 0.0);

		int numMembers = initialBalance.size();
		int numCurrencies = initialBalance[0].size();
		for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			double asplTemp = 0.0;
			double novTemp = 0.0;
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				double currentBalanceInDollars = initialBalance[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			currentNOV[memberIndex] = novTemp;
			currentASP[memberIndex] = asplTemp;
		}
		return doSettlement_naive_v1(0, currentNOV, currentSP, currentASP, trades, spl, aspl, initialBalance, exchangeRates);
	}
}