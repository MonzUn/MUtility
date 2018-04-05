#pragma once
#include "MUtilityByte.h"
#include "MUtilityLog.h"
#include "MUtilityTypes.h"
#include "MUtilityStrongID.h"
#include <atomic>
#include <cassert>
#include <deque>
#include <mutex>
#include <stdint.h>
#include <type_traits>

namespace MUtility
{
	template<typename IDType>
	class MUtilityIDBank // TODODB: Remove redundant MUTility from name
	{
	public:
		IDType GetID()
		{
			IDType toReturn;
			m_Lock.lock();
			if (m_RecycledIDs.size() > 0)
			{
				toReturn = m_RecycledIDs.front();
				m_RecycledIDs.pop_front();
			}
			else
			{
				toReturn = IDType(m_NextID++);
			}
			++m_Count;
			m_Lock.unlock();

			return toReturn;
		}

		bool ReturnID(IDType toReturn)
		{
			if (toReturn < 0 || toReturn >= m_NextID)
				return false;

#if COMPILE_MODE == COMPILE_MODE_DEBUG
			if (IsIDRecycled(toReturn))
			{
				assert(false && "Attempted to return the same ID multiple times. This check is removed in release mode so if this asserts is triggered in debug mode it could cause issues in release mode");
				return false;
			}
#endif
			m_Lock.lock();
			m_RecycledIDs.push_back(toReturn);
			m_Lock.unlock();
			--m_Count;

			return true;
		}

		IDType PeekNextNewID() const
		{
			return IDType(m_NextID);
		}

		bool IsIDActive(IDType ID) const
		{
			return ID >= 0 && ID < m_NextID && !IsIDRecycled(ID);
		}

		bool IsIDRecycled(IDType ID) const
		{
			bool result = false;
			m_Lock.lock();
			for (int i = 0; i < m_RecycledIDs.size(); ++i)
			{
				if (m_RecycledIDs[i] == ID)
				{
					result = true;
					break;
				}
			}
			m_Lock.unlock();
			return result;
		}

		bool IsIDLast(IDType ID) const
		{
			return ID == (m_NextID - 1);
		}

		uint32_t GetCount() const
		{
			return m_Count;
		}

	private:
		std::atomic<int32_t>	m_NextID = 0;
		std::deque<IDType>		m_RecycledIDs;
		std::atomic<uint32_t>	m_Count = 0;

		mutable std::mutex m_Lock;
	};

	class MUtilityBitMaskIDBank // TODODB: Remove redundant MUtility from name
	{
	public:
		MUtilityBitmaskID GetID();

		bool ReturnID(MUtilityBitmaskID idToReturn);
		MUtilityBitmaskID PeekNextID() const;

		bool IsIDActive(MUtilityBitmaskID id) const;
		bool IsIDRecycled(MUtilityBitmaskID id) const;

		uint32_t GetCount() const;

	private:
		std::atomic<MUtilityBitmaskID>	m_NextID = 1;
		std::atomic<MUtilityBitmaskID>	m_RecycledIDs = 0;
		std::atomic<uint32_t>			m_Count = 0;
	};
}