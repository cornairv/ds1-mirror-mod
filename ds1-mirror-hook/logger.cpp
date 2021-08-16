#include "pch.h"
#include "logger.h"

void Logger::Init(std::string filename, bool show_console)
{
	log_out.open(filename, std::ios_base::app);
	console = show_console;
	if (console)
	{
		AllocConsole();
	}
}

Logger::~Logger()
{
	if (log_out.is_open())
	{
		log_out.close();
		FreeConsole();
	}
}

Logger& operator<< (Logger& logger, LogEndl s)
{
	time_t now = std::time(nullptr);
	struct tm timebuf;
	localtime_s(&timebuf, &now);
	logger.prefix_sstream << std::put_time(&timebuf, "%F %T ");

	switch (s)
	{
	case LogEndl::INFO:
		logger.prefix_sstream << "[INFO] ";
		break;
	case LogEndl::ERRO:
		logger.prefix_sstream << "[ERRO] ";
		break;
	case LogEndl::WARN:
		logger.prefix_sstream << "[WARN] ";
		break;
	case LogEndl::ENDL:
		break;
	}
	logger.flush();
	return logger;
}

void Logger::flush()
{
	prefix_sstream << sstream.str() << "\n";
	log_out << prefix_sstream.str();
	if (console)
	{
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), prefix_sstream.str().c_str(), prefix_sstream.str().length(), nullptr, nullptr);
	}
	prefix_sstream.clear();
	prefix_sstream.str(std::string());
	sstream.clear();
	sstream.str(std::string());
}

void MyDebugBreak()
{
	Log << "debug" << LogEndl::ENDL;
}
