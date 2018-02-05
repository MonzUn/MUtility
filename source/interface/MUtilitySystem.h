#pragma once
#include "MUtilityPlatformDefinitions.h"
#include <string>

namespace MUtility
{
	int		GetPid();
	void	GetExecutableDirectoryPath(std::string& outPath);
}