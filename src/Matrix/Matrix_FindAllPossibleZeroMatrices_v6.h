#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n)

	Space Complexity: O(m*n)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v6
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector< std::vector<int> > rowWizeSum(matrix.size(), std::vector<int>((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0));
			//std::vector<int> currentRowMultiplier((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0);
			//std::vector<int> prevRow((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0);

			int totalMatrices = 0;
			for (int currRow = 0; currRow < matrix.size(); ++currRow)
			{
				int numMatrices = 0;
				for (int currColumn = 0; currColumn < matrix[currRow].size(); ++currColumn)
				{
					

					if (matrix[currRow][currColumn] == 0)
					{
					//	++currentRowMultiplier[currColumn];
						++numMatrices;
					//	int prevRowShort = (currRow == 0 ? 0 : solution[currRow - 1][currColumn]) + numMatrices;
					//	int currRowShort = currentRowMultiplier[currColumn] * numMatrices;
					//	solution[currRow][currColumn] = std::min(prevRowShort, currRowShort);

					//	prevRow[currColumn] = numMatrices;

						rowWizeSum[currRow][currColumn] = numMatrices;
						int minSum = rowWizeSum[currRow][currColumn];
						for (int row = currRow; row >= 0 && matrix[row][currColumn] == 0; --row)
						{
							minSum = std::min(minSum, rowWizeSum[row][currColumn]);
							totalMatrices += minSum;
						}
					}
					else
					{
					//	currentRowMultiplier[currColumn] = 0;
						numMatrices = 0;
					}

				}
			}

			
			//for (int currRow = 0; currRow < matrix.size(); ++currRow)
			//{
			//	for (int currColumn = 0; currColumn < matrix[currRow].size(); ++currColumn)
			//	{
			//		totalMatrices += solution[currRow][currColumn];
			//	}
			//}

			return totalMatrices;

		}


	}

}