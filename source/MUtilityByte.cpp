#include "interface/MUtilityByte.h"

using MUtility::BitSet;

uint64_t MUtility::GetSetBitCount(const BitSet& bitset)
{
	return PopCount(bitset);
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

	return BitscanForward(bitset);
}

int32_t MUtility::GetHighestSetBitIndex(const BitSet& bitset)
{
	if (bitset == EMPTY_BITSET)
		return EMPTY_BITSET;

	return BitscanReverse(bitset);
}

bool MUtility::IsBitSet(const BitSet& bitset, uint32_t indexToTest)
{
	return TestBit(bitset, indexToTest);
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

std::string MUtility::BitSetToString(const BitSet& bitset)
{
	std::string toReturn = "";
	toReturn.reserve(BITSET_SIZE_BITS);
	for (int i = BITSET_SIZE_BITS - 1; i >= 0; --i)
	{
		toReturn[i] = (((bitset & (static_cast<BitSet>(1) << (BITSET_SIZE_BITS - i))) != 0) ? '1' : '0');
	}
	return toReturn;
}