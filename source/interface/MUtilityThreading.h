#pragma once
#include <thread>
#include <mutex>

namespace MutilityThreading
{
	void JoinThread( std::thread& thread )
	{
		if ( thread.joinable() )
			thread.join();
		//else
			// Logger::Log( "Attempted to join unjoinable thread", "ThreadingUtility", LogSeverity::WARNING_MSG ); TODODB: Log
	}

	void LockMutexes( std::initializer_list <std::mutex*> mutexes )
	{
		for ( auto mutex : mutexes )
		{
			mutex->lock();
		}
	}

	void UnlockMutexes( std::initializer_list<std::mutex*> mutexes )
	{
		for ( auto mutex : mutexes )
		{
			mutex->unlock();
		}
	}
}