#include "interface/MUtilityFile.h"

#include "interface/MUtilityError.h"
#include "interface/MUtilityPlatformDefinitions.h"
#include "interface/MUtilityWindowsInclude.h"

#include <string>
#include <direct.h>

bool MUtilityFile::CreateDir(const char* directoryPath)
{
	int result;
	
	#if PLATFORM == PLATFORM_WINDOWS
		result = _mkdir(directoryPath);
	#else
		result = mkdir(directoryPath.c_str(), 0777);
	#endif
	if (result != 0)
	{
		switch (errno)
		{
		case EEXIST:
			return true;

		case ENOENT:
			MUtilityError::Error = std::string(__func__) + ": Unable to create diretory on path \"" + std::string(directoryPath) + "\" since the path is invalid";
			return false;

		default:
			MUtilityError::Error = std::string(__func__) + ": Unable to create directory - Unknown error", "FileUtility";
			return false;
			break;
		}
	}
		
	return result == 0;
}

bool MUtilityFile::DirectoryExists(const char* folderPath)
{
	bool toReturn = false;
	struct stat info;
	if (stat(folderPath, &info) != 0)
	{
		MLOG_WARNING("Failed to access path \"" << folderPath << "\"", MUTILITY_LOG_CATEGORY_FILE);
		return false;
	}

	if (info.st_mode & S_IFDIR)
		toReturn = true;
	
	return toReturn;
}

bool MUtilityFile::FileExists(const char* filePath)
{
	bool toReturn = false;
	std::ifstream file(filePath);
	if (file.good())
	{
		toReturn = true;
		file.close();
	}

	return toReturn;
}
}