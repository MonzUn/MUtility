#include "Interface/MUtilityString.h"
#include <cctype>

bool MUtilityString::IsStringNumber(const std::string& string)
{
	if(string.empty())
		return false;

	if(string.size() == 1 && !std::isdigit(string[0])) // A minus sign alone is not a a number
		return false;

	std::string::const_iterator it = string[0] == '-' ? string.begin() + 1 : string.begin(); // Allow the first character to be a minus sign
	while (it != string.end() && std::isdigit(*it)) ++it;
	return it == string.end();
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