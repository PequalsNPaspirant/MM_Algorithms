#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n * m)

	Space Complexity: O(m*n)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v6
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector< std::vector<int> > height1MatricesEndingAt(matrix.size(), std::vector<int>((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0));

			int totalMatrices = 0;
			for (int currRow = 0; currRow < matrix.size(); ++currRow)
			{
				int numMatricesOfHeight1 = 0;
				for (int currColumn = 0; currColumn < matrix[currRow].size(); ++currColumn)
				{
					//Find total possible matrices ending at matrix[currRow][currColumn]
					if (matrix[currRow][currColumn] == 0)
					{
						++numMatricesOfHeight1;
						height1MatricesEndingAt[currRow][currColumn] = numMatricesOfHeight1;
						int minSum = height1MatricesEndingAt[currRow][currColumn];
						for (int row = currRow; row >= 0 && matrix[row][currColumn] == 0; --row)
						{
							minSum = std::min(minSum, height1MatricesEndingAt[row][currColumn]);
							totalMatrices += minSum;
						}
					}
					else
					{
						numMatricesOfHeight1 = 0;
					}

				}
			}

			return totalMatrices;

		}


	}

}