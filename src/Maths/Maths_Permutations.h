//Goal

#pragma once

#include <vector>

using namespace std;

namespace mm {

	class Permutations
	{
	public:
		static void generatePermutations(initializer_list<int> list, bool repeatition, vector< vector<int> >& output);

		static void generatePermutationSetWithRepeatition(vector< vector<int> >& output,
			vector<int>& currenResultSet, const int& totalPositions, const int& currentPosition,
			const int& totalVariables, const vector<int>& source);

		static void generatePermutationSetWithNoRepeatition(vector< vector<int> >& output,
			vector<int>& currenResultSet, const int& totalPositions, const int& currentPosition,
			const int& totalVariables, const vector<int>& source);

		static void generatePermutationsUsingRecursion(initializer_list<int> list, bool repeatition, vector< vector<int> >& output);

		static void generateWordPermutations(const string& charSet);

	private:
		static long factorial(int n);

		static void generateWordPermutations(string& str, const string& charSet, const int index, int& totalCombincations);
	};
}
