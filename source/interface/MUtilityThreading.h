#pragma once
#include <thread>
#include <mutex>

namespace MUtilityThreading
{
	void JoinThread( std::thread& thread );
	void LockMutexes( std::initializer_list <std::mutex*> mutexes );
	void UnlockMutexes( std::initializer_list<std::mutex*> mutexes );
}