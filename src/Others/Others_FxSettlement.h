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

#pragma once

#include <sstream> // for std::stringstream
#include <unordered_map>
#include <limits>

#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	constexpr double compile_time_pow(double base, int power)
	{
		if (power < 0)
			return 1.0 / compile_time_pow(base, -power);
		else if (power == 0)
			return 1.0;
		else if (power == 1)
			return base;
		
		if (power % 2 == 1)
			return base * compile_time_pow(base * base, power / 2);
		else
			return compile_time_pow(base * base, power / 2);
	}

	extern string testDataPath;
	extern string tradesFileName;
	extern string initialBalancesFileName;
	extern string splFileName;
	extern string asplFileName;
	extern string fxRatesFileName;
	extern string resultsFileName;
	extern string settledAmountTag;
	extern string settledTradeIdsTag;
	constexpr int maxPrecision = 10;
	//FloatingPointComparator zero(0.000001);
	constexpr const double zero = compile_time_pow(10, -maxPrecision + 1);
	constexpr bool GlobalFlagWriteTestCasesToFile = false;
	constexpr bool globalFlagOverwriteResults = false;

	struct TestCaseInput
	{
		int numMembers;
		int numCurrencies;
		int numTrades;
		int numTestCases;
	};

	string to_string_max_precision(double value);


	class FloatingPointComparator
	{
	public:
		//implicit constructor
		FloatingPointComparator(double val)
			: val_(val)
		{
		}

		friend bool operator==(const FloatingPointComparator& lhs, const FloatingPointComparator& rhs)
		{
			return true;
		}

	private:
		double val_;
	};

	enum class Currency
	{
		USD = 0,
		EUR,
		CAD,
		AUD,
		GBP,
		NZD,
		JPY,
		SGD,
		CHF,
		HUF,
		MXN,
		HKD,
		SEK,
		KRW,
		NOK,
		INR,
		CSK,
		BRL,
		CLP,
		CNY,
		CZK,
		DKK,
		ILS,
		TWD,
		RUB,
		PKR,
		maxCurrency
	};

	struct Trade
	{
		int id_;
		int partyId_;
		int cPartyId_;
		Currency buyCurr_;
		Currency sellCurr_;
		double buyVol_;
		double sellVol_;

		friend ostream& operator<<(ostream& o, const Trade& trade)
		{
			o << "\n{ "
				<< trade.id_ << ", "
				<< trade.partyId_ << ", "
				<< trade.cPartyId_ << ", "
				<< "Currency(" << static_cast<int>(trade.buyCurr_) << "), "
				<< "Currency(" << static_cast<int>(trade.sellCurr_) << "), "
				<< trade.buyVol_ << ", "
				<< trade.sellVol_
				<< " }";

			return o;
		}
	};

	struct TestCase
	{
		string fileNamePrefix_;
		vector<Trade> trades_;
		vector<double> spl_;
		vector<double> aspl_;
		vector<double> initialBalance_;
		vector<double> exchangeRates_;
		mutable double settledAmount_;
		mutable vector<int> settledTradeIds_;
		mutable bool resultsAvailable;
	};

	struct TestStats
	{
		string fileNamePrefix;
		bool testCaseResult;
		string algoType;
		int numMembers;
		int numCurrencies;
		int numTrades;
		int tradesSettled;
		double amountSettled;
		double percentageAmtSettled_;
		string durationStr;
		unsigned long long numberOfFunctionCalls;
		unsigned long long sizeOfHeap;

		static TestStats currentTestStats;
	};

	void createTestCases();
	vector<TestCase>& getTestCases();

	//Settlement Algos:
	enum class AlgoType
	{
		greedy = 0,
		sequential,
		naive_v1,
		naive_v2,
		naive_v3,
		branch_and_bound_v1,
		branch_and_bound_v2,
		branch_and_bound_v3a,
		branch_and_bound_v3b,
		branch_and_bound_v4a,
		branch_and_bound_v4b,
		branch_and_bound_v5a,
		branch_and_bound_v5b,
		branch_and_bound_v6a,
		branch_and_bound_v7a,
		branch_and_bound_v8a,
		branch_and_bound_v9a,
		branch_and_bound_v10a,
		branch_and_bound_v11a,
		branch_and_bound_v12a,
		branch_and_bound_v13a,
		branch_and_bound_v13b,

		totalAlgos
	};

	struct AlgoInfo
	{
		string algoName;
		int maxTrades;
	};

	template<typename T = void>
	AlgoInfo getAlgoInfo(AlgoType type)
	{ 
		static unordered_map<AlgoType, AlgoInfo> AlgoTypeInfo{
		{ AlgoType::greedy,							{ "greedy", numeric_limits<int>::max() } },
		{ AlgoType::sequential ,					{ "sequential", numeric_limits<int>::max() } },
		{ AlgoType::naive_v1 ,						{ "naive_v1", 20} },
		{ AlgoType::naive_v2 ,						{ "naive_v2", 20} },
		{ AlgoType::naive_v3 ,						{ "naive_v3", 22 } },
		{ AlgoType::branch_and_bound_v1,			{ "branch_and_bound_v1", 20	} },
		{ AlgoType::branch_and_bound_v2,			{ "branch_and_bound_v2", 20 } },
		{ AlgoType::branch_and_bound_v3a,			{ "branch_and_bound_v3a", 20 } },
		{ AlgoType::branch_and_bound_v3b,			{ "branch_and_bound_v3b", 20 } },
		{ AlgoType::branch_and_bound_v4a,			{ "branch_and_bound_v4a", 20 } },
		{ AlgoType::branch_and_bound_v4b,			{ "branch_and_bound_v4b", 20 } },
		{ AlgoType::branch_and_bound_v5a,			{ "branch_and_bound_v5a", 23 } }, 
		{ AlgoType::branch_and_bound_v5b,			{ "branch_and_bound_v5b", 22 } },
		{ AlgoType::branch_and_bound_v6a,			{ "branch_and_bound_v6a", 23 } },
		{ AlgoType::branch_and_bound_v7a,			{ "branch_and_bound_v7a", 27 } },
		{ AlgoType::branch_and_bound_v8a,			{ "branch_and_bound_v8a", 27 } },
		{ AlgoType::branch_and_bound_v9a,			{ "branch_and_bound_v9a", 27 } },
		{ AlgoType::branch_and_bound_v10a,			{ "branch_and_bound_v10a", 28 } },
		{ AlgoType::branch_and_bound_v11a,			{ "branch_and_bound_v11a", 28 } },
		{ AlgoType::branch_and_bound_v12a,			{ "branch_and_bound_v12a", 50 } },
		{ AlgoType::branch_and_bound_v13a,			{ "branch_and_bound_v13a", 50 } },
		{ AlgoType::branch_and_bound_v13b,			{ "branch_and_bound_v13b", 50 } }

		}; 
		return AlgoTypeInfo[type];
	}

	double doSettlement_greedy_v1(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, vector<double>& currentBalance, const vector<double>& exchangeRates);
	double doSettlement_sequential(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_naive_v1(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_naive_v2(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_naive_v3(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v1(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v2(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v3a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v3b(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v4a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v4b(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v5a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v5b(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, const vector<double>& initialBalance, const vector<double>& exchangeRates);

}