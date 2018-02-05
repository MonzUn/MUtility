#pragma once
#include <string>
#include <limits>

namespace MUtilityString
{
	bool IsStringNumber(const std::string& string);
	bool IsStringNumberExcept(const std::string& string, char exception, uint32_t maxExceptions = std::numeric_limits<uint32_t>::max());
}