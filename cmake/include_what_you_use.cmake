cmake_minimum_required(VERSION 3.10)

find_program(CMAKE_C_INCLUDE_WHAT_YOU_USE NAMES include-what-you-use iwyu)
if(CMAKE_C_INCLUDE_WHAT_YOU_USE)
    message(STATUS "Found include-what-you-use: ${IWYU}")
    string(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_LC)
    list(APPEND CMAKE_C_INCLUDE_WHAT_YOU_USE
		"-Xiwyu"
		"--mapping_file=${PROJECT_SOURCE_DIR}/misc/${CMAKE_SYSTEM_NAME_LC}.imp")
	set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE CMAKE_C_INCLUDE_WHAT_YOU_USE)
endif()
