//Goal

#pragma once

#include <vector>

using namespace std;

namespace mm {

	class Permutations
	{
	public:
		static void generatePermutations(initializer_list<int> list, bool repeatition, vector< vector<int> >& output);
		static void generatePermutationsUsingRecursion(initializer_list<int> list, bool repeatition, vector< vector<int> >& output);
		static void generateWordPermutations(const string& charSet);
		static vector<int> generateRandomPermutation(const vector<int>& inputSet, int size);
		static vector<int> generateRandomSelection(const vector<int>& inputSet, int size);

	private:
		static long factorial(int n);

		static void generatePermutationSetWithRepeatition(vector< vector<int> >& output,
			vector<int>& currenResultSet, const int& totalPositions, const int& currentPosition,
			const int& totalVariables, const vector<int>& source);

		static void generatePermutationSetWithNoRepeatition(vector< vector<int> >& output,
			vector<int>& currenResultSet, const int& totalPositions, const int& currentPosition,
			const int& totalVariables, const vector<int>& source);

		static void generateWordPermutations(string& str, const string& charSet, const int index, int& totalCombincations);
	};
}
