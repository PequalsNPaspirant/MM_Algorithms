#pragma once

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
	};

}