#pragma once
#include "MUtilityByte.h"
#include "MUtilityLog.h"
#include "MUtilityTypes.h"
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <type_traits>
#include <deque>

namespace MUtility
{
	class MUtilityIDBank
	{
	public:
		MUtilityID GetID();
		bool ReturnID(MUtilityID idToreturn);

		MUtilityID PeekNextID() const;

		bool IsIDActive(MUtilityID id) const;
		bool IsIDRecycled(MUtilityID id) const;

		uint32_t GetCount() const;

	private:
		std::atomic<MUtilityID> m_NextID = 0;
		std::deque<MUtilityID> m_RecycledIDs;
		std::atomic<uint32_t>  m_Count = 0;
		mutable std::mutex m_Lock;
	};

	class MUtilityBitMaskIDBank
	{
	public:
		MUtilityBitMaskID GetID();
		bool ReturnID(MUtilityBitMaskID idToReturn);

		MUtilityBitMaskID PeekNextID() const;

		bool IsIDActive(MUtilityBitMaskID id) const;
		bool IsIDRecycled(MUtilityBitMaskID id) const;

		uint32_t GetCount() const;

	private:
		std::atomic<MUtilityBitMaskID>	m_NextID = 1;
		std::atomic<MUtilityBitMaskID>	m_RecycledIDs = 0;
		std::atomic<uint32_t>			m_Count = 0;
	};
}