#pragma once
#include "MUtilityLog.h"
#include "MUtilityTypes.h"
#include <mutex>
#include <stdint.h>
#include <vector>

#define MUTILITY_LOG_CATEGORY_IDBANK "IDBank"

class MUtilityIDBank
{
public:
	MUtilityID GetID()
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

	bool ReturnID(MUtilityID idToreturn)
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

	bool IsIDActive(MUtilityID id)
	{
		if (id < 0)
			return false;

		m_Lock.lock();
		if (id >= m_NextID || IsIDRecycled(id))
		{
			m_Lock.unlock();
			return false;
		}

		m_Lock.unlock();
		return true;
	}

	bool IsIDRecycled(MUtilityID id)
	{
		for (int i = 0; i < m_RecycledIDs.size(); ++i)
		{
			if (m_RecycledIDs[i] == id)
				return true;
		}
		return false;
	}

private:
	MUtilityID m_NextID = 0;
	std::vector<MUtilityID> m_RecycledIDs;
	std::mutex m_Lock;
};