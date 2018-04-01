cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Project name
project(MUtility CXX)

# OS Name
string(TOLOWER ${CMAKE_SYSTEM_NAME} OperatingSystemNameLowerCase)

# Root directory
set(ProjectRoot ${CMAKE_CURRENT_LIST_DIR}/../)
get_filename_component(ProjectRootAbsolute ${ProjectRoot} ABSOLUTE)

# Visual Studio settings
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /D _CRT_SECURE_NO_WARNINGS")
endif(MSVC)

# List source file directories
file(GLOB_RECURSE SOURCE_DIRECTORIES
	"../source/*.h"
	"../source/*.cpp"
)

# Create static library
ADD_LIBRARY( ${PROJECT_NAME} STATIC
   	${SOURCE_DIRECTORIES}
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${ProjectRootAbsolute}/output/")

# Make sure that the linker language gets set to C++
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# --- DEBUG COPY PASTE ---
#message("|||--- TEST ---||| ${var}")