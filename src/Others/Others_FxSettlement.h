#pragma once

namespace mm {

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

	//FloatingPointComparator zero(0.000001);
	constexpr const double zero = 0.000001;

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
		CUR1,
		CUR2,
		CUR3,
		CUR4,
		CUR5,
		CUR6,
		CUR7,
		CUR8,
		CUR9,
		CUR10,
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
	};

	struct TestCase
	{
		vector<Trade> trades_;
		vector< vector<double> > spl_;
		vector<double> aspl_;
		vector< vector<double> > initialBalance_;
		vector<double> exchangeRates_;
		double settledAmount_;
		vector<int> settledTradeIds_;
	};

	vector<TestCase>& getTestCases();
}