#include "interface/MUtilityIDBank.h"
#include "interface/MUtilityMath.h"

#define MUTILITY_LOG_CATEGORY_IDBANK "IDBank"

using MUtility::BitSet;

// ---------- MUtilityIDBank ----------

MUtilityID MUtilityIDBank::GetID()
{
	MUtilityID toReturn;
	m_Lock.lock();
	if (m_RecycledIDs.size() > 0)
	{
		toReturn = m_RecycledIDs.back();
		m_RecycledIDs.pop_back();
	}
	else
	{
		toReturn = m_NextID++;
	}
	m_Lock.unlock();

	return toReturn;
}

bool MUtilityIDBank::ReturnID(MUtilityID idToreturn)
{
	if (idToreturn < 0)
		return false;

	m_Lock.lock();
	if (idToreturn >= m_NextID)
	{
		MLOG_WARNING("Attempted to return an ID that has not yet been assigned; ID = " << idToreturn, MUTILITY_LOG_CATEGORY_IDBANK);
		m_Lock.unlock();
		return false;
	}

#ifdef _DEBUG
	if (IsIDRecycled(idToreturn))
	{
		MLOG_WARNING("Attempted to return an already returned ID; ID = " << idToreturn, MUTILITY_LOG_CATEGORY_IDBANK);
		m_Lock.unlock();
		return false;
	}
#endif

	m_RecycledIDs.push_back(idToreturn);

	m_Lock.unlock();
	return true;
}

bool MUtilityIDBank::IsIDActive(MUtilityID id)
{
	if (id < 0)
		return false;

	bool result = false;
	m_Lock.lock();
	if (id < m_NextID && !IsIDRecycled(id))
		result = true;

	m_Lock.unlock();
	return result;
}

bool MUtilityIDBank::IsIDRecycled(MUtilityID id)
{
	for (int i = 0; i < m_RecycledIDs.size(); ++i)
	{
		if (m_RecycledIDs[i] == id)
			return true;
	}
	return false;
}

// ---------- MUtilityBitwiseIDBank ----------

MUtilityBitwiseID MUtilityBitwiseIDBank::MUtilityBitwiseIDBank::GetID()
{
	MUtilityBitwiseID toReturn = 0;
	if (m_RecycledIDs == 0)
	{
		if (1 << m_NextID != 0)
		{
			toReturn = m_NextID;
			m_NextID = static_cast<BitSet>(1) << (MUtilityMath::FastLog2(m_NextID) + 1);
		}
		else
			MLOG_WARNING("Attempted to get next ID but the underlying type has run out of bits; Bitcount of underlying type = " << MUtility::BitSizeof(m_NextID), MUTILITY_LOG_CATEGORY_IDBANK);
	}
	else
	{
		toReturn = MUtility::GetFirstSetBit(m_RecycledIDs);
		m_RecycledIDs &= ~toReturn;
	}

	return toReturn;
}

bool MUtilityBitwiseIDBank::ReturnID(MUtilityBitwiseID idToReturn)
{
	if (idToReturn == MUtility::EMPTY_BITSET)
		MLOG_WARNING("Attempted to return empty bitset", MUTILITY_LOG_CATEGORY_IDBANK);

	if (MUtility::GetSetBitCount(idToReturn) > 1)
	{
		char* bitsetString = new char[MUtility::BITSET_SIZE_BITS + 1];
		MUtility::BitSetToString(idToReturn, bitsetString, MUtility::BITSET_SIZE_BITS + 1);
		MLOG_WARNING("Attempted to return multiple IDs in one call; only one ID can be returned per call; ID = " << bitsetString, MUTILITY_LOG_CATEGORY_IDBANK);
		delete[] bitsetString;
		return false;
	}

	if (idToReturn > m_NextID)
	{
		char* bitsetString = new char[MUtility::BITSET_SIZE_BITS + 1];
		MUtility::BitSetToString(idToReturn, bitsetString, MUtility::BITSET_SIZE_BITS + 1);
		MLOG_WARNING("Attempted to return an ID that has not yet been assigned; ID = " << bitsetString, MUTILITY_LOG_CATEGORY_IDBANK);
		delete[] bitsetString;
		return false;
	}

	m_RecycledIDs |= idToReturn;
	return true;
}

bool MUtilityBitwiseIDBank::IsIDActive(MUtilityBitwiseID id)
{
	if (id == MUtility::EMPTY_BITSET)
		return false;

	return !IsIDRecycled(id) && id < m_NextID;
}

bool MUtilityBitwiseIDBank::IsIDRecycled(MUtilityBitwiseID id)
{
	return (id & m_RecycledIDs) != 0;
}