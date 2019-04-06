//Goal:
//Try various string operations

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <sstream>
using namespace std;

namespace mm {

	//------------------ Remove extra spaces in string ----------------------------------

	int jumpAllConsecutiveSpaces(const string& input, int pos)
	{
		while (input[pos] == ' ') pos++;
		return pos;
	}

	string removeExtraSpaces(const string& input)
	{
		string output("");
		int i = 0;
		while (input[i] != '\0')
		{
			if (i == 0 && input[i] == ' ')
				i = jumpAllConsecutiveSpaces(input, i + 1);

			output += input[i];
			if (input[i] == ' ')
				i = jumpAllConsecutiveSpaces(input, i + 1);
			else
				i++;
		}

		if (output[output.length() - 1] == ' ')
		{
			output[output.length() - 1] = '\0';
			output.resize(output.length() - 1);
		}

		return output;
	}

	string& removeExtraSpaces2(string& input)
	{
		int ctr = 0;
		int i = 0;
		while (input[i] != '\0')
		{
			if (i == 0 && input[i] == ' ')
				i = jumpAllConsecutiveSpaces(input, i + 1);

			input[ctr++] = input[i];
			if (input[i] == ' ')
				i = jumpAllConsecutiveSpaces(input, i + 1);
			else
				i++;
		}

		if (input[ctr - 1] == ' ')
			--ctr;

		input[ctr] = '\0';
		input.resize(ctr);

		return input;
	}


	//------------------ Reverse the string ----------------------------------

	void reverseString(string& str)
	{
		cout << endl << str << " <= reverse => ";
		int len = str.length();
		for (int i = 0; i < len / 2; i++)
			std::swap(str[i], str[len - i - 1]);
		cout << str;
	}

	void splitString(vector<string>& theStringVector, const string& theString, const string& theDelimiter)
	{
		size_t  start = 0, end = 0;

		while (end != string::npos)
		{
			end = theString.find(theDelimiter, start);

			// If at end, use length=maxLength.  Else use length=end-start.
			theStringVector.push_back(theString.substr(start,
				(end == string::npos) ? string::npos : end - start));

			// If at end, use start=maxSize.  Else use start=end+delimiter.
			start = ((end > (string::npos - theDelimiter.size()))
				? string::npos : end + theDelimiter.size());
		}
	}

	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}


	bool checkIfSubstring(const string& subString, const string& mainString)
	{
		return true;
	}

	void testRemoveExtraSpaces(string example)
	{
		cout << "\n\nInput : '" << example << "'";
		cout << "\nOutput: '" << removeExtraSpaces(example) << "'";
		cout << "\nOutput: '" << removeExtraSpaces2(example) << "'";
	}

	void testRemoveExtraSpaces()
	{
		testRemoveExtraSpaces("this is one example.");
		testRemoveExtraSpaces(" this is one   example. ");
		testRemoveExtraSpaces("    this    is    one   example.    ");
		testRemoveExtraSpaces(".    this    is    one   example.    .");

		//Results:
		//Input: 'this is one example.'
		//Output : 'this is one example.'
		//Output : 'this is one example.'
		//
		//Input : ' this is one   example. '
		//Output : 'this is one example.'
		//Output : 'this is one example.'
		//
		//Input : '    this    is    one   example.    '
		//Output : 'this is one example.'
		//Output : 'this is one example.'
		//
		//Input : '.    this    is    one   example.    .'
		//Output : '. this is one example. .'
		//Output : '. this is one example. .'
	}

	void testReverseString()
	{
		std::vector<string> s = { "Example", " string with spaces   ", "A", "", "step on no pets" };
		for (string str : s) reverseString(str);
	}

	void testPrintNumCommaSeparated()
	{
		std::stringstream ss;
		ss.imbue(std::locale(""));
		long n = 123456789;
		ss << std::fixed << n;
		cout << "\nComma Separated Number: " << ss.str();
	}

	void StringUtils()
	{
		cout << "\n\n--------------- Testing StringUtils ---------------";

		//testRemoveExtraSpaces();
		testReverseString();
		testPrintNumCommaSeparated();
	}

}