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

	string testDataPath{ "../../../test/data/FxSettlement/test_cases/" };
	string tradesFileName{ "trades.csv" };
	string initialBalancesFileName{ "initial_balances.csv" };
	string splFileName{ "spl.csv" };
	string asplFileName{ "aspl.csv" };
	string fxRatesFileName{ "fx_rates.csv" };
	string resultsFileName{ "results.csv" };
	string settledAmountTag{ "Settled Amount: " };
	string settledTradeIdsTag{ "Settled Trade Ids:" };

	string to_string_max_precision(double value)
	{
		std::stringstream buffer;
		//buffer.precision(maxPrecision);
		buffer.precision(std::numeric_limits<double>::digits10);
		buffer << std::fixed << value;
		return buffer.str();
	}

	bool rmt(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		double novVal = 0.0;
		vector< vector<double> > updatedBalance(initialBalance.begin(), initialBalance.end());

		for (int i = 0; i < trades.size(); ++i)
		{
			if (!trades[i].isSettled_) continue;

			int partyIndex = trades[i].partyId_;
			int cPartyIndex = trades[i].cPartyId_;

			updatedBalance[partyIndex][static_cast<int>(trades[i].buyCurr_)] += trades[i].buyVol_;
			updatedBalance[partyIndex][static_cast<int>(trades[i].sellCurr_)] -= trades[i].sellVol_;
			updatedBalance[cPartyIndex][static_cast<int>(trades[i].buyCurr_)] -= trades[i].buyVol_;
			updatedBalance[cPartyIndex][static_cast<int>(trades[i].sellCurr_)] += trades[i].sellVol_;
		}

		int numMembers = updatedBalance.size();
		int numCurrencies = updatedBalance[0].size();
		vector<double> currentAspl(numMembers);
		vector<double> currentNov(numMembers);
		for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
		{
			double asplTemp = 0.0;
			double novTemp = 0.0;
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				if (updatedBalance[memberIndex][currencyIndex] < -spl[memberIndex][currencyIndex])
					return false;

				double currentBalanceInDollars = updatedBalance[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (novTemp < -zero)
				return false;

			if (asplTemp < (-aspl[memberIndex] - zero))
				return false;
		}

		return true;
	}

	bool verifySettlement(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		return rmt(trades, spl, aspl, initialBalance, exchangeRates);
	}

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

	void testFxSettlement(vector<TestCase>& testCases)
	{
		vector<TestStats> stats;

		static unordered_map<AlgoType, string> AlgoTypeStrings{
			{ AlgoType::naive_v1 , "naive_v1"}
		};
		int columnWidth[7] = { 12, 12, 12, 12, 15, 18, 12 };
		cout << "\n"
			<< setw(columnWidth[0]) << std::left << "TestIndex"
			<< setw(columnWidth[1]) << std::left << "AlgoType"
			<< setw(columnWidth[2]) << std::left << "numTrades"
			<< setw(columnWidth[3]) << std::left << "numMembers"
			<< setw(columnWidth[4]) << std::left << "numCurrencies"
			<< setw(columnWidth[6]) << std::left << "TestResult"
			<< setw(columnWidth[5]) << std::right << "Duration";

		for (int testCaseIndex = 0; testCaseIndex < testCases.size(); ++testCaseIndex)
		{
			double actualSettledAmount = 0.0;
			for (int i = 0; i < static_cast<int>(AlgoType::totalAlgos); ++i)
			{
				std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
				switch (AlgoType(i))
				{
				case AlgoType::naive_v1:
					actualSettledAmount = doSettlement_naive_v1(
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				default:
					assert(false, "Algo type '" + to_string(i) + "' is not suported");
				}
				std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
				unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

				bool verified = verifySettlement(
					testCases[testCaseIndex].trades_,
					testCases[testCaseIndex].spl_,
					testCases[testCaseIndex].aspl_,
					testCases[testCaseIndex].initialBalance_,
					testCases[testCaseIndex].exchangeRates_
				);
				vector<int> actualSettledTradeIds;
				for (int i = 0; i < testCases[testCaseIndex].trades_.size(); ++i)
					if (testCases[testCaseIndex].trades_[i].isSettled_)
						actualSettledTradeIds.push_back(testCases[testCaseIndex].trades_[i].id_);

				if ((GlobalFlagCreateTestCases || overrideTestResults)
					&& !testCases[testCaseIndex].fileNamePrefix_.empty()) //File prefix is empty only for hardcoded tests
				{
					//Write results to csv file
					ofstream resultsFile{ testDataPath + testCases[testCaseIndex].fileNamePrefix_ + "_" + resultsFileName };
					if (resultsFile.is_open())
					{
						string settledAmount{ settledAmountTag + to_string_max_precision(actualSettledAmount) };
						resultsFile.write(settledAmount.c_str(), settledAmount.length());
						string settledTrades{ "\n" + settledTradeIdsTag };
						resultsFile.write(settledTrades.c_str(), settledTrades.length());
						for (int i = 0; i < actualSettledTradeIds.size(); ++i)
						{
							string tradeId{ "\n" + to_string(actualSettledTradeIds[i]) };
							resultsFile.write(tradeId.c_str(), tradeId.length());
						}
					}
				}
				else
				{
					//MM_EXPECT_TRUE(verified == true, verified);
					//MM_EXPECT_TRUE(fabs(actualSettledAmount - testCases[testCaseIndex].settledAmount_) < zero, actualSettledAmount, testCases[testCaseIndex].settledAmount_);
					//MM_EXPECT_TRUE(actualSettledTradeIds == testCases[testCaseIndex].settledTradeIds_, actualSettledTradeIds, testCases[testCaseIndex].settledTradeIds_);
				}

				std::stringstream buffer;
				buffer.imbue(std::locale(""));
				buffer << std::fixed << duration;

				TestStats testStats{
					testCaseIndex,
					AlgoTypeStrings[AlgoType(i)],
					testCases[testCaseIndex].trades_.size(),
					testCases[testCaseIndex].aspl_.size(),
					testCases[testCaseIndex].spl_.size() / testCases[testCaseIndex].aspl_.size(),
					buffer.str() + " ns",
					verified
				};
				//printOrWrite(testStats);
				cout << "\n"
					<< setw(columnWidth[0]) << std::left << testStats.testCaseIndex
					<< setw(columnWidth[1]) << std::left << testStats.algoType
					<< setw(columnWidth[2]) << std::left << testStats.numTrades
					<< setw(columnWidth[3]) << std::left << testStats.numMembers
					<< setw(columnWidth[4]) << std::left << testStats.numCurrencies
					<< setw(columnWidth[6]) << std::left << (testStats.testCaseResult ? "SUCCESS" : "FAILED")
					<< setw(columnWidth[5]) << std::right << testStats.durationStr;
				
				stats.push_back(std::move(testStats));
			}
		}
	}

	MM_DECLARE_FLAG(Others_FxSettlement);

	MM_UNIT_TEST(Others_FxSettlement_sanity, Others_FxSettlement)
	{
		if(GlobalFlagCreateTestCases)
			createTestCases();
		testFxSettlement(getTestCases());
	}
}