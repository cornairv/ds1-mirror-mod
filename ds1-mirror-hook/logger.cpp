#include "pch.h"
#include "logger.h"
#include <iomanip>

bool Logger::enabled = true;

Logger::Logger()
{
	std::string filename = "mirror-mod.log";
	console = true;

	logOut.open(filename, std::ios_base::app);
	if (console)
	{
		AllocConsole();
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
	if (console)
	{
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), prefixSstream.str().c_str(), prefixSstream.str().length(), nullptr, nullptr);
	}
	prefixSstream.clear();
	prefixSstream.str(std::string());
	sstream.clear();
	sstream.str(std::string());
}

void __declspec(noinline) MyDebugBreak()
{
	LOG << "debug" << LogEndl::ENDL;
}
