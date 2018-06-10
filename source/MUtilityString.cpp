#include "Interface/MUtilityString.h"
#include <algorithm>
#include <cctype>

bool MUtility::IsStringNumber(const std::string& string)
{
	if(string.empty())
		return false;

	if(string.size() == 1 && !std::isdigit(string[0])) // A minus sign alone is not a a number
		return false;

	std::string::const_iterator it = string[0] == '-' ? string.begin() + 1 : string.begin(); // Allow the first character to be a minus sign
	while (it != string.end() && std::isdigit(*it)) ++it;
	return it == string.end();
}

bool MUtility::IsStringNumberExcept(const std::string& string, char exception, uint32_t maxExceptions) // TODODB: Take string as exception and except on all characters in that string
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

int32_t MUtility::StringToInt(const std::string& str)
{
	return IsStringNumber(str) ? std::stoi(str) : -1;
}

double MUtility::StringToDouble(const std::string& str)
{
	return (MUtility::IsStringNumberExcept(str, '.', 1) || MUtility::IsStringNumberExcept(str, ',', 1)) ? std::stod(str) : -1.0f;
}

bool MUtility::StringToBool(const std::string& str)
{
	std::string lowerCaseStr = str;
	std::transform(lowerCaseStr.begin(), lowerCaseStr.end(), lowerCaseStr.begin(), ::tolower);
	return lowerCaseStr == "true" ? true : false;
}