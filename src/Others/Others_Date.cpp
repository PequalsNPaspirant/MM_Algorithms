#include <iostream>
#include <string>
using namespace std;

#include "Others/Others_Date.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	class bad_date
	{
	public:
		bad_date(const string& s)
			: message(s)
		{
		}
		bad_date(string&& s)
			: message(std::move(s))
		{
		}

	private:
		string message;
	};

	class Date
	{
	public:
		Date(unsigned int input) // Input will be in the form of 20180605
		{
			if (input > 99991231 || input < 00000101)
				throw bad_date("Date out of range");

			day = input % 100;
			month = (input / 100) % 100;
			year = (input / 10000);

			if (month == 0 || month > 12)
				throw bad_date("Bad month");

			if (day == 0 || day > Date::getDaysPerMonth(month, year))
				throw bad_date("Bad day");
		}

		unsigned int getNumberOfDaysSince0000_00_00() const
		{
			static unsigned int cumulativeSumOfDays[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			if (cumulativeSumOfDays[12] == 31) // If not yet initialized
			{
				for (int i = 1; i < 13; ++i)
					cumulativeSumOfDays[i] += cumulativeSumOfDays[i - 1];
			}

			unsigned int numberOfDays = cumulativeSumOfDays[month - 1] + day;
			if (isLeapYear(year) && month > 2)
				numberOfDays += 1;

			numberOfDays += (year * 365);
			numberOfDays += (year/4 - year/100 + year/400 - 1); // Consider extra days for all leap years between [0, (year - 1)]

			return numberOfDays;
		}

		friend unsigned int operator-(const Date& from, const Date& to)
		{
			return to.getNumberOfDaysSince0000_00_00() - from.getNumberOfDaysSince0000_00_00();

			/*
			//count days in range: from - year end
			unsigned int count = Date::getDaysPerMonth(from.month, from.year) - from.day + 1;
			for (int i = from.month + 1; i <= 12; ++i)
				count += Date::getDaysPerMonth(i, from.year);

			//count days for every year in range: from - to
			for (int i = from.year + 1; i < to.year; ++i)
				if (Date::isLeapYear(i))
					count += 366;
				else
					count += 365;

			//count days in range: start of year - to
			for (int i = 1; i < to.month; ++i)
				count += Date::getDaysPerMonth(i, to.year);
			count += to.day;

			if (to < from)
				count = -count;

			return count;
			*/
		}

		friend bool operator<(const Date& lhs, const Date& rhs)
		{
			if ((lhs.year < rhs.year)
				|| (lhs.year == rhs.year && lhs.month < rhs.month)
				|| (lhs.year == rhs.year && lhs.month == rhs.month && lhs.day < rhs.day))
				return true;

			return false;
		}

		static bool isLeapYear(unsigned int year)
		{
			if (year % 100 == 0)
			{
				if (year % 400 == 0)
					return true;
			}
			else if (year % 4 == 0)
				return true;

			return false;
		}

		static unsigned int getDaysPerMonth(unsigned int month, unsigned int year)
		{
			static const unsigned int daysPerMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			if (Date::isLeapYear(year) && month == 2)
				return 29;
			else
				return daysPerMonth[month];
		}

		friend ostream& operator<<(ostream& o, const Date& d)
		{
			o << "{ " << d.year << " " << d.month << " " << d.day << " }";
			return o;
		}

		unsigned int getDay()
		{
			return day;
		}

		unsigned int getMonth()
		{
			return month;
		}

		unsigned int getYear()
		{
			return year;
		}

	private:
		unsigned int day;
		unsigned int month;
		unsigned int year;
	};
	
	MM_DECLARE_FLAG(Others_DateUnitTest);

	MM_UNIT_TEST(Others_DateUnitTest_sanity, Others_DateUnitTest)
	{
		// Test Date creation

		struct testStructCreation
		{
			testStructCreation(unsigned int rd, bool c, unsigned int y, unsigned int m, unsigned int d)
				: rawDate(rd),
				objectCreationResult(c),
				year(y),
				month(m),
				day(d)
			{
			}

			unsigned int rawDate;
			bool objectCreationResult;
			unsigned int year;
			unsigned int month;
			unsigned int day;
		};

		vector<testStructCreation> testDataCreation = {
			{ 20180605, true, 2018, 06, 05 },
			{    80605, true,    8, 06, 05 },
			{ 20160229, true, 2016, 02, 29 },
			{      101, true,    0, 01, 01 },
			{ 20000229, true, 2000, 02, 29 },
			{ 20180605, true, 2018, 06, 05 },
			{ 20180605, true, 2018, 06, 05 },

			{ 20180635, false, 0000, 00, 00 },
			{ 20180631, false, 0000, 00, 00 },
			{ 20160230, false, 0000, 00, 00 },
			{ 20181305, false, 0000, 00, 00 },
			{        1, false, 0000, 00, 00 },
			{       99, false, 0000, 00, 00 },
			{      100, false, 0000, 00, 00 },
			{ 23000229, false, 0000, 00, 00 }
		};

		for (int i = 0; i < testDataCreation.size(); ++i)
		{
			bool dateCreatedSuccessfully = true;
			unsigned int day, month, year;
			try
			{
				Date d(testDataCreation[i].rawDate);
				day = d.getDay();
				month = d.getMonth();
				year = d.getYear();
				if (day != testDataCreation[i].day || month != testDataCreation[i].month || year != testDataCreation[i].year)
					dateCreatedSuccessfully = false;
			}
			catch (bad_date e)
			{
				dateCreatedSuccessfully = false;
			}

			MM_EXPECT_TRUE(dateCreatedSuccessfully == testDataCreation[i].objectCreationResult, dateCreatedSuccessfully, testDataCreation[i].objectCreationResult, testDataCreation[i].rawDate, day, month, year);
		}
		
		// Test Date::operator-()

		struct testStructSubstraction
		{
			testStructSubstraction(const Date& f, const Date& t, unsigned int c)
				: from(f),
				to(t),
				count(c)
			{
			}

			Date from;
			Date to;
			unsigned int count;
		};

		vector<testStructSubstraction> testDataSubstraction = {
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 },
			{ 20180605, 20180605, 0 }
		};

		for (int i = 0; i < testDataSubstraction.size(); ++i)
		{
			unsigned int atualCount = testDataSubstraction[i].from - testDataSubstraction[i].to;
			MM_EXPECT_TRUE(atualCount == testDataSubstraction[i].count, testDataSubstraction[i].from, testDataSubstraction[i].to, testDataSubstraction[i].count, atualCount);
		}

		// Test Date::operator<()

		struct testStructComparison
		{
			testStructComparison(const Date& l, const Date& r, unsigned int c)
				: lhs(l),
				rhs(r),
				isLessThan(c)
			{
			}

			Date lhs;
			Date rhs;
			bool isLessThan;
		};

		vector<testStructComparison> testDataComparison = {
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false },
			{ 20180605, 20180605, false }
		};

		for (int i = 0; i < testDataComparison.size(); ++i)
		{
			bool atualResult = testDataComparison[i].lhs - testDataComparison[i].rhs;
			MM_EXPECT_TRUE(atualResult == testDataComparison[i].isLessThan, testDataComparison[i].lhs, testDataComparison[i].rhs, testDataComparison[i].isLessThan, atualResult);
		}
	}

}