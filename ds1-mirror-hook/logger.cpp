#include "pch.h"
#include "logger.h"
#include "config.h"
#include <iomanip>

bool Logger::enabled = false;

Logger::Logger()
{
	if (!config.logFile.empty())
	{
		logOut.open(config.logFile, std::ios_base::app);
		if (logOut.fail())
		{
			std::string errorMsg = "Failed to open file for logging: " + config.logFile;
			MessageBoxA(nullptr, errorMsg.c_str(), "DSMirror Alert", MB_OK);
		}
	}
	if (config.showConsole)
	{
		if (AllocConsole() == 0)
		{
			std::string errorMsg = "Failed to allocate console for logging: " + std::to_string(GetLastError());
			MessageBoxA(nullptr, errorMsg.c_str(), "DSMirror Alert", MB_OK);
		}
	}
}

Logger::~Logger()
{
	if (logOut.is_open())
	{
		logOut.close();
		FreeConsole();
	}
}

Logger& operator<< (Logger& logger, LogEndl s)
{
	time_t now = std::time(nullptr);
	struct tm timebuf;
	localtime_s(&timebuf, &now);
	logger.prefixSstream << std::put_time(&timebuf, "%F %T ");

	switch (s)
	{
	case LogEndl::INFO:
		logger.prefixSstream << "[INFO] ";
		break;
	case LogEndl::ERRO:
		MessageBoxA(nullptr, logger.sstream.str().c_str(), "DSMirror Error", MB_OK);
		logger.prefixSstream << "[ERRO] ";
		break;
	case LogEndl::WARN:
		logger.prefixSstream << "[WARN] ";
		break;
	case LogEndl::ENDL:
		break;
	}
	logger.flush();
	return logger;
}

void Logger::flush()
{
	prefixSstream << sstream.str() << "\n";
	logOut << prefixSstream.str();
	logOut.flush();
	if (config.showConsole)
	{
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), prefixSstream.str().c_str(), prefixSstream.str().length(), nullptr, nullptr);
	}
	clearBuffer();
}

void Logger::clearBuffer()
{
	prefixSstream.clear();
	prefixSstream.str(std::string());
	sstream.clear();
	sstream.str(std::string());
}

void __declspec(noinline) MyDebugBreak()
{
	LOG << "debug" << LogEndl::ENDL;
}
