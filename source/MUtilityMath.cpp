#include "interface/MUtilityMath.h"
#include "interface/MUtilityByte.h"

uint32_t MUtilityMath::FastLog2(uint64_t value)
{
	return MUtility::BitscanReverse(value);
}