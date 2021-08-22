#pragma once

#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <memory>

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
	std::ofstream log_out;
	std::stringstream sstream;
	std::stringstream prefix_sstream;
	bool console;

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
