// Test.cpp : Defines the entry point for the console application.

#include <iostream>

#include "Utils/Utils_WindowsUtils.h"
#include "Timer/Timer_Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//CPU Friendly
	MM_DEFINE_FLAG(false, CPU_accessMatrixRowwise); //-----TODO
	MM_DEFINE_FLAG(false, CPU_findCasheSize); //-----TODO
	MM_DEFINE_FLAG(false, CPU_falseSharing); //-----TODO
	MM_DEFINE_FLAG(false, CPU_cacheCoherency); //-----TODO
	MM_DEFINE_FLAG(false, CPU_branchPrediction); //-----TODO
	//Binary Search Tree(BST)
	MM_DEFINE_FLAG(false, BST_checkIfPreorderTraversalIsBST);
	//Binary Tree
	MM_DEFINE_FLAG(false, BinaryTree_buildMaxBinaryTreeUnitTest);
	MM_DEFINE_FLAG(false, BinaryTree_BuildFromArrayRepresentation);
	MM_DEFINE_FLAG(false, BinaryTree_Traversals);
	MM_DEFINE_FLAG(false, BinaryTree_CheckIfIdentical_UnitTests);
	MM_DEFINE_FLAG(false, BinaryTree_CheckIfBST_UnitTests); //-----TODO
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
	MM_DEFINE_FLAG(false, Puzzles_JosephusProblem); // -----TODO
	MM_DEFINE_FLAG(false, Puzzles_EggDropping);  //--------TODO
	MM_DEFINE_FLAG(false, Puzzles_TowerOfHanoi_test);
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
	MM_DEFINE_FLAG(true, Others_FxSettlement);
	MM_DEFINE_FLAG(false, Others_GTS);
	MM_DEFINE_FLAG(false, Others_FxRatesCache); //----TODO
	MM_DEFINE_FLAG(false, Others_RankInSortedArray);
	//Others_h
	MM_DEFINE_FLAG(false, Others_h_findDistrinctNumbersGenerated);
	//Others_L
	MM_DEFINE_FLAG(false, Others_L_MaxFrequencyStack);
	//Linear Recurrence Relations
	MM_DEFINE_FLAG(false, LLR_Fibonacci);
	//Bitwise operations
	MM_DEFINE_FLAG(false, bitwiseOperations_reverseBits);
	//Arithmatic Operations
	MM_DEFINE_FLAG(false, arithmeticOperations_matrix);
	MM_DEFINE_FLAG(false, arithmeticOperations_power);
	//STL
	MM_DEFINE_FLAG(false, MM_shared_ptr_UnitTest);
	MM_DEFINE_FLAG(false, SharedPtrCyclicReferenceMemoryLeak);
	MM_DEFINE_FLAG(false, SharedPtrCyclicReferenceNoMemoryLeak);
	MM_DEFINE_FLAG(false, STL_SharedPtrToBaseDerived);
	MM_DEFINE_FLAG(false, STL_VectorUniquePtrToInt);
	MM_DEFINE_FLAG(false, STL_unordered_map_UnitTest);
	MM_DEFINE_FLAG(false, STL_vectorUnitTest);
	//Geometry
	//Pattern Matching
	//Miscellaneous
	MM_DEFINE_FLAG(false, Miscellaneous_Others_atoi); //-----TODO
	MM_DEFINE_FLAG(false, ReadWriteSubscriptOperators); //-----TODO
}

using namespace mm;

int main(int argc, char* argv[])
{
	mm::MaximizeWindow();

	MM_RUN_UNIT_TESTS

	cout << "\n\n\n" << mm::Timer::getCurrentLocalTimeInNanoSeconds() << " CONGRATULATIONS!!! End of program reached successfully.\n\n\n";

	std::cin.get();
	return 0;
}

