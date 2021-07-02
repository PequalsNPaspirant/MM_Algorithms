#pragma once

#include <vector>

namespace mm {

	namespace Matrix_FindAllPossibleZeroMatrices_v5
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


					for (int endRow = startRow; endRow < matrix.size(); ++endRow)
					{
						for (int endColumn = startColumn; endColumn < matrix[endRow].size(); ++endColumn)
						{
							if (endColumn == startColumn)
								currentRowSum[endColumn] = matrix[endRow][endColumn];
							else
								currentRowSum[endColumn] = currentRowSum[endColumn - 1] + matrix[endRow][endColumn];

							if (endRow == startRow)
								cumulativeSum[endRow][endColumn] = currentRowSum[endColumn];
							else
								cumulativeSum[endRow][endColumn] = cumulativeSum[endRow - 1][endColumn] + currentRowSum[endColumn];

							//bool valid = true;
							//for (int currRow = startRow; currRow <= endRow && valid; ++currRow)
							//{
							//	for (int currColumn = startColumn; currColumn <= endColumn && valid; ++currColumn)
							//	{

							//		if (matrix[currRow][currColumn] != 0)
							//			valid = false;

							//	}
							//}

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