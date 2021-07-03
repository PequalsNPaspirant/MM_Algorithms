#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n * m)

	Space Complexity: O(m*n)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v7
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector< std::vector<int> > rowWizeSum(matrix.size(), std::vector<int>((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0));
			std::vector<int> prevRowSolution((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0);

			int totalMatrices = 0;
			for (int currRow = 0; currRow < matrix.size(); ++currRow)
			{
				int numMatrices = 0;
				for (int currColumn = 0; currColumn < matrix[currRow].size(); ++currColumn)
				{

					if (matrix[currRow][currColumn] == 0)
					{
						++numMatrices;
						int currentElementSolutions = numMatrices;

						rowWizeSum[currRow][currColumn] = numMatrices;
						if (currRow > 0 && rowWizeSum[currRow - 1][currColumn] <= rowWizeSum[currRow][currColumn])
						{
							currentElementSolutions += prevRowSolution[currColumn];
						}
						else
						{
							int minSum = numMatrices;
							for (int row = currRow - 1; row >= 0 && matrix[row][currColumn] == 0; --row)
							{
								minSum = std::min(minSum, rowWizeSum[row][currColumn]);
								currentElementSolutions += minSum;
							}
						}

						prevRowSolution[currColumn] = currentElementSolutions;
						totalMatrices += currentElementSolutions;
					}
					else
					{
						numMatrices = 0;
						prevRowSolution[currColumn] = 0;
					}

				}
			}

			return totalMatrices;

		}


	}

}