#pragma once
#include <thread>
#include <mutex>

namespace MUtilityThreading // TODODB: Add to MUtility namespace instead
{
	void JoinThread(std::thread& thread);
	void LockMutexes(std::initializer_list <std::mutex*> mutexes);
	void UnlockMutexes(std::initializer_list<std::mutex*> mutexes);
}