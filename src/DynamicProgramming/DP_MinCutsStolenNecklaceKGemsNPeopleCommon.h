#pragma once

#include "Utils/Utils_PrintUtils.h"

namespace mm {

	using GemType = int;
	using Count = int;

	struct MinCutsStolenNecklaceResults
	{
		std::vector<int> owners;
		int minCuts;

		void calculateMinCuts()
		{
			minCuts = 0;
			for (size_t i = 1; i < owners.size(); ++i)
				if (owners[i - 1] != owners[i])
					++minCuts;
		}

		friend ostream& operator<<(ostream& o, const MinCutsStolenNecklaceResults& results)
		{
			o << "owners: " << results.owners;
			o << " minCuts: " << results.minCuts;

			return o;
		}

		friend bool operator==(const MinCutsStolenNecklaceResults& lhs, const MinCutsStolenNecklaceResults& rhs)
		{
			return lhs.owners == rhs.owners && lhs.minCuts == rhs.minCuts;
		}
	};

}