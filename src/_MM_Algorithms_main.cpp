// Test.cpp : Defines the entry point for the console application.

#include <iostream>

#include "WindowsUtils/WindowsUtils.h"
#include "Timer/Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//Binary Search Tree(BST)
	MM_DEFINE_FLAG(false, BST_checkIfPreorderTraversalIsBST);
	//Binary Tree
	MM_DEFINE_FLAG(false, BinaryTree_buildMaxBinaryTreeUnitTest);
	MM_DEFINE_FLAG(false, BinaryTree_BuildFromArrayRepresentation);
	MM_DEFINE_FLAG(false, BinaryTree_Traversals);
	//Dynamic Programming
	MM_DEFINE_FLAG(false, DP_HouseRobbery);
	MM_DEFINE_FLAG(false, DP_Fibonacci);
	MM_DEFINE_FLAG(false, DP_Large_Fibonacci);
	MM_DEFINE_FLAG(false, DP_WaysToClimbNSteps);
	MM_DEFINE_FLAG(false, DP_LongestCommonSubsequenceLength);
	MM_DEFINE_FLAG(false, DP_LongestCommonSubsequenceString);
	MM_DEFINE_FLAG(false, DP_LongestCommonSubsequenceString2);
	MM_DEFINE_FLAG(false, DP_LongestCommonSubsequenceStringsAll);
	MM_DEFINE_FLAG(false, DP_LongestCommonSubsequenceLengthOfkStrings);
	MM_DEFINE_FLAG(false, DP_LongestCommonSubstring);
	MM_DEFINE_FLAG(false, DP_KnapsackProblem_0_1);
	MM_DEFINE_FLAG(false, DP_RodCutting);
	MM_DEFINE_FLAG(false, DP_MaxProductRodCutting);
	MM_DEFINE_FLAG(false, DP_MinCoinsChange);
	//Greedy
	//Trie
	MM_DEFINE_FLAG(false, Trie_implementationUsingArray);
	MM_DEFINE_FLAG(false, Trie_implementationUsingHashMap);
	//Backtracking
	//Divide & Conquer
	//Stack
	//Heaps
	//Puzzles
	//Binary
	MM_DEFINE_FLAG(false, Binary_reverseBitsInInteger);
	//Queue
	//Linked Lists
	MM_DEFINE_FLAG(false, AddNumbersInLinkListFormUnitTest);
	MM_DEFINE_FLAG(false, LinkedList_ReverseSingleLinkedList);
	//Sorting
	MM_DEFINE_FLAG(false, canSingleSwapSortArrayUnitTest);
	//String
	MM_DEFINE_FLAG(false, String_FirstNonRepeatingChar);
	MM_DEFINE_FLAG(false, String_FirstKNonRepeatingChars);
	MM_DEFINE_FLAG(false, String_FirstKNonRepeatingChars_optimized);
	//Matrix
	//Graphs
	//Array
	MM_DEFINE_FLAG(false, SmallestPositiveIntNotPresentInArrayUnitTest);
	//OTHERS	
	MM_DEFINE_FLAG(false, StockPriceHistoryUnitTest);
	MM_DEFINE_FLAG(false, getMaxProfitFromShareTradingUnitTest);
	MM_DEFINE_FLAG(false, toggle1bitForMaxAdjacencyUnitTest);
	MM_DEFINE_FLAG(false, simpleElevatorUnitTest);
	MM_DEFINE_FLAG(false, FixedPriceInstrumentsPricingUnitTest);
	MM_DEFINE_FLAG(false, checkIfNumberIsPalindrom);
	MM_DEFINE_FLAG(false, Others_DateUnitTest);
	MM_DEFINE_FLAG(false, Others_BigIntegerMultiplication);
	MM_DEFINE_FLAG(false, Others_EquityOrderManagementSystem);
	MM_DEFINE_FLAG(false, Others_CarRentalSystem);
	MM_DEFINE_FLAG(false, Others_PersonSort);
	MM_DEFINE_FLAG(false, Others_FxSettlement);

	MM_DEFINE_FLAG(false, Others_h_findDistrinctNumbersGenerated);
}

using namespace mm;

int main(int argc, char* argv[])
{
	MaximizeWindow();				

	MM_RUN_UNIT_TESTS

	cout << "\n\n\n" << Timer::getCurrentLocalTimeInNanoSeconds() << " CONGRATULATIONS!!! End of program reached successfully.\n\n\n";

	std::cin.get();
	return 0;
}

