#pragma once

#include "pch.h"
#include <string>
#include <map>

void configure(std::string iniFile);

class MyINIReader
{
public:
	MyINIReader(const std::string& filename);
	bool GetString(const std::string& section, const std::string& name, std::string& out);
	bool GetBoolean(const std::string& section, const std::string& name, bool& out);
	bool GetInteger(const std::string& section, const std::string& name, int& out);
	void LogError();
	std::string error;

private:
	std::map<std::string, std::string> values;
	static std::string MakeKey(const std::string& section, const std::string& name);
	static int ValueHandler(void* user, const char* section, const char* name, const char* value);
};

struct MyConfig
{
	bool invertDirections = false;
	bool invertAttacks = false;
	bool invertWeaponSwitch = false;

	int toggleKey = '0';

	std::string loadDll = "";
	std::string logFile = "";
	bool showConsole = false;
};

extern struct MyConfig config;