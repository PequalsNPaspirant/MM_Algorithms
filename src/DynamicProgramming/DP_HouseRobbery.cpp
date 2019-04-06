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
#include "DynamicProgramming/DP_HouseRobbery.h"

namespace mm {

	//========================= Method 1 =========================
	int HouseRobbery::solve(const vector<int>& treasuryInput, vector<int>& solution)
	{
		vector<int> totalAmount(treasuryInput.size() + 1);
		vector<vector<int>> solutions(treasuryInput.size() + 1, vector<int>(0)); //solution stores the indices of selected houses 

		totalAmount[0] = 0;

		//Assume that we have only one house (house at index 0) and so we selected it as a solution
		totalAmount[1] = treasuryInput[0];
		solutions[1].push_back(0);

		for (int currentHouseIndex = 1; currentHouseIndex < treasuryInput.size(); ++currentHouseIndex)
		{
			int currentTreasury = treasuryInput[currentHouseIndex];
			if ((totalAmount[currentHouseIndex - 1] + currentTreasury) > totalAmount[currentHouseIndex])
			{
				//We are discarding last house and selecting current house
				totalAmount[currentHouseIndex + 1] = totalAmount[currentHouseIndex - 1] + currentTreasury;
				solutions[currentHouseIndex + 1] = solutions[currentHouseIndex - 1];
				solutions[currentHouseIndex + 1].push_back(currentHouseIndex);
			}
			else
			{
				//We are discarding current house
				totalAmount[currentHouseIndex + 1] = totalAmount[currentHouseIndex];
				solutions[currentHouseIndex + 1] = solutions[currentHouseIndex];
			}
		}

		solution = solutions[treasuryInput.size()];
		return totalAmount[treasuryInput.size()];
	}

	//========================= Method 2 =========================
	int HouseRobbery::solve2(const vector<int>& treasuryInput, vector<int>& solution)
	{
		//Solution vector stores the amount robbed from each house
		vector<int> solutionPrevToPrev(treasuryInput.size(), 0);
		vector<int> solutionPrev(treasuryInput.size(), 0);

		int amountPrevToPrev = 0;
		vector<int>* pSolutionPrevToPrev = &solutionPrevToPrev;

		//Assume that we have only one house (house at index 0) and so we selected it as a solution
		int amountPrev = treasuryInput[0];
		vector<int>* pSolutionPrev = &solutionPrev;
		(*pSolutionPrev)[0] = treasuryInput[0];

		int amountCurrent = amountPrev;
		vector<int>* pSolutionCurrent = pSolutionPrev;

		for (int currentHouseIndex = 1; currentHouseIndex < treasuryInput.size(); ++currentHouseIndex)
		{
			if ((amountPrevToPrev + treasuryInput[currentHouseIndex]) > amountPrev)
			{
				//We are discarding last house and selecting current house
				amountCurrent = amountPrevToPrev + treasuryInput[currentHouseIndex];
				pSolutionCurrent = pSolutionPrevToPrev;
				(*pSolutionCurrent)[currentHouseIndex - 1] = 0; //This is optional, all objects are already zero
				(*pSolutionCurrent)[currentHouseIndex] = treasuryInput[currentHouseIndex];
			}
			else
			{
				//We are discarding current house
				amountCurrent = amountPrev;
				pSolutionCurrent = pSolutionPrev;
				(*pSolutionCurrent)[currentHouseIndex] = 0; //This is optional, all objects are already zero
			}

			amountPrevToPrev = amountPrev;
			amountPrev = amountCurrent;

			pSolutionPrevToPrev = pSolutionPrev;
			pSolutionPrev = pSolutionCurrent;
		}

		solution = *pSolutionCurrent;
		return amountCurrent;
	}

	//========================= Method 3 =========================

	HouseRobberyProblem::House::House(int houseIndex, int treasury)
		: m_houseIndex(houseIndex),
		m_treasury(treasury)
	{
	}

	HouseRobberyProblem::Solution::Solution()
		: m_totalAmount(0)
	{
	}

	HouseRobberyProblem::Solution::Solution(const House& house)
	{
		m_totalAmount = house.m_treasury;
		m_houseNumbers.push_back(house.m_houseIndex);
	}

	const HouseRobberyProblem::Solution HouseRobberyProblem::Solution::operator+(const House& house) const
	{
		Solution retVal;
		retVal.m_totalAmount = m_totalAmount + house.m_treasury;
		retVal.m_houseNumbers = m_houseNumbers;
		retVal.m_houseNumbers.push_back(house.m_houseIndex);
		return retVal;
	}

	HouseRobberyProblem::HouseRobberyProblem(const vector<int>& treasurySet)
		: m_numHouses(treasurySet.size()),
		m_treasurySet(treasurySet),
		m_allSolutions(m_numHouses + 1)
	{
	}

	HouseRobberyProblem::~HouseRobberyProblem()
	{
	}

	HouseRobberyProblem::House HouseRobberyProblem::getHouse(int houseIndex)
	{
		return House(houseIndex - 1, m_treasurySet[houseIndex - 1]);
	}

	int HouseRobberyProblem::solve(vector<int>& solution)
	{
		m_allSolutions[0] = Solution();
		m_allSolutions[1] = Solution(getHouse(1));
		for (int i = 2; i <= m_numHouses; i++)
		{
			m_allSolutions[i] = max(m_allSolutions[i - 1], m_allSolutions[i - 2] + getHouse(i));
		}

		solution = m_allSolutions[m_numHouses].m_houseNumbers;
		return m_allSolutions[m_numHouses].m_totalAmount;
	}

}
