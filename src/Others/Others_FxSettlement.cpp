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

	bool rmt(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		double novVal = 0.0;
		vector< vector<double> > updatedBalance(initialBalance.begin(), initialBalance.end());

		for (int i = 0; i < trades.size(); ++i)
		{
			if (!trades[i].isSettled_) continue;

			int partyIndex = trades[i].partyId_ - 1;
			int cPartyIndex = trades[i].cPartyId_ - 1;

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

			if (asplTemp < -aspl[memberIndex])
				return false;
		}

		return true;
	}

	bool verifySettlement(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		return rmt(trades, spl, aspl, initialBalance, exchangeRates);
	}

	double doSettlement_v1(int currentTradeIndex, vector<double>& currentNOV, vector< vector<double> >& currentSP, vector<double>& currentASP,
		vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
	{
		if (currentTradeIndex == trades.size())
			return 0.0;

		//Do not settle this trade
		double exclude = doSettlement_v1(currentTradeIndex + 1, currentNOV, currentSP, currentASP,
			trades, spl, aspl, initialBalance, exchangeRates);

		//Try to settle this trade
		double include = 0.0;

		int partyIndex = trades[currentTradeIndex].partyId_ - 1;
		int cPartyIndex = trades[currentTradeIndex].cPartyId_ - 1;
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
			include = currentSettledAmount + doSettlement_v1(currentTradeIndex + 1, currentNOV, currentSP, currentASP,
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

	double doSettlement_v1(vector<Trade>& trades, const vector< vector<double> >& spl, const vector<double>& aspl, const vector< vector<double> >& initialBalance, const vector<double>& exchangeRates)
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
		return doSettlement_v1(0, currentNOV, currentSP, currentASP, trades, spl, aspl, initialBalance, exchangeRates);
	}

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

	void testFxSettlement(vector<TestCase>& testCases)
	{
		for (int testCaseIndex = 0; testCaseIndex < testCases.size(); ++testCaseIndex)
		{
			double actualSettledAmount = doSettlement_v1(testCases[testCaseIndex].trades_, testCases[testCaseIndex].spl_, testCases[testCaseIndex].aspl_, testCases[testCaseIndex].initialBalance_, testCases[testCaseIndex].exchangeRates_);
			bool verified = verifySettlement(testCases[testCaseIndex].trades_, testCases[testCaseIndex].spl_, testCases[testCaseIndex].aspl_, testCases[testCaseIndex].initialBalance_, testCases[testCaseIndex].exchangeRates_);
			vector<int> actualSettledTradeIds;
			for (int i = 0; i < testCases[testCaseIndex].trades_.size(); ++i)
				if (testCases[testCaseIndex].trades_[i].isSettled_)
					actualSettledTradeIds.push_back(testCases[testCaseIndex].trades_[i].id_);

			MM_EXPECT_TRUE(verified == true, verified);
			MM_EXPECT_TRUE(fabs(actualSettledAmount - testCases[testCaseIndex].settledAmount_) < zero, actualSettledAmount, testCases[testCaseIndex].settledAmount_);
			MM_EXPECT_TRUE(actualSettledTradeIds == testCases[testCaseIndex].settledTradeIds_, actualSettledTradeIds, testCases[testCaseIndex].settledTradeIds_);
		}
	}

	vector<TestCase>& getTestCases();

	MM_DECLARE_FLAG(Others_FxSettlement);

	MM_UNIT_TEST(Others_FxSettlement_sanity, Others_FxSettlement)
	{
		testFxSettlement(getTestCases());
	}

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
			vector<int> settledTradeIds{
				1
			};
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
			vector<int> settledTradeIds{
				1
			};
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
			vector<int> settledTradeIds{
				1
			};
			testCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(exchangeRates), settledAmount, std::move(settledTradeIds) });
		}

		return testCases;
	}
}