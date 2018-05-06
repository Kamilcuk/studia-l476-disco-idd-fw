
#message(STATUS ARM-GNU-C.cmake)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

if (NOT PROCESSOR_FLAGS)
	MESSAGE(FATAL_ERROR "PROCESSOR_FLASG is not set")
endif()

set(COMPILER_FLAGS "${PROCESSOR_FLAGS} -ffunction-sections -fdata-sections -fomit-frame-pointer")
string(APPEND CMAKE_ASM_FLAGS_INIT           " ${COMPILER_FLAGS} -x assembler-with-cpp ")
string(APPEND CMAKE_C_FLAGS_INIT             " ${COMPILER_FLAGS} ")
string(APPEND CMAKE_CXX_FLAGS_INIT           " ${COMPILER_FLAGS} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT    " -Wl,--gc-sections ")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " -Wl,--gc-sections ")
string(APPEND CMAKE_MODULE_LINKER_FLAGS_INIT " -Wl,--gc-sections ")

#link_libraries(-specs=nano.specs -lc -lnosys)
#set(CMAKE_C_COMPILER_FORCED true)
