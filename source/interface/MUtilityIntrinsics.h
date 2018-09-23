#pragma once
#include "MUtilityByte.h"
#include "MUtilityPlatformDefinitions.h"

#if COMPILER == COMPILER_MSVC
#include <intrin.h>
#endif

// TODODB: Unit test

namespace MUtility
{
	inline bool TestBit(const int64_t toTest, int64_t bitIndex)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return _bittest64(&toTest, bitIndex);
		#else
			return (toTest & (1ULL << bitIndex)) != 0;
		#endif
	}

	inline void SetBit(int64_t& toSet, int64_t bitIndex)
	{
#if PLATFORM == PLATFORM_WINDOWS && COMPILER== COMPILER_MSVC
		_bittestandset64(&toSet, bitIndex);
#else
		toSet |= (1ULL << bitIndex);
#endif
	}

	inline void ResetBit(int64_t& toReset, int64_t bitIndex)
	{
#if  PLATFORM == PLATFORM_WINDOWS && COMPILER== COMPILER_MSVC
		_bittestandreset64(&toReset, bitIndex);
#else
		toReset &= ~(1ULL << bitIndex);
#endif
	}

	inline int64_t PopCount(const uint64_t toCount)
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

	inline bool BitscanForward(const int64_t& toScan, unsigned long* outIndex)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return _BitScanForward64(outIndex, toScan);
		#else
			for (uint64_t i = 0; i < BitSizeof(toScan) - 1; ++i)
			{
				if ((toScan & (1ULL << i)) != 0)
				{
					if(outIndex != nullptr)
						*outIndex = i;

					return true;
				}
			}
			return false;
		#endif
	}

	inline bool InverseBitscanForward(const int64_t& toScan, unsigned long* outIndex)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return _BitScanForward64(outIndex, ~toScan);
		#else
			for (uint64_t i = 0; i < BitSizeof(toScan) - 1; ++i)
			{
				if ((toScan & (1ULL << i)) == 0)
				{
					if (outIndex != nullptr)
						*outIndex = i;

					return true;
				}
			}
			return false;
		#endif
	}

	inline bool BitscanReverse(const int64_t& toScan, unsigned long* outIndex)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return _BitScanReverse64(outIndex, toScan);
		#else	
			for (int64_t i = static_cast<int64_t>(BitSizeof(toScan) - 1); i >= 0; --i)
			{
				if ((toScan & (1ULL << i)) != 0)
				{
					if (outIndex != nullptr)
						*outIndex = i;

					return true;
				}
			}
			return -1;
		#endif
	}

	inline bool InverseBitscanReverse(const int64_t& toScan, unsigned long* outIndex)
	{
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			return _BitScanReverse64(outIndex, ~toScan);
		#else	
			for (int64_t i = static_cast<int64_t>(BitSizeof(toScan) - 1); i >= 0; --i)
			{
				if ((toScan & (1ULL << i)) == 0)
				{
					if (outIndex != nullptr)
						*outIndex = i;

					return true;
				}
			}
			return false;
		#endif
	}
}