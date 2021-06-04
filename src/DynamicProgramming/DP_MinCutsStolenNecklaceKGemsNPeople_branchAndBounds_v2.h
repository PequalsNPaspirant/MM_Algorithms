#pragma once

#include <vector>
#include <unordered_map>

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeopleCommon.h"

namespace mm {

	namespace MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v2 {

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<GemType>& necklace,
			const vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results);
	}
}