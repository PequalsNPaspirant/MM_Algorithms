#include <vector>
#include <queue>
#include <bitset>
#include <numeric>
#include <cassert>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	/*
	Whats new - v3b
	Instead of running rmt tests every time we condsider a trade as settled, rmt test is done only at the bottom level of solution tree
	Whats new - v4b
	Use improved upper bound
	*/

	bool verifySettlement_v4b(
		bitset<128>& rmtPassed,
		const vector< vector<double> >& updatedBalance,		
		const vector< int >& memberIndices,
		const vector< vector<double> >& spl,
		const vector<double>& aspl, 
		 const vector<double>& exchangeRates)
	{
		// TODO: Avoid checking SPL for all currencies. 
		// It can be checked only for two currencies which are changes because of settlement of the current trade if we remember SPL results for all currencies.
		//rmt
		//int numMembers = updatedBalance.size();
		bool rmtSuccessful = true;
		int numCurrencies = updatedBalance[0].size();
		for (int i = 0; i < memberIndices.size(); ++i)
		{
			int memberIndex = memberIndices[i];
			double asplTemp = 0.0;
			double novTemp = 0.0;
			bool splPassed = true;
			for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
			{
				if (updatedBalance[memberIndex][currencyIndex] + zero < -spl[memberIndex][currencyIndex])
				{
					//rmtPassed[memberIndex] = false;
					splPassed = false;
					break;
				}

				double currentBalanceInDollars = updatedBalance[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (!splPassed || (asplTemp + zero < -aspl[memberIndex]) || (novTemp < -zero))
			{
				rmtPassed[memberIndex] = false;
				rmtSuccessful = false;
			}
			else
				rmtPassed[memberIndex] = true;
		}
		
		return rmtSuccessful;
	}

	struct fxDecisionTreeNode_v4b
	{
		int level;
		double upperbound;

		vector< vector<double> > currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bitset<128> rmtPassed{ 0 };

		inline void calculateAndSetUpperBound(
			const vector< vector<double> >& cumulativeBalance, 
			const double cumulativeSettledAmount,
			const vector< vector<double> >& spl,
			const vector<double>& aspl,
			const vector<double>& exchangeRates
		)
		{
			double excessSettledAmountInDollars = 0.0;
			vector< vector<double> > totalBalance(currentBalance);
			int numMembers = spl.size();
			int numCurrencies = spl[0].size();
			for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
			{
				for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
				{
					totalBalance[memberIndex][currencyIndex] += cumulativeBalance[memberIndex][currencyIndex];
					if (totalBalance[memberIndex][currencyIndex] + zero < -spl[memberIndex][currencyIndex])
						excessSettledAmountInDollars += ((-totalBalance[memberIndex][currencyIndex] - spl[memberIndex][currencyIndex]) * exchangeRates[currencyIndex]);
				}
			}

			if (cumulativeSettledAmount > excessSettledAmountInDollars)
			{
				int x = 0;
				++x;
			}
			upperbound = settledAmount + cumulativeSettledAmount - excessSettledAmountInDollars / 2.0;
		}
	};
	struct fxDecisionTreeNodeCompare_v4b
	{
		bool operator()(const fxDecisionTreeNode_v4b* lhs, const fxDecisionTreeNode_v4b* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	double doSettlement_branch_and_bound_v4b(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector< vector<double> >& spl,
		const vector<double>& aspl,
		const vector< vector<double> >& initialBalance,
		const vector<double>& exchangeRates)
	{
		std::sort(trades.begin(), trades.end(),
			[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
			return (lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] + lhs.sellVol_ * exchangeRates[static_cast<int>(lhs.sellCurr_)])
			> (rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)] + rhs.sellVol_ * exchangeRates[static_cast<int>(rhs.sellCurr_)]);
		});

		int initialHeapCapacity = 10000;
		vector<vector<fxDecisionTreeNode_v4b>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v4b>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v4b*, fxDecisionTreeNodeCompare_v4b> fxMaxHeap_v4b(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v4b.addToData(&heapObjectsGrowingPool[0][i]);

		vector< vector< vector<double> > > cumulativeBalance(trades.size(), vector< vector<double> >(spl.size(), vector<double>(spl[0].size(), 0.0)));
		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
			{
				cumulativeBalance[i] = cumulativeBalance[i + 1];
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];
			}

			cumulativeBalance[i][trades[i].partyId_][static_cast<int>(trades[i].buyCurr_)] += trades[i].buyVol_;
			cumulativeBalance[i][trades[i].partyId_][static_cast<int>(trades[i].sellCurr_)] -= trades[i].sellVol_;
			cumulativeBalance[i][trades[i].cPartyId_][static_cast<int>(trades[i].buyCurr_)] -= trades[i].buyVol_;
			cumulativeBalance[i][trades[i].cPartyId_][static_cast<int>(trades[i].sellCurr_)] += trades[i].sellVol_;

			cumulativeSettledAmount[i] += (
				trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]
				+ trades[i].sellVol_ * exchangeRates[static_cast<int>(trades[i].sellCurr_)]);
		}

		fxDecisionTreeNode_v4b* pObj = fxMaxHeap_v4b.getNextAvailableElement();
		fxDecisionTreeNode_v4b& current = *pObj;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		
		std::vector<int> memberIndices(aspl.size());
		std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
		current.rmtPassed.flip();
		verifySettlement_v4b(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(cumulativeBalance[0], cumulativeSettledAmount[0], spl, aspl, exchangeRates);
		fxMaxHeap_v4b.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v4b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v4b.size())
				sizeOfHeap = fxMaxHeap_v4b.size();

			fxDecisionTreeNode_v4b* pCurrent = fxMaxHeap_v4b.top();
			fxDecisionTreeNode_v4b& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			if (current.level == trades.size())
			{
				if (maxValue < current.settledAmount)
				{
					std::vector<int> memberIndices(aspl.size());
					std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
					verifySettlement_v4b(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
					if (current.rmtPassed.all())
					{
						maxValue = current.settledAmount;
						settleFlagsOut = current.settleFlags;

						if ((current.upperbound + zero) < maxValue)
							break;
					}
				}

				fxMaxHeap_v4b.pop();
				continue;
			}

			//include this item
			if (fxMaxHeap_v4b.capacity() == fxMaxHeap_v4b.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v4b>(initialHeapCapacity));
				fxMaxHeap_v4b.reserve(fxMaxHeap_v4b.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v4b.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v4b* pInclude = fxMaxHeap_v4b.getNextAvailableElement();
			fxDecisionTreeNode_v4b& include = *pInclude;
			include = current;
			// Update current balance
			int partyId = trades[include.level].partyId_;
			int cPartyId = trades[include.level].cPartyId_;
			int buyCurrId = static_cast<int>(trades[include.level].buyCurr_);
			int sellCurrId = static_cast<int>(trades[include.level].sellCurr_);
			include.currentBalance[partyId][buyCurrId] += trades[include.level].buyVol_;
			include.currentBalance[partyId][sellCurrId] -= trades[include.level].sellVol_;
			include.currentBalance[cPartyId][buyCurrId] -= trades[include.level].buyVol_;
			include.currentBalance[cPartyId][sellCurrId] += trades[include.level].sellVol_;
			

			include.settledAmount += (
				trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]
				+ trades[include.level].sellVol_ * exchangeRates[static_cast<int>(trades[include.level].sellCurr_)]);
			include.settleFlags[include.level] = true;

			//if (current.level == trades.size() - 1)
			//{
			//	verifySettlement_v4b(include.rmtPassed, include.currentBalance, { partyId, cPartyId }, spl, aspl, exchangeRates);
			//	if (include.rmtPassed.all() && maxValue < include.settledAmount)
			//	{
			//		maxValue = include.settledAmount;
			//		settleFlagsOut = include.settleFlags;

			//		if ((current.upperbound + zero) < maxValue)
			//			break;
			//	}
			//}

			//exclude current
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			//if ((current.upperbound + zero) < maxValue)
			//	fxMaxHeap_v4b.pop();

			if (current.level < trades.size() - 1)
			{
				//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
				include.calculateAndSetUpperBound(
					cumulativeBalance[include.level + 1], 
					cumulativeSettledAmount[include.level + 1], 
					spl, 
					aspl, 
					exchangeRates);

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				
			}
			else
			{
				include.upperbound = include.settledAmount;
			}

			if ((include.upperbound + zero) >= maxValue)
				fxMaxHeap_v4b.push(pInclude);
		}

		fxMaxHeap_v4b.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}