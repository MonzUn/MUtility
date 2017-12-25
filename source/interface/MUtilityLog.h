#pragma once
#include "MUtilityMacros.h"
#include <cstdint>
#include <string>
#include <sstream>

#ifndef MUTILITY_DISABLE_LOGGING

	#ifdef _DEBUG

		#define MLOG_ERROR(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_ERROR, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::InputStream.str(std::string())
		#define MLOG_WARNING(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_WARNING, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::InputStream.str(std::string())
		#define MLOG_INFO(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_INFO, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::InputStream.str(std::string())
		#define MLOG_DEBUG(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_DEBUG, MUtilityLogMode::Debug, __FILE__, MUTILITY_STRINGIFY(__LINE__), __func__), MUtilityLog::InputStreamstr.str(std::string())

	#else

		#define MLOG_ERROR(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_ERROR), MUtilityLog::InputStream.str(std::string())
		#define MLOG_WARNING(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_WARNING), MUtilityLog::InputStream.str(std::string())
		#define MLOG_INFO(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_INFO), MUtilityLog::InputStream.str(std::string())
		#define MLOG_DEBUG(message, category) MUtilityLog::InputStream << message, MUtilityLog::Log(MUtilityLog::InputStream.str(), category, MUtilityLogLevel::LOG_DEBUG), MUtilityLog::InputStream.str(std::string())

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
#ifndef MUTILITY_DISABLE_LOGGING
	static std::stringstream InputStream; // Do not use externally! Only used for MUtility logging macros
#endif
	void Initialize();
	void Shutdown();

	void SetInterest(const std::string& category, MUtilityLogLevel::LogLevel newInterestLevels);
	void Log(const std::string& message, const std::string& category, MUtilityLogLevel::LogLevel logLevel, MUtilityLogMode logMode = MUtilityLogMode::Normal, const char* file = nullptr, const char* line = nullptr, const char* functionName = nullptr);
	void FlushToDisk();
}