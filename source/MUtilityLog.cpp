#include "interface/MUtilityLog.h"
#include "interface/MUtilityFile.h"
#include <fstream>
#include <mutex>
#include <sstream>
#include <unordered_map>

#define MUTILITY_LOG_CATEGORY_LOG "MUtilityLog"

static std::mutex LOG_LOCK;
static int32_t MAX_UNREAD_MESSAGES = 100;

struct LogValuePair
{
	LogValuePair(MUtilityLogLevel::LogLevel initialInterestLevels) : InterestLevels(initialInterestLevels) {}

	MUtilityLogLevel::LogLevel	InterestLevels;
	std::stringstream			Log;
};

std::unordered_map<std::string, LogValuePair> Logs;
std::stringstream MainLog;
std::stringstream FullInterestLog;

std::vector<std::string> UnreadMessages;

typedef std::unordered_map<std::string, LogValuePair>::iterator LogMapIterator;

LogMapIterator RegisterCategory(const char* categoryName, MUtilityLogLevel::LogLevel initialInterestLevels);

void MUtilityLog::Shutdown()
{
	FlushToDisk();
}

void MUtilityLog::SetInterest(const std::string& category, MUtilityLogLevel::LogLevel newInterestLevels)
{
	LOG_LOCK.lock();
	LogMapIterator iterator = Logs.find(category);
	if (iterator == Logs.end())
		iterator = RegisterCategory(category.c_str(), MUtilityLogLevel::ALL); // If the category is not yet registered; register it.

	iterator->second.InterestLevels = newInterestLevels;
	LOG_LOCK.unlock();
}

void MUtilityLog::Log(const std::string& message, const std::string& category, MUtilityLogLevel::LogLevel logLevel, MUtilityLogMode logMode, const char* file, const char* line, const char* functionName)
{
	LOG_LOCK.lock();
	LogMapIterator iterator = Logs.find(category);
	if (iterator == Logs.end())
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
		MLOG_WARNING("Invalid loglevel supplied; call ignored", MUTILITY_LOG_CATEGORY_LOG);
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
		MLOG_WARNING("Invalid logMode supplied; call ignored", MUTILITY_LOG_CATEGORY_LOG);
		return;
	}

	const std::string fullMessage = localStream.str();

	if (logLevel & iterator->second.InterestLevels)
	{
		iterator->second.Log << fullMessage;
		MainLog << fullMessage;

		if (UnreadMessages.size() < MAX_UNREAD_MESSAGES)
			UnreadMessages.push_back(fullMessage);
	}
	FullInterestLog << fullMessage;
	LOG_LOCK.unlock();
}

void MUtilityLog::FlushToDisk()
{
	LOG_LOCK.lock();
	MUtilityFile::CreateFolder("logs");

	std::ofstream outStream;
	outStream.open("logs/mainLog.txt", std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << MainLog.rdbuf();
		outStream.close();
	}

	outStream.open("logs/fullInterestLog.txt", std::ofstream::out | std::ofstream::trunc);
	if (outStream.is_open())
	{
		outStream << FullInterestLog.rdbuf();
		outStream.close();
	}

	MUtilityFile::CreateFolder("logs/categories");
	for (auto it = Logs.begin(); it != Logs.end(); ++it)
	{
		outStream.open("logs/categories/" + it->first + ".txt", std::ofstream::out | std::ofstream::trunc);
		if (outStream.is_open())
		{
			outStream << it->second.Log.rdbuf();
			outStream.close();
		}
	}
	LOG_LOCK.unlock();
}

void MUtilityLog::SetMaxUnradMessageCount(int32_t maxUnreadMessages)
{
	LOG_LOCK.lock();
	MAX_UNREAD_MESSAGES = maxUnreadMessages;
	LOG_LOCK.unlock();
}

bool MUtilityLog::FetchUnreadMessages(std::string& outConcatenatedMessages)
{
	LOG_LOCK.lock();
	bool newMessagesExists = UnreadMessages.size() > 0;

	for (int i = 0; i < UnreadMessages.size(); ++i)
	{
		outConcatenatedMessages += UnreadMessages[i];
	}
	UnreadMessages.clear();
	LOG_LOCK.unlock();

	return newMessagesExists;
}

bool MUtilityLog::FetchUnreadMessages(std::vector<std::string>& outMessageList)
{
	LOG_LOCK.lock();
	bool newMessagesExists = UnreadMessages.size() > 0;

	outMessageList = UnreadMessages;
	UnreadMessages.clear();
	LOG_LOCK.unlock();

	return newMessagesExists;
}

std::string MUtilityLog::GetLog(const std::string& category)
{
	LOG_LOCK.lock();
	std::string toReturn = "";

	LogMapIterator iterator = Logs.find(category);
	if (iterator != Logs.end())
	{
		toReturn = iterator->second.Log.str();
	}
	MLOG_WARNING("Attempted to get log for category \"" << category << "\" but no such category exists", MUTILITY_LOG_CATEGORY_LOG);

	LOG_LOCK.unlock();
	return toReturn;
}

std::string MUtilityLog::GetAllInterestLog()
{
	LOG_LOCK.lock();
	std::string toReturn = MainLog.str();
	LOG_LOCK.unlock();
	return toReturn;
}

std::string MUtilityLog::GetFullLog()
{
	LOG_LOCK.lock();
	std::string toReturn = FullInterestLog.str();
	LOG_LOCK.unlock();
	return toReturn;
}

LogMapIterator RegisterCategory(const char* categoryName, MUtilityLogLevel::LogLevel initialInterestLevels)
{
	if (Logs.find(categoryName) != Logs.end())
	{
		MLOG_WARNING("Attempted to register logger category \"" << std::string(categoryName) + "\" multiple times; call ignored", MUTILITY_LOG_CATEGORY_LOG);
		return Logs.end();
	}

	return Logs.emplace(categoryName, initialInterestLevels).first;
}