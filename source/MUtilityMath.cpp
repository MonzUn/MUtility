#include "Interface/MUtilityMath.h"
#include "Interface/MUtilityIntrinsics.h"

uint32_t MUtilityMath::FastLog2(uint64_t value)
{
	return MUtility::BitscanReverse(value);
}