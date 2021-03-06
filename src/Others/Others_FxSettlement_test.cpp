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
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
#include <fstream>
#include <limits> // for std::numeric_limits<double>::digits10
#include <chrono>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap
#include "Others/Others_FxSettlement.h"
#include "Utils/Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

#include "Others/Others_FxSettlement_branch_and_bound_v6a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v7a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v8a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v9a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v10a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v11a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v12a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v13a.h"
#include "Others/Others_FxSettlement_branch_and_bound_v13b.h"
#include "Others/Others_FxSettlement_branch_and_bound_v14a.h"

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

	bool verifySettlement(
		const vector<bool>& settleFlags, 
		const vector<Trade>& trades, 
		const vector<double>& spl, 
		const vector<double>& aspl, 
		const vector<double>& initialBalance, 
		const vector<double>& exchangeRates, 
		double actualSettledAmount,
		bool verifySettledAmount)
	{
		//rmt
		size_t numMembers = aspl.size();
		size_t numCurrencies = spl.size() / aspl.size();
		double novVal = 0.0;
		vector<double> updatedBalance{ initialBalance };

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

			updatedBalance[numMembers * partyIndex  + static_cast<int>(trades[i].buyCurr_)] += trades[i].buyVol_;
			updatedBalance[numMembers * partyIndex  + static_cast<int>(trades[i].sellCurr_)] -= trades[i].sellVol_;
			updatedBalance[numMembers * cPartyIndex + static_cast<int>(trades[i].buyCurr_)] -= trades[i].buyVol_;
			updatedBalance[numMembers * cPartyIndex + static_cast<int>(trades[i].sellCurr_)] += trades[i].sellVol_;
		}

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

		return !verifySettledAmount || fabs(actualSettledAmount - amountSettled) < zero;
	}

	void testFxSettlement(const vector<TestCase>& testCases)
	{
		vector<TestStats> stats;
		int columnWidth[12] = { 12, 12, 30, 10, 15, 12, 15, 18, 15, 20, 15, 15 };
		cout << "\n"
			<< setw(columnWidth[0]) << std::left << "FilePrefix"
			<< setw(columnWidth[1]) << std::left << "TestResult"
			<< setw(columnWidth[2]) << std::left << "AlgoType"
			<< setw(columnWidth[3]) << std::right << "numMembers"
			<< setw(columnWidth[4]) << std::right << "numCurrencies"
			<< setw(columnWidth[5]) << std::right << "numTrades"
			<< setw(columnWidth[6]) << std::right << "TradesSettled"
			<< setw(columnWidth[7]) << std::right << "AmountSettled"
			<< setw(columnWidth[8]) << std::right << "% Amt Settled"
			<< setw(columnWidth[9]) << std::right << "Duration"
			<< setw(columnWidth[10]) << std::right << "FunCalls"
			<< setw(columnWidth[11]) << std::right << "HeapSize";

		for (int testCaseIndex = 0; testCaseIndex < testCases.size(); ++testCaseIndex)
		{
			//Only for debugging purpose
			//testCaseIndex = 2;

			cout << "\n";
			double actualSettledAmount = 0.0;

			double totalTradesAmount = 0.0;
			for (int tradeIndex = 0; tradeIndex < testCases[testCaseIndex].trades_.size(); ++tradeIndex)
			{
				const Trade& currentTrade = testCases[testCaseIndex].trades_[tradeIndex];
				double currentTradeValue = currentTrade.buyVol_ * testCases[testCaseIndex].exchangeRates_[static_cast<int>(currentTrade.buyCurr_)]
					+ currentTrade.sellVol_ * testCases[testCaseIndex].exchangeRates_[static_cast<int>(currentTrade.sellCurr_)];
				totalTradesAmount += currentTradeValue;
			}

			for (int i = 0; i < static_cast<int>(AlgoType::totalAlgos); ++i)
			{
				//Only for debugging purpose
				//i = static_cast<int>(AlgoType::branch_and_bound_v13a);

				if (getAlgoInfo(AlgoType(i)).maxTrades < testCases[testCaseIndex].trades_.size())
					continue;

				TestStats::currentTestStats.numberOfFunctionCalls = 0;
				TestStats::currentTestStats.sizeOfHeap = 0;
				vector<bool> settleFlags(testCases[testCaseIndex].trades_.size(), false);
				vector<Trade> trades{ testCases[testCaseIndex].trades_ };
				vector<double> currentBalances{ testCases[testCaseIndex].initialBalance_ };
				size_t numMembers = testCases[testCaseIndex].aspl_.size();
				size_t numCurrencies = testCases[testCaseIndex].spl_.size() / numMembers;
				vector< vector<double> > cumulativeBalance(trades.size(), vector<double>(numMembers * numCurrencies, 0.0));
				vector<double> cumulativeSettledAmount(trades.size(), 0.0);

				std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

				switch (AlgoType(i))
				{
				case AlgoType::greedy:
				{
					actualSettledAmount = doSettlement_greedy_v1(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::sequential:
					actualSettledAmount = doSettlement_sequential(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::naive_v1:
					actualSettledAmount = doSettlement_naive_v1(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::naive_v2:
					actualSettledAmount = doSettlement_naive_v2(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::naive_v3:
					actualSettledAmount = doSettlement_naive_v3(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				case AlgoType::branch_and_bound_v1:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v1(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v2:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v2(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v3a:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v3a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v3b:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v3b(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v4a:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v4a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v4b:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v4b(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v5a:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v5a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v5b:
				{
					actualSettledAmount = doSettlement_branch_and_bound_v5b(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_
					);
					break;
				}
				case AlgoType::branch_and_bound_v6a:
				{
					int initialHeapCapacity = 1'000'000;
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v6a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v6a>(initialHeapCapacity, fxDecisionTreeNode_v6a{ testCases[testCaseIndex].initialBalance_.size() }));
					MM_Heap<fxDecisionTreeNode_v6a*, fxDecisionTreeNodeCompare_v6a> fxMaxHeap_v6a(initialHeapCapacity);
					//initialize the pool indices
					for (int i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v6a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v6a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v6a,
						heapObjectsGrowingPool,
						initialHeapCapacity
					);
					break;
				}
				case AlgoType::branch_and_bound_v7a:
				{
					int initialHeapCapacity = 1'000'000;
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v7a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v7a>(initialHeapCapacity, fxDecisionTreeNode_v7a{ testCases[testCaseIndex].initialBalance_.size() }));
					MM_Heap<fxDecisionTreeNode_v7a*, fxDecisionTreeNodeCompare_v7a> fxMaxHeap_v7a(initialHeapCapacity);
					//initialize the pool indices
					for (int i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v7a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v7a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						testCases[testCaseIndex].initialBalance_,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v7a,
						heapObjectsGrowingPool,
						initialHeapCapacity
					);
					break;
				}
				case AlgoType::branch_and_bound_v8a:
				{
					int initialHeapCapacity = 1'000'000;
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v8a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v8a>(initialHeapCapacity, fxDecisionTreeNode_v8a{ testCases[testCaseIndex].initialBalance_.size() }));
					MM_Heap<fxDecisionTreeNode_v8a*, fxDecisionTreeNodeCompare_v8a> fxMaxHeap_v8a(initialHeapCapacity);
					//initialize the pool indices
					for (int i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v8a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v8a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v8a,
						heapObjectsGrowingPool,
						initialHeapCapacity
					);
					break;
				}
				case AlgoType::branch_and_bound_v9a:
				{
					int initialHeapCapacity = 1'000'000;
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v9a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v9a>(initialHeapCapacity, fxDecisionTreeNode_v9a{ testCases[testCaseIndex].initialBalance_.size() }));
					MM_Heap<fxDecisionTreeNode_v9a*, fxDecisionTreeNodeCompare_v9a> fxMaxHeap_v9a(initialHeapCapacity);
					//initialize the pool indices
					for (int i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v9a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v9a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v9a,
						heapObjectsGrowingPool,
						initialHeapCapacity
					);
					break;
				}
				case AlgoType::branch_and_bound_v10a:
				{
					//int initialHeapCapacity = 1'000'000;
					size_t initialHeapCapacity = 2 * trades.size();
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v10a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v10a>(initialHeapCapacity, fxDecisionTreeNode_v10a{ testCases[testCaseIndex].initialBalance_.size() }));
					MM_Heap<fxDecisionTreeNode_v10a*, fxDecisionTreeNodeCompare_v10a> fxMaxHeap_v10a(initialHeapCapacity);
					//initialize the pool indices
					for (size_t i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v10a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v10a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v10a,
						heapObjectsGrowingPool,
						initialHeapCapacity
					);
					break;
				}
				case AlgoType::branch_and_bound_v11a:
				{
					//int initialHeapCapacity = 1'000'000;
					size_t initialHeapCapacity = 2 * trades.size();
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v11a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v11a>(initialHeapCapacity, fxDecisionTreeNode_v11a{ testCases[testCaseIndex].initialBalance_.size(), trades.size() }));
					MM_Heap<fxDecisionTreeNode_v11a*, fxDecisionTreeNodeCompare_v11a> fxMaxHeap_v11a(initialHeapCapacity);
					//initialize the pool indices
					for (size_t i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v11a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v11a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v11a,
						heapObjectsGrowingPool,
						initialHeapCapacity,
						cumulativeBalance,
						cumulativeSettledAmount
					);
					break;
				}
				case AlgoType::branch_and_bound_v12a:
				{
					//int initialHeapCapacity = 1'000'000;
					size_t initialHeapCapacity = 2 * trades.size();
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v12a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v12a>(initialHeapCapacity, fxDecisionTreeNode_v12a{ testCases[testCaseIndex].initialBalance_.size(), trades.size() }));
					MM_Heap<fxDecisionTreeNode_v12a*, fxDecisionTreeNodeCompare_v12a> fxMaxHeap_v12a(initialHeapCapacity);
					//initialize the pool indices
					for (size_t i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v12a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v12a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v12a,
						heapObjectsGrowingPool,
						initialHeapCapacity,
						cumulativeBalance,
						cumulativeSettledAmount
					);
					break;
				}
				case AlgoType::branch_and_bound_v13a:
				{
					//int initialHeapCapacity = 1'000'000;
					size_t initialHeapCapacity = 2 * trades.size();
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v13a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v13a>(initialHeapCapacity, fxDecisionTreeNode_v13a{ testCases[testCaseIndex].initialBalance_.size(), trades.size() }));
					MM_Heap<fxDecisionTreeNode_v13a*, fxDecisionTreeNodeCompare_v13a> fxMaxHeap_v13a(initialHeapCapacity);
					//initialize the pool indices
					for (size_t i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v13a.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v13a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v13a,
						heapObjectsGrowingPool,
						initialHeapCapacity,
						cumulativeBalance,
						cumulativeSettledAmount
					);
					break;
				}
				case AlgoType::branch_and_bound_v13b:
				{
					//int initialHeapCapacity = 1'000'000;
					size_t initialHeapCapacity = 2 * trades.size();
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<fxDecisionTreeNode_v13b>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v13b>(initialHeapCapacity, fxDecisionTreeNode_v13b{ testCases[testCaseIndex].initialBalance_.size(), trades.size() }));
					MM_Heap<fxDecisionTreeNode_v13b*, fxDecisionTreeNodeCompare_v13b> fxMaxHeap_v13b(initialHeapCapacity);
					//initialize the pool indices
					for (size_t i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap_v13b.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v13b(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap_v13b,
						heapObjectsGrowingPool,
						initialHeapCapacity,
						cumulativeBalance,
						cumulativeSettledAmount
					);
					break;
				}
				case AlgoType::branch_and_bound_v14a:
				{
					using decisionTreeNodeType = fxDecisionTreeNode_v14a;
					using decisionTreeNodeCompare = fxDecisionTreeNodeCompare_v14a;
					//int initialHeapCapacity = 1'000'000;
					size_t initialHeapCapacity = 2 * trades.size();
					//Total memory = 1,000,000 * object size = 1,000,000 * (24 + (8 * members * currencies)) bytes = (24 + (8 * members * currencies)) MB
					vector<vector<decisionTreeNodeType>> heapObjectsGrowingPool(1, vector<decisionTreeNodeType>(initialHeapCapacity, decisionTreeNodeType{ testCases[testCaseIndex].initialBalance_.size(), trades.size() }));
					MM_Heap<decisionTreeNodeType*, decisionTreeNodeCompare> fxMaxHeap(initialHeapCapacity);
					//initialize the pool indices
					for (size_t i = 0; i < initialHeapCapacity; ++i)
						fxMaxHeap.addToData(&heapObjectsGrowingPool[0][i]);

					start = std::chrono::high_resolution_clock::now();
					actualSettledAmount = doSettlement_branch_and_bound_v14a(
						settleFlags,
						trades,
						testCases[testCaseIndex].spl_,
						testCases[testCaseIndex].aspl_,
						currentBalances,
						testCases[testCaseIndex].exchangeRates_,
						fxMaxHeap,
						heapObjectsGrowingPool,
						initialHeapCapacity,
						cumulativeBalance,
						cumulativeSettledAmount
					);
					break;
				}

				default:
					assert(false); // , "Algo type '" + to_string(i) + "' is not suported");
				}
				std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
				unsigned long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

				bool approximateAlgo = false;
				if (AlgoType(i) == AlgoType::greedy || AlgoType(i) == AlgoType::sequential)
					approximateAlgo = true;

				bool verifySettledAmount = !approximateAlgo;
				bool verified = verifySettlement(
					settleFlags,
					trades,
					testCases[testCaseIndex].spl_,
					testCases[testCaseIndex].aspl_,
					testCases[testCaseIndex].initialBalance_,
					testCases[testCaseIndex].exchangeRates_,
					actualSettledAmount,
					verifySettledAmount
				);
				vector<int> actualSettledTradeIds;
				for (int i = 0; i < settleFlags.size(); ++i)
					if (settleFlags[i] == true)
						actualSettledTradeIds.push_back(trades[i].id_);

				std::sort(actualSettledTradeIds.begin(), actualSettledTradeIds.end());

				std::stringstream buffer;
				buffer.imbue(std::locale(""));
				buffer << std::fixed << duration;

				TestStats::currentTestStats.fileNamePrefix = testCases[testCaseIndex].fileNamePrefix_;
				TestStats::currentTestStats.testCaseResult = verified;
				TestStats::currentTestStats.algoType = getAlgoInfo(AlgoType(i)).algoName;
				TestStats::currentTestStats.numMembers = testCases[testCaseIndex].aspl_.size();
				TestStats::currentTestStats.numCurrencies = testCases[testCaseIndex].spl_.size() / testCases[testCaseIndex].aspl_.size();
				TestStats::currentTestStats.numTrades = testCases[testCaseIndex].trades_.size();
				TestStats::currentTestStats.tradesSettled = actualSettledTradeIds.size();
				TestStats::currentTestStats.amountSettled = actualSettledAmount;
				TestStats::currentTestStats.percentageAmtSettled_ = actualSettledAmount / totalTradesAmount * 100.0;
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
					<< setw(columnWidth[8]) << std::right << TestStats::currentTestStats.percentageAmtSettled_
					<< setw(columnWidth[9]) << std::right << TestStats::currentTestStats.durationStr
					<< setw(columnWidth[10]) << std::right << TestStats::currentTestStats.numberOfFunctionCalls
					<< setw(columnWidth[11]) << std::right << TestStats::currentTestStats.sizeOfHeap;

				
				stats.push_back(std::move(TestStats::currentTestStats));

				MM_EXPECT_TRUE(verified == true, verified);

				if (!approximateAlgo)
				{
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
	}

	MM_DECLARE_FLAG(Others_FxSettlement);

	MM_UNIT_TEST(Others_FxSettlement_sanity, Others_FxSettlement)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);
		MM_SET_PAUSE_ON_ERROR(false);

		//createTestCases();
		testFxSettlement(getTestCases());
	}
}