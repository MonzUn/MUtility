#include "interface/MUtilityString.h"
#include <cctype>

bool MUtilityString::IsStringNumber(const std::string& string)
{
	std::string::const_iterator it = string.begin();
	while (it != string.end() && std::isdigit(*it)) ++it;
	return !string.empty() && it == string.end();
}