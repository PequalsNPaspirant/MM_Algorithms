#pragma once

#include <sstream> // for std::stringstream
#include <unordered_map>
#include <limits>

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
	constexpr bool globalFlagOverwriteResults = true;

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
		vector< vector<double> > spl_;
		vector<double> aspl_;
		vector< vector<double> > initialBalance_;
		vector<double> exchangeRates_;
		double settledAmount_;
		vector<int> settledTradeIds_;
		bool resultsAvailable;
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
		naive_v1 = 0,
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
		{ AlgoType::naive_v1 ,						{ "naive_v1", 20} },
		{ AlgoType::naive_v2 ,						{ "naive_v2", 20} },
		{ AlgoType::naive_v3 ,						{ "naive_v3", 30 } },
		{ AlgoType::branch_and_bound_v1,			{ "branch_and_bound_v1", 20	} },
		{ AlgoType::branch_and_bound_v2,			{ "branch_and_bound_v2", 20 } },
		{ AlgoType::branch_and_bound_v3a,			{ "branch_and_bound_v3a", 20 } },
		{ AlgoType::branch_and_bound_v3b,			{ "branch_and_bound_v3b", 20 } },
		{ AlgoType::branch_and_bound_v4a,			{ "branch_and_bound_v4a", 20 } },
		{ AlgoType::branch_and_bound_v4b,			{ "branch_and_bound_v4b", 20 } },
		{ AlgoType::branch_and_bound_v5a,			{ "branch_and_bound_v5a", numeric_limits<int>::max() } },
		{ AlgoType::branch_and_bound_v5b,			{ "branch_and_bound_v5b", numeric_limits<int>::max() } }
		}; 
		return AlgoTypeInfo[type];
	}

	double doSettlement_naive_v1(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_naive_v2(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_naive_v3(vector<bool>& settleFlagsOut, const vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v1(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v2(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v3a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v3b(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v4a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v4b(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v5a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
	double doSettlement_branch_and_bound_v5b(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);

}