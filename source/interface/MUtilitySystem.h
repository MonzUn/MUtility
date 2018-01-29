#pragma once
#include "MUtilityPlatformDefinitions.h"
#include <string>

namespace MUtility
{
	int		GetPid();
	void	GetExecutablePath(std::string& outPath);
}