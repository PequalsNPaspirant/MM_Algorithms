//Goal
//Solve puzzles using dynamic programming

#pragma once

#include <iostream>
#include <vector>
#include <time.h>
#include <sstream> //for std::stringstream
#include <fstream> //for file read/write
#include <iomanip> //for setw(n)
using namespace std;

#include "my_math_lib/Permutations.h"

namespace mm {

		// Global Configurtion
		extern bool displayOnScreen_DP;
		extern bool writeToTextFile_DP;
		extern const string reportLocation_DP;
		extern const string textFileName_DP;
		//--------------------------------------------------------------------------------------------

	
		//-------------------------------------------------------------------------------------------------------------------------
		//			Array sequence having max sum
		//-------------------------------------------------------------------------------------------------------------------------

		/*
		Problem Description:
		Given an array of integers (positive, negative or zero), find any continuous sequence for which the sum is maximum.
		*/

		class MaxSumSequence
		{
		private:

			class Solution
			{
			public:
				Solution() : m_sum(0), m_start(0), m_end(0) {}

				friend const Solution& max(const Solution& first, const Solution& second, const Solution& third)
				{
					if (first.m_sum >= second.m_sum && first.m_sum >= third.m_sum)
						return first;
					else if (second.m_sum >= first.m_sum && second.m_sum >= third.m_sum)
						return second;
					else
						return third;
				}

				//private:
				int m_sum;
				int m_start;
				int m_end;
			};

			Solution createSolution(int numIndex)
			{
				Solution temp;
				temp.m_start = numIndex - 1;
				temp.m_end = numIndex - 1;
				temp.m_sum = m_array[numIndex - 1];
				return temp;
			}

			//Solution createSolution(int start, int end)
			//{
			//	Solution temp;
			//	temp.m_start = start - 1;
			//	temp.m_end = end - 1;
			//	temp.m_sum = 0;
			//	for(int i = temp.m_start; i <= temp.m_end; i++)
			//		temp.m_sum += m_numbers[i];
			//	return temp;
			//}

			Solution createSolution(const Solution& reference, int newEnd)
			{
				Solution temp;
				temp.m_start = reference.m_start;
				temp.m_end = newEnd - 1;
				temp.m_sum = reference.m_sum;
				for (int i = reference.m_end + 1; i <= temp.m_end; i++)
					temp.m_sum += m_array[i];
				return temp;
			}

			const vector<int>& m_array;
			vector<Solution> m_allSolutions;

		public:
			MaxSumSequence(const vector<int>& array)
				: m_array(array),
				m_allSolutions(array.size() + 1)
			{
			}

			~MaxSumSequence()
			{
			}

			void solve()
			{
				m_allSolutions[0] = Solution();
				m_allSolutions[1] = createSolution(1);
				for (size_t i = 2; i <= m_array.size(); i++)
				{
					//m_allSolutions[i] = max(m_allSolutions[i - 1], createSolution(m_allSolutions[i - 1].m_start, i), createSolution(i));
					m_allSolutions[i] = max(m_allSolutions[i - 1], createSolution(m_allSolutions[i - 1], i), createSolution(i));
				}
			}

			static ofstream& GetOutputFile()
			{
				static ofstream outputFile(reportLocation_DP + textFileName_DP + "_MaxSumSequence");
				return outputFile;
			}

			void createBuffer(std::ostream& base)
			{
				static int testCaseCounter;
				base << "\n\nTest Case No. = " << ++testCaseCounter;
				base << "\n\tTotal Numbers in array = " << m_array.size();
				base << "\n\tIndex  :";
				for (size_t i = 0; i < m_array.size(); i++)
				{
					base << setw(4) << i + 1;
					if (i < m_array.size() - 1)
						base << ",";
				}
				base << "\n\tNumbers:";
				for (size_t i = 0; i < m_array.size(); i++)
				{
					base << setw(4) << m_array[i];
					if (i < m_array.size() - 1)
						base << ",";
				}
				base << "\n\tSolution: Total Treasury = " << m_allSolutions[m_array.size()].m_sum;
				base << "  Start Index: " << m_allSolutions[m_array.size()].m_start + 1;
				base << "  End Index: " << m_allSolutions[m_array.size()].m_end + 1;
			}

			void printSolution()
			{
				if (displayOnScreen_DP)
					createBuffer(cout);
			}

			void writeSolutionToFile()
			{
				if (!writeToTextFile_DP)
					return;

				std::stringstream buffer;
				createBuffer(buffer);
				string outputString(buffer.str());

				GetOutputFile().write(outputString.c_str(), outputString.length());
			}
		};

		

		//-------------------------------------------------------------------------------------------------------------------------
		//			Longest common substring
		//-------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------------------------------------------
		//			Fibonacci numbers
		//-------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------------------------------------------
		//			??
		//-------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------------
	//			END
	//-------------------------------------------------------------------------------------------------------------------------




	/*

	#include <iostream>

	using namespace std;

	class myclass
	{
	public:
	myclass(int n);
	~myclass();
	void run();
	void toggle(char &charIn);
	void print();

	private:
	int mTotal;
	char mOpenValue;
	char mCloseValue;
	int mXorValue;
	char *mArray;
	};

	myclass::myclass(int n)
	{
	mTotal = n;
	mOpenValue = 'O';
	mCloseValue = 'C';
	mXorValue = mOpenValue + mCloseValue;
	mArray = new char[100];
	}

	myclass::~myclass()
	{
	delete [] mArray;
	}

	void myclass::run()
	{
	for(int i = 0; i , mTotal; i++)
	mArray[i] = mCloseValue;

	print();
	for(int i = 0; i < mTotal; i++)
	{
	int personIndex = i + 1;
	for(int windowIndex = i; windowIndex < mTotal; windowIndex += personIndex)
	toggle(mArray[windowIndex]);

	cout << "\nAfter personIndex = " << personIndex << "\n";
	print();
	}
	}

	void myclass::toggle(char &charIn)
	{
	charIn = mXorValue - charIn;
	}

	void myclass::print()
	{
	int openCount = 0, closeCount = 0;
	for(int windowIndex = 0; windowIndex < mTotal; windowIndex++)
	{
	if(mArray[windowIndex] == mOpenValue) openCount++;
	else closeCount++;
	}

	cout << "\nClose Count = " << closeCount;
	cout << "\nOpen Count = " << openCount;
	cout << endl << endl;

	for(int windowIndex = 0; windowIndex < mTotal; windowIndex++)
	{
	if(windowIndex % 5 == 0) cout << "   ";
	if(windowIndex % 10 == 0) cout << endl;
	cout << mArray[windowIndex] << " ";
	}
	}

	int main()
	{
	myclass m(100);
	m.run();

	return 0;
	}





	#include <iostream>

	using namespace std;

	class myclass
	{
	public:
	myclass(int n = 1);
	~myclass();
	void run();
	void toggle(char &charIn);
	void print();
	void setSize(int n);

	private:
	int mTotal;
	const char mOpenValue;
	const char mCloseValue;
	const int mXorValue;
	char *mArray;
	};

	myclass::myclass(int n *//* = 1 *//*)
		:mArray(NULL), mOpenValue('O'), mCloseValue('C'), mXorValue(mOpenValue + mCloseValue)
	{
	mTotal = n;
	mArray = new char[mTotal];
	for (int i = 0; i < mTotal; i++)
		mArray[i] = mCloseValue;

	cout << "\nInitial Values: ";
	print();
	}

	myclass::~myclass()
	{
		delete[] mArray;
	}

	void myclass::setSize(int n)
	{
		delete[] mArray;
		mTotal = n;
		mArray = new char[mTotal];
		for (int i = 0; i < mTotal; i++)
			mArray[i] = mCloseValue;
	}

	void myclass::run()
	{
		for (int i = 0; i < mTotal; i++)
		{
			int personIndex = i + 1;
			for (int windowIndex = i; windowIndex < mTotal; windowIndex += personIndex)
				toggle(mArray[windowIndex]);

			//cout << "\n\nAfter personIndex = " << personIndex << " ->";
			//print();
		}
		print();
	}

	void myclass::toggle(char &charIn)
	{
		charIn = mXorValue - charIn;
	}

	void myclass::print()
	{
		int openCount = 0, closeCount = 0;
		cout << "\nOpen Windows Index: ";
		for (int windowIndex = 0; windowIndex < mTotal; windowIndex++)
		{
			if (mArray[windowIndex] == mOpenValue)
			{
				cout << (windowIndex + 1) << " ";
				openCount++;
			}
			else
			{
				closeCount++;
			}
		}

		cout << "\nClose Count = " << closeCount << " Open Count = " << openCount << endl;

		//            for(int windowIndex = 0; windowIndex < mTotal; )
		//            {
		//                            cout << mArray[windowIndex++] << " ";
		//        if(windowIndex % 5 == 0) cout << "   ";
		//            if(windowIndex % 10 == 0) cout << endl;
		//            }
	}

	int main()
	{
		myclass m;
		for (int i = 0; i < 100; i++)
		{
			m.setSize(i);
			m.run();
			cin.get();
		}

		return 0;
	}





	*/

}
