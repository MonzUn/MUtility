#pragma once
#include "MUtilityByte.h"
#include "MUtilityLog.h"
#include "MUtilityTypes.h"
#include <atomic>
#include <mutex>
#include <stdint.h>
#include <type_traits>
#include <vector>

class MUtilityIDBank
{
public:
	MUtilityID GetID();
	bool ReturnID(MUtilityID idToreturn);
	bool IsIDActive(MUtilityID id);
	bool IsIDRecycled(MUtilityID id);

private:
	MUtilityID m_NextID = 0;
	std::vector<MUtilityID> m_RecycledIDs;
	std::mutex m_Lock;
};

class MUtilityBitwiseIDBank
{
public:
	MUtilityBitwiseID GetID();
	bool ReturnID(MUtilityBitwiseID idToReturn);
	bool IsIDActive(MUtilityBitwiseID id);
	bool IsIDRecycled(MUtilityBitwiseID id);

private:
	std::atomic<MUtilityBitwiseID> m_NextID = 1;
	std::atomic<MUtilityBitwiseID> m_RecycledIDs = 0;
};