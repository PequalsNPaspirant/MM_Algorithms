#include <vector>
#include <queue>
#include <bitset>
#include <numeric>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h" //For class MM_Heap

namespace mm {

	bool verifySettlement_v4(
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

	struct fxDecisionTreeNode_v4
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
	struct fxDecisionTreeNodeCompare_v4
	{
		bool operator()(const fxDecisionTreeNode_v4* lhs, const fxDecisionTreeNode_v4* rhs) const
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	double doSettlement_branch_and_bound_v4(
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
		vector<vector<fxDecisionTreeNode_v4>> heapObjectsGrowingPool(1, vector<fxDecisionTreeNode_v4>(initialHeapCapacity));
		MM_Heap<fxDecisionTreeNode_v4*, fxDecisionTreeNodeCompare_v4> fxMaxHeap_v4(initialHeapCapacity);
		//initialize the pool indices
		for (int i = 0; i < initialHeapCapacity; ++i)
			fxMaxHeap_v4.addToData(&heapObjectsGrowingPool[0][i]);

		vector<double> cumulativeSettledAmount(trades.size(), 0.0);
		for (int i = trades.size() - 1; i >= 0; --i)
		{
			if (i < trades.size() - 1)
				cumulativeSettledAmount[i] = cumulativeSettledAmount[i + 1];

			cumulativeSettledAmount[i] += (trades[i].buyVol_ * exchangeRates[static_cast<int>(trades[i].buyCurr_)]);
		}

		fxDecisionTreeNode_v4* pObj = fxMaxHeap_v4.getNextAvailableElement();
		fxDecisionTreeNode_v4& current = *pObj;
		current.level = -1;
		current.currentBalance = initialBalance;
		current.settledAmount = 0.0;
		current.upperbound = 0.0;
		current.settleFlags.resize(trades.size(), false);
		
		std::vector<int> memberIndices(aspl.size());
		std::iota(memberIndices.begin(), memberIndices.end(), 0); // Fill with 0, 1, ..., aspl.size() - 1
		current.rmtPassed.flip();
		verifySettlement_v4(current.rmtPassed, current.currentBalance, memberIndices, spl, aspl, exchangeRates);
		current.calculateAndSetUpperBound(cumulativeSettledAmount[0]);
		fxMaxHeap_v4.push(pObj);

		double maxValue = 0.0;
		int numberOfFunctionCalls = 0;
		int sizeOfHeap = 0;

		while (!fxMaxHeap_v4.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < fxMaxHeap_v4.size())
				sizeOfHeap = fxMaxHeap_v4.size();

			fxDecisionTreeNode_v4* pCurrent = fxMaxHeap_v4.top();
			fxDecisionTreeNode_v4& current = *pCurrent;

			if ((current.upperbound + zero) < maxValue)
				break;

			current.level += 1;

			//include this item
			if (fxMaxHeap_v4.capacity() == fxMaxHeap_v4.size()) //need to grow pool
			{
				heapObjectsGrowingPool.push_back(vector<fxDecisionTreeNode_v4>(initialHeapCapacity));
				fxMaxHeap_v4.reserve(fxMaxHeap_v4.capacity() + initialHeapCapacity);
				int lastIndex = heapObjectsGrowingPool.size() - 1;
				for (int i = 0; i < initialHeapCapacity; ++i)
					fxMaxHeap_v4.addToData(&heapObjectsGrowingPool[lastIndex][i]);
			}

			fxDecisionTreeNode_v4* pInclude = fxMaxHeap_v4.getNextAvailableElement();
			fxDecisionTreeNode_v4& include = *pInclude;
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
			

			verifySettlement_v4(include.rmtPassed, include.currentBalance, { partyId, cPartyId }, spl, aspl, exchangeRates);
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
			//	fxMaxHeap_v4.pop();

			if (current.level < trades.size() - 1)
			{
				//include.calculateAndSetUpperBound(cumulativeBalance[include.level + 1], cumulativeSettledAmount[include.level + 1], spl, aspl, exchangeRates);
				include.calculateAndSetUpperBound(cumulativeSettledAmount[include.level + 1]);

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				if ((include.upperbound + zero) >= maxValue)
					fxMaxHeap_v4.push(pInclude);
			}
			else
				fxMaxHeap_v4.pop();
		}

		fxMaxHeap_v4.clear();
		TestStats::currentTestStats.numberOfFunctionCalls = numberOfFunctionCalls;
		TestStats::currentTestStats.sizeOfHeap = sizeOfHeap;

		return maxValue;
	}

}