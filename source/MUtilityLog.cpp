#include "interface/MUtilityLog.h"
#include "MUtilityLogInternal.h"
#include "interface/MUtilityFile.h"
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

	std::mutex m_LogLock;
	int32_t m_MaxUnreadMessages = 100;
}

// ---------- INTERFACE ----------

void MUtilityLog::Initialize()
{
	m_Logs = new std::unordered_map<std::string, LogValuePair>();
	m_UnreadMessages = new std::vector<std::string>();

	m_InputStream = new std::stringstream();
	m_MainLog = new std::stringstream();
	m_FullInterestLog = new std::stringstream();
}

void MUtilityLog::Shutdown()
{
	FlushToDisk();

	delete m_Logs;
	delete m_UnreadMessages;
	delete m_InputStream;
	delete m_MainLog;
	delete m_FullInterestLog;
}

void MUtilityLog::SetInterest(const std::string& category, MUtilityLogLevel::LogLevel newInterestLevels)
{
	m_LogLock.lock();
	LogMapIterator iterator = m_Logs->find(category);
	if (iterator == m_Logs->end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	iterator->second.InterestLevels = newInterestLevels;
	m_LogLock.unlock();
}

void MUtilityLog::Log(const std::string& message, const std::string& category, MUtilityLogLevel::LogLevel logLevel, MUtilityLogMode logMode, const char* file, const char* line, const char* functionName)
{
	m_LogLock.lock();
	LogMapIterator iterator = m_Logs->find(category);
	if (iterator == m_Logs->end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	std::stringstream localStream;

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
		localStream << "Category: " << category << '\n' << message << "\n\n";
		break;
	case MUtilityLogMode::Debug:
		localStream << "Category: " << category << '\n' << "File: " << file << '\n' << "Line: " << line << '\n' << "Function: " << functionName << "\n - " << message << "\n\n";
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
}

void MUtilityLog::FlushToDisk()
{
	m_LogLock.lock();
	MUtilityFile::CreateDir("logs");

	std::ofstream outStream;
	outStream.open("logs/mainLog.txt", std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << m_MainLog->rdbuf();
		outStream.close();
	}

	outStream.open("logs/fullInterestLog.txt", std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << m_FullInterestLog->rdbuf();
		outStream.close();
	}

	MUtilityFile::CreateDir("logs/categories");
	for (auto it = m_Logs->begin(); it != m_Logs->end(); ++it)
	{
		outStream.open("logs/categories/" + it->first + ".txt", std::ofstream::out | std::ofstream::trunc);
		if (outStream.is_open())
		{
			outStream << it->second.Log.rdbuf();
			outStream.close();
		}
	}m_LogLock.unlock();
}

void MUtilityLog::SetMaxUnradMessageCount(int32_t maxUnreadMessages)
{
	m_LogLock.lock();
	m_MaxUnreadMessages = maxUnreadMessages;
	m_LogLock.unlock();
}

bool MUtilityLog::FetchUnreadMessages(std::string& outConcatenatedMessages)
{
	m_LogLock.lock();
	bool newMessagesExists = m_UnreadMessages->size() > 0;

	for (int i = 0; i < m_UnreadMessages->size(); ++i)
	{
		outConcatenatedMessages += (*m_UnreadMessages)[i];
	}
	m_UnreadMessages->clear();
	m_LogLock.unlock();

	return newMessagesExists;
}

bool MUtilityLog::FetchUnreadMessages(std::vector<std::string>& outMessageList)
{
	m_LogLock.lock();
	bool newMessagesExists = m_UnreadMessages->size() > 0;

	outMessageList = *m_UnreadMessages;
	m_UnreadMessages->clear();
	m_LogLock.unlock();

	return newMessagesExists;
}

std::stringstream& MUtilityLog::GetInputStream()
{
	return *m_InputStream;
}

std::string MUtilityLog::GetLog(const std::string& category)
{
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