#include "interface/MUtilityString.h"
#include <cctype>

bool MUtilityString::IsStringNumber(const std::string& string)
{
	std::string::const_iterator it = string.begin();
	while (it != string.end() && std::isdigit(*it)) ++it;
	return !string.empty() && it == string.end();
}

bool MUtilityString::IsStringNumberExcept(const std::string& string, char exception, uint32_t maxExceptions)
{
	std::string::const_iterator it = string.begin();
	uint32_t exceptionCount = 0;
	while (it != string.end())
	{
		if (!std::isdigit(*it))
		{
			if (*it == exception)
			{
				++exceptionCount;
				if (exceptionCount > maxExceptions)
					break;
			}
			else
				break;
		}
		++it;
	}

	return !string.empty() && it == string.end();
}