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

	void Init(std::string filename, bool console);
	~Logger();

	template <typename T>
	friend Logger& operator<< (Logger& logger, const T& x);
	friend Logger& operator<< (Logger& logger, LogEndl s);

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

extern Logger Log;
