#include "interface/MUtilitySystem.h"
#include <algorithm>

#if PLATFORM == PLATFORM_WINDOWS
#include <process.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/types.h>
#include <unistd.h>
#endif

constexpr uint32_t EXECUTABLE_PATH_MAX_LENGTH = 10000;

int MUtility::GetPid()
{
#if PLATFORM == PLATFORM_WINDOWS
	return _getpid();
#else
	return static_cast<int>(getpid());
#endif
}

void MUtility::GetExecutableDirectoryPath(std::string& outPath) // TODODB: Implement support for linux platform
{
#if PLATFORM != PLATFORM_WINDOWS
		return "Executable path is only available on the windows platform";
#else
		char path[EXECUTABLE_PATH_MAX_LENGTH];
		GetModuleFileName(NULL, path, EXECUTABLE_PATH_MAX_LENGTH);
		PathRemoveFileSpec(path);
		outPath = std::string(path);
		std::replace(outPath.begin(), outPath.end(), '\\', '/');
#endif
}