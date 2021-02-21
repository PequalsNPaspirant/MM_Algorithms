//Goal:
//Generate all permutations

#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <unordered_set>
using namespace std;

#include "Maths/Maths_Permutations.h"
#include "Utils/Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	long Permutations::factorial(int n)
	{
		const int Limit = 50;
		if (n > Limit)
		{
			cout << "Can not calculate";
			return 0;
		}

		static long fact[Limit];
		fact[0] = 1;
		static int availableFactIndex = 0;

		if (n > availableFactIndex)
		{
			long result = fact[availableFactIndex];
			for (int i = availableFactIndex + 1; i <= n; i++)
			{
				result *= i;
				fact[i] = result;
			}
			availableFactIndex = n;
		}

		return fact[n];
	}

	void Permutations::generatePermutations(initializer_list<int> list, bool repeatition, vector< vector<int> >& output)
	{
		int totalVariables = list.size();
		int totalPositions = list.size();

		const int* begin = list.begin();
		vector<int> source;
		source.reserve(list.size());
		for (int i = 0; i < totalVariables; i++)
			source.push_back(*begin++);

		if (repeatition)
		{
			//Arranging n variables in p position - first position can be filled-in in n ways, second also can be in n ways as repetition is allowed
			//This all p potions can be filled-in in n * n * n * ... p times i.e. n ^ p
			int totalSets = pow(totalVariables, totalPositions);
			output.reserve(totalSets);
			for (int i = 0; i < totalSets; i++)
			{
				vector<int> currentSet(totalPositions);
				for (int j = 0; j < totalPositions; j++)
				{
					int remainingPositions = totalPositions - j - 1;
					int remainingPermutations = pow(totalVariables, remainingPositions);
					int index = (i / remainingPermutations) % totalVariables;
					currentSet[j] = source[index];
				}
				output.push_back(std::move(currentSet));
				currentSet.clear();
			}
		}
		else
		{
			//Arranging n variables in p position - first position can be filled-in in n ways, second can be in (n-1) ways as repetition is NOT allowed
			//This all p potions can be filled-in in n * (n-1) * (n-2) * ... * (n-(p-1)) when p < n. 
			//When n = p, its n!. When p > n, its n! as extra positions will be vacant.
			int totalSets = factorial(totalVariables);
			output.resize(totalSets);
			for (int i = 0; i < totalSets; i++)
			{
				vector<int> currentSet(totalPositions);
				vector<int> tempSource(source.begin(), source.end());
				for (int j = 0; j < totalPositions; j++)
				{
					int remainingVariables = totalVariables - j - 1;
					int remainingPositions = totalPositions - j - 1;
					int remainingPermutations = factorial(remainingPositions);
					int currentVariables = tempSource.size();  //OR remainingVariables + 1
					int index = (i / remainingPermutations) % currentVariables;
					currentSet[j] = tempSource[index];
					tempSource.erase(tempSource.begin() + index);
				}
				output[i] = currentSet;
				currentSet.clear();
			}
		}
	}

	void Permutations::generatePermutationSetWithRepeatition(vector< vector<int> >& output,
		vector<int>& currenResultSet, const int& totalPositions, const int& currentPosition,
		const int& totalVariables, const vector<int>& source)
	{
		if (currentPosition >= totalPositions)
		{
			output.push_back(currenResultSet);
			return;
		}

		for (int j = 0; j < totalVariables; j++)
		{
			currenResultSet[currentPosition] = source[j];
			generatePermutationSetWithRepeatition(output, currenResultSet, totalPositions, currentPosition + 1, totalVariables, source);
		}
	}

	void Permutations::generatePermutationSetWithNoRepeatition(vector< vector<int> >& output,
		vector<int>& currenResultSet, const int& totalPositions, const int& currentPosition,
		const int& totalVariables, const vector<int>& source)
	{
		if (currentPosition >= totalPositions)
		{
			output.push_back(currenResultSet);
			return;
		}

		for (int j = 0; j < totalVariables; j++)
		{
			currenResultSet[currentPosition] = source[j];
			vector<int> sourceForNextIteration(source.begin(), source.end());
			sourceForNextIteration.erase(sourceForNextIteration.begin() + j);
			generatePermutationSetWithNoRepeatition(output, currenResultSet, totalPositions, currentPosition + 1, totalVariables - 1, sourceForNextIteration);
		}
	}

	void Permutations::generatePermutationsUsingRecursion(initializer_list<int> list, bool repeatition, vector< vector<int> >& output)
	{
		int totalVariables = list.size();
		int totalPositions = list.size();

		const int* begin = list.begin();
		vector<int> source;
		for (int i = 0; i < totalVariables; i++)
			source.push_back(*begin++);

		if (repeatition)
		{
			//Arranging n variables in p position - first position can be filled-in in n ways, second also can be in n ways as repetition is allowed
			//This all p potions can be filled-in in n * n * n * ... p times i.e. n ^ p
			vector<int> currentSet(totalPositions);
			generatePermutationSetWithRepeatition(output, currentSet, totalPositions, 0, totalVariables, source);
		}
		else
		{
			//Arranging n variables in p position - first position can be filled-in in n ways, second can be in (n-1) ways as repetition is NOT allowed
			//This all p potions can be filled-in in n * (n-1) * (n-2) * ... * (n-(p-1)) when p < n. 
			//When n = p, its n!. When p > n, its n! as extra positions will be vacant.
			vector<int> currentSet(totalPositions);
			generatePermutationSetWithNoRepeatition(output, currentSet, totalPositions, 0, totalVariables, source);
		}
	}


	//produce all combinations of words from given string
	void Permutations::generateWordPermutations(string& str, const string& charSet, const int index, int& totalCombinations)
	{
		if (index == charSet.length())
		{
			cout << endl << totalCombinations++ << " : " << str;
			return;
		}

		for (int i = 0; i < charSet.length(); i++)
		{
			str[index] = charSet[i];
			generateWordPermutations(str, charSet, index + 1, totalCombinations);
		}
	}

	void Permutations::generateWordPermutations(const string& charSet)
	{
		cout << "\n\nTest for : " << charSet << "------------------------------";
		string temp(charSet);
		int totalCombincations = 0;
		generateWordPermutations(temp, charSet, 0, totalCombincations);
	}

	vector<int> Permutations::generateRandomPermutation(const vector<int>& inputSet, int size)
	{
		vector<int> retVal;
		retVal.reserve(size);

		std::random_device rd;
		std::default_random_engine e{ rd() }; //or use specific engine e.g. std::mt19937_64 m64;
		int lastId = inputSet.size() - 1;
		std::uniform_int_distribution<int> uid{ 0, lastId };
		using params = std::uniform_int_distribution<int>::param_type;

		vector<int> inputSetCopy{ inputSet.begin(), inputSet.end() };
		for (int i = 0; i < size; ++i)
		{
			uid.param(params{ 0, lastId - i });
			int p = uid(e);
			retVal.push_back(inputSetCopy[p]);
			std::swap(inputSetCopy[p], inputSetCopy[lastId - i]);
		}

		return retVal;
	}

	vector<int> Permutations::generateRandomSelection(const vector<int>& inputSet, int size)
	{
		vector<int> retVal;
		retVal.reserve(size);

		std::random_device rd;
		std::default_random_engine e{ rd() }; //or use specific engine e.g. std::mt19937_64 m64;
		std::uniform_int_distribution<int> uid{ 0, static_cast<int>(inputSet.size()) - 1 };
		using params = std::uniform_int_distribution<int>::param_type;

		vector<int> inputSetCopy{ inputSet.begin(), inputSet.end() };
		unordered_set<int> retSet;
		for (int i = 0; i < size; ++i)
		{
			uid.param(params{ 0, static_cast<int>(inputSet.size()) - size + i });
			int p = uid(e);
			int ele = inputSetCopy[inputSet.size() - size + i];
			if(retSet.find(ele) != retSet.end())
				retSet.insert(inputSetCopy[p]);
			else
				retSet.insert(ele);
		}

		for (const auto& e : retSet)
			retVal.push_back(e);

		return retVal;
	}

	void testPermutations()
	{
		//Permutations::generateWordPermutations("");
		//Permutations::generateWordPermutations("A");
		//Permutations::generateWordPermutations("AB");
		//Permutations::generateWordPermutations("ABC");
		//Permutations::generateWordPermutations("ABCD");
		//Permutations::generateWordPermutations("ABCDE");
		//Permutations::generateWordPermutations("ABCDEF");

		for (int i = 1; i < 6; ++i)
		{
			cout << "\n" << Permutations::generateRandomPermutation({1,2,3,4,5,6,7,8,9,10}, i);
			cout << "\n" << Permutations::generateRandomSelection({ 1,2,3,4,5,6,7,8,9,10 }, i);
		}
	}

	MM_DECLARE_FLAG(Maths_Permutations);

	MM_UNIT_TEST(Maths_Permutations_test_1, Maths_Permutations)
	{
		testPermutations();
	}
}