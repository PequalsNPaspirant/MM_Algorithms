#pragma once

#include <vector>

namespace mm {

	namespace Matrix_FindAllPossibleZeroMatrices_v6
	{
		int findAllPossibleZeroMatrices(const std::vector< std::vector<int> >& matrix)
		{
			int totalMatrices = 0;

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

							++totalMatrices;

						}

					}


				}
			}

			return totalMatrices;

		}


	}

}