#include <vector>
#include <queue>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	bool verifySettlement_v3(
		const vector< vector<double> >& updatedBalance,
		const vector< vector<double> >& spl, 
		const vector<double>& aspl, 
		 const vector<double>& exchangeRates)
	{
		//rmt
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
				if (updatedBalance[memberIndex][currencyIndex] + zero < -spl[memberIndex][currencyIndex])
					return false;

				double currentBalanceInDollars = updatedBalance[memberIndex][currencyIndex] * exchangeRates[currencyIndex];
				novTemp += currentBalanceInDollars;
				if (currentBalanceInDollars < -zero)
					asplTemp += currentBalanceInDollars;
			}

			if (novTemp < -zero)
				return false;

			if (asplTemp + zero < -aspl[memberIndex])
				return false;
		}

		return true;
	}

	struct fxDecisionTreeNode_v3
	{
		int level;
		double upperbound;

		vector< vector<double> > currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bool rmtPassed;

		inline void calculateAndSetUpperBound(const double cumulativeSettledAmount)
		{
			upperbound = settledAmount + cumulativeSettledAmount;
		}
	};
	struct fxDecisionTreeNodeCompare_v3
	{
		bool operator()(const fxDecisionTreeNode_v3* lhs, const fxDecisionTreeNode_v3* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	double doSettlement_branch_and_bound_v3(
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
		vector<vector<fxDecisionTreeNode_v3>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v3>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v3*, fxDecisionTreeNodeCompare_v3> fxMaxHeap_v3(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v3.addToData(&heapObjectsGrowingPool[0][i]);

		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];

			cumulativeSettledAmount[i] += (trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]);
		}

		fxDecisionTreeNode_v3* pObj = fxMaxHeap_v3.getNextAvailableElement();
		fxDecisionTreeNode_v3& current = *pObj;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		current.rmtPassed = false;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		current.calculateAndSetUpperBound(cumulativeSettledAmount[0]);
		fxMaxHeap_v3.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v3.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v3.size())
				sizeOfHeap = fxMaxHeap_v3.size();

			fxDecisionTreeNode_v3* pCurrent = fxMaxHeap_v3.top();
			fxDecisionTreeNode_v3& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			//include this item
			if (fxMaxHeap_v3.capacity() == fxMaxHeap_v3.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v3>(initialHeapCapacity));
				fxMaxHeap_v3.reserve(fxMaxHeap_v3.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v3.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v3* pInclude = fxMaxHeap_v3.getNextAvailableElement();
			fxDecisionTreeNode_v3& include = *pInclude;
			include = current;
			// Update current balance
			include.currentBalance[trades[include.level].partyId_][static_cast<int>(trades[include.level].buyCurr_)] += trades[include.level].buyVol_;
			include.currentBalance[trades[include.level].partyId_][static_cast<int>(trades[include.level].sellCurr_)] -= trades[include.level].sellVol_;
			include.currentBalance[trades[include.level].cPartyId_][static_cast<int>(trades[include.level].buyCurr_)] -= trades[include.level].buyVol_;
			include.currentBalance[trades[include.level].cPartyId_][static_cast<int>(trades[include.level].sellCurr_)] += trades[include.level].sellVol_;
			
			// Do rmt tests and update maxValue if rmt tests are passed
			include.rmtPassed = verifySettlement_v3(include.currentBalance, spl, aspl, exchangeRates);
			include.settledAmount += (trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]);
			include.settleFlags[include.level] = true;
			if (include.rmtPassed && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				settleFlagsOut = include.settleFlags;

				if ((current.upperbound + zero) < maxValue)
					break;
			}

			//exclude current
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			//if ((current.upperbound + zero) < maxValue)
			//	fxMaxHeap_v3.pop();

			if (current.level < trades.size() - 1)
			{
				//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
				include.calculateAndSetUpperBound(cumulativeSettledAmount[include.level + 1]);

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if ((include.upperbound + zero) >= maxValue)
					fxMaxHeap_v3.push(pInclude);
			}
			else
				fxMaxHeap_v3.pop();
		}

		//while (!fxMaxHeap_v3.empty())
		//	fxMaxHeap_v3.pop();
		fxMaxHeap_v3.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}