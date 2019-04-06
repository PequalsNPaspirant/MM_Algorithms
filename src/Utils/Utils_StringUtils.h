//Goal:
//Try various string operations

#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace mm {

	//------------------ Remove extra spaces in string ----------------------------------

	int jumpAllConsecutiveSpaces(const string& input, int pos);
	string removeExtraSpaces(const string& input);

	string& removeExtraSpaces2(string& input);



	//------------------ Reverse the string ----------------------------------
	void reverseString(string& str);

	void splitString(vector<string>& theStringVector, const string& theString, const string& theDelimiter);

	static inline std::string &ltrim(std::string &s);

	// trim from end
	static inline std::string &rtrim(std::string &s);

	// trim from both ends
	static inline std::string &trim(std::string &s);

}