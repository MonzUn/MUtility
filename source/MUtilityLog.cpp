#include "Interface/MUtilityLog.h"
#include "MUtilityLogInternal.h"
#include "Interface/MUtilityFile.h"
#include "Interface/MUtilitySystem.h"
#include <atomic>
#include <fstream>
#include <mutex>
#include <sstream>

#define LOG_CATEGORY_LOG "MUtilityLog"

namespace MUtilityLog
{
	std::unordered_map<std::string, LogValuePair>* m_Logs;
	std::vector<std::string>* m_UnreadMessages;

	std::stringstream* m_InputStream;
	std::stringstream* m_MainLog;
	std::stringstream* m_FullInterestLog;

	MUtilityLogOutputTrigger m_OutputTrigger = MUtilityLogOutputTrigger::Shutdown;

	std::mutex m_LogLock;
	int32_t m_MaxUnreadMessages = 100;

	std::atomic<bool> m_Initialized = false;
}

using namespace MUtility;

// ---------- INTERFACE ----------

void MUtilityLog::Initialize()
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (m_Initialized)
	{
		MLOG_WARNING("Attempted to initialize MutilityLog multiple times", LOG_CATEGORY_LOG);
		return;
	}
#endif

	m_Logs = new std::unordered_map<std::string, LogValuePair>();
	m_UnreadMessages = new std::vector<std::string>();

	m_InputStream = new std::stringstream();
	m_MainLog = new std::stringstream();
	m_FullInterestLog = new std::stringstream();
	
	m_Initialized = true;
}

void MUtilityLog::Shutdown()
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return;
#endif
	if(m_OutputTrigger == MUtilityLogOutputTrigger::Shutdown)
		FlushToDisk();

	delete m_Logs;
	delete m_UnreadMessages;
	delete m_InputStream;
	delete m_MainLog;
	delete m_FullInterestLog;

	m_Initialized = false;
}

void MUtilityLog::SetInterest(const std::string& category, MUtilityLogLevel::LogLevel newInterestLevels)
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if(!m_Initialized)
		return;
#endif

	m_LogLock.lock();
	LogMapIterator iterator = m_Logs->find(category);
	if (iterator == m_Logs->end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	iterator->second.InterestLevels = newInterestLevels;
	m_LogLock.unlock();
}

void MUtilityLog::Log(const std::string& message, const std::string& category, MUtilityLogLevel::LogLevel logLevel, MUtilityLogMode logMode, const char* file, const char* line, const char* functionName)
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return;
#endif

	m_LogLock.lock();
	LogMapIterator iterator = m_Logs->find(category);
	if (iterator == m_Logs->end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	std::stringstream localStream;

	if (!m_FullInterestLog->str().empty())
		localStream << "\n\n";

	switch (logLevel)
	{
	case MUtilityLogLevel::LOG_ERROR:
		localStream << "Error\n";
		break;

	case MUtilityLogLevel::LOG_WARNING:
		localStream << "Warning\n";
		break;

	case MUtilityLogLevel::LOG_INFO:
		localStream << "Info\n";
		break;

	case MUtilityLogLevel::LOG_DEBUG:
		localStream << "Debug\n";
		break;

	default:
		MLOG_WARNING("Invalid loglevel supplied; call ignored", LOG_CATEGORY_LOG);
		m_LogLock.unlock();
		return;
	}

	switch (logMode)
	{
	case MUtilityLogMode::Normal:
		localStream << "Category: " << category << '\n' << message;
		break;
	case MUtilityLogMode::Debug:
		localStream << "Category: " << category << '\n' << "File: " << file << '\n' << "Line: " << line << '\n' << "Function: " << functionName << "\n - " << message;
		break;
	default:
		MLOG_WARNING("Invalid logMode supplied; call ignored", LOG_CATEGORY_LOG);
		m_LogLock.unlock();
		return;
	}

	const std::string fullMessage = localStream.str();

	if (logLevel & iterator->second.InterestLevels)
	{
		iterator->second.Log << fullMessage;
		*m_MainLog << fullMessage;

		if (m_UnreadMessages->size() < m_MaxUnreadMessages)
			m_UnreadMessages->push_back(fullMessage);
	}
	*m_FullInterestLog << fullMessage;
	m_LogLock.unlock();


	if(m_OutputTrigger == MUtilityLogOutputTrigger::Log)
		FlushToDisk();
}

void MUtilityLog::FlushToDisk()
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return;
#endif

	m_LogLock.lock();
	std::string logsDir = std::string(MUtility::GetExecutableDirectoryPath() + "/logs");
	std::string mainLogDir = logsDir + "/mainLog.txt";
	std::string fullInterestLogDir = logsDir + "/fullInterestLog.txt";
	std::string categoriesDir = logsDir + "/categories";
	if(!DirectoryExists(logsDir.c_str()))
		CreateDir(logsDir.c_str());

	std::ofstream outStream;
	outStream.open(mainLogDir, std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << m_MainLog->str();
		outStream.close();
	}

	outStream.open(fullInterestLogDir, std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << m_FullInterestLog->str();
		outStream.close();
	}

	
	if(!DirectoryExists(categoriesDir.c_str()))
		MUtility::CreateDir(categoriesDir.c_str());

	for (auto it = m_Logs->begin(); it != m_Logs->end(); ++it)
	{
		outStream.open(categoriesDir + "/" + it->first + ".txt", std::ofstream::out | std::ofstream::trunc);
		if (outStream.is_open())
		{
			outStream << it->second.Log.str();
			outStream.close();
		}
	}
	m_LogLock.unlock();
}

void MUtilityLog::SetMaxUnradMessageCount(int32_t maxUnreadMessages)
{
	m_LogLock.lock();
	m_MaxUnreadMessages = maxUnreadMessages;
	m_LogLock.unlock();
}

bool MUtilityLog::GetUnreadMessages(std::string& outConcatenatedMessages)
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return false;
#endif

	m_LogLock.lock();
	bool newMessagesExists = m_UnreadMessages->size() > 0;

	for (int i = 0; i < m_UnreadMessages->size(); ++i)
	{
		outConcatenatedMessages += (*m_UnreadMessages)[i];
	}
	m_LogLock.unlock();

	return newMessagesExists;
}

bool MUtilityLog::GetUnreadMessages(std::vector<std::string>& outMessageList)
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return false;
#endif

	m_LogLock.lock();
	bool newMessagesExists = m_UnreadMessages->size() > 0;

	outMessageList = *m_UnreadMessages;
	m_LogLock.unlock();

	return newMessagesExists;
}

void MUtilityLog::ClearUnreadMessages()
{
	m_LogLock.lock();
	m_UnreadMessages->clear();
	m_LogLock.unlock();
}

bool MUtilityLog::IsInitialized()
{
	return m_Initialized;
}

std::stringstream* MUtilityLog::GetInputStream()
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return nullptr;
#endif

	return m_InputStream;
}

std::string MUtilityLog::GetLog(const std::string& category)
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return "";
#endif

	m_LogLock.lock();
	std::string toReturn = "";

	LogMapIterator iterator = m_Logs->find(category);
	if (iterator != m_Logs->end())
	{
		toReturn = iterator->second.Log.str();
	}
	MLOG_WARNING("Attempted to get log for category \"" << category << "\" but no such category exists", LOG_CATEGORY_LOG);

	m_LogLock.unlock();
	return toReturn;
}

std::string MUtilityLog::GetAllInterestLog()
{
#if COMPILE_MODE == COMPILE_MODE_DEBUG
	if (!m_Initialized)
		return "";
#endif

	m_LogLock.lock();
	std::string toReturn = m_MainLog->str();
	m_LogLock.unlock();
	return toReturn;
}

std::string MUtilityLog::GetFullLog()
{
	m_LogLock.lock();
	std::string toReturn = m_FullInterestLog->str();
	m_LogLock.unlock();
	return toReturn;
}

MUtilityLogOutputTrigger MUtilityLog::GetOutputTrigger()
{
	return m_OutputTrigger;
}

void MUtilityLog::SetOutputTrigger(MUtilityLogOutputTrigger newTrigger)
{
	m_OutputTrigger = newTrigger;
}

// ---------- INTERNAL ----------

LogMapIterator MUtilityLog::RegisterCategory(const char* categoryName, MUtilityLogLevel::LogLevel initialInterestLevels)
{
	if (m_Logs->find(categoryName) != m_Logs->end())
	{
		MLOG_WARNING("Attempted to register logger category \"" << std::string(categoryName) + "\" multiple times; call ignored", LOG_CATEGORY_LOG);
		return m_Logs->end();
	}

	return m_Logs->emplace(categoryName, initialInterestLevels).first;
}