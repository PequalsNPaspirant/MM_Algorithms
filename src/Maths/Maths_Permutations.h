//Goal

#pragma once

#include <vector>

using namespace std;

namespace mm {

	class Permutations
	{
	public:
		static void generatePermutations(initializer_list<int> list, bool repeatition, vector< vector<int> >& output);
		static void generatePermutationsUsingRecursion(initializer_list<size_t> list, bool repeatition, vector< vector<size_t> >& output);
		static void generateWordPermutations(const string& charSet);
		static vector<int> generateRandomPermutation(const vector<int>& inputSet, int size);
		static vector<int> generateRandomSelection(const vector<int>& inputSet, int size);

	private:
		static size_t factorial(size_t n);

		static void generatePermutationSetWithRepeatition(vector< vector<size_t> >& output,
			vector<size_t>& currenResultSet, const size_t& totalPositions, const size_t& currentPosition,
			const size_t& totalVariables, const vector<size_t>& source);

		static void generatePermutationSetWithNoRepeatition(vector< vector<size_t> >& output,
			vector<size_t>& currenResultSet, const size_t& totalPositions, const size_t& currentPosition,
			const size_t& totalVariables, const vector<size_t>& source);

		static void generateWordPermutations(string& str, const string& charSet, const int index, int& totalCombincations);
	};
}
