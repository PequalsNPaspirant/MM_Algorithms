//Goal
//Solve puzzles using dynamic programming

#include <iostream>
#include <vector>
#include <time.h>
#include <sstream> //for std::stringstream
#include <fstream> //for file read/write
#include <iomanip> //for setw(n)
using namespace std;

#include "Maths/Maths_Permutations.h"
#include "DynamicProgramming/DP_MaxSumSequence.h"

namespace mm {

		// Global Configurtion
		bool displayOnScreen_DP = false;

		bool writeToTextFile_DP = true;
		const string reportLocation_DP = "C:\\@_Programming\\";
		const string textFileName_DP = "dymanicPogrammingReport.txt";
		//--------------------------------------------------------------------------------------------

		
		void testMaxSumSequence(const vector<int>& array)
		{
			MaxSumSequence problem(array);
			problem.solve();
			problem.printSolution();
			problem.writeSolutionToFile();
		}

		void testMaxSumSequence()
		{
			int numTestCases = 20;
			vector<int> array;

			cout << "\n\nProblm: MaxSumSequence\nTrying sets of size 1 to 20...";
			//Generate sets of sizes from 1 to 20
			srand(time(nullptr));
			for (int i = 1; i <= numTestCases; i++)
			{
				//Generate random treasury
				array.resize(i);
				for (int houseNumber = 1; houseNumber <= i; houseNumber++)
				{
					array[houseNumber - 1] = i - rand() % (i * 2);  //This will generate random numbers between i - (i * 2 - 1) and i i.e. from -i+1 to i
				}

				testMaxSumSequence(array);
			}

			cout << "\n\nTrying sets of size 20...";
			//Generate various sets of size 20
			srand(time(nullptr));
			int size = 20;
			array.resize(size);
			for (int i = 0; i < numTestCases; i++)
			{
				//Generate random treasury
				for (int j = 0; j < size; j++)
				{
					array[j] = size - rand() % (size * 2);
				}

				testMaxSumSequence(array);
			}

			cout << "\n\nTrying permutation sets of size 2...";
			bool repeatition = true;
			vector< vector<int> > output;
			Permutations::generatePermutations({ -1, 1 }, repeatition, output);
			for (size_t i = 0; i < output.size(); i++)
			{
				testMaxSumSequence(output[i]);
			}

			cout << "\n\nTrying permutation sets of size 3...";
			output.clear();
			Permutations::generatePermutations({ -1, 0, 1 }, repeatition, output);
			for (size_t i = 0; i < output.size(); i++)
			{
				testMaxSumSequence(output[i]);
			}

			cout << "\n\nTrying permutation sets of size 3...";
			output.clear();
			Permutations::generatePermutations({ -1, 2, 3 }, repeatition, output);
			for (size_t i = 0; i < output.size(); i++)
			{
				testMaxSumSequence(output[i]);
			}

			cout << "\n\nTrying permutation sets of size 4...";
			output.clear();
			Permutations::generatePermutations({ -1, -2, 3, 4 }, repeatition, output);
			for (size_t i = 0; i < output.size(); i++)
			{
				testMaxSumSequence(output[i]);
			}

			cout << "\n\nTrying permutation sets of size 5...";
			output.clear();
			Permutations::generatePermutations({ -1, -2, -3, 4, 5 }, repeatition, output);
			for (size_t i = 0; i < output.size(); i++)
			{
				testMaxSumSequence(output[i]);
			}

			MaxSumSequence::GetOutputFile().close();
			cout << "\n\nEnd of test.";
		}


	void dynamicProgrammingMaxSumSequence()
	{
		//testHouseRobberyProblem();
		testMaxSumSequence();
	}

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
