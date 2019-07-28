#pragma once

#include <sstream> // for std::stringstream

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
	constexpr bool GlobalFlagCreateTestCases = false;
	constexpr bool overrideTestResults = false;
	constexpr const int numTestCases = 1; // 10;
	constexpr const int maxCurrencies = 5; // 10;

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

	enum class AlgoType
	{
		naive_v1 = 0,

		totalAlgos
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
		bool isSettled_ = false;

		friend ostream& operator<<(ostream& o, const Trade& trade)
		{
			o << "\n{ "
				<< trade.id_ << ", "
				<< trade.partyId_ << ", "
				<< trade.cPartyId_ << ", "
				<< "Currency(" << static_cast<int>(trade.buyCurr_) << "), "
				<< "Currency(" << static_cast<int>(trade.sellCurr_) << "), "
				<< trade.buyVol_ << ", "
				<< trade.sellVol_ << ", "
				<< std::boolalpha << trade.isSettled_ << " }";

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
	};

	struct TestStats
	{
		int testCaseIndex;
		string algoType;
		int numTrades;
		int numMembers;
		int numCurrencies;
		string durationStr;
		bool testCaseResult;
	};

	void createTestCases();
	vector<TestCase>& getTestCases();

	//Settlement Algos:
	double doSettlement_naive_v1(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates);
}