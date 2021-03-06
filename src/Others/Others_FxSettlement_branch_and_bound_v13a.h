//=======================================================================================================//
//   Copyright (c) 2018 Maruti Mhetre                                                                    //
//   All rights reserved.                                                                                //
//=======================================================================================================//
//   Redistribution and use of this software in source and binary forms, with or without modification,   //
//   are permitted for personal, educational or non-commercial purposes provided that the following      //
//   conditions are met:                                                                                 //
//   1. Redistributions of source code must retain the above copyright notice, this list of conditions   //
//      and the following disclaimer.                                                                    //
//   2. Redistributions in binary form must reproduce the above copyright notice, this list of           //
//      conditions and the following disclaimer in the documentation and/or other materials provided     //
//      with the distribution.                                                                           //
//   3. Neither the name of the copyright holder nor the names of its contributors may be used to        //
//      endorse or promote products derived from this software without specific prior written            //
//      permission.                                                                                      //
//=======================================================================================================//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR      //
//   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND    //
//   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR          //
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL   //
//   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   //
//   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  //
//   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT   //
//   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     //
//=======================================================================================================//

#pragma once

#include <vector>
#include <queue>
#include <bitset>
#include <numeric>
#include <cassert>
using namespace std;

#include "Others/Others_FxSettlement.h"

namespace mm {

	struct fxDecisionTreeNode_v13a
	{
		//Total size of object in bytes = 4 + 8 + (8 * members * currencies) + 8 + 4 * (members * currencies) / 32 + 4 = 24 + (8 * members * currencies) + 4 * (members * currencies) / 32 ~ 24 + (8 * members * currencies)
		int level;
		double upperbound;
		bool upperboundRmtPassed = false;

		vector<double> currentBalance;
		double settledAmount;
		//double actualSettledAmount;
		vector<bool> settleFlags;
		bitset<128> rmtPassed{ 0 }; //4 bytes (4 * 32 = 128)

		fxDecisionTreeNode_v13a(size_t totalBalances, size_t totalTrades)
			: currentBalance(totalBalances, 0.0),
			settleFlags(totalTrades, false)
		{

		}

		fxDecisionTreeNode_v13a(const fxDecisionTreeNode_v13a& rhs)
			: level{ rhs.level },
			upperbound{ rhs.upperbound },
			upperboundRmtPassed{ rhs.upperboundRmtPassed },
			currentBalance{ rhs.currentBalance },
			settledAmount{ rhs.settledAmount },
			//actualSettledAmount{ rhs.actualSettledAmount },
			settleFlags{ rhs.settleFlags },
			rmtPassed{ rhs.rmtPassed }
		{
		}

		fxDecisionTreeNode_v13a(fxDecisionTreeNode_v13a&& rhs) = delete;
		//	: level{ std::move(rhs.level) },
		//	upperbound{ std::move(rhs.upperbound) },
		//	upperboundRmtPassed{ std::move(rhs.upperboundRmtPassed) },
		//	currentBalance{ std::move(rhs.currentBalance) },
		//	settledAmount{ std::move(rhs.settledAmount) },
		//	settleFlags{ std::move(rhs.settleFlags) },
		//	rmtPassed{ std::move(rhs.rmtPassed) }
		//{
		//}

		fxDecisionTreeNode_v13a& operator=(const fxDecisionTreeNode_v13a& rhs)
		{
			level = rhs.level;
			upperbound = rhs.upperbound;
			upperboundRmtPassed = rhs.upperboundRmtPassed;

			if (currentBalance.size() != rhs.currentBalance.size())
			{
				//Ideally we should not come here
				int* p = nullptr;
				*p = 10;
			}
			for(int i = 0; i < currentBalance.size(); ++i)
				currentBalance[i] = rhs.currentBalance[i];
			
			settledAmount = rhs.settledAmount;
			//actualSettledAmount = rhs.actualSettledAmount;

			if (settleFlags.size() != rhs.settleFlags.size())
			{
				//Ideally we should not come here
				int* p = nullptr;
				*p = 10;
			}
			for (int i = 0; i < settleFlags.size(); ++i)
				settleFlags[i] = rhs.settleFlags[i];

			rmtPassed = rhs.rmtPassed;

			return *this;
		}

		void calculateAndSetUpperBound(
			const vector<double>& cumulativeBalance,
			const double cumulativeSettledAmount,
			const vector<double>& spl,
			const vector<double>& aspl,
			const vector<double>& exchangeRates
		);
	};
	struct fxDecisionTreeNodeCompare_v13a
	{
		bool operator()(const fxDecisionTreeNode_v13a* lhs, const fxDecisionTreeNode_v13a* rhs) const
		{
			if (lhs->upperbound < rhs->upperbound)
				return true;
			else if (lhs->upperbound > rhs->upperbound)
				return false;
			else
				return lhs->settledAmount < rhs->settledAmount;
		}
	};

	double doSettlement_branch_and_bound_v13a(vector<bool>& settleFlagsOut, vector<Trade>& trades, const vector<double>& spl, const vector<double>& aspl, vector<double>& initialBalance, const vector<double>& exchangeRates, MM_Heap<fxDecisionTreeNode_v13a*, fxDecisionTreeNodeCompare_v13a>& fxMaxHeap_v13a, vector<vector<fxDecisionTreeNode_v13a>>& heapObjectsGrowingPool, size_t initialHeapCapacity, vector< vector<double> >& cumulativeBalance, vector<double>& cumulativeSettledAmount);

}