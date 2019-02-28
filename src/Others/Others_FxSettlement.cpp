#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	class trade
	{
	public:
		int id_;
		int party_;
		int cParty_;
		int buyCurr_;
		int sellCurr_;
		double buyVol_;
		double sellVol_;
	};

	class member
	{
	public:
		int id_;
		double aspl_;
		vector<double> spl_;
		vector<double> balance_;
	};

	enum class currency
	{
		USD = 1,
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
		CUR10
	};

	void doSettlement(vector<trade>& trades, const vector<member>& members, const vector<double>& exchangeRates)
	{

	}

	void testFxSettlement()
	{

	}

	MM_DECLARE_FLAG(Others_FxSettlement);

	MM_UNIT_TEST(Others_FxSettlement_sanity, Others_FxSettlement)
	{
		testFxSettlement();
	}
}