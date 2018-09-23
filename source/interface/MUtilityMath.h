#pragma once
#include "MUtilityIntrinsics.h"
#include <cassert>
#include <stdint.h>

namespace MUtilityMath
{
	inline uint32_t FastLog2(uint64_t value)
	{
		assert(value > 0 && "Atempting to calculate Log2 of 0");
		unsigned long toReturn = 0;
		assert(static_cast<uint32_t>(MUtility::BitscanReverse(value, &toReturn)) && "Failed to calculate log2 through BitscanReverse");
		return toReturn;
	}
}