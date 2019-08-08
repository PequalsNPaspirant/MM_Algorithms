#include <vector>
using namespace std;

#include "Others/Others_FxSettlement.h"

namespace mm {

	struct Result
	{
		double settledAmount;

	};

	double doSettlement_naive_v1(
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
		vector< vector<double> > currentBalanceExclude{ currentBalanceOut };
		double exclude = doSettlement_naive_v1(currentTradeIndex + 1, settleFlagsExclude, currentBalanceExclude, trades, spl, aspl, exchangeRates);

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
		const vector< vector<double> >& spl, 
		const vector<double>& aspl, 
		const vector< vector<double> >& initialBalance, 
		const vector<double>& exchangeRates)
	{
		vector< vector<double> > currentBalance{ initialBalance };
		return doSettlement_naive_v1(0, settleFlagsOut, currentBalance, trades, spl, aspl, exchangeRates);
	}
}