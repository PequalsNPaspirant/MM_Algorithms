//Goal
//Solve puzzles using dynamic programming

#include <iostream>
#include <vector>
#include <time.h>
#include <sstream> //for std::stringstream
#include <fstream> //for file read/write
#include <iomanip> //for setw(n)
using namespace std;

#include "my_math_lib/Permutations.h"
#include "DynamicProgramming/DP_HouseRobbery.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	void testHouseRobberyProblem(const vector<int>& treasurySet)
	{
		//cout << "\n\nPuzzle-------------------------------------------------------";
		//cout << "\nHouse Index         : ";
		//for (int i = 0; i < treasurySet.size(); ++i)
		//	cout << setw(4) << i << ",   ";
		//cout << "\nAmount in each house: ";
		//for (int i = 0; i < treasurySet.size(); ++i)
		//	cout << setw(4) << treasurySet[i] << ",   ";

		vector<int> solution;

		//cout << "\n\nMethod 1-------------------------------------------------------";
		solution.clear();
		int amount1 = HouseRobbery::solve2(treasurySet, solution);
		//cout << "\nTotal amount : " << amount1;
		//cout << "\nAmountFromEachHouse : ";
		//for (int i = 0; i < solution.size(); ++i)
		//	cout << setw(4) << solution[i] << ",   ";

		//cout << "\n\nMethod 2-------------------------------------------------------";
		solution.clear();
		int amount2 = HouseRobbery::solve(treasurySet, solution);
		//cout << "\nTotal amount : " << amount2;
		//cout << "\nIndices of houses robbed: ";
		//for (int i = 0; i < solution.size(); ++i)
		//	cout << setw(4) << solution[i] << ",   ";

		//cout << "\n\nMethod 3-------------------------------------------------------";
		solution.clear();
		HouseRobberyProblem problem(treasurySet);
		int amount3 = problem.solve(solution);
		//cout << "\nTotal amount : " << amount3;
		//cout << "\nIndices of houses robbed: ";
		//for (int i = 0; i < solution.size(); ++i)
		//	cout << setw(4) << solution[i] << ",   ";

		//MM_EXPECT_EQUAL(amount1, amount2)
		//__PRINT_VARIABLES_AND_VALUES__(amount1 == amount2 && amount2 == amount3, amount1, amount2, amount3)
		MM_EXPECT_TRUE(amount1 == amount2 && amount2 == amount3, amount1, amount2, amount3)
		
		//cout << "\n\nPress any key to solve next case:\n";
		//cin.get();
	}

	MM_DECLARE_FLAG(DP_HouseRobbery)

	MM_UNIT_TEST(testHouseRobberyProblem, DP_HouseRobbery)
	{
		int numTestCases = 20;
		vector<int> treasurySet;

		//cout << "\n\nProblem: HouseRobbery\nTrying sets of size 1 to 20...";
		//Generate sets of sizes from 1 to 20
		srand(time(nullptr));
		for (int i = 1; i <= numTestCases; i++)
		{
			//Generate random treasury
			treasurySet.resize(i);
			for (int houseNumber = 1; houseNumber <= i; houseNumber++)
			{
				treasurySet[houseNumber - 1] = rand() % (i * 2) + 1;
			}

			testHouseRobberyProblem(treasurySet);
		}

		//cout << "\n\nTrying sets of size 20...";
		//Generate various sets of size 20
		srand(time(nullptr));
		int size = 20;
		treasurySet.resize(size);
		for (int i = 0; i < numTestCases; i++)
		{
			//Generate random treasury
			for (int j = 0; j < size; j++)
			{
				treasurySet[j] = rand() % (size * 2) + 1;
			}

			testHouseRobberyProblem(treasurySet);
		}

		//cout << "\n\nTrying permutation sets of size 2...";
		bool repeatition = true;
		vector< vector<int> > output;
		Permutations::generatePermutations({ 1, 2 }, repeatition, output);
		for (size_t i = 0; i < output.size(); i++)
		{
			testHouseRobberyProblem(output[i]);
		}

		//cout << "\n\nTrying permutation sets of size 3...";
		output.clear();
		Permutations::generatePermutations({ 1, 2, 3 }, repeatition, output);
		for (size_t i = 0; i < output.size(); i++)
		{
			testHouseRobberyProblem(output[i]);
		}

		//cout << "\n\nTrying permutation sets of size 4...";
		output.clear();
		Permutations::generatePermutations({ 1, 2, 3, 4 }, repeatition, output);
		for (size_t i = 0; i < output.size(); i++)
		{
			testHouseRobberyProblem(output[i]);
		}

		//cout << "\n\nTrying permutation sets of size 5...";
		output.clear();
		Permutations::generatePermutations({ 1, 2, 3, 4, 5 }, repeatition, output);
		for (size_t i = 0; i < output.size(); i++)
		{
			testHouseRobberyProblem(output[i]);
		}

		//cout << "\n\nEnd of test.";
	}

}
