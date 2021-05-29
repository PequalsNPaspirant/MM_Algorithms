#pragma once

#include <vector>

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeopleCommon.h"

namespace mm {

	namespace MinCutsStolenNecklaceKGemsNPeople_recursive_v4 {

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<GemType>& necklace,
			const vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results);
	}
}