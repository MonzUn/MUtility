#pragma once
#include <string>

namespace MUtilityFile
{
	bool CreateDir(const char* directoryPath); // Cannot be named "CreateDirectory" due to conflict with windows.h

	bool DirectoryExists(const char* folderPath);
	bool FileExists(const char* filePath);

	std::string GetDirectoryPathFromFilePath(const std::string& filePath);
	std::string	GetFileContentAsString(const std::string& filePath);
	uint64_t GetFileContentSize(const std::string& filePath);
}