#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert> //for assert()
#include <cmath>
using namespace std;

#include "Others/Others_PersonSort.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//class Person definition
	class Person
	{
	public:
		//A Person can not exist without a salary, age and name
		Person(const double& salary, const unsigned int& age, const string& name)
			: salary_(salary),
			age_(age),
			name_(name)
		{
		}

		~Person()
		{}

		bool operator==(const Person& rhs) const
		{
			return (fabs(salary_ - rhs.salary_) < Person::precision &&
				age_ == rhs.age_ &&
				name_ == rhs.name_);
		}

		static void sort_inplace(vector<Person>& persons)
		{
			std::sort(persons.begin(), persons.end(), [](const Person& lhs, const Person& rhs) -> bool {
				if (fabs(lhs.salary_ - rhs.salary_) < Person::precision)
				{
					if (lhs.age_ == rhs.age_)
						return lhs.name_ < rhs.name_;
					else
						return lhs.age_ < rhs.age_;
				}
				else
					return lhs.salary_ > rhs.salary_;
			});
		}

	private:
		double salary_;
		unsigned int age_;
		string name_;

		static double precision;
	};

	double Person::precision = 0.0001;

	//Testing:
	void testAllPermutations(int currentIndex, vector<Person>& group, const vector<Person>& sorted)
	{
		if (currentIndex == group.size())
		{
			static int numTestCases = -1;
			cout << "\nSorting test case no.: " << ++numTestCases;
			vector<Person> copy{ group };
			Person::sort_inplace(copy);
			assert(copy == sorted);
			return;
		}

		for (int i = currentIndex; i < group.size(); ++i)
		{
			std::swap(group[currentIndex], group[i]);

			testAllPermutations(currentIndex + 1, group, sorted);

			//restore original position to backtrack
			std::swap(group[currentIndex], group[i]);
		}
	}

	void testPersonSort()
	{
		cout << "\nstarting tests for class Person";

		Person p1(100.25, 25, "abc");
		Person p2(100.25001, 25, "abc");
		Person p3(200.12, 30, "xyz");

		assert(p1 == p2);
		assert(!(p1 == p3));

		vector<Person> sorted{
			{ 100.25, 25, "abc" },
			{ 100.25, 25, "xyz" },
			{ 100.25, 30, "pqr" },
			{ 99.25, 25, "efg" },
			{ 99.25, 30, "xyz" }
		};
		
		vector<Person> group{ sorted };
		// This functions tests all permutations. So test it on small size vector. 
		// for size = 5, it tests sorting function for 120 times. (5! = 120)
		testAllPermutations(0, group, sorted);

		cout << "\nfinished tests for class Person";
	}

	MM_DECLARE_FLAG(Others_PersonSort);

	MM_UNIT_TEST(Others_PersonSort_sanity, Others_PersonSort)
	{
		testPersonSort();
	}
}