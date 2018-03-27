#pragma once
#include "MUtilityPlatformDefinitions.h"
#include <limits.h>
#include <stdint.h>

namespace MUtility
{
	typedef uint8_t Byte;
	
	constexpr int32_t BITS_PER_BYTE = CHAR_BIT;
	
	template <typename T>
	uint64_t BitSizeof(const T& type)
	{
		return sizeof(T) * BITS_PER_BYTE;
	}
}