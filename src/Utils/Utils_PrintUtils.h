#pragma once

#include <iostream>
#include <vector>
using namespace std;

namespace mm {

	template<typename T>
	ostream& operator<<(ostream& o, const vector<T>& vec)
	{
		if (vec.empty())
		{
			o << "{ }";
			return o;
		}

		o << "{ ";
		o << vec[0];
		for (int i = 1; i < vec.size(); ++i)
		{
			o << ", ";
			o << vec[i];
		}
		o << " }";

		return o;
	}

	template<typename T1, typename T2>
	ostream& operator<<(ostream& o, const pair<T1, T2>& p)
	{
		o << "{ " << p.first << ", " << p.second << " }";
		return o;
	}
}