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

	//-------------------------------------------------------------------------------------------------------------------------
	//			House Robbery Puzzle
	//-------------------------------------------------------------------------------------------------------------------------
	/*
	Problem Description:
	A robber enters a colony of houses numbered from 1 to n. Every house has a number printed on the top of it.
	That number is the amount of money inside that house.
	However, the only constraint stopping you from robbing each of them is that adjacent houses have security system connected
	and it will automatically contact the police if two adjacent houses were broken into.
	How can the robber maximise his robbery?
	*/

	class HouseRobbery
	{
	public:
		//========================= Method 1 =========================
		static int solve(const vector<int>& treasuryInput, vector<int>& solution);
		//========================= Method 2 =========================
		static int solve2(const vector<int>& treasuryInput, vector<int>& solution);
	};

	//========================= Method 3 =========================
	class HouseRobberyProblem
	{
	private:
		struct House
		{
			House(int houseIndex, int treasury);
			int m_houseIndex;
			int m_treasury;
		};

		class Solution
		{
		public:
			Solution();
			Solution(const House& house);
			const Solution operator+(const House& house) const;
			friend const Solution& max(const Solution& lhs, const Solution& rhs)
			{
				if (lhs.m_totalAmount >= rhs.m_totalAmount)
					return lhs;
				else
					return rhs;
			}

			//private:
			int m_totalAmount;
			vector<int> m_houseNumbers;
		};

		House getHouse(int houseIndex);

		int m_numHouses;
		const vector<int>& m_treasurySet;
		vector<Solution> m_allSolutions;

	public:
		HouseRobberyProblem(const vector<int>& treasurySet);
		~HouseRobberyProblem();
		int solve(vector<int>& solution);
	};
}
