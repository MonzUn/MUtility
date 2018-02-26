#include "interface/MUtilityThreading.h"
#include "interface/MUtilityLog.h"

#define LOG_CATEGORY_THREADING "MUtilityThreading"

void MUtilityThreading::JoinThread( std::thread& thread )
{
	if ( thread.joinable() )
		thread.join();
	else
		MLOG_WARNING( "Attempted to join unjoinable thread", LOG_CATEGORY_THREADING );
}

void MUtilityThreading::LockMutexes( std::initializer_list <std::mutex*> mutexes )
{
	for (auto mutex : mutexes)
	{
		mutex->lock();
	}
}

void MUtilityThreading::UnlockMutexes( std::initializer_list<std::mutex*> mutexes )
{
	for (auto mutex : mutexes)
	{
		mutex->unlock();
	}
}