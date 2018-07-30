#pragma once
#include "MUtilityPlatformDefinitions.h"
#include <string>

#if PLATFORM == PLATFORM_WINDOWS
#include <winerror.h>
#endif

namespace MUtility
{
	int			GetPid();
	std::string GetExecutableDirectoryPath();

	void		OpenBrowserOnURL(const char* URL);
	void		UnblockSTDIn();

#if PLATFORM == PLATFORM_WINDOWS
	std::string GetHResultErrorCodeString(HRESULT result);
	std::string GetHResultErrorDescriptionString(HRESULT result);
#endif
}