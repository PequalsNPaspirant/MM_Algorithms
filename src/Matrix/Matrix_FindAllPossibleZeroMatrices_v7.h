#pragma once

#include <vector>

namespace mm {

	/*
	m = number of rows in Matrix
	n = number of columns in Matrix

	Time Complexity: O(m*n * m)

	Space Complexity: O(m*n + n)
	*/

	namespace Matrix_FindAllPossibleZeroMatrices_v7
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			std::vector< std::vector<int> > height1MatricesEndingAt(matrix.size(), std::vector<int>((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0));
			std::vector<int> prevRowSolution((matrix.empty() ? 0 : static_cast<int>(matrix[0].size())), 0);

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
						int currentElementSolutions = numMatricesOfHeight1; //currentElementSolutions = all possible matrices ending at current element

						height1MatricesEndingAt[currRow][currColumn] = numMatricesOfHeight1;
						if (currRow > 0 && height1MatricesEndingAt[currRow - 1][currColumn] <= height1MatricesEndingAt[currRow][currColumn])
						{
							currentElementSolutions += prevRowSolution[currColumn];
						}
						else
						{
							int minSum = numMatricesOfHeight1;
							for (int row = currRow - 1; row >= 0 && matrix[row][currColumn] == 0; --row)
							{
								minSum = std::min(minSum, height1MatricesEndingAt[row][currColumn]);
								currentElementSolutions += minSum;
							}
						}

						prevRowSolution[currColumn] = currentElementSolutions;
						totalMatrices += currentElementSolutions;
					}
					else
					{
						numMatricesOfHeight1 = 0;
						prevRowSolution[currColumn] = 0;
					}

				}
			}

			return totalMatrices;

		}


	}

}