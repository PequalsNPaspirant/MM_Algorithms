#include <iostream>
using namespace std;

#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Logger.h"

namespace mm {

	Logger::Logger(const string& logFileFullName, bool printInLogFile /*= true*/, bool printOnScreen /*= true*/, bool addTime /* = true*/)
		: m_printInLogFile(true), m_printOnScreen(printOnScreen), m_addTime(addTime)
	{
		string logFileName(logFileFullName);

		try
		{
			Timer t;
			size_t start = logFileFullName.find_last_of('\\') + 1;
			//logFileName = logFileName.substr(start, logFileFullName.length() - start);
			string fullFileNameWithTimestamp(logFileFullName + "_" + t.getCurrentLocalTimeInNanoSeconds() + ".cpplog");
			m_logFile.open(fullFileNameWithTimestamp);
		}
		catch (std::ofstream::failure &writeErr)
		{
			cout << "\nERROR: Can not open file: " << logFileFullName << endl;
		}
		catch (...)
		{
			cout << "\nUNKNOWN ERROR while opening file: " << logFileFullName << endl;
		}
	}

	Logger::~Logger()
	{
		close();
	}

	void Logger::close()
	{
		if (m_logFile.is_open())
			m_logFile.close();
	}

	void Logger::setPrintOnScreen(bool printOnScreen)
	{
		m_printOnScreen = printOnScreen;
	}

	void Logger::setPrintInLogFile(bool printInLogFile)
	{
		m_printInLogFile = printInLogFile;
	}

	void Logger::setAddTime(bool addTime)
	{
		m_addTime = addTime;
	}

	void Logger::log(const string& logMessage)
	{
		if (m_addTime)
		{
			string time("\n" + Timer::getCurrentLocalTimeInNanoSeconds() + ": ");
			m_logFile.write(time.c_str(), time.length());
		}
			
		if (m_printInLogFile)
		{
			if (m_logFile.is_open())
			{
				m_logFile.write(logMessage.c_str(), logMessage.length());
				//flush() will ensure than the contents are written to file on disk immediately, Otherwise we need to wait until close() is called on it
				m_logFile.flush();
			}
		}

		if (m_printOnScreen)
			cout << logMessage;
	}

	template<>
	Logger& operator<< (Logger &logger, const string& msg)
	{
		logger.log(msg);
		return logger;
	}

}