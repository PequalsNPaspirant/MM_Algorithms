// Goal:
// Encrypt and decrypt password

// Encrypt and decrypt password

// Common headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>
#include <stdio.h> // defines FILENAME_MAX
#include <cstring>
#include <cmath>
#include <cwchar>
#include <limits.h> //For PATH_MAX
#include <limits>

#include <windows.h>
#undef max

using namespace std;

#if defined _MSC_VER

#include <direct.h>
#define GET_CWD(path) _getcwd(path, sizeof(path))

#elif defined __GNUC__

#include <unistd.h>
#define GET_CWD(path) getcwd(path, sizeof(path))

#endif

namespace mm {

#if defined _MSC_VER

	wstring getCurrentProgramName()
	{
		const int len = 256;
		WCHAR pBuf[len];
		int bytes = GetModuleFileName(NULL, pBuf, len);
		if (bytes == 0)
			return L"";
		else
			return wstring(pBuf);
	}

	void printCurrentProgramName()
	{
		wcout << "\nRunning program: " << getCurrentProgramName() << endl;
	}

#elif defined __GNUC__

	std::string getCurrentProgramName()
	{
		const int len = 256;
		char pBuf[len];
		int bytes = GetModuleFileName(NULL, pBuf, len);
		if (bytes == 0)
			return "";
		else
			return string(pBuf);
	}

	void printCurrentProgramName()
	{
		cout << "\nRunning program: " << getCurrentProgramName() << endl;
	}

#elif defined __Unix__

	std::string get_selfpath()
	{
		char buff[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
		if (len != -1)
		{
			buff[len] = '\0';
			return std::string(buff);
		}
	}

	void printCurrentProgramName()
	{
		cout << "\nRunning program: " << get_selfpath() << endl;
	}

#endif

	/*
	To identify compiler types:
	https://sourceforge.net/p/predef/wiki/Compilers/

	*/

	//Configuration options defined as global constants
	map<string, string> globalConfig;

	void encryptOrDecryptAndWriteToFile(string iFile, string oFile, char* key, int keyLen)
	{
		ifstream inputFile(iFile, ios::binary);

		if (inputFile.is_open())
		{
			ofstream outputFile(oFile, ios::binary);

			char* inputString = new char[keyLen];
			char* outputString = new char[keyLen + 1];

			inputFile.seekg(0, ios::beg);
			streamoff startPos = inputFile.tellg();
			inputFile.seekg(0, ios::end);
			streamoff endPos = inputFile.tellg();

			inputFile.seekg(0, ios::beg);

			while (!inputFile.eof())
			{
				streamoff currPos = inputFile.tellg();

				int limit = keyLen;
				if (keyLen > (endPos - currPos))
					limit = int(endPos - currPos);

				memset(inputString, 0, keyLen);
				inputFile.read(&inputString[0], keyLen);
				memset(outputString, 0, keyLen);
				for (int i = 0; i < limit; i++)
				{
					char chFile = inputString[i];
					char chKey = key[i];
					char ch = chFile ^ key[i];
					outputString[i] = ch;
				}

				outputFile.write(&outputString[0], limit);
			}

			inputFile.close();
			outputFile.close();
			delete[] inputString;
			delete[] outputString;
		}
		else
			cout << "\nCould not open input file " << iFile;
	}

	void encryptOrDecryptAndDisplayOnSceen(string iFile, char* key, int keyLen)
	{
		ifstream inputFile(iFile, ios::binary);

		if (inputFile.is_open())
		{
			char* inputString = new char[keyLen];
			char* outputString = new char[keyLen + 1];
			cout << "\n\n";
			inputFile.seekg(0, ios::beg);
			streamoff startPos = inputFile.tellg();
			inputFile.seekg(0, ios::end);
			streamoff endPos = inputFile.tellg();

			inputFile.seekg(0, ios::beg);

			while (!inputFile.eof())
			{
				streamoff currPos = inputFile.tellg();

				int limit = keyLen;
				if (keyLen > (endPos - currPos))
					limit = int(endPos - currPos);

				memset(inputString, 0, keyLen);
				inputFile.read(&inputString[0], keyLen);
				memset(outputString, 0, keyLen);
				for (int i = 0; i < limit; i++)
				{
					char chFile = inputString[i];
					char chKey = key[i];
					char ch = chFile ^ key[i];
					outputString[i] = ch;
				}

				outputString[limit] = '\0';
				cout << outputString;
			}

			inputFile.close();
			delete[] inputString;
			delete[] outputString;
		}
		else
			cout << "\nCould not open input file " << iFile;
	}

	void writeKey(string oFile, char* key, int keyLen)
	{
		ofstream outputFile(oFile, ios::binary);

		if (outputFile.is_open())
		{
			string buffer("\nkey[] = {");
			for (int i = 0; i < keyLen; i++)
			{
				string text = to_string(int(key[i]));
				buffer += (text + ", ");
			}
			buffer[buffer.length() - 2] = '}';
			outputFile.write(&buffer[0], buffer.length());
			outputFile.close();
		}

		outputFile.open(oFile, ios::binary | ios::app);
		if (outputFile.is_open())
		{
			string str("\n\n Key: ");
			outputFile.write(&str[0], str.length());
			outputFile.write(key, keyLen);
			outputFile.close();
		}
	}

	void printKey(char* key, int keyLen)
	{
		cout << endl << "Printing key";
		cout << endl << "char key[" << keyLen << "] = {";
		for (int i = 0; i < keyLen; i++)
		{
			cout << int(key[i]) << ", ";
		}
		cout << "};" << endl;

		cout << endl << "Printing key finished";
	}

	bool compareFiles(string file1, string file2)
	{
		ifstream leftFile(file1);
		ifstream rightFile(file2);
		bool result = true;

		if (!leftFile.is_open() || !rightFile.is_open())
			return false;

		string leftLine, rightLine;
		while (!leftFile.eof() && !rightFile.eof())
		{

			leftFile >> leftLine;
			rightFile >> rightLine;

			if (leftLine != rightLine)
			{
				result = false;
				break;
			}
		}

		if (!leftFile.eof() || !rightFile.eof())
			result = false;

		leftFile.close();
		rightFile.close();

		return result;
	}

	//This encryption is done using matrix multiplication
	string encryptKey(string userEnteredPassword, string encryptionKeyForPassword, string& encryptionKeyForFile)
	{
		//encryptionKeyForPassword is 4x4 unit matrix
		// [encryptionKeyForPassword] = [1]
		//encryptedPassword is matrix multiplication of userEnteredPassword and encryptionKeyForPassword
		// [userEnteredPassword] x [encryptionKeyForPassword] = [encryptedPassword]
		//The above encryption is irrerersible because we can not find matrix inverse of unit matrix encryptionKeyForPassword
		// [userEnteredPassword] = [encryptedPassword] x [encryptionKeyForPassword]^-1

		//Check if length of the user-entered-password is wrong 
		if (userEnteredPassword.length() != encryptionKeyForPassword.length())
			return "";

		//Step 1
		string str1;
		int i;
		for (i = 0; userEnteredPassword[i] != '\0' && encryptionKeyForPassword[i] != '\0'; i++)
			str1 += (userEnteredPassword[i] ^ encryptionKeyForPassword[i]);

		//Step 2
		string str2;
		for (int i = 0; encryptionKeyForPassword[i] != '\0'; i++)
		{
			char ch = '\0';
			for (int j = 0; j < sqrt(sqrt(encryptionKeyForPassword.length())); j++)
				ch += str1[(i / int(sqrt(encryptionKeyForPassword.length()))) + j] * encryptionKeyForPassword[(i % int(sqrt(encryptionKeyForPassword.length()))) + int(sqrt(encryptionKeyForPassword.length())) * j];

			str2 += ch;
		}

		//Step 3
		string str3;
		for (int i = encryptionKeyForPassword.length() - 1; i > -1; i--)
		{
			char ch = '\0';
			for (int j = 0; j < sqrt(sqrt(encryptionKeyForPassword.length())); j++)
				ch += str1[sqrt(sqrt(encryptionKeyForPassword.length())) + (i / sqrt(encryptionKeyForPassword.length())) + j] * encryptionKeyForPassword[(sqrt(encryptionKeyForPassword.length()) * 2) + (i % int(sqrt(encryptionKeyForPassword.length()))) + sqrt(encryptionKeyForPassword.length()) * j];

			str3 += ch;
		}

		//Step 4
		string encryptedPassword;
		for (int i = encryptionKeyForPassword.length() - 1; i > -1; i--)
			encryptedPassword += (str2[i] + str3[i]);

		//Just to make encryptionKeyForFile more complex, create a combination of above steps
		encryptionKeyForFile = encryptionKeyForPassword + str1 + str2 + str3;

		return encryptedPassword;
	}

	//Use of following function is deprecated
	string getPasswordFromFile()
	{
		ifstream pwdFile(globalConfig["currentWorkingDirectory"] + "\\pwd.txt");

		string line;
		if (pwdFile.is_open())
		{
			pwdFile.seekg(0, ios::beg);

			while (!pwdFile.eof())
			{
				std::getline(pwdFile, line);
				pwdFile.close();
				break;
			}
		}

		return line;
	}

	bool checkPassword(string& encryptionKeyForFile)
	{
		string userEnteredPassword;
		if (globalConfig["askUserForPassword"] == "false")
		{
			//userEnteredPassword = getPasswordFromFile();
			userEnteredPassword = globalConfig["userPassword"];
		}
		else
		{
			cout << "Enter Password: ";
			cin >> userEnteredPassword;
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//cout << "userEnteredPassword: " << userEnteredPassword << endl;
		}
		const string encryptionKeyForPassword = "[dP_hwS\\\"ICoFm>]";
		//const string storedEncryptedPassword = "¬ùm½ñpè‹2Ë—eÍí(";
		const string storedEncryptedPassword = "Âr4ÔôöqAjˆrA0ž";
		string encryptedPassword = encryptKey(userEnteredPassword, encryptionKeyForPassword, encryptionKeyForFile);

		//Do not display encrypted password on screen, it displayes different characters. Save it to file to get correct characters.
		//cout << endl << "storedString: " << storedString;
		//It displays: "¼∙m╜±pΦïÉ2╦ùe═φ(";

		if (globalConfig["writeEncryptedPasswordToFile"] == "true")
		{
			cout << endl << "Saving to file...";
			ofstream outputFile(globalConfig["currentWorkingDirectory"] + "\\encryptedPassword.txt", ios::binary);
			if (outputFile.is_open())
			{
				outputFile.write(&encryptedPassword[0], encryptedPassword.length());
				outputFile.close();
			}
		}

		if (encryptedPassword == storedEncryptedPassword)
		{
			cout << endl << "Password validated successfully";
			return true;
		}
		else
		{
			cout << "userEnteredPassword: " << userEnteredPassword << endl;
			cout << "encryptedPassword: " << encryptedPassword << endl;
			cout << "storedEncryptedPassword: " << storedEncryptedPassword << endl;
			return false;
		}
	}

	void split(vector<string>& theStringVector, const string& theString, const string& theDelimiter)
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


	string getCurrentDirectory()
	{
		char cCurrentPath[FILENAME_MAX];
		char* returnString = GET_CWD(cCurrentPath);
		return string(cCurrentPath);
	}

	string getFullFilePath(const string& inputPath)
	{
		//Check if file present
		ifstream file(inputPath);
		if (file.good())
			return inputPath;
		else
		{
			//Check in current directory
			string currentDirPath = globalConfig["currentWorkingDirectory"] + "\\" + inputPath;
			ifstream file(currentDirPath);
			if (file.good())
				return currentDirPath;
			else
			{
				cout << "\nFile '" << inputPath << "' not found. Exiting program...\n";
				exit(0);
			}
		}
	}

	void loadConfig()
	{
		ifstream configFile(getFullFilePath(globalConfig["configFilePath"]));

		if (configFile.is_open())
		{
			configFile.seekg(0, ios::beg);
			string line;

			while (!configFile.eof())
			{
				std::getline(configFile, line);
				//Skip empty lines in config file
				if (line.length() == 0)
					continue;
				vector<string> tokens;
				split(tokens, line, "=");
				if (tokens.size() == 2)
					globalConfig[trim(tokens[0])] = trim(tokens[1]);
			}

			configFile.close();
		}
		else
		{
			cout << endl << " File not found: \"" << globalConfig["configFilePath"] << "\"" << endl;
		}

		/*
		Config File Contents:

		currentWorkingDirectory = C:\\
		askUserForPassword = true
		writeEncryptPasswordToFile = true
		verifyResults = true
		inputFileFullPath = input.txt
		outputFileFullPath = output.txt
		outputToFile = true
		outputOnScreen = true

		*/
	}

	void startSettingsMode()
	{
		printCurrentProgramName();

		bool loop = true;
		int index = 0;
		while (loop)
		{
			cout << "\nCurrent Configuration: ";
			auto it = globalConfig.begin();
			for (int i = 1; it != globalConfig.end(); ++it, ++i)
			{
				cout << endl << i << ". " << it->first << ": " << it->second;
			}
			cout << "\nEnter the index to change OR enter 0 to exit: ";
			cin >> index;
			if (index == 0)
				break;
			it = globalConfig.begin();
			while (--index) ++it;
			cout << "\nEnter new value for " << it->first << " : ";
			string newValue;
			cin >> newValue;
			it->second = newValue;
		}
	}

	int PasswordEncryption(int argc, char* argv[])
	{
		//Default config:
		globalConfig["currentWorkingDirectory"] = getCurrentDirectory();
		globalConfig["configFilePath"] = getCurrentDirectory() + "\\config.cfg";

		globalConfig["askUserForPassword"] = "false";
		globalConfig["userPassword"] = "";
		globalConfig["writeEncryptedPasswordToFile"] = "false";
		globalConfig["verifyResults"] = "false";
		globalConfig["inputFileFullPath"] = "input.doc";
		globalConfig["outputFileFullPath"] = "output.txt";
		globalConfig["outputToFile"] = "false";
		globalConfig["outputOnScreen"] = "true";

		//for(int i = 0; i < argc; i++)
		//	cout << "Argument No. " << i << " is: " << argv[i] << endl;

		if (argc == 2)
		{
			string inputStr(argv[1]);
			if (inputStr == "-settings")
				startSettingsMode(); //Use of this mode is deprecated
			else
				globalConfig["configFilePath"] = inputStr;

		}

		loadConfig();

		string keyForFile;
		if (!checkPassword(keyForFile))
		{
			cout << endl << "Wrong Password!\n\nPress any key to exit...";
			cin.get();
			return 0;
		}

		const int keyLen = keyForFile.length();
		char* key = new char[keyLen];
		for (int i = 0; i < keyLen; i++) key[i] = keyForFile[i];

		string inputFileName = getFullFilePath(globalConfig["inputFileFullPath"]);
		string outputFileName = globalConfig["outputFileFullPath"];

		if (globalConfig["outputOnScreen"] == "true")
			encryptOrDecryptAndDisplayOnSceen(inputFileName, key, keyLen);
		if (globalConfig["outputToFile"] == "true")
			encryptOrDecryptAndWriteToFile(inputFileName, outputFileName, key, keyLen);

		if (globalConfig["verifyResults"] == "true")
		{
			encryptOrDecryptAndWriteToFile(outputFileName, globalConfig["currentWorkingDirectory"] + "\\decrypted_for_verification.txt", key, keyLen);
			bool result = compareFiles(inputFileName, globalConfig["currentWorkingDirectory"] + "\\decrypted_for_verification.txt");
			cout << "\n\nComparison result: " << ((result == true) ? "TRUE" : "FALSE");
		}

		delete[] key;

		cout << endl << "Press any key to exit...";
		cin.get();
		return 0;
	}


	//int main(int argc, char* argv[])
	//{
	//	PasswordEncryption(argc, argv);
	//
	//	return 0;
	//}

	void PasswordEncryption()
	{
		PasswordEncryption(0, nullptr);
	}

}