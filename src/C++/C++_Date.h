

namespace mm {

	class ExBadDate
	{
	public:
		ExBadDate(unsigned int dayIn, unsigned int monthIn, unsigned int yearIn)
			: day(dayIn),
			month(monthIn),
			year(yearIn)
		{
		}

	private:
		unsigned int day;
		unsigned int month;
		unsigned int year;
	};

	class SimpleDate
	{
	public:
		SimpleDate(unsigned int dayIn, unsigned int monthIn, unsigned int yearIn)
			: day(dayIn),
			month(monthIn),
			year(yearIn)
		{
			static unsigned int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			//Validations:
			if (day < 1 || day >(days[month] + (isLeapYear() ? 1 : 0))
				|| month < 1 || month > 12)
				throw ExBadDate(day, month, year);
		}

		bool isLeapYear()
		{
			//if (year % 100 == 0)
			//	return year % 400 == 0;
			//else
			//	return year % 4 == 0;

			return (year % 100 == 0 ? year / 100 : year) % 4 == 0;
		}

		bool operator<(const SimpleDate& rhs)
		{
			//return (year * 10000 + month * 100 + day) < (rhs.year * 10000 + rhs.month * 100 + rhs.day);
			/*
			day range - 1 to 31 - needs just last 5 bits
			month range - 1 to 12 - needs just last 4 bits
			year range - any
			*/
			return ((year << 9) | (month << 5) | (day)) < ((rhs.year << 9) | (rhs.month << 5) | (rhs.day));
		}

	private:
		unsigned int day;
		unsigned int month;
		unsigned int year;
	};

}