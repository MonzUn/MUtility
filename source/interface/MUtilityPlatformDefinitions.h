#pragma once
#define PLATFORM_WINDOWS	1
#define PLATFORM_LINUX		2
#define PLATFORM_MAC		3

#define COMPILER_MSVC		1

#define COMPILE_MODE_DEBUG		1
#define COMPILE_MODE_RELEASE	2

// OS
#if defined _WIN32
	#define PLATFORM PLATFORM_WINDOWS
#elif defined __APPLE__
	#define PLATFORM PLATFORM_MAC
#elif defined __linux
	#define PLATFORM PLATFORM_LINUX
#endif

// Compiler
#if defined _MSC_VER
	#define COMPILER COMPILER_MSVC
#endif

// Mode
#ifdef _DEBUG
	#define COMPILE_MODE COMPILE_MODE_DEBUG
#else
	#define COMPILE_MODE COMPILE_MODE_RELEASE
#endif

#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
	#if COMPILE_MODE == COMPILE_MODE_DEBUG
		#define WINDOWS_DEBUG
	#endif
#endif