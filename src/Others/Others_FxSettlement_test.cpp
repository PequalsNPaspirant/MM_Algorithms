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

	TestStats TestStats::currentTestStats;

	string to_string_max_precision(double value)
	{
		std::stringstream buffer;
		//buffer.precision(maxPrecision);
		buffer.precision(std::numeric_limits<double>::digits10);
		buffer << std::fixed << value;
		return buffer.str();
	}

	bool verifySettlement(const vector<bool>& settleFlags, const vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates, double actualSettledAmount)
	{
		//rmt
		double novVal = 0.0;
		vector< vector<double> > updatedBalance{ initialBalance };

		double amountSettled = 0.0;
		for (int i = 0; i < trades.size(); ++i)
		{
			if (!settleFlags[i]) continue;

			amountSettled += (
				trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]
				+ trades[i].sellVol_ * exchangeRates[static_cast<int>(trades[i].sellCurr_)]
				);

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
				if (updatedBalance[memberIndex][currencyIndex] + zero < -spl[memberIndex][currencyIndex])
					return false;

				double currentBalanceInDollars = updatedBalance[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (novTemp < -zero)
				return false;

			if (asplTemp + zero < -aspl[memberIndex])
				return false;
		}

		return fabs(actualSettledAmount - amountSettled) < zero;
	}

	void testFxSettlement(vector<TestCase>& testCases)
	{
		vector<TestStats> stats;
		int columnWidth[11] = { 12, 12, 30, 10, 15, 12, 15, 18, 20, 15, 15 };
		cout << "\n"
			<< setw(columnWidth[0]) << std::left << "FilePrefix"
			<< setw(columnWidth[1]) << std::left << "TestResult"
			<< setw(columnWidth[2]) << std::left << "AlgoType"
			<< setw(columnWidth[3]) << std::right << "numMembers"
			<< setw(columnWidth[4]) << std::right << "numCurrencies"
			<< setw(columnWidth[5]) << std::right << "numTrades"
			<< setw(columnWidth[6]) << std::right << "TradesSettled"
			<< setw(columnWidth[7]) << std::right << "AmountSettled"
			<< setw(columnWidth[8]) << std::right << "Duration"
			<< setw(columnWidth[9]) << std::right << "FunCalls"
			<< setw(columnWidth[10]) << std::right << "HeapSize";

		for (int testCaseIndex = 0; testCaseIndex < testCases.size(); ++testCaseIndex)
		{
			cout << "\n";
			double actualSettledAmount = 0.0;
			for (int i = 0; i < static_cast<int>(AlgoType::totalAlgos); ++i)
			{
				if (getAlgoInfo(AlgoType(i)).maxTrades < testCases[testCaseIndex].trades_.size())
					continue;

				TestStats::currentTestStats.numberOfFunctionCalls = 0;
				TestStats::currentTestStats.sizeOfHeap = 0;
				vector<bool> settleFlags(testCases[testCaseIndex].trades_.size(), false);
				std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
				switch (AlgoType(i))
				{
				case AlgoType::naive_v1:
					actualSettledAmount = doSettlement_naive_v1(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::naive_v2:
					actualSettledAmount = doSettlement_naive_v2(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::naive_v3:
					actualSettledAmount = doSettlement_naive_v3(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v1:
					actualSettledAmount = doSettlement_branch_and_bound_v1(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v2:
					actualSettledAmount = doSettlement_branch_and_bound_v2(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v3a:
					actualSettledAmount = doSettlement_branch_and_bound_v3a(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v3b:
					actualSettledAmount = doSettlement_branch_and_bound_v3b(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v4a:
					actualSettledAmount = doSettlement_branch_and_bound_v4a(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v4b:
					actualSettledAmount = doSettlement_branch_and_bound_v4b(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v5a:
					actualSettledAmount = doSettlement_branch_and_bound_v5a(
						settleFlags,
						testCases[testCaseIndex].trades_,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v5b:
					actualSettledAmount = doSettlement_branch_and_bound_v5b(
						settleFlags,
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
					settleFlags,
					testCases[testCaseIndex].trades_,
					testCases[testCaseIndex].spl_,
					testCases[testCaseIndex].aspl_,
					testCases[testCaseIndex].initialBalance_,
					testCases[testCaseIndex].exchangeRates_,
					actualSettledAmount
				);
				vector<int> actualSettledTradeIds;
				for (int i = 0; i < settleFlags.size(); ++i)
					if (settleFlags[i] == true)
						actualSettledTradeIds.push_back(testCases[testCaseIndex].trades_[i].id_);

				std::sort(actualSettledTradeIds.begin(), actualSettledTradeIds.end());

				std::stringstream buffer;
				buffer.imbue(std::locale(""));
				buffer << std::fixed << duration;

				TestStats::currentTestStats.fileNamePrefix = testCases[testCaseIndex].fileNamePrefix_;
				TestStats::currentTestStats.testCaseResult = verified;
				TestStats::currentTestStats.algoType = getAlgoInfo(AlgoType(i)).algoName;
				TestStats::currentTestStats.numMembers = testCases[testCaseIndex].aspl_.size();
				TestStats::currentTestStats.numCurrencies = testCases[testCaseIndex].spl_[0].size();
				TestStats::currentTestStats.numTrades = testCases[testCaseIndex].trades_.size();
				TestStats::currentTestStats.tradesSettled = actualSettledTradeIds.size();
				TestStats::currentTestStats.amountSettled = actualSettledAmount;
				TestStats::currentTestStats.durationStr = buffer.str() + " ns";

				//printOrWrite(testStats);
				std::cout.imbue(std::locale(""));
				std::cout << std::fixed;
				std::cout << std::setprecision(4);
				cout << "\n"
					<< setw(columnWidth[0]) << std::left << TestStats::currentTestStats.fileNamePrefix
					<< setw(columnWidth[1]) << std::left << (TestStats::currentTestStats.testCaseResult ? "SUCCESS" : "FAILED")
					<< setw(columnWidth[2]) << std::left << TestStats::currentTestStats.algoType
					<< setw(columnWidth[3]) << std::right << TestStats::currentTestStats.numMembers
					<< setw(columnWidth[4]) << std::right << TestStats::currentTestStats.numCurrencies
					<< setw(columnWidth[5]) << std::right << TestStats::currentTestStats.numTrades
					<< setw(columnWidth[6]) << std::right << TestStats::currentTestStats.tradesSettled
					<< setw(columnWidth[7]) << std::right << TestStats::currentTestStats.amountSettled
					<< setw(columnWidth[8]) << std::right << TestStats::currentTestStats.durationStr
					<< setw(columnWidth[9]) << std::right << TestStats::currentTestStats.numberOfFunctionCalls
					<< setw(columnWidth[10]) << std::right << TestStats::currentTestStats.sizeOfHeap;

				
				stats.push_back(std::move(TestStats::currentTestStats));

				MM_EXPECT_TRUE(verified == true, verified);

				if (testCases[testCaseIndex].resultsAvailable && !globalFlagOverwriteResults)
				{
					MM_EXPECT_TRUE(fabs(actualSettledAmount - testCases[testCaseIndex].settledAmount_) < zero, actualSettledAmount, testCases[testCaseIndex].settledAmount_);
					MM_EXPECT_TRUE(actualSettledTradeIds == testCases[testCaseIndex].settledTradeIds_, actualSettledTradeIds, testCases[testCaseIndex].settledTradeIds_);
				}
				else if (!testCases[testCaseIndex].fileNamePrefix_.empty()) //File prefix is empty only for hardcoded tests
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

						//Results are written to file. Now update the cached results
						testCases[testCaseIndex].resultsAvailable = true;
						testCases[testCaseIndex].settledAmount_ = actualSettledAmount;
						testCases[testCaseIndex].settledTradeIds_ = actualSettledTradeIds;
					}
				}
			}
		}
	}

	MM_DECLARE_FLAG(Others_FxSettlement);

	MM_UNIT_TEST(Others_FxSettlement_sanity, Others_FxSettlement)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);

		//createTestCases();
		testFxSettlement(getTestCases());
	}
}