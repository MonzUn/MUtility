#include "interface/MUtilityFile.h"
#include "interface/MUtilityLog.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#define MUTILITY_LOG_CATEGORY_LOG "MUtilityLog"

struct LogValuePair
{
	LogValuePair(MUtilityLogLevel::LogLevel initialInterestLevels) : InterestLevels(initialInterestLevels) {}

	MUtilityLogLevel::LogLevel	InterestLevels;
	std::stringstream			Log;
};

std::unordered_map<std::string, LogValuePair> Logs;
std::stringstream MainLog;
std::stringstream FullInterestLog;

typedef std::unordered_map<std::string, LogValuePair>::iterator LogMapIterator;

LogMapIterator RegisterCategory(const char* categoryName, MUtilityLogLevel::LogLevel initialInterestLevels);

void MUtilityLog::Initialize()
{
	
}

void MUtilityLog::Shutdown()
{
	FlushToDisk();
}

void MUtilityLog::SetInterest(const std::string& category, MUtilityLogLevel::LogLevel newInterestLevels)
{
	LogMapIterator iterator = Logs.find(category);
	if (iterator == Logs.end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	iterator->second.InterestLevels = newInterestLevels;
}

void MUtilityLog::Log(const std::string& message, const std::string& category, MUtilityLogLevel::LogLevel logLevel, MUtilityLogMode logMode, const char* file, const char* line, const char* functionName)
{
	LogMapIterator iterator = Logs.find(category);
	if (iterator == Logs.end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	std::stringstream localStream;

	switch (logLevel)
	{
	case MUtilityLogLevel::LOG_ERROR:
		localStream << "Error\n";
		break;

	case MUtilityLogLevel::LOG_WARNING:
		localStream << "Warning\n";
		break;

	case MUtilityLogLevel::LOG_INFO:
		localStream << "Info\n";
		break;

	case MUtilityLogLevel::LOG_DEBUG:
		localStream << "Debug\n";
		break;

	default:
		MLOG_WARNING("Invalid loglevel supplied; call ignored", MUTILITY_LOG_CATEGORY_LOG);
		return;
	}

	switch (logMode)
	{
	case MUtilityLogMode::Normal:
		localStream << "Category: " << category << '\n' << message << "\n\n";
		break;
	case MUtilityLogMode::Debug:
		localStream << "Category: " << category << '\n' << "File: " << file << '\n' << "Line: " << line << '\n' << "Function: " << functionName << "\n - " << message << "\n\n";
		break;
	default:
		MLOG_WARNING("Invalid logMode supplied; call ignored", MUTILITY_LOG_CATEGORY_LOG);
		return;
	}

	const std::string fullMessage = localStream.str();

	if (logLevel & iterator->second.InterestLevels)
	{
		iterator->second.Log << fullMessage;
		MainLog << fullMessage;
		std::cout << fullMessage;
	}

	FullInterestLog << fullMessage;
}

void MUtilityLog::FlushToDisk()
{
	MUtilityFile::CreateFolder("logs");

	std::ofstream outStream;
	outStream.open("logs/mainLog.txt", std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << MainLog.rdbuf();
		outStream.close();
	}

	outStream.open("logs/fullInterestLog.txt", std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << FullInterestLog.rdbuf();
		outStream.close();
	}

	MUtilityFile::CreateFolder("logs/categories");
	for (auto it = Logs.begin(); it != Logs.end(); ++it)
	{
		outStream.open("logs/categories/" + it->first + ".txt", std::ofstream::out | std::ofstream::trunc);
		if (outStream.is_open())
		{
			outStream << it->second.Log.rdbuf();
			outStream.close();
		}
	}
}

LogMapIterator RegisterCategory(const char* categoryName, MUtilityLogLevel::LogLevel initialInterestLevels)
{
	if (Logs.find(categoryName) != Logs.end())
	{
		MLOG_WARNING("Attempted to register logger category \"" << std::string(categoryName) + "\" multiple times; call ignored", MUTILITY_LOG_CATEGORY_LOG);
		return Logs.end();
	}

	return Logs.emplace(categoryName, initialInterestLevels).first;
}