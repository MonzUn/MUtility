#pragma once
#include "MUtilityPlatformDefinitions.h"

#if PLATFORM == PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#endif