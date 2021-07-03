#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n * m*n + m*n)

	Space Complexity: O(2 * m*n + m)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v3
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector< std::vector<int> > cumulativeSum(matrix.size(), std::vector<int>((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0));
			std::vector<int> currentRowSum((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0);
			std::vector< std::vector<int> > solution( matrix.size(), std::vector<int>( (matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0 ) );

			for (int startRow = 0; startRow < matrix.size(); ++startRow)
			{
				for (int startColumn = 0; startColumn < matrix[startRow].size(); ++startColumn)
				{

					int lastSearchColumn = static_cast<int>(matrix[startRow].size());
					for (int endRow = startRow; endRow < matrix.size() && startColumn < lastSearchColumn; ++endRow)
					{
						for (int endColumn = startColumn; endColumn < lastSearchColumn; ++endColumn)
						{
							if (matrix[endRow][endColumn] == 1)
							{
								lastSearchColumn = endColumn;
								break;
							}

							if (endColumn == startColumn)
								currentRowSum[endColumn] = matrix[endRow][endColumn];
							else
								currentRowSum[endColumn] = currentRowSum[endColumn - 1] + matrix[endRow][endColumn];

							if (endRow == startRow)
								cumulativeSum[endRow][endColumn] = currentRowSum[endColumn];
							else
								cumulativeSum[endRow][endColumn] = cumulativeSum[endRow - 1][endColumn] + currentRowSum[endColumn];

							if (cumulativeSum[endRow][endColumn] == 0)
								++solution[endRow][endColumn];

						}

					}


				}
			}

			int totalMatrices = 0;
			for (int currRow = 0; currRow < matrix.size(); ++currRow)
			{
				for (int currColumn = 0; currColumn < matrix[currRow].size(); ++currColumn)
				{
					totalMatrices += solution[currRow][currColumn];
				}
			}

			return totalMatrices;

		}


	}
	   
}