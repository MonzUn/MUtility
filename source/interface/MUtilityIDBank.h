#pragma once
#include "MUtilityByte.h"
#include "MUtilityDynamicBitset.h"
#include "MUtilityThreading.h"
#include "MUtilityTypes.h"
#include <atomic>
#include <cassert>
#include <deque>
#include <mutex>
#include <stdint.h>
#include <type_traits>

#pragma warning( push )
#pragma warning( disable : 4018) // Fix issues causing these warnings (See comment regarding these warnings further down)
#pragma warning( disable : 4244)

namespace MUtility
{
	template<typename IDType>
	class MUtilityIDBank // TODODB: Remove redundant MUTility from name
	{
	public:
		MUtilityIDBank() = default;
		MUtilityIDBank(const MUtilityIDBank& other)
		{
			MUtilityThreading::LockMutexes({&m_Lock, &other.m_Lock});
			m_NextID = other.m_NextID.load();
			m_IDBitSet = other.m_IDBitSet;
			m_Count = other.m_Count.load();
			MUtilityThreading::UnlockMutexes({&m_Lock, &other.m_Lock});
		}
		~MUtilityIDBank() = default;

		IDType GetID()
		{
			IDType toReturn;
			m_Lock.lock();
			toReturn = IDType(m_IDBitSet.FindFirstUnsetBitExpand());
			if (toReturn == -1)
			{
				toReturn = IDType(m_IDBitSet.Capacity());
				m_IDBitSet.Reserve();
			}
			m_IDBitSet.Set(toReturn);
			m_Lock.unlock();

			if(toReturn == m_NextID)
				++m_NextID;

			++m_Count;

			return toReturn;
		}

		bool ReturnID(IDType toReturn)
		{
			if (toReturn < 0 || toReturn >= m_NextID)
			{
				assert(false && "Attempted to return an invalid ID");
				return false;
			}

#if COMPILE_MODE == COMPILE_MODE_DEBUG
			if (IsIDInactive(toReturn))
			{
				assert(false && "Attempted to return the same ID multiple times. This check is removed in release mode so if this asserts is triggered in debug mode it could cause issues in release mode"); // TODODB: Create a system for managing asserts depending on level of importance
				return false;
			}
#endif
			m_Lock.lock();
			m_IDBitSet.Reset(toReturn);
			m_Lock.unlock();
			--m_Count;

			return true;
		}

		IDType PeekNextNewID() const
		{
			return IDType(m_NextID);
		}

		bool IsIDValid(IDType ID) const
		{
			return ID >= 0 && ID < m_NextID;;
		}

		bool IsIDActive(IDType ID) const
		{
			m_Lock.lock();
			bool result = ID >= 0 && ID < m_NextID && m_IDBitSet.Test(ID);
			m_Lock.unlock();

			return result;
		}

		bool IsIDInactive(IDType ID) const
		{
			m_Lock.lock();
			bool result = ID < 0 || ID >= m_NextID || !m_IDBitSet.Test(ID);
			m_Lock.unlock();

			return result;
		}

		bool IsIDHighest(IDType ID) const
		{
			return ID == (m_NextID - 1);
		}

		uint32_t GetTotalCount() const
		{
			return m_NextID;
		}

		uint32_t GetActiveCount() const
		{
			return m_Count;
		}

	private:
		std::atomic<int32_t>	m_NextID = 0; // TODODB: This needs some kind of templating to work with both strongIDs and regular integer IDs (Triggers warning C4018 and C4244)
		DynamicBitset			m_IDBitSet;
		std::atomic<uint32_t>	m_Count = 0;

		mutable std::mutex m_Lock;
	};

	// TODODB: Remove redundant MUtility from name
	// TODODB: Update to use bitset like regular template version
	// TDODDB: See if this can be made a tempalte specialization of the regular templated version
	class MUtilityBitMaskIDBank 
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

#pragma warning( pop )