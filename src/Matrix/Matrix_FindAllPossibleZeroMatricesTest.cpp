#include <random>
#include <chrono>
#include <iomanip>

#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v1.h"
#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v2.h"
#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v3.h"
#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v4.h"
#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v5.h"
#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v6.h"
#include "Matrix/Matrix_FindAllPossibleZeroMatrices_v7.h"

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	namespace Matrix_FindAllPossibleZeroMatrices
	{
		template<typename Fun>
		int testEachAlgo(Fun fun, const std::vector< std::vector<int> >& matrix, int expectedValue)
		{
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			int actualvalue = fun(matrix);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			std::cout << "\n" << "actualvalue: " << setw(8) << actualvalue << " duration: " << setw(15) << duration << " ns";

			return actualvalue;
		}

		void testAll()
		{
			int numRows = 10;
			int numColumns = 10;

			std::random_device rd;
			std::mt19937 mt(rd());

			cout.imbue(std::locale(""));

			for (int rows = 1; rows <= numRows; ++rows)
			{
				for (int columns = 1; columns <= numColumns; ++columns)
				{
					std::vector< std::vector<int> > inMatrix( rows, std::vector<int>(columns, 0) );

					//set 1's at some random locations in matrix
					int numOnes = rows * columns / 3;
					for (int i = 0; i < numOnes; ++i)
					{
						std::uniform_int_distribution<int> distRow(0, rows - 1);
						std::uniform_int_distribution<int> distColumn(0, columns - 1);
						inMatrix[distRow(mt)][distColumn(mt)] = 1;
					}

					//print matrix
					std::cout << "\n";
					for (int currRow = 0; currRow < inMatrix.size(); ++currRow)
					{
						std::cout << "\n";
						for (int currColumn = 0; currColumn < inMatrix[currRow].size(); ++currColumn)
						{
							std::cout << inMatrix[currRow][currColumn] << " ";
						}
						
					}

					//call all algorithms
					int expectedValue = -1;
					expectedValue = testEachAlgo(Matrix_FindAllPossibleZeroMatrices_v1::findAllPossibleZeroMatrices, inMatrix, expectedValue);
					expectedValue = testEachAlgo(Matrix_FindAllPossibleZeroMatrices_v2::findAllPossibleZeroMatrices, inMatrix, expectedValue);
					expectedValue = testEachAlgo(Matrix_FindAllPossibleZeroMatrices_v3::findAllPossibleZeroMatrices, inMatrix, expectedValue);
					expectedValue = testEachAlgo(Matrix_FindAllPossibleZeroMatrices_v4::findAllPossibleZeroMatrices, inMatrix, expectedValue);
					expectedValue = testEachAlgo(Matrix_FindAllPossibleZeroMatrices_v5::findAllPossibleZeroMatrices, inMatrix, expectedValue);
				}
			}
		}

	}


	MM_DECLARE_FLAG(Matrix_FindAllPossibleZeroMatrices);

	MM_UNIT_TEST(Matrix_FindAllPossibleZeroMatrices_test_1, Matrix_FindAllPossibleZeroMatrices)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);
		MM_SET_PAUSE_ON_ERROR(false);

		Matrix_FindAllPossibleZeroMatrices::testAll();
	}


}