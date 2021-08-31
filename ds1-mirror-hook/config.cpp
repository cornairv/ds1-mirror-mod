#include "pch.h"
#include "config.h"
#include "logger.h"
#include <algorithm>
#include <sstream>
#include <cerrno>
#include <ini.h>

struct MyConfig config;

MyINIReader::MyINIReader(const std::string& filename)
{
    int errorCode = ini_parse(filename.c_str(), ValueHandler, this);
    if (errorCode < 0)
    {
        error = "error reading ini file; using defaults";
    }
    else if (errorCode > 0)
    {
        error = "error parsing ini file on line " + std::to_string(errorCode) + "; using defaults";
    }
}

bool MyINIReader::GetString(const std::string& section, const std::string& name, std::string& out)
{
    error.clear();
    std::string key = MakeKey(section, name);
    auto it = values.find(key);
    if (it == values.end())
    {
        error = "[" + section + "] " + name + ": key not found; using default";
        return true;
    }
    else
    {
        out = it->second;
        return true;
    }
}

bool MyINIReader::GetBoolean(const std::string& section, const std::string& name, bool& out)
{
    std::string valstr;
    if (!GetString(section, name, valstr))
    {
        return false;
    }

    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);

    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
    {
        out = true;
        return true;
    }
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
    {
        out = false;
        return true;
    }
    else
    {
        error = "[" + section + "] " + name + ": invalid boolean; using default";
        return false;
    }
}

bool MyINIReader::GetInteger(const std::string& section, const std::string& name, int& out)
{
    std::string valstr;
    if (!GetString(section, name, valstr))
    {
        return false;
    }

    char* end;
    errno = 0;
    long val = strtol(valstr.c_str(), &end, 0);
    if (errno == 0 && end > valstr.c_str())
    {
        out = val;
        return true;
    }
    else
    {
        error = "[" + section + "] " + name + ": invalid integer; using default";
        return false;
    }
}

void MyINIReader::LogError()
{
    if (!error.empty())
    {
        LOG << error << LogEndl::WARN;
    }
}

std::string MyINIReader::MakeKey(const std::string& section, const std::string& name)
{
    std::string key = section + "=" + name;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int MyINIReader::ValueHandler(void* user, const char* section, const char* name, const char* value)
{
    MyINIReader* reader = static_cast<MyINIReader*>(user);
    std::string key = MakeKey(section, name);
    if (reader->values[key].size() > 0)
    {
        reader->values[key] += "\n";
    }
    reader->values[key] += value ? value : "";
    return 1;
}

void configure(std::string filePath)
{
    MyINIReader reader(filePath);

    std::string section = "debug";
    if (reader.GetString(section, "log_file", config.logFile))
    {
        Logger::enabled = true;
    }
    std::string logError = reader.error;
    if (reader.GetBoolean(section, "show_console", config.showConsole))
    {
        Logger::enabled = true;
    }
    if (!logError.empty())
    {
        LOG << logError << LogEndl::WARN;
    }
    reader.LogError();

    section = "input";
    reader.GetBoolean(section, "invert_attacks", config.invertAttacks);
    reader.LogError();
    reader.GetBoolean(section, "invert_weapon_switch", config.invertWeaponSwitch);
    reader.LogError();
    reader.GetBoolean(section, "invert_directions", config.invertDirections);
    reader.LogError();
    reader.GetInteger(section, "toggle_key", config.toggleKey);
    reader.LogError();

    section = "compatibility";
    reader.GetString(section, "load_dll", config.loadDll);
    reader.LogError();
}