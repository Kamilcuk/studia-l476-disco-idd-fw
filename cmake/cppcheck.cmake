cmake_minimum_required(VERSION 3.10)

find_program(CMAKE_C_CPPCHECK NAMES cppcheck)
if (CMAKE_C_CPPCHECK)
	message(STATUS "Found cppcheck: ${CMAKE_C_CPPCHECK}")
    list(APPEND CMAKE_C_CPPCHECK
            "--inconclusive"
            "--inline-suppr"
            "--std=c11"
            "--template=\"{file}:{line}: {severity}: {id}: {message}\"")
    set(CMAKE_CXX_CPPCHECK "${CMAKE_C_CPPCHECK}")
endif()
