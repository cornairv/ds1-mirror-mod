#pragma once

#include <fstream>
#include <sstream>
#include <INIReader.h>

enum class LogEndl { INFO, ERRO, WARN, ENDL };

class Logger
{
public:
	static Logger& getInstance()
	{
		static Logger singleton;
		return singleton;
	}
	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;

	Logger();
	~Logger();

	template <typename T>
	friend Logger& operator<< (Logger& logger, const T& x);
	friend Logger& operator<< (Logger& logger, LogEndl s);

	static bool enabled;

private:
	std::ofstream logOut;
	std::stringstream sstream;
	std::stringstream prefixSstream;

	void flush();
};

template <typename T>
Logger& operator<< (Logger& logger, const T& x)
{
	logger.sstream << x;
	return logger;
}

void MyDebugBreak();

#define LOG if (!Logger::enabled) {} else Logger::getInstance()

#define CHECK(expr) \
	do { \
		long hr = (long)(expr); \
		if (hr != 0) \
			LOG << "(" << __FILE__ << ":" << ##__LINE__ << ") Expression returned " << hr << ": " << #expr << LogEndl::ERRO; \
	} while (0)
