#include "Interface/MUtilityIDBank.h"
#include "Interface/MUtilityMath.h"
#include "Interface/MUtilityBitset.h"

#define LOG_CATEGORY_IDBANK "IDBank"

using namespace MUtility;
using MUtility::BitSet;

// ---------- MUtilityBitMaskIDBank ----------

MUtilityBitmaskID MUtilityBitMaskIDBank::GetID()
{
	MUtilityBitmaskID toReturn = 0;
	if (m_RecycledIDs == 0)
	{
		if (1 << m_NextID != 0)
		{
			toReturn = m_NextID;
			m_NextID = static_cast<BitSet>(1) << (MUtilityMath::FastLog2(m_NextID) + 1);
		}
		else
			MLOG_WARNING("Attempted to get next ID but the underlying type has run out of bits; Bitcount of underlying type = " << MUtility::BitSizeof(m_NextID), LOG_CATEGORY_IDBANK);
	}
	else
	{
		toReturn = MUtility::GetLowestSetBit(m_RecycledIDs);
		m_RecycledIDs &= ~toReturn;
	}

	++m_Count;
	return toReturn;
}

bool MUtilityBitMaskIDBank::ReturnID(MUtilityBitmaskID idToReturn)
{
	if (idToReturn == MUtility::EMPTY_BITSET)
		MLOG_WARNING("Attempted to return empty bitset", LOG_CATEGORY_IDBANK);

	if (MUtility::GetSetBitCount(idToReturn) > 1)
	{
		char* bitsetString = new char[MUtility::BITSET_SIZE_BITS + 1];
		MUtility::BitSetToString(idToReturn, bitsetString, MUtility::BITSET_SIZE_BITS + 1);
		MLOG_WARNING("Attempted to return multiple IDs in one call; only one ID can be returned per call; ID = " << bitsetString, LOG_CATEGORY_IDBANK);
		delete[] bitsetString;
		return false;
	}

	if (idToReturn > m_NextID)
	{
		char* bitsetString = new char[MUtility::BITSET_SIZE_BITS + 1];
		MUtility::BitSetToString(idToReturn, bitsetString, MUtility::BITSET_SIZE_BITS + 1);
		MLOG_WARNING("Attempted to return an ID that has not yet been assigned; ID = " << bitsetString, LOG_CATEGORY_IDBANK);
		delete[] bitsetString;
		return false;
	}

	--m_Count;
	m_RecycledIDs |= idToReturn;
	return true;
}

MUtilityBitmaskID MUtilityBitMaskIDBank::PeekNextID() const
{
	return m_NextID;
}

bool MUtilityBitMaskIDBank::IsIDActive(MUtilityBitmaskID id) const
{
	return id != MUtility::EMPTY_BITSET && id < m_NextID && !IsIDRecycled(id);
}

bool MUtilityBitMaskIDBank::IsIDRecycled(MUtilityBitmaskID id) const
{
	return (id & m_RecycledIDs) != 0;
}

uint32_t MUtilityBitMaskIDBank::GetCount() const
{
	return m_Count;
}