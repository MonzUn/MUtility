#include "interface/MUtilityByte.h"

using MUtility::BitSet;

int32_t MUtility::GetSetBitCount(const BitSet& bitset)
{
	int32_t counter = 0;
	for (int32_t i = 0; i < static_cast<int32_t>(BitSizeof(bitset)); ++i)
	{
		if ((bitset & (static_cast<BitSet>(1) << i)) != 0)
			counter++;
	}
	return counter;
}

BitSet MUtility::GetLowestSetBit(const BitSet& bitset)
{
	return static_cast<BitSet>(1) << GetLowestSetBitIndex(bitset);
}

BitSet MUtility::GetHighestSetBit(const BitSet& bitset)
{
	return static_cast<BitSet>(1) << GetHighestSetBitIndex(bitset);
}

int32_t MUtility::GetLowestSetBitIndex(const BitSet& bitset)
{
	if (bitset == EMPTY_BITSET)
		return EMPTY_BITSET;

	for (int32_t i = 0; i < static_cast<int32_t>(BitSizeof(bitset) - 1); ++i)
	{
		if ((bitset & (static_cast<BitSet>(1) << i)) != 0)
			return i;
	}
	return EMPTY_BITSET;
}

int32_t MUtility::GetHighestSetBitIndex(const BitSet& bitset)
{
	if (bitset == EMPTY_BITSET)
		return EMPTY_BITSET;

	for (int32_t i = static_cast<int32_t>(BitSizeof(bitset) - 1); i > 0; --i)
	{
		if ((bitset & (static_cast<BitSet>(1) << i)) != 0)
			return i;
	}
	return EMPTY_BITSET;
}

void MUtility::BitSetToString(const BitSet& bitset, char* buffer, int32_t bufferSize)
{
	if (bufferSize < BITSET_SIZE_BITS + 1)
		return;

	buffer[BITSET_SIZE_BITS] = '\0';
	for (int i = BITSET_SIZE_BITS - 1; i >= 0; --i)
	{
		buffer[i] = (((bitset & (static_cast<BitSet>(1) << (BITSET_SIZE_BITS - i))) != 0) ? '1' : '0');
	}
}