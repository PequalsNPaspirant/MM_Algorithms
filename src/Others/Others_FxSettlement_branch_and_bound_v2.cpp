#include <vector>
#include <queue>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	bool verifySettlement_v2(
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

	struct fxDecisionTreeNode_v2
	{
		int level;
		double upperbound;

		vector< vector<double> > currentBalance;
		double settledAmount;
		vector<bool> settleFlags;
		bool rmtPassed;

		inline void calculateAndSetUpperBound(
			//const vector< vector<double> >& cumulativeBalance,
			const double cumulativeSettledAmount
			//const vector< vector<double> >& spl, 
			//const vector<double>& aspl, 
			//const vector<double>& exchangeRates
		)
		{
			//double invalidAmountInDollars = 0.0;
			//vector< vector<double> > totalBalance(currentBalance);
			//for (int memberIndex = 0; memberIndex < totalBalance.size(); ++memberIndex)
			//{
			//	for (int currencyIndex = 0; currencyIndex < totalBalance.size(); ++currencyIndex)
			//	{
			//		totalBalance[memberIndex][currencyIndex] += cumulativeBalance[memberIndex][currencyIndex];
			//		//if (totalBalance[memberIndex][currencyIndex] < -spl[memberIndex][currencyIndex])
			//		//	invalidAmountInDollars += ((-totalBalance[memberIndex][currencyIndex] - spl[memberIndex][currencyIndex]) * exchangeRates[currencyIndex]);
			//	}
			//}

			upperbound = settledAmount + cumulativeSettledAmount;
			//upperbound -= (invalidAmountInDollars / 2.0);
			//if (upperbound < 0.0)
			//	upperbound = 0.0;
		}
	};
	struct fxDecisionTreeNodeCompare_v2
	{
		bool operator()(const fxDecisionTreeNode_v2* lhs, const fxDecisionTreeNode_v2* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};
	//std::priority_queue<fxDecisionTreeNode_v2, vector<fxDecisionTreeNode_v2>, fxDecisionTreeNodeCompare_v2> fxMaxHeap_v2;

	double doSettlement_branch_and_bound_v2(
		vector<bool>& settleFlagsOut,
		vector<Trade>& trades,
		const vector< vector<double> >& spl,
		const vector<double>& aspl,
		const vector< vector<double> >& initialBalance,
		const vector<double>& exchangeRates)
	{
		//calculate value to weight ratio
		//for (int i = 0; i < values.size(); ++i)
		//	valueToWeightsRatio[i] = values[i] / weights[i];

		////initialize the indices
		//for (int i = 0; i < values.size(); ++i)
		//	sortedIndices[i] = i;

		std::sort(trades.begin(), trades.end(),
			[&exchangeRates](const Trade& lhs, const Trade& rhs) -> bool {
			return lhs.buyVol_ * exchangeRates[static_cast<int>(lhs.buyCurr_)] > rhs.buyVol_ * exchangeRates[static_cast<int>(rhs.buyCurr_)];
		});

		int initialHeapCapacity = 10000;
		vector<vector<fxDecisionTreeNode_v2>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v2>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v2*, fxDecisionTreeNodeCompare_v2> fxMaxHeap_v2(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v2.addToData(&heapObjectsGrowingPool[0][i]);

		//vector< vector< vector<double> > > cumulativeBalance(trades.size(), vector< vector<double> >(spl.size(), vector<double>(spl[0].size(), 0.0)));
		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
			{
				//cumulativeBalance[i] = cumulativeBalance[i + 1];
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];
			}

			//cumulativeBalance[i][trades[i].partyId_][static_cast<int>(trades[i].buyCurr_)] += trades[i].buyVol_;
			//cumulativeBalance[i][trades[i].partyId_][static_cast<int>(trades[i].sellCurr_)] -= trades[i].sellVol_;
			//cumulativeBalance[i][trades[i].cPartyId_][static_cast<int>(trades[i].buyCurr_)] -= trades[i].buyVol_;
			//cumulativeBalance[i][trades[i].cPartyId_][static_cast<int>(trades[i].sellCurr_)] += trades[i].sellVol_;

			cumulativeSettledAmount[i] += (trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]);
		}

		fxDecisionTreeNode_v2* pObj = fxMaxHeap_v2.getNextAvailableElement();
		fxDecisionTreeNode_v2& current = *pObj;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		//vector<bool> settleFlags;
		current.rmtPassed = false;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		//current.calculateAndSetUpperBound(cumulativeBalance[0], cumulativeSettledAmount[0], spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(cumulativeSettledAmount[0]);
		fxMaxHeap_v2.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v2.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v2.size())
				sizeOfHeap = fxMaxHeap_v2.size();

			//const fxMaxHeap_v2.top();
			fxDecisionTreeNode_v2* pCurrent = fxMaxHeap_v2.top();
			fxDecisionTreeNode_v2& current = *pCurrent;
			//fxMaxHeap_v2.pop();

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			//include this item
			//fxDecisionTreeNode_v2 include = current;
			if (fxMaxHeap_v2.capacity() == fxMaxHeap_v2.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v2>(initialHeapCapacity));
				fxMaxHeap_v2.reserve(fxMaxHeap_v2.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v2.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v2* pInclude = fxMaxHeap_v2.getNextAvailableElement();
			fxDecisionTreeNode_v2& include = *pInclude;
			include = current;
			// Update current balance
			include.currentBalance[trades[include.level].partyId_][static_cast<int>(trades[include.level].buyCurr_)] += trades[include.level].buyVol_;
			include.currentBalance[trades[include.level].partyId_][static_cast<int>(trades[include.level].sellCurr_)] -= trades[include.level].sellVol_;
			include.currentBalance[trades[include.level].cPartyId_][static_cast<int>(trades[include.level].buyCurr_)] -= trades[include.level].buyVol_;
			include.currentBalance[trades[include.level].cPartyId_][static_cast<int>(trades[include.level].sellCurr_)] += trades[include.level].sellVol_;
			
			// Do rmt tests and update maxValue is rmt tests are passed
			include.rmtPassed = verifySettlement_v2(include.currentBalance, spl, aspl, exchangeRates);
			include.settledAmount += (trades[include.level].buyVol_ * exchangeRates[static_cast<int>(trades[include.level].buyCurr_)]);
			include.settleFlags[include.level] = true;
			if (include.rmtPassed && maxValue < include.settledAmount)
			{
				maxValue = include.settledAmount;
				settleFlagsOut = include.settleFlags;
			}

			//exclude current
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if ((current.upperbound + zero) < maxValue)
				fxMaxHeap_v2.pop();

			if (current.level == trades.size() - 1)
			{
				fxMaxHeap_v2.pop();
				continue;
			}

			//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
			include.calculateAndSetUpperBound(cumulativeSettledAmount[include.level + 1]);

			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if ((include.upperbound + zero) >= maxValue)
				fxMaxHeap_v2.push(pInclude);
		}

		while (!fxMaxHeap_v2.empty())
			fxMaxHeap_v2.pop();

		return maxValue;
	}

}