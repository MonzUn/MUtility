#include "Interface/MUtilitySystem.h"
#include "Interface/MUtilityLog.h"
#include <algorithm>

#if PLATFORM == PLATFORM_WINDOWS
#include <process.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#define LOG_CATEGORY_SYSTEM "MUtilitySystem"

constexpr uint32_t EXECUTABLE_PATH_MAX_LENGTH = 10000;

int MUtility::GetPid()
{
#if PLATFORM == PLATFORM_WINDOWS
	return _getpid();
#else
	return static_cast<int>(getpid());
#endif
}

std::string MUtility::GetExecutableDirectoryPath() // TODODB: Implement support for linux platform
{
	std::string returnPath = "FAILED TO FIND EXECUTABLE PATH";
#if PLATFORM != PLATFORM_WINDOWS
		return "Executable path is only available on the windows platform";
#else
		char path[EXECUTABLE_PATH_MAX_LENGTH];
		GetModuleFileName(nullptr, path, EXECUTABLE_PATH_MAX_LENGTH);
		PathRemoveFileSpec(path);
		returnPath = path;
		std::replace(returnPath.begin(), returnPath.end(), '\\', '/');
#endif

		return returnPath;
}

void MUtility::OpenBrowserOnURL(const char* URL)
{
#if PLATFORM != PLATFORM_WINDOWS
	MLOG_WARNING("Function called on non supported system", LOG_CATEGORY_SYSTEM);
	return;
#else
	ShellExecute(nullptr, nullptr, URL, nullptr, nullptr, SW_SHOW);
#endif
}

void MUtility::UnblockSTDIn()
{
#if PLATFORM != PLATFORM_WINDOWS
	MLOG_WARNING("Function called on non supported system", LOG_CATEGORY_SYSTEM);
	return;
#else
	INPUT_RECORD inputRecord[2];
	inputRecord[0].EventType = KEY_EVENT;
	inputRecord[0].Event.KeyEvent.bKeyDown = TRUE;
	inputRecord[0].Event.KeyEvent.dwControlKeyState = 0;
	inputRecord[0].Event.KeyEvent.uChar.UnicodeChar = VK_RETURN;
	inputRecord[0].Event.KeyEvent.wRepeatCount = 1;
	inputRecord[0].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
	inputRecord[0].Event.KeyEvent.wVirtualScanCode = MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);
	inputRecord[1] = inputRecord[0];
	inputRecord[1].Event.KeyEvent.bKeyDown = FALSE;

	DWORD temp;
	WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), inputRecord, 2, &temp);
#endif
}