#pragma once

namespace MUtilityFile
{
	bool CreateDir(const char* directoryPath); // Cannot be named "CreateDirectory" due to conflict with windows.h

	bool DirectoryExists(const char* folderPath);
	bool FileExists(const char* filePath);
}