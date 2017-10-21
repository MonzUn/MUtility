#include "interface/MUtilityFile.h"

#include "interface/MUtilityError.h"
#include "interface/MUtilityPlatformDefinitions.h"
#include "interface/MUtilityWindowsInclude.h"

#include <string>
#include <direct.h>

bool MUtilityFile::CreateFolder(const char* directoryPath)
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