#pragma once
#include "MUtilityPlatformDefinitions.h"
#include <string>

namespace MUtility
{
	int			GetPid();
	std::string GetExecutableDirectoryPath();

	void		OpenBrowserOnURL(const char* URL);
	void		UnblockSTDIn();
}