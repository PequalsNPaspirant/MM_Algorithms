#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
#include <random>
#include <fstream>
#include <unordered_map>
#include <set>
#include <cassert>
using namespace std;

#include "Others/Others_FxSettlement.h"
#include "Utils/Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	inline std::size_t hash_combine_helper(std::size_t& seed) { return seed; }

	template <typename First, typename... Rest>
	static inline std::size_t hash_combine_helper(std::size_t& seed, const First& first, Rest... rest)  // Similar to boost::hash_combine<T>
	{
		/*
		The magic number below is the reciprocal of the golden ratio.
		Reference:
		https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
		http://burtleburtle.net/bob/hash/doobs.html
		phi = (1 + sqrt(5)) / 2
		2^32 / phi = 0x9e3779b9
		*/
		std::hash<First> hasher{};
		seed ^= hasher(first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return hash_combine_helper(seed, rest...);
	}

	template <typename... All>
	inline std::size_t hash_combine(All... all)
	{
		std::size_t seed = 0;
		return hash_combine_helper(seed, all...);
	}

	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2> &p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);
			return hash_combine(h1, h2);
		}
	};

	const vector<double> allFxRates{
		1.0,			//USD
		1.0 / 0.88,		//EUR
		1.0 / 1.31,		//CAD
		1.0 / 1.42,		//AUD
		1.0 / 0.80,		//GBP
		1.0 / 1.49,		//NZD
		1.0 / 107.89,	//JPY
		1.0 / 1.36,		//SGD
		1.0 / 0.98,		//CHF
		1.0 / 288.97,	//HUF
		1.0 / 18.99,	//MXN
		1.0 / 7.83,		//HKD
		1.0 / 9.37,		//SEK
		1.0 / 1177.32,	//KRW
		1.0 / 8.54,		//NOK
		1.0 / 68.56,	//INR
		1.0 / 22.69,	//CSK
		1.0 / 3.74, 	//BRL
		1.0 / 680.27,	//CLP
		1.0 / 6.88,		//CNY
		1.0 / 22.71,	//CZK
		1.0 / 6.63,		//DKK
		1.0 / 3.55,		//ILS
		1.0 / 31.06,	//TWD
		1.0 / 63.13,	//RUB
		1.0 / 157.73	//PKR
						//maxCurrency
	};

	void createTestCases()
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist_double(100.0, 100000.0);
		
		int filePrefix = 0;
		for (int c = 3; c <= maxCurrencies; ++c)
		{
			int numMembers = c;
			int numCurrencies = c;
			std::uniform_int_distribution<int> dist_member(0, numMembers - 1);
			std::uniform_int_distribution<int> dist_curr(0, numCurrencies - 1);
			int numTrades = numCurrencies * (numCurrencies - 1) / 2;
			for (int ti = 0; ti < numTestCases; ++ti)
			{
				//Create Trades
				vector<Trade> trades;
				for (int tradeIndex = 0; tradeIndex < numTrades; ++tradeIndex)
				{
					int buyCurr = dist_curr(mt);
					int sellCurr = dist_curr(mt);
					if (buyCurr == sellCurr)
						sellCurr = (sellCurr + 1) % numCurrencies;

					int party = dist_member(mt);
					int cParty = dist_member(mt);
					if (party == cParty)
						cParty = (cParty + 1) % numCurrencies;

					double buyAmount = dist_double(mt);
					double sellAmount = dist_double(mt);
					trades.push_back({ tradeIndex, party, cParty, Currency(buyCurr), Currency(sellCurr), buyAmount, sellAmount, false });
				}

				//Create SPLs
				vector< vector<double> > spl(numMembers, vector<double>(numCurrencies, 0.0));
				for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
				{
					for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
					{
						double splAmount = dist_double(mt);
						spl[memberIndex][currencyIndex] = splAmount;
					}
				}

				//create ASPLs
				vector<double> aspl(numMembers, 0.0);
				for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
				{
					double amount = dist_double(mt);
					aspl[memberIndex] = amount;
				}

				//create initial balances
				vector< vector<double> > initialBalance(numMembers, vector<double>(numCurrencies, 0.0));
				for (int memberIndex = 0; memberIndex < numMembers; ++memberIndex)
				{
					for (int currencyIndex = 0; currencyIndex < numCurrencies; ++currencyIndex)
					{
						double balance = dist_double(mt);
						initialBalance[memberIndex][currencyIndex] = balance;
					}
				}

				double settledAmount = 500.0;
				vector<int> settledTradeIds{ 1 };

				//Print all
				//cout << "\n{ //trades = " << trades.size() << " members = " << numMembers << " currencies = " << numCurrencies;
				//cout << "\nvector<Trade> trades " << trades << " ;";
				//cout << "\nvector< vector<double> > spl " << spl << " ;";
				//cout << "\nvector<double> aspl " << aspl << " ;";
				//cout << "\nvector< vector<double> > initialBalance " << initialBalance << " ;";
				//cout << "\ndouble settledAmount = 500.0;";
				//cout << "\nvector<int> settledTradeIds{ 1 };";
				//cout << "\ntestCases.push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(allFxRates), settledAmount, std::move(settledTradeIds) });";

				//Add to test data
				//getTestCases().push_back({ std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(allFxRates), settledAmount, std::move(settledTradeIds) });

				//Write to csv files

				//std::filesystem::exists("helloworld.txt"); //available in C++17 onwards
				auto checkIfFileExists = [](const std::string& name) -> bool {
					ifstream f(name);
					return f.good();
				};
				while (checkIfFileExists(testDataPath + to_string(++filePrefix) + "_" + tradesFileName));

				string tradesFileFullName{ testDataPath + to_string(filePrefix) + "_" + tradesFileName };
				string initialBalancesFileFullName{ testDataPath + to_string(filePrefix) + "_" + initialBalancesFileName };
				string splFileFullName{ testDataPath + to_string(filePrefix) + "_" + splFileName };
				string asplFileFullName{ testDataPath + to_string(filePrefix) + "_" + asplFileName };
				string fxRatesFileFullName{ testDataPath + to_string(filePrefix) + "_" + fxRatesFileName };

				ofstream tradesFile;
				ofstream initialBalancesFile;
				ofstream splFile;
				ofstream asplFile;
				ofstream fxRatesFile;
				ofstream resultsFile;

				try
				{
					tradesFile.open(tradesFileFullName);
					initialBalancesFile.open(initialBalancesFileFullName);
					splFile.open(splFileFullName);
					asplFile.open(asplFileFullName);
					fxRatesFile.open(fxRatesFileFullName);
				}
				catch (std::ofstream::failure &writeErr)
				{
					cout << "\nERROR: Can not open file: " << endl;
					throw;
				}
				catch (...)
				{
					cout << "\nUNKNOWN ERROR while opening file: " << endl;
					throw;
				}

				//Write trades
				if(tradesFile.is_open())
				{
					string columnNames{ "ID,PARTY_ID,CP_ID,BUY_CURR,SELL_CURR,BUY_VOL,SELL_VOL" };
					tradesFile.write(columnNames.c_str(), columnNames.length());
					for (int i = 0; i < trades.size(); ++i)
					{
						string row{
							"\n" +
							to_string(trades[i].id_) + "," +
							to_string(trades[i].partyId_) + "," +
							to_string(trades[i].cPartyId_) + "," +
							to_string(static_cast<int>(trades[i].buyCurr_)) + "," +
							to_string(static_cast<int>(trades[i].sellCurr_)) + "," +
							to_string_max_precision(trades[i].buyVol_) + "," +
							to_string_max_precision(trades[i].sellVol_)
						};
						tradesFile.write(row.c_str(), row.length());
					}
				}
				//Write initial_balances
				if (initialBalancesFile.is_open())
				{
					string columnNames{ "MEMBER_ID,CURR_ID,BALANCE" };
					initialBalancesFile.write(columnNames.c_str(), columnNames.length());
					for (int m = 0; m < initialBalance.size(); ++m)
					{
						for (int c = 0; c < initialBalance[m].size(); ++c)
						{
							string row{
								"\n" +
								to_string(m) + "," +
								to_string(c) + "," +
								to_string_max_precision(initialBalance[m][c])
							};
							initialBalancesFile.write(row.c_str(), row.length());
						}
					}
				}
				//Write spl
				if (splFile.is_open())
				{
					string columnNames{ "MEMBER_ID,CURR_ID,SPL" };
					splFile.write(columnNames.c_str(), columnNames.length());
					for (int m = 0; m < spl.size(); ++m)
					{
						for (int c = 0; c < spl[m].size(); ++c)
						{
							string row{
								"\n" +
								to_string(m) + "," +
								to_string(c) + "," +
								to_string_max_precision(spl[m][c])
							};
							splFile.write(row.c_str(), row.length());
						}
					}
				}
				//Write aspl
				if (asplFile.is_open())
				{
					string columnNames{ "MEMBER_ID,ASPL" };
					asplFile.write(columnNames.c_str(), columnNames.length());
					for (int m = 0; m < aspl.size(); ++m)
					{
						string row{
							"\n" +
							to_string(m) + "," +
							to_string_max_precision(aspl[m])
						};
						asplFile.write(row.c_str(), row.length());
					}
				}
				//Write fx_rates
				if (fxRatesFile.is_open())
				{
					string columnNames{ "CURR_ID,FX_RATE" };
					fxRatesFile.write(columnNames.c_str(), columnNames.length());
					for (int c = 0; c < allFxRates.size(); ++c)
					{
						string row{
							"\n" +
							to_string(c) + "," +
							to_string_max_precision(allFxRates[c])
						};
						fxRatesFile.write(row.c_str(), row.length());
					}
				}
			}
		}
	}

	vector<TestCase>& getTestCases()
	{
		static vector<TestCase> testCases;

		if (!testCases.empty())
			return testCases;

		//Add hardcoded test cases
		{//1 trade, 2 members, 2 currencies
			vector<Trade> trades{
			{ 0, 0, 1, Currency(0), Currency(1), 500.0, 440.0, false }
			};
			vector< vector<double> > spl{
			{ 500.0, 400.0 },
			{ 300.0, 250.0 }
			};
			vector<double> aspl{
				2000.0,
				2000.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 440.0 },
			{ 500.0, 0.0 },
			};
			vector<double> fxRates{
				1.0, //USD = 1
				1.14 //EUR
			};
			double settledAmount = 500.0;
			vector<int> settledTradeIds{ 0 };
			testCases.push_back({ "", std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(fxRates), settledAmount, std::move(settledTradeIds) });
		}

		{//1 trade, 2 members, 2 currencies
			vector<Trade> trades{
				{ 0, 0, 1, Currency(0), Currency(1), 500.0, 440.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 0.0 },
			{ 0.0, 0.0 }
			};
			vector<double> aspl{
				0.0,
				0.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 440.0 },
			{ 500.0, 0.0 },
			};
			vector<double> fxRates{
				1.0, //USD = 1
				1.14 //EUR
			};
			double settledAmount = 500.0;
			vector<int> settledTradeIds{ 0 };
			testCases.push_back({ "", std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(fxRates), settledAmount, std::move(settledTradeIds) });
		}

		{//1 trade, 2 members, 2 currencies
			vector<Trade> trades{
				{ 0, 0, 1, Currency(0), Currency(1), 500.0, 440.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 440.0 },
			{ 500.0, 0.0 }
			};
			vector<double> aspl{
				500.0,
				500.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 10.0 },
			{ 10.0, 0.0 },
			};
			vector<double> fxRates{
				1.0, //USD = 1
				1.14 //EUR
			};
			double settledAmount = 500.0;
			vector<int> settledTradeIds{ 0 };
			testCases.push_back({ "", std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(fxRates), settledAmount, std::move(settledTradeIds) });
		}

		{//2 trades, 2 members, 2 currencies
			vector<Trade> trades{
			{ 0, 0, 1, Currency(0), Currency(1), 500.0, 440.0, false },
			{ 1, 0, 1, Currency(1), Currency(0), 440.0, 500.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 0.0 },
			{ 0.0, 0.0 }
			};
			vector<double> aspl{
				0.0,
				0.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 0.0 },
			{ 0.0, 0.0 },
			};
			vector<double> fxRates{
				1.0, //USD = 1
				500.0/440.0 //EUR
			};
			double settledAmount = 1000.0;
			vector<int> settledTradeIds{ 0, 1 };
			testCases.push_back({ "", std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(fxRates), settledAmount, std::move(settledTradeIds) });
		}

		{//3 trades, 3 members, 3 currencies
			vector<Trade> trades{
			{ 0, 0, 1, Currency(0), Currency(1), 100.0, 88.0, false },
			{ 1, 1, 2, Currency(1), Currency(2), 88.0, 131.0, false },
			{ 2, 2, 0, Currency(2), Currency(0), 131.0, 100.0, false }
			};
			vector< vector<double> > spl{
			{ 0.0, 0.0, 131.0 },
			{ 100.0, 0.0, 0.0 },
			{ 0.0, 88.0, 0.0 }
			};
			vector<double> aspl{
				131.0,
				100.0,
				100.0
			};
			vector< vector<double> > initialBalance{
			{ 0.0, 88.0, 0.0 },
			{ 0.0, 0.0, 131.0 },
			{ 100.0, 0.0, 0.0 }
			};
			vector<double> fxRates{
				1.0, //USD = 1
				100.0 / 88.0, //EUR
				100.0 / 131.0  //CAD
			};
			double settledAmount = 300.0;
			vector<int> settledTradeIds{ 0, 1, 2 };
			testCases.push_back({ "", std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(fxRates), settledAmount, std::move(settledTradeIds) });
		}

		//Read test cases from csv files
		for (int filePrefix = 1; ; ++filePrefix)
		{
			ifstream tradesFile;
			ifstream initialBalancesFile;
			ifstream splFile;
			ifstream asplFile;
			ifstream fxRatesFile;
			ifstream resultsFile;
			string tradesFileFullName{ testDataPath + to_string(filePrefix) + "_" + tradesFileName };
			string initialBalancesFileFullName{ testDataPath + to_string(filePrefix) + "_" + initialBalancesFileName };
			string splFileFullName{ testDataPath + to_string(filePrefix) + "_" + splFileName };
			string asplFileFullName{ testDataPath + to_string(filePrefix) + "_" + asplFileName };
			string fxRatesFileFullName{ testDataPath + to_string(filePrefix) + "_" + fxRatesFileName };
			string resultsFileFullName{ testDataPath + to_string(filePrefix) + "_" + resultsFileName };

			try
			{
				tradesFile.open(tradesFileFullName);
				initialBalancesFile.open(initialBalancesFileFullName);
				splFile.open(splFileFullName);
				asplFile.open(asplFileFullName);
				fxRatesFile.open(fxRatesFileFullName);
				resultsFile.open(resultsFileFullName);
			}
			catch (std::ifstream::failure &readErr)
			{
				cout << "\nERROR: Can not open file: " << endl;
				break;
			}
			catch (...)
			{
				cout << "\nUNKNOWN ERROR while opening file: " << endl;
				break;
			}

			if (!tradesFile.good()
				|| !initialBalancesFile.good()
				|| !splFile.good()
				|| !asplFile.good()
				|| !fxRatesFile.good())
				break;

			if (!GlobalFlagCreateTestCases && !resultsFile.good())
				break;

			vector< vector<double> > initialBalance{};
			vector<Trade> trades{};
			vector< vector<double> > spl{};
			vector<double> aspl{};
			vector<double> fxRates{};

			unordered_map<int, int> memberIdUMap;
			unordered_map<int, int> currencyIdUMap;
			int numMembers = 0;
			int numCurrencies = 0;

			//Read initial_balances
			unordered_map<std::pair<int, int>, double, pair_hash> memberCurrencyBalanceUMap;
			set<int> memberIdSet;
			set<int> currencyIdSet;
			if (initialBalancesFile.is_open())
			{
				bool isColumnHeader = true;
				string line;
				while (std::getline(initialBalancesFile, line, '\n'))
				{
					if (line.empty())
						continue;
					if (isColumnHeader)
					{
						isColumnHeader = false;
						continue;
					}
					int start = 0;
					int end = line.find_first_of(',', start);
					int memberId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					int currencyId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					assert(end == string::npos);
					double initialBalanceAmount = stod(line.substr(start));

					memberCurrencyBalanceUMap[{memberId, currencyId}] = initialBalanceAmount;
					memberIdSet.insert(memberId);
					currencyIdSet.insert(currencyId);
				}

				int index = -1;
				for (auto it = memberIdSet.begin(); it != memberIdSet.end(); ++it)
					memberIdUMap[*it] = ++index;
				index = -1;
				for (auto it = currencyIdSet.begin(); it != currencyIdSet.end(); ++it)
					currencyIdUMap[*it] = ++index;

				numMembers = memberIdUMap.size();
				numCurrencies = currencyIdUMap.size();

				initialBalance.resize(numMembers);
				spl.resize(numMembers);
				aspl.resize(numMembers);
				fxRates.resize(numCurrencies);
				for (int i = 0; i < numMembers; ++i)
				{
					initialBalance[i].resize(numCurrencies);
					spl[i].resize(numCurrencies);
				}

				for (auto it = memberCurrencyBalanceUMap.begin(); it != memberCurrencyBalanceUMap.end(); ++it)
				{
					pair<int, int> memberCurrency = it->first;
					int memberIndex = memberIdUMap[memberCurrency.first];
					int currencyIndex = currencyIdUMap[memberCurrency.second];
					initialBalance[memberIndex][currencyIndex] = it->second;
				}

				initialBalancesFile.close();
			}
			else
				break;

			//Read trades
			if (tradesFile.is_open())
			{
				bool isColumnHeader = true;
				string line;
				while (std::getline(tradesFile, line, '\n'))
				{
					if (line.empty())
						continue;
					if (isColumnHeader)
					{
						isColumnHeader = false;
						continue;
					}
					int start = 0;
					int end = line.find_first_of(',', start);
					int tradeId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					int partyId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					int cPartyId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					int buyCurr = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					int sellCurr = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					double buyVol = stod(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					assert(end == string::npos);
					double sellVol = stod(line.substr(start));

					trades.push_back({ tradeId, partyId, cPartyId, Currency(buyCurr), Currency(sellCurr), buyVol, sellVol });
				}

				tradesFile.close();
			}
			else
				break;

			//Read SPLs
			if (splFile.is_open())
			{
				bool isColumnHeader = true;
				string line;
				while (std::getline(splFile, line, '\n'))
				{
					if (line.empty())
						continue;
					if (isColumnHeader)
					{
						isColumnHeader = false;
						continue;
					}
					int start = 0;
					int end = line.find_first_of(',', start);
					int memberId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					int currencyId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					assert(end == string::npos);
					double splVal = stod(line.substr(start));

					int memberIndex = memberIdUMap[memberId];
					int currencyIndex = currencyIdUMap[currencyId];
					spl[memberIndex][currencyIndex] = splVal;
				}

				splFile.close();
			}
			else
				break;

			//Read ASPLs
			if (asplFile.is_open())
			{
				bool isColumnHeader = true;
				string line;
				while (std::getline(asplFile, line, '\n'))
				{
					if (line.empty())
						continue;
					if (isColumnHeader)
					{
						isColumnHeader = false;
						continue;
					}
					int start = 0;
					int end = line.find_first_of(',', start);
					int memberId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					assert(end == string::npos);
					double asplVal = stod(line.substr(start));

					int memberIndex = memberIdUMap[memberId];
					aspl[memberIndex] = asplVal;
				}

				asplFile.close();
			}
			else
				break;

			//Read Fx Rates
			if (fxRatesFile.is_open())
			{
				bool isColumnHeader = true;
				string line;
				while (std::getline(fxRatesFile, line, '\n'))
				{
					if (line.empty())
						continue;
					if (isColumnHeader)
					{
						isColumnHeader = false;
						continue;
					}
					int start = 0;
					int end = line.find_first_of(',', start);
					int currencyId = stoi(line.substr(start, end - start));

					start = end + 1;
					end = line.find_first_of(',', start);
					assert(end == string::npos);
					double fxRateVal = stod(line.substr(start));

					int currencyIndex = currencyIdUMap[currencyId];
					fxRates[currencyIndex] = fxRateVal;
				}

				fxRatesFile.close();
			}
			else
				break;

			double settledAmount = 0.0;
			vector<int> settledTradeIds{};
			if (!GlobalFlagCreateTestCases && resultsFile.is_open())
			{
				string line;
				while (std::getline(resultsFile, line, '\n'))
				{
					if (line.find(settledAmountTag) != string::npos)
					{
						settledAmount = stod(line.substr(settledAmountTag.length()));
					}
					else if (line != settledTradeIdsTag)
					{
						settledTradeIds.push_back(stoi(line));
					}
				}
			}
			
			testCases.push_back({ to_string(filePrefix), std::move(trades), std::move(spl), std::move(aspl), std::move(initialBalance), std::move(fxRates), settledAmount, std::move(settledTradeIds) });
		}

		return testCases;
	}
}