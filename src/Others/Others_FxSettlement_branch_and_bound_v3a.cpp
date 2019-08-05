#include <vector>
#include <queue>
#include <bitset>
#include <numeric>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	/*
	Whats new:
	Instead of running emt tests for all members every time, we are keeping a cached rmt results for every member
	and redoing rmt tests for only two members involved in the trade when it is settled
	*/

	bool verifySettlement_v3a(
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

	struct fxDecisionTreeNode_v3a
	{
		int level;
		double upperbound;

		vector< vector<double> > currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bitset<128> rmtPassed{ 0 };

		inline void calculateAndSetUpperBound(const double cumulativeSettledAmount)
		{
			upperbound = settledAmount + cumulativeSettledAmount;
		}
	};
	struct fxDecisionTreeNodeCompare_v3a
	{
		bool operator()(const fxDecisionTreeNode_v3a* lhs, const fxDecisionTreeNode_v3a* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	double doSettlement_branch_and_bound_v3a(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector< vector<double> >& spl,
		const vector<double>& aspl,
		const vector< vector<double> >& initialBalance,
		const vector<double>& exchangeRates)
	{
		std::sort(trades.begin(), trades.end(),
			[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
			return lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] > rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)];
		});

		int initialHeapCapacity = 10000;
		vector<vector<fxDecisionTreeNode_v3a>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v3a>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v3a*, fxDecisionTreeNodeCompare_v3a> fxMaxHeap_v3a(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v3a.addToData(&heapObjectsGrowingPool[0][i]);

		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];

			cumulativeSettledAmount[i] += (trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]);
		}

		fxDecisionTreeNode_v3a* pObj = fxMaxHeap_v3a.getNextAvailableElement();
		fxDecisionTreeNode_v3a& current = *pObj;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		
		std::vector<int> memberIndices(aspl.size());
		std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
		current.rmtPassed.flip();
		verifySettlement_v3a(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(cumulativeSettledAmount[0]);
		fxMaxHeap_v3a.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v3a.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v3a.size())
				sizeOfHeap = fxMaxHeap_v3a.size();

			fxDecisionTreeNode_v3a* pCurrent = fxMaxHeap_v3a.top();
			fxDecisionTreeNode_v3a& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			//include this item
			if (fxMaxHeap_v3a.capacity() == fxMaxHeap_v3a.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v3a>(initialHeapCapacity));
				fxMaxHeap_v3a.reserve(fxMaxHeap_v3a.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v3a.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v3a* pInclude = fxMaxHeap_v3a.getNextAvailableElement();
			fxDecisionTreeNode_v3a& include = *pInclude;
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
			
			verifySettlement_v3a(include.rmtPassed, include.currentBalance, { partyId, cPartyId }, spl, aspl, exchangeRates);
			include.settledAmount += (trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]);
			include.settleFlags[include.level] = true;
			if (include.rmtPassed.all() && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				settleFlagsOut = include.settleFlags;

				if ((current.upperbound + zero) < maxValue)
					break;
			}

			//exclude current
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			//if ((current.upperbound + zero) < maxValue)
			//	fxMaxHeap_v3a.pop();

			if (current.level < trades.size() - 1)
			{
				//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
				include.calculateAndSetUpperBound(cumulativeSettledAmount[include.level + 1]);

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if ((include.upperbound + zero) >= maxValue)
					fxMaxHeap_v3a.push(pInclude);
			}
			else
				fxMaxHeap_v3a.pop();
		}

		fxMaxHeap_v3a.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}