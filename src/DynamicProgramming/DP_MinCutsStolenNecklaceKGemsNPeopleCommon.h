#pragma once

#include "Utils/Utils_PrintUtils.h"

namespace mm {

constexpr const int MAX_SOLUTIONS = 10;

	using GemType = char;
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

		friend bool operator<(const MinCutsStolenNecklaceResults& lhs, const MinCutsStolenNecklaceResults& rhs)
		{
			if (lhs.minCuts != rhs.minCuts)
				return lhs.minCuts < rhs.minCuts;

			if (lhs.owners.size() != rhs.owners.size())
				return lhs.owners.size() < rhs.owners.size();

			for (int i = 0; i < lhs.owners.size(); ++i)
			{
				if (lhs.owners[i] != rhs.owners[i])
					return lhs.owners[i] < rhs.owners[i];
			}

			return false;
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