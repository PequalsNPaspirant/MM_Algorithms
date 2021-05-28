#pragma once

#include <vector>

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeopleCommon.h"

namespace mm {

	namespace MinCutsStolenNecklaceKGemsNPeople_recursive_v3 {

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<int>& necklace,
			const vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results);
	}
}