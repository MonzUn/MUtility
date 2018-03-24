#pragma once
#include "MUtilityMacros.h"
#include "MUtilityPlatformDefinitions.h"
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

#ifndef MUTILITY_DISABLE_LOGGING

	#if COMPILE_MODE == COMPILE_MODE_DEBUG

		#define MLOG_ERROR(message, category) MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_ERROR, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION
		#define MLOG_WARNING(message, category) MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_WARNING, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION
		#define MLOG_INFO(message, category) MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_INFO, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION
		#define MLOG_DEBUG(message, category) MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_DEBUG, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION

	#else

		#define MLOG_ERROR(message, category)  MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_ERROR), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION
		#define MLOG_WARNING(message, category)  MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_WARNING), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION
		#define MLOG_INFO(message, category)  MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_INFO), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION
		#define MLOG_DEBUG(message, category) MUtilityLog::IsInitialized() ? *MUtilityLog::GetInputStream() << message, MUtilityLog::Log(MUtilityLog::GetInputStream()->str(), category, MUtilityLogLevel::LOG_DEBUG), MUtilityLog::GetInputStream()->str(std::string()) : MUTILITY_EMPTY_EXPRESSION

	#endif

#else

	#define MLOG_ERROR(message, category)
	#define MLOG_WARNING(message, category)
	#define MLOG_INFO(message, category)
	#define MLOG_DEBUG(message, category)

#endif

namespace MUtilityLogLevel
{
	enum LogLevel : uint8_t
	{
		LOG_ERROR	= 0x01,
		LOG_WARNING = 0x02,
		LOG_INFO	= 0x04,
		LOG_DEBUG	= 0x08,

		// Not a valid log types. Only allowed when registering interest. 
		NONE		= 0x00,
		ALL			= 0xFF, 
	};
}

enum class MUtilityLogMode
{
	Normal,
	Debug
};

namespace MUtilityLog
{
	void Initialize();
	void Shutdown();

	void SetInterest(const std::string& category, MUtilityLogLevel::LogLevel newInterestLevels);
	void Log(const std::string& message, const std::string& category, MUtilityLogLevel::LogLevel logLevel, MUtilityLogMode logMode = MUtilityLogMode::Normal, const char* file = nullptr, const char* line = nullptr, const char* functionName = nullptr);
	void FlushToDisk();

	void SetMaxUnradMessageCount(int32_t maxUnreadMessages);

	bool GetUnreadMessages(std::string& outConcatenatedMessages);
	bool GetUnreadMessages(std::vector<std::string>& outMessageList);
	void ClearUnreadMessages();

	bool IsInitialized();

	std::stringstream* GetInputStream(); // Intended for MLOG macros
	std::string GetLog(const std::string& category);
	std::string GetAllInterestLog();
	std::string GetFullLog();
};