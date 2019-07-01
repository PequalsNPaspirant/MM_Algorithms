#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "Utils/Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	vector<TestCase>& getTestCases()
	{
		static vector<TestCase> testCases;

		{//1 trade, 2 members, 2 currencies
			vector<Trade> trades{
			{ 1, 1, 2, Currency(0), Currency(1), 500.0, 440.0, false }
			};
			vector< vector<double> > spl{
			{ 500.0, 400.0 },
			{ 300.0, 250.0 }
			};
			vector<double> aspl{
				2000.0,
				2000.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 440.0 },
			{ 500.0, 0.0 },
			};
			vector<double> exchangeRates{
				1.0, //USD = 1
				1.14 //EUR
				 //CAD
				 //AUD
				 //GBP
				 //NZD
				 //JPY
				 //SGD
				 //CHF
				 //HUF
				 //MXN
				 //HKD
				 //SEK
				 //KRW
				 //NOK
				 //INR
				 //CUR1
				 //CUR2
				 //CUR3
				 //CUR4
				 //CUR5
				 //CUR6
				 //CUR7
				 //CUR8
				 //CUR9
				 //CUR10
				 //maxCurrency
			};
			double settledAmount = 500.0;
			vector<int> settledTradeIds{ 1 };
			testCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(exchangeRates), settledAmount, std::move(settledTradeIds) });
		}

		{//1 trade, 2 members, 2 currencies
			vector<Trade> trades{
				{ 1, 1, 2, Currency(0), Currency(1), 500.0, 440.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 0.0 },
			{ 0.0, 0.0 }
			};
			vector<double> aspl{
				0.0,
				0.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 440.0 },
			{ 500.0, 0.0 },
			};
			vector<double> exchangeRates{
				1.0, //USD = 1
				1.14 //EUR
			};
			double settledAmount = 500.0;
			vector<int> settledTradeIds{ 1 };
			testCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(exchangeRates), settledAmount, std::move(settledTradeIds) });
		}

		{//1 trade, 2 members, 2 currencies
			vector<Trade> trades{
				{ 1, 1, 2, Currency(0), Currency(1), 500.0, 440.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 440.0 },
			{ 500.0, 0.0 }
			};
			vector<double> aspl{
				500.0,
				500.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 10.0 },
			{ 10.0, 0.0 },
			};
			vector<double> exchangeRates{
				1.0, //USD = 1
				1.14 //EUR
			};
			double settledAmount = 500.0;
			vector<int> settledTradeIds{ 1 };
			testCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(exchangeRates), settledAmount, std::move(settledTradeIds) });
		}

		{//2 trades, 2 members, 2 currencies
			vector<Trade> trades{
			{ 1, 1, 2, Currency(0), Currency(1), 500.0, 440.0, false },
			{ 2, 1, 2, Currency(1), Currency(0), 440.0, 500.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 0.0 },
			{ 0.0, 0.0 }
			};
			vector<double> aspl{
				0.0,
				0.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 0.0 },
			{ 0.0, 0.0 },
			};
			vector<double> exchangeRates{
				1.0, //USD = 1
				500.0/440.0 //EUR
			};
			double settledAmount = 1000.0;
			vector<int> settledTradeIds{ 1, 2 };
			testCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(exchangeRates), settledAmount, std::move(settledTradeIds) });
		}

		{//3 trades, 3 members, 3 currencies
			vector<Trade> trades{
			{ 1, 1, 2, Currency(0), Currency(1), 100.0, 88.0, false },
			{ 2, 2, 3, Currency(1), Currency(2), 88.0, 131.0, false },
			{ 3, 3, 1, Currency(2), Currency(0), 131.0, 100.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 0.0, 131.0 },
			{ 100.0, 0.0, 0.0 },
			{ 0.0, 88.0, 0.0 }
			};
			vector<double> aspl{
				131.0,
				100.0,
				100.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 88.0, 0.0 },
			{ 0.0, 0.0, 131.0 },
			{ 100.0, 0.0, 0.0 }
			};
			vector<double> exchangeRates{
				1.0, //USD = 1
				100.0 / 88.0, //EUR
				100.0 / 131.0  //CAD
			};
			double settledAmount = 300.0;
			vector<int> settledTradeIds{ 1, 2, 3 };
			testCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(exchangeRates), settledAmount, std::move(settledTradeIds) });
		}

		return testCases;
	}
}