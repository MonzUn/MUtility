#pragma once
#include "interface/MUtilityLog.h"
#include <unordered_map>

struct LogValuePair
{
	LogValuePair(MUtilityLogLevel::LogLevel initialInterestLevels) : InterestLevels(initialInterestLevels) {}

	MUtilityLogLevel::LogLevel	InterestLevels;
	std::stringstream			Log;
};
typedef std::unordered_map<std::string, LogValuePair>::iterator LogMapIterator;

namespace MUtilityLog
{
	LogMapIterator RegisterCategory(const char* categoryName, MUtilityLogLevel::LogLevel initialInterestLevels);
};