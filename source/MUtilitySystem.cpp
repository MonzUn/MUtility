#include "interface/MUtilitySystem.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <process.h>  
#else
#include <sys/types.h>
#include <unistd.h>
#endif

int MUtility::GetPid()
{
#if PLATFORM == PLATFORM_WINDOWS
	return _getpid();
#else
	return static_cast<int>(getpid());
#endif
}