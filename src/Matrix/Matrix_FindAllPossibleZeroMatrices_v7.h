#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n)

	Space Complexity: O(n)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v7
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector<int> currentRowMultiplier((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0);
			int totalMatrices = 0;

			for (int currRow = 0; currRow < matrix.size(); ++currRow)
			{
				int numMatrices = 0;
				for (int currColumn = 0; currColumn < matrix[currRow].size(); ++currColumn)
				{

					if (matrix[currRow][currColumn] == 0)
					{
						++currentRowMultiplier[currColumn];
						++numMatrices;
						totalMatrices += currentRowMultiplier[currColumn] * numMatrices;
					}
					else
					{
						currentRowMultiplier[currColumn] = 0;
						numMatrices = 0;
					}

				}
			}

			return totalMatrices;

		}


	}

}