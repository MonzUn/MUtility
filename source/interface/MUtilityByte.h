#pragma once
#include "MUtilityPlatformDefinitions.h"
#include <limits.h>
#include <string>
#include <stdint.h>

#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
#include <intrin.h>
#endif

namespace MUtility
{
	typedef uint8_t Byte;
	typedef uint64_t BitSet;
	constexpr BitSet EMPTY_BITSET = 0;

	constexpr int32_t BITS_PER_BYTE = CHAR_BIT;
	constexpr int32_t BITSET_SIZE_BYTES = sizeof(BitSet);
	constexpr int32_t BITSET_SIZE_BITS = BITSET_SIZE_BYTES * BITS_PER_BYTE;

	template <typename T>
	uint64_t BitSizeof(const T& type)
	{
		return sizeof(T) * BITS_PER_BYTE;
	}

	template<typename T>
	uint32_t BitscanForward(const T& toScan)
	{
		unsigned long toReturn = 0;
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			_BitScanForward64(&toReturn, toScan);
		#endif
		return toReturn;
	}

	template<typename T>
	uint32_t BitscanReverse(const T& toScan)
	{
		unsigned long toReturn = 0;
		#if PLATFORM == PLATFORM_WINDOWS && COMPILER == COMPILER_MSVC
			_BitScanReverse64(&toReturn, toScan);
		#endif
		return toReturn;
	}


	int32_t GetSetBitCount(const BitSet& bitset);
	BitSet	GetLowestSetBit(const BitSet& bitset);
	BitSet	GetHighestSetBit(const BitSet& bitset);
	int32_t	GetLowestSetBitIndex(const BitSet& bitset);
	int32_t	GetHighestSetBitIndex(const BitSet& bitset);
	void	BitSetToString(const BitSet& bitset, char* buffer, int32_t bufferSize);
	std::string BitSetToString(const BitSet& bitset);
}