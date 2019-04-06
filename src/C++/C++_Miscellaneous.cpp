//Goal
//All miscellaneous problems

#include <iostream>
#include <string>
#include <stdio.h>  /* defines FILENAME_MAX */
#include <windows.h>
#include <vector>
#include <map>
#include <unordered_set>
#include <cmath>
#include <limits.h> //For PATH_MAX
#include <windows.h>
using namespace std;

//-----------------------------------------------------------------------------------------------------
//Algorithm to check if sequence of brackets is valid

#if defined _MSC_VER

#include <direct.h>
#define GET_CWD(path) _getcwd(path, sizeof(path))

#elif defined __GNUC__

#include <unistd.h>
#define GET_CWD(path) getcwd(path, sizeof(path))

#endif

namespace mm {

	bool checkIfBracketSeqValid(const string& str)
	{
		int score = 0;
		for (int i = 0; score >= 0 && i < str.size(); i++)
		{
			if (str[i] == '(')
				score++;
			else if (str[i] == ')')
				score--;
		}
		if (score == 0)
			return true;
		else
			return false;
	}

	//Assume precedence is {, [ and then (, so {[()]}, {{[[((()))]]}}, {()}, [()], (()) are correct but [{()}] is incorrect
	bool checkIfAllBracketsSeqValid(string str)
	{
		int CurlyBracketCounter = 0, SquareBracketCounter = 0, RoundBracketCounter = 0;
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] == '(') RoundBracketCounter++;
			else if (str[i] == ')') RoundBracketCounter--;
			else if (str[i] == '{') CurlyBracketCounter++;
			else if (str[i] == '}') CurlyBracketCounter--;
			else if (str[i] == '[') SquareBracketCounter++;
			else if (str[i] == ']') SquareBracketCounter--;
			if (RoundBracketCounter == -1 || CurlyBracketCounter == -1 || SquareBracketCounter == -1)
				return false;
			if (RoundBracketCounter > 0 && (str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']'))
				return false;
			if (SquareBracketCounter > 0 && (str[i] == '{' || str[i] == '}'))
				return false;
		}
		if (RoundBracketCounter == 0 && CurlyBracketCounter == 0 && SquareBracketCounter == 0)
			return true;
		else
			return false;
	}

	void testCheckIfBracketSeqValid()
	{
		cout << "\n\n------------- checkIfBracketSeqValid ------------------------";
		string str = "anything(means any) as (EXAMPLE(example))";
		cout << endl << str << ((checkIfBracketSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "dad(dad))adna(dsd)dadd";
		cout << endl << str << ((checkIfBracketSeqValid(str) == true) ? "=> VALID" : "=> INVALID");

		cout << "\n\n------------- checkIfAllBracketsSeqValid ------------------------";
		str = "anything(means any) as (EXAMPLE(example))";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "dad(dad))adna(dsd)dadd";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "{ [()] }";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "{ { [[((()))]] } }";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "{ () }";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "[()]";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "(())";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
		str = "[{()}]";
		cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	}

	//-----------------------------------------------------------------------------------------------------

	//Arrange all zeros & negative elements at left side, and positive at right side

	//Traversing from both ends. It does not maintains the order
	void clustorArray1(int arrayInt[], int size)
	{
		int noOfSwaps = 0;
		cout << "\nInput: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";

		int leftPos = 0;
		int rightPos = size - 1;
		while (leftPos < rightPos)
		{
			while (leftPos < size && arrayInt[leftPos] <= 0) leftPos++;
			while (rightPos >= 0 && arrayInt[rightPos] > 0) rightPos--;
			if (leftPos < rightPos)
			{
				swap(arrayInt[leftPos], arrayInt[rightPos]);
				noOfSwaps++;
			}
		}

		cout << " || Output: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";
		cout << "\nNo. of Swaps: " << noOfSwaps;
	}

	//Traversing from left end. It maintains the order of only left elements. No. of Swaps are more.
	void clustorArray2(int arrayInt[], int size)
	{
		int noOfSwaps = 0;
		cout << "\nInput: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";

		int separatingWall = 0;
		for (int i = 0; i < size; i++)
		{
			if (arrayInt[i] <= 0)
			{
				if (i != separatingWall) //No. of swaps are reduced by this check, but need to increase separatingWall anyway
				{
					swap(arrayInt[separatingWall], arrayInt[i]);
					noOfSwaps++;
				}
				separatingWall++;
			}
		}

		cout << " || Output: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";
		cout << "\nNo. of Swaps: " << noOfSwaps;
	}

	//Rotate anticlockwise by one element
	void rightRotate(int arrayInt[], int startIndex, int endIndex, double& noOfSwaps)
	{
		int backup = arrayInt[endIndex];
		for (int i = endIndex; i > startIndex; i--)
		{
			arrayInt[i] = arrayInt[i - 1];
			noOfSwaps += 0.5;
		}
		arrayInt[startIndex] = backup;
		noOfSwaps += 0.5;
	}

	//Traversing from left end. It maintains the order in left and right groups. Needs most no. of swaps
	void clustorArray3(int arrayInt[], int size)
	{
		double noOfSwaps = 0;
		cout << "\nInput: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";

		int separatingWall = 0;
		for (int i = 0; i < size; i++)
		{
			if (arrayInt[i] > 0)
			{
				for (int j = i + 1; j < size; j++)
				{
					if (arrayInt[j] <= 0)
					{
						rightRotate(arrayInt, i, j, noOfSwaps);
						break;
					}
				}
			}
		}

		cout << " || Output: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";
		cout << "\nNo. of Swaps: " << noOfSwaps;
	}

	/*
	--A Juggling Algorithm
	Rotate anticlockwise
	Instead of moving one by one, divide the array in different sets
	where number of sets is equal to GCD of n and d and move the elements within sets.
	If GCD is 1 as is for the above example array(n = 7 and d = 2), then elements will be moved within one set only,
	we just start with temp = arr[0] and keep moving arr[I + d] to arr[I] and finally store temp at the right place.
	*/
	int gcd(int a, int b)
	{
		int max = a;
		int min = b;
		if (b > a)
		{
			max = b;
			min = a;
		}

		while (min != 0)
		{
			int lastMax = max;
			max = min;
			min = lastMax % min;
		}
		return max;
	}

	void rightRotateBy(int arrayInt[], int startIndex, int endIndex, int rotateby, double& noOfSwaps)
	{
		if (rotateby <= 0) return;

		int size = endIndex - startIndex + 1;
		int gcdVal = gcd(size, rotateby);
		for (int i = 0; i < gcdVal; i++)
		{
			int startWith = startIndex + i;
			int j = startWith;
			int temp = arrayInt[j];
			noOfSwaps += 0.5;
			while (1)
			{
				int k = j - rotateby;
				if (k < startIndex)
					k = (endIndex + 1) - (startIndex - k);
				if (startWith == k)
					break;
				arrayInt[j] = arrayInt[k];
				noOfSwaps += 0.5;
				j = k;
			}
			arrayInt[j] = temp;
			noOfSwaps += 0.5;
		}
	}

	void leftShift(int arrayInt[], int startIndex, int endIndex, int shiftBy, double& noOfSwaps)
	{
		int rotateBy = endIndex - startIndex + 1;
		rightRotateBy(arrayInt, startIndex - shiftBy, endIndex, rotateBy, noOfSwaps);
		//Reverse the first (endIndex - startIndex) elements
		//-----No need to reverse the elements
		//int start = startIndex - shiftBy;
		//int end = start + rotateBy - 1;
		//for (int i = start; i < end / 2; i++)
		//{
		//	swap(arrayInt[i], arrayInt[end - i]);
		//	noOfSwaps += 1;
		//}
	}

	//Traversing from left end. It maintains the order in left and right groups. Needs most no. of swaps
	void clustorArray4(int arrayInt[], int size)
	{
		double noOfSwaps = 0;
		cout << "\nInput: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";

		int separatingWall = 0;
		for (int i = 0; i < size;)
		{
			if (arrayInt[i] > 0)
			{
				int shiftBy = 0;
				int shiftStart = -1;
				int j;
				//Find next first 0 or negative element
				for (j = i + 1; j < size; j++)
				{
					shiftBy++;
					if (arrayInt[j] <= 0)
					{
						shiftStart = j;
						break;
					}
				}
				//no 0 or negative element found
				if (shiftStart == -1)
					break;
				//Find how many successive 0 or negative elements
				int k = j + 1;
				for (; k < size; k++)
				{
					if (arrayInt[k] > 0)
						break;
				}
				leftShift(arrayInt, shiftStart, k - 1, shiftBy, noOfSwaps);
				int totalElementsToShift = k - shiftStart;
				i += totalElementsToShift;
			}
			else
				i++;
		}

		cout << " || Output: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";
		cout << "\nNo. of Swaps: " << noOfSwaps;
	}

	// Traversing from left end.It maintains the order in left and right groups.Needs most no.of swaps
	void clustorArray4Simplified(int arrayInt[], int size)
	{
		double noOfSwaps = 0;
		cout << "\nInput: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";

		int separatingWall = 0;
		for (int i = 0; i < size;)
		{
			if (arrayInt[i] > 0)
			{
				int rotateStart = i;
				//Find next first 0 or negative element
				int j = i + 1;
				while (j < size && arrayInt[j] > 0) j++;
				//Find how many successive 0 or negative elements
				if (j == size)
					break;
				int k = j + 1;
				int rotateBy = 1;
				for (; k < size && arrayInt[k] <= 0; k++)
					rotateBy++;

				rightRotateBy(arrayInt, rotateStart, k - 1, rotateBy, noOfSwaps);
				i += rotateBy;
			}
			else
				i++;
		}

		cout << " || Output: ";
		for (int i = 0; i < size; i++)
			cout << arrayInt[i] << " ";
		cout << "\nNo. of Swaps: " << noOfSwaps;
	}

	void testClustorArray()
	{
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			int arrayInt2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			int arrayInt3[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			int arrayInt4[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			int arrayInt5[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
			int arrayInt2[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
			int arrayInt3[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
			int arrayInt4[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
			int arrayInt5[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
			int arrayInt2[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
			int arrayInt3[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
			int arrayInt4[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
			int arrayInt5[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt2[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt3[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt4[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt5[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt2[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt3[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt4[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
			int arrayInt5[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
			int arrayInt2[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
			int arrayInt3[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
			int arrayInt4[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
			int arrayInt5[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}
		{
			cout << "\n-----------------------------------------------------";
			int arrayInt1[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
			int arrayInt2[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
			int arrayInt3[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
			int arrayInt4[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
			int arrayInt5[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
			clustorArray1(arrayInt1, 10);
			clustorArray2(arrayInt2, 10);
			clustorArray3(arrayInt3, 10);
			clustorArray4(arrayInt4, 10);
			clustorArray4Simplified(arrayInt5, 10);
		}

		/*
			-----------------------------------------------------
			Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
			No. of Swaps: 0
			Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
			No. of Swaps: 0
			Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
			No. of Swaps: 0
			Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
			No. of Swaps: 0
			Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
			No. of Swaps: 0
			-----------------------------------------------------
			Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -10 -3 -9 -6 5 7 8 4 2
			No. of Swaps: 3
			Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 7 8 4 5
			No. of Swaps: 4
			Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 4 5 7 8
			No. of Swaps: 9
			Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 4 5 7 8
			No. of Swaps: 8
			Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 4 5 7 8
			No. of Swaps: 8
			-----------------------------------------------------
			Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -7 -6 -3 -5 4 2 1 8 9 10
			No. of Swaps: 3
			Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 2 1 4 8 9 10
			No. of Swaps: 4
			Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 1 2 4 8 9 10
			No. of Swaps: 7.5
			Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 1 2 4 8 9 10
			No. of Swaps: 6.5
			Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 1 2 4 8 9 10
			No. of Swaps: 6.5
			-----------------------------------------------------
			Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -10 -9 -8 -4 -5 -6 -7 3 2 1
			No. of Swaps: 3
			Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 2 3 1
			No. of Swaps: 7
			Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 1 2 3
			No. of Swaps: 14
			Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 1 2 3
			No. of Swaps: 5.5
			Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 1 2 3
			No. of Swaps: 5.5
			-----------------------------------------------------
			Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
			No. of Swaps: 0
			Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
			No. of Swaps: 0
			Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
			No. of Swaps: 0
			Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
			No. of Swaps: 0
			Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
			No. of Swaps: 0
			-----------------------------------------------------
			Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 3 2 1 8 9 10
			No. of Swaps: 3
			Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 2 3 1 8 9 10
			No. of Swaps: 4
			Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 1 2 3 8 9 10
			No. of Swaps: 8
			Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 1 2 3 8 9 10
			No. of Swaps: 4
			Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 1 2 3 8 9 10
			No. of Swaps: 4
			-----------------------------------------------------
			Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 0 -5 -3 0 0 7 4
			No. of Swaps: 2
			Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 7 4
			No. of Swaps: 5
			Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 4 7
			No. of Swaps: 6.5
			Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 4 7
			No. of Swaps: 5
			Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 4 7
			No. of Swaps: 5

		*/
	}

	//-----------------------------------------------------------------------------------------------------
	//Given an array and a number n, find all the pairs from array that sum upto n

	struct Pair
	{
		int first;
		int second;
	};

	//Assuming the numbers provided are non-negative and non repeating
	void getPairs(vector<Pair>& result, vector<int>& input, int sum)
	{
		vector<bool> cache(sum + 1);
		for (int i = 0; i <= sum; i++)
			cache[i] = false;

		for (int i = 0; i < input.size(); i++)
			if (input[i] <= sum)
				cache[input[i]] = true;

		for (int i = 0; i <= sum / 2; i++)
			if (cache[i] == true && cache[sum - i] == true)
			{
				Pair newPair;
				newPair.first = i;
				newPair.second = sum - i;
				result.push_back(newPair);
			}
	}

	void testFindPairsFromArrayThatSumUptoNumberFirstApproach()
	{
		cout << "\n --------- testFindPairsFromArrayThatSumUptoNumberFirstApproach --------";

		vector<Pair> result;
		vector<int> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		int sum = 15;
		getPairs(result, input, sum);
		for (int i = 0; i < result.size(); i++)
			cout << "\nPair: " << result[i].first << " ," << result[i].second;
	}

	// ---------------- Second approach

	/* FOLLOWING FUNCTIONS ARE ONLY FOR SORTING
	PURPOSE */
	void exchange(int *a, int *b)
	{
		int temp;
		temp = *a;
		*a = *b;
		*b = temp;
	}

	int partition(int A[], int si, int ei)
	{
		int x = A[ei];
		int i = (si - 1);
		int j;

		for (j = si; j <= ei - 1; j++)
		{
			if (A[j] <= x)
			{
				i++;
				exchange(&A[i], &A[j]);
			}
		}
		exchange(&A[i + 1], &A[ei]);
		return (i + 1);
	}

	//For better implementation of quicksort, refer "sorting.cpp"

	/* Implementation of Quick Sort
	A[] --> Array to be sorted
	si  --> Starting index
	ei  --> Ending index
	*/
	void quickSort(int A[], int si, int ei)
	{
		int pi;    /* Partitioning index */
		if (si < ei)
		{
			pi = partition(A, si, ei);
			quickSort(A, si, pi - 1);
			quickSort(A, pi + 1, ei);
		}
	}

	bool hasArrayTwoCandidates(int A[], int arr_size, int sum)
	{
		int l, r;

		/* Sort the elements */
		quickSort(A, 0, arr_size - 1);

		/* Now look for the two candidates in the sorted
		array*/
		l = 0;
		r = arr_size - 1;
		while (l < r)
		{
			if (A[l] + A[r] == sum)
				return 1;
			else if (A[l] + A[r] < sum)
				l++;
			else // A[i] + A[j] > sum
				r--;
		}
		return 0;
	}

	void testFindPairsFromArrayThatSumUptoNumberSecondApproach()
	{
		cout << "\n --------- testFindPairsFromArrayThatSumUptoNumberSecondApproach --------";

		int A[] = { 1, 4, 45, 6, 10, -8 };
		int n = 16;
		int arr_size = 6;

		if (hasArrayTwoCandidates(A, arr_size, n))
			printf("\nArray has two elements with sum 16");
		else
			printf("\nArray doesn't have two elements with sum 16 ");

		getchar();
	}

	// ----------------------- Third approach - Best approach


	bool has_pair(vector<int>& data, int sum)
	{
		if (data.size() == 0)
		{
			return (sum == 0);
		}
		unordered_multiset <int> mys; // To make it work with repeated numbers/pairs

		for (int i = 0; i < data.size(); i++)
		{
			unordered_set<int>::const_iterator got = mys.find(sum - data[i]);
			if (got != mys.end())
			{
				cout << "\nThe pair found and they are " << data[i] << " and " << sum - data[i];
				mys.erase(got);
			}
			else
				mys.insert(data[i]);
		}
		return false;
	}

	void testFindPairsFromArrayThatSumUptoNumberThirdApproach()
	{
		cout << "\n --------- testFindPairsFromArrayThatSumUptoNumberThirdApproach --------";
		vector<int> data = { 1, 4, 45, 7, 7, 6, 6, -8, 8, 5, 5 };
		int sum = 12;
		has_pair(data, sum);
	}

	void testFindPairsFromArrayThatSumUptoNumber()
	{
		testFindPairsFromArrayThatSumUptoNumberFirstApproach();
		testFindPairsFromArrayThatSumUptoNumberSecondApproach();
		testFindPairsFromArrayThatSumUptoNumberThirdApproach();
	}

	//-----------------------------------------------------------------------------------------------
	//Implement power function

	double getPowerOfNum(double base, int index)
	{
		if (base == 0) return 0;

		bool isNegativeIndex = false;
		if (index < 0)
		{
			isNegativeIndex = true;
			index = -index;
		}
		double result = 1;
		for (int i = 0; i < index; i++)
			result *= base;
		if (isNegativeIndex)
			return 1 / result;
		else
			return result;
	}

	double getPowerOfNumSecondMethod(double base, int index)
	{
		if (base == 0) return 0;

		bool isNegativeIndex = false;
		if (index < 0)
		{
			isNegativeIndex = true;
			index = -index;
		}
		double result = 1;
		if (index == 0)
			return result;
		else if (index == 1)
			return base;
		else if (index % 2 == 0)
			result = getPowerOfNumSecondMethod(base * base, index / 2);
		else
			result = base * getPowerOfNumSecondMethod(base * base, index / 2);

		if (isNegativeIndex)
			return 1 / result;
		else
			return result;
	}

	double getPowerOfNumThirdMethod(double base, int index)
	{
		if (base == 0) return 0;
		if (index == 0) return 1; //This case is NOT handled below, and this shoudl be after base = 0, because 0^0 = 0
		if (base == 1) return 1;
		if (base == -1) return (index & 1) ? -1 : 1;

		index = abs(index);

		double result = 1;
		if (index % 2 != 0)
			result = base;
		for (; index > 1; index /= 2)
			base = base * base;

		result = result * base;

		return (index > 0) ? result : 1 / result;
	}

	void testPowerFunction()
	{
		double base;
		int index;

		for (double base : {-0.12, -1.0, 0.0, 1.0, 1.5, 2.345})
			for (int index : {-3, -2, -1, 0, 1, 2, 3})
			{
				cout << "\n\nbase = " << base << " index = " << index;
				cout << "\nFirst method: " << getPowerOfNum(base, index);
				cout << "\nSecond method: " << getPowerOfNum(base, index);
				cout << "\nThird method: " << getPowerOfNum(base, index);
			}
	}

	//-----------------------------------------------------------------------------------------------
	//Merge the set of timings/numbers
	//Find the free time slot of friends when busy time sets are given

	struct BusyTime
	{
		BusyTime(double startIn, double endIn)
			:start(startIn), end(endIn)
		{}
		double start, end;
	};

	void mergeBusyTimes(map<double, double>& total, vector<vector<BusyTime>>& individuals)
	{
		for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
		{
			for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
			{
				BusyTime current = individuals[numFriends][friendBusyTimeIndex];
				//Find the entry in map for current.start,
				//If found, compare end time and update if current.end > mapEntry.end
				//else get closest entry and merge if there is any common time
				//else add a new entry
				//We need to iterate through map if we dont find entry with same start point
			}
		}
	}

	void testInviteFriendsForParty()
	{
		//Convert the time into 24 hr format for easy comparison
		//Assume that these timings belong to same day and the problem boundaries are limited to that day
		//BusyTime busyTime1[] = { {8, 9}, {10.30, 11}, {5, 6} };
		vector<BusyTime> busyTime1 = { { 8, 9 }, { 10.30, 11 }, { 17, 18 } };
		//BusyTime busyTime2[] = { { 7, 8 },{ 3, 4 },{ 1, 2 }, {12.30, 1} };
		vector<BusyTime> busyTime2 = { { 7, 8 },{ 15, 16 },{ 13, 14 },{ 12.30, 13 } };
		//BusyTime busyTime3[] = { { 5, 6 }, { 1, 2 }, { 8, 9 } };
		vector<BusyTime> busyTime3 = { { 5, 6 },{ 13, 14 },{ 20, 21 } };


		vector<vector<BusyTime>> individuals;
		individuals.push_back(busyTime2);
		individuals.push_back(busyTime3);

		//use map of start time to end time to ensure sorting of start time to reduce the search complexity => O(nlogn)
		map<double, double> total;
		for (int i = 0; i < busyTime1.size(); i++)
		{
			BusyTime time = busyTime1[i];
			total[time.start] = time.end;
		}

		mergeBusyTimes(total, individuals);
	}

	//------------------------------------------------

	void scratchOutBusyTime(map<double, int>& result, int size, double shortestInterval, vector<vector<BusyTime>>& individuals)
	{
		for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
		{
			for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
			{
				BusyTime current = individuals[numFriends][friendBusyTimeIndex];
				for (double start = current.start; start < current.end; start += shortestInterval)
					result[int(start / shortestInterval)] = 1;
			}
		}
	}

	double findShortestInterval(vector<vector<BusyTime>>& individuals)
	{
		double shortestInterval = 24; //initialize with total hrs to find minimum
		for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
		{
			for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
			{
				BusyTime current = individuals[numFriends][friendBusyTimeIndex];
				double duration = current.end - current.start;
				if (shortestInterval > duration)
					shortestInterval = duration;
			}
		}

		return shortestInterval;
	}

	double findGCD(double first, double second)
	{
		const double accuracy = 0.000001;
		double min = first;
		if (min > second)
			min = second;
		double ratio1 = first / min;
		double ratio2 = second / min;
		if (fabs(ratio1 - int(ratio1)) < accuracy && fabs(ratio2 - int(ratio2)) < accuracy)
			return min;
		else
			return findGCD(min, fabs(first - second));
	}

	double findShortestIntervalCorrectWay(vector<vector<BusyTime>>& individuals)
	{
		double shortestInterval = 24.0; //initialize with total hrs to find minimum
		for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
		{
			for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
			{
				BusyTime current = individuals[numFriends][friendBusyTimeIndex];
				double duration = current.start - int(current.start);
				shortestInterval = findGCD(shortestInterval, duration);
				duration = current.end - int(current.end);
				shortestInterval = findGCD(shortestInterval, duration);
			}
		}

		return shortestInterval;
	}

	void testScratchOutBusyTime()
	{
		//Convert the time into 24 hr format for easy comparison
		//Assume that these timings belong to same day and the problem boundaries are limited to that day
		//Another best way is to convert it into minutes to avoid floating point operation errors
		//BusyTime busyTime1[] = { {8, 9}, {10.30, 11}, {5, 6} };
		vector<BusyTime> busyTime1 = { { 8, 9 },{ 10.30, 11 },{ 17, 18 } };
		//BusyTime busyTime2[] = { { 7, 8 },{ 3, 4 },{ 1, 2 }, {12.30, 1} };
		vector<BusyTime> busyTime2 = { { 7, 8 },{ 15, 16 },{ 13, 14 },{ 12.30, 13 } };
		//BusyTime busyTime3[] = { { 5, 6 }, { 1, 2 }, { 8, 9 } };
		vector<BusyTime> busyTime3 = { { 5, 6 },{ 13, 14 },{ 20, 21 } };

		vector<vector<BusyTime>> individuals;
		individuals.push_back(busyTime2);
		individuals.push_back(busyTime3);

		//Shortest duration finding wont work if some friend in above set has busy duration like 20.10 - 21.35
		//In this case, the shortest duration may be 
		double shortestInterval = findShortestIntervalCorrectWay(individuals);
		const int size = 24.0 / shortestInterval;
		double* timePoints = new double[size];
		timePoints[0] = 0.0;
		for (int i = 1; i < size; i++)
		{
			timePoints[i] = timePoints[i - 1] + shortestInterval;
		}
		map<double, int> result;
		for (int i = 0; i < size; i++)
		{
			result[timePoints[i]] = 0;
		}
		scratchOutBusyTime(result, size, shortestInterval, individuals);
	}

	void arrayOfReferences()
	{
		/*

		C++ Standard 8.3.2/4:
		There shall be no references to references, no arrays of references, and no pointers to references.

		A reference is analogous to an alias.
		References are not objects. They don't have storage of their own, they just reference existing objects.
		For this reason it doesn't make sense to have arrays of references.

		struct cintref
		{
		cintref(const int & ref) : ref(ref) {}
		operator const int &() { return ref; }
		private:
		const int & ref;
		void operator=(const cintref &);
		};

		int main()
		{
		int a=1,b=2,c=3;
		//typedef const int &  cintref;
		cintref arr[] = {a,b,c,8};
		}



		*/
	}

	//#define MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(a, b) {int x[((a - b) == 0 ? 0 : 1)];}
	//#define MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(a, b) {int x[((a - b) != 0 ? 0 : 1)];}
#define MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(a, b) {int x[((a == b) ? 0 : 1)];}
#define MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(a, b) {int x[((a == b) ? 1 : 0)];}

#define MACRO_COMPILE_TIME_ASSERT(EXPRESSION) {int x[((EXPRESSION) ? 0 : 1)];} //Asserts when expression is true

//inline void inlinefun_compile_time_assert_if_equal(const int a, const int b)
//{
//	int x[((a - b) == 0 ? 0 : 1)];
//}
//
//inline void inlinefun_compile_time_assert_if_not_equal(const int a, const int b)
//{
//	int x[((a - b) != 0 ? 0 : 1)];
//}

	void testStaticAssert()
	{
		//inlinefun_compile_time_assert_if_equal(sizeof(double), sizeof(int));
		//inlinefun_compile_time_assert_if_not_equal(sizeof(double), sizeof(int));

		MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(sizeof(double), sizeof(int));
		//MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(4, sizeof(int));			//error C2466: cannot allocate an array of constant size 0
		//MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(sizeof(double), sizeof(int));		 //error C2466: cannot allocate an array of constant size 0
		MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(4, sizeof(int));

		MACRO_COMPILE_TIME_ASSERT(sizeof(double) == sizeof(int));
		//MACRO_COMPILE_TIME_ASSERT(4 == sizeof(int));		//error C2466: cannot allocate an array of constant size 0
	}


	//Take a vector of 1000 randon integers, and convert it into min heap (in place) (How to heapify?)

	class A
	{
	public:
		static void fun()
		{
			A a;
			a.one();
			a.two();
		}

	private:
		void one() {}
		void two() {}
	};

	void testBasicEncapsulation()
	{
		//fun() is static function, but its member function, so it can access A's private members
		A::fun();
	}

#if defined _MSC_VER

	void printCurrentProgramName2()
	{
		const int len = 256;
		WCHAR pBuf[len];
		int bytes = GetModuleFileName(NULL, pBuf, len);
		if (bytes == 0)
			return;
		else
			wcout << "\nCurrent Program: " << pBuf << endl;
	}

#elif defined __GNUC__

	void printCurrentProgramName2()
	{
		const int len = 256;
		char pBuf[len];
		int bytes = GetModuleFileName(NULL, pBuf, len);
		if (bytes == 0)
			return;
		else
			cout << "\nCurrent Program Name: " << pBuf << endl;
	}

#elif defined __Unix__

	void printCurrentProgramName2()
	{
		char buff[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
		if (len != -1)
		{
			buff[len] = '\0';
			cout << "\nRunning Program: " << std::string(buff) << endl;
		}
	}

#endif

	void printCurrentDirectory()
	{
		char cCurrentPath[FILENAME_MAX];
		char* returnString = GET_CWD(cCurrentPath);
		cout << "The current working directory is: " << cCurrentPath << endl;
		cout << "The retval is: " << returnString << endl;
	}



	//------------------------------------------------------------------------------------------------
	void miscellaneous()
	{
		//checkAllDataTypeLowerAndUpperLimits();
		//tryVariousPrintingFormatsOfNumbers();
		//testEndianess();
		//testCheckIfBracketSeqValid();
		testClustorArray();
		//testCombinations();
		//testFindPairsFromArrayThatSumUptoNumber();
		//testPowerFunction();
		printCurrentProgramName2();
		//printCurrentDirectory();
	}

}