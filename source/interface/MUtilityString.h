#pragma once
#include <string>
#include <limits>

namespace MUtility
{
	bool	IsStringNumber(const std::string& string);
	bool	IsStringNumberExcept(const std::string& string, char exception, uint32_t maxExceptions = std::numeric_limits<uint32_t>::max());

	int32_t StringToInt(const std::string& str);
	double	StringToDouble(const std::string& str);
	bool	StringToBool(const std::string& str);
}