#include <vector>
using namespace std;

#include "Others/Others_FxSettlement.h"

namespace mm {

	struct Result
	{
		double settledAmount;

	};

	double doSettlement_naive_v2(
		int currentTradeIndex, 
		vector<bool>& settleFlagsOut, 
		vector< vector<double> >& currentBalanceOut,
		const vector<Trade>& trades, 
		const vector< vector<double> >& spl, 
		const vector<double>& aspl, 
		const vector<double>& exchangeRates)
	{
		++(TestStats::currentTestStats.numberOfFunctionCalls);

		if (currentTradeIndex == trades.size())
			return 0.0;

		//Do not settle this trade
		vector<bool> settleFlagsExclude{ settleFlagsOut };
		//vector<double> currentNOVExclude{ currentNOV };
		//vector< vector<double> > currentSPExclude{ currentSP };
		//vector<double> currentASPExclude{ currentASP };
		vector< vector<double> > currentBalanceExclude{ currentBalanceOut };
		double exclude = doSettlement_naive_v2(currentTradeIndex + 1, settleFlagsExclude, currentBalanceExclude, trades, spl, aspl, exchangeRates);

		//Try to settle this trade
		vector<bool> settleFlagsInclude{ settleFlagsOut };
		int partyIndex = trades[currentTradeIndex].partyId_;
		int cPartyIndex = trades[currentTradeIndex].cPartyId_;
		int buyCurrIndex = static_cast<int>(trades[currentTradeIndex].buyCurr_);
		int sellCurrIndex = static_cast<int>(trades[currentTradeIndex].sellCurr_);
		int numMembers = currentBalanceOut.size();
		int numCurrencies = currentBalanceOut[0].size();
		
		vector< vector<double> > currentBalanceInclude{ currentBalanceOut };
		double buyVolInDollars = trades[currentTradeIndex].buyVol_ * exchangeRates[buyCurrIndex];
		double sellVolInDollars = trades[currentTradeIndex].sellVol_ * exchangeRates[sellCurrIndex];
		if (fabs(buyVolInDollars - sellVolInDollars) > zero)
		{
			//Need to correct fx rates
			int i = 0;
		}
		double currentSettledAmount = buyVolInDollars + sellVolInDollars;
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

			for (int memberIndex = 0; rmtTestResult && memberIndex < numMembers; ++memberIndex)
			{
				//int memberIndex = memberIndices[i];
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
		//vector< vector<double> > currentSPInclude{ currentSP };
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
		const vector< vector<double> >& spl, 
		const vector<double>& aspl, 
		const vector< vector<double> >& initialBalance, 
		const vector<double>& exchangeRates)
	{
		//vector<double> currentNOV(aspl.size(), 0.0);
		//vector< vector<double> > currentSP{ initialBalance };
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
		vector< vector<double> > currentBalance{ initialBalance };
		return doSettlement_naive_v2(0, settleFlagsOut, currentBalance, trades, spl, aspl, exchangeRates);
	}
}