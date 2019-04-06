#pragma once

#include <string>
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
using namespace std;

#include "Timer/Timer_Timer.h"

namespace mm {

	class Logger
	{
	public:
		Logger(const string& logFileFullName, bool printInLogFile = true, bool printOnScreen = true, bool addTime = true);
		~Logger();

		void setAddTime(bool printOnScreen);
		void setPrintOnScreen(bool printOnScreen);
		void setPrintInLogFile(bool printInLogFile);

		void log(const string& logMessage);
		template<typename T>
		friend Logger& operator<< (Logger &logger, T object);
		void close();

	private:
		ofstream m_logFile;
		//string m_logFileName;
		//string m_logLocation;
		Timer m_timer;
		bool m_addTime;
		bool m_printOnScreen;
		bool m_printInLogFile;
	};


	template<typename T>
	Logger& operator<< (Logger &logger, T object)
	{
		stringstream sstr;
		sstr << object;
		logger.log(sstr.str());
		return logger;
	}

}