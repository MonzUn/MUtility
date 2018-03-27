#pragma once
#include "MUtilityByte.h"
#include "MUtilityPlatformDefinitions.h"

#if COMPILER == COMPILER_MSVC
#include <intrin.h>
#endif

namespace MUtility
{
	template<typename T>
	inline bool TestBit(const T& toTest, uint32_t bitIndex)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return _bittest64(&static_cast<int64_t>(toTest), bitIndex);
		#else
			return (toTest & (1ULL << bitIndex)) != 0;
		#endif
	}

	template<typename T>
	inline int64_t PopCount(const T& toCount)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return __popcnt64(toCount);
		#else 
			int64_t counter = 0;
			for (int64_t i = 0; i < static_cast<int64_t>(BitSizeof(toCount)); ++i)
			{
				if ((toCount & (1ULL << i)) != 0)
					counter++;
			}
			return counter;
		#endif
	}

	template<typename T>
	inline int32_t BitscanForward(const T& toScan)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			unsigned long toReturn = 0;
			_BitScanForward64(&toReturn, toScan);
			return toReturn;
		#else
			for (int32_t i = 0; i < static_cast<int32_t>(BitSizeof(toScan) - 1); ++i)
			{
				if ((toScan & (1ULL << i)) != 0)
					return i;
			}

			return -1;
		#endif
	}

	template<typename T>
	inline int32_t BitscanReverse(const T& toScan)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			unsigned long toReturn = 0;
			_BitScanReverse64(&toReturn, toScan);
			return toReturn;
		#else	
			for (int32_t i = static_cast<int32_t>(BitSizeof(toScan) - 1); i >= 0; --i)
			{
				if ((toScan & (1ULL << i)) != 0)
					return i;
			}
			return -1;
		#endif
	}
}