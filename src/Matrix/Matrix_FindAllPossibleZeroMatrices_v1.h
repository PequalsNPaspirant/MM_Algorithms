#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n * m*n * m*n + m*n)

	Space Complexity: O(m*n)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v1
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector< std::vector<int> > solution( matrix.size(), std::vector<int>( (matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0 ) );

			for (int startRow = 0; startRow < matrix.size(); ++startRow)
			{
				for (int startColumn = 0; startColumn < matrix[startRow].size(); ++startColumn)
				{


					for (int endRow = startRow; endRow < matrix.size(); ++endRow)
					{
						for (int endColumn = startColumn; endColumn < matrix[endRow].size(); ++endColumn)
						{

							bool valid = true;
							for (int currRow = startRow; currRow <= endRow && valid; ++currRow)
							{
								for (int currColumn = startColumn; currColumn <= endColumn && valid; ++currColumn)
								{

									if (matrix[currRow][currColumn] != 0)
										valid = false;

								}
							}

							if (valid)
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