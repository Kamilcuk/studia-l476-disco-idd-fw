cmake_minimum_required(VERSION 3.6)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third_party/kamillibc/cmake")

include(Toolchain/arm-none-eabi-gcc)
set(CMAKE_SYSTEM_NAME ARM)
set(CMAKE_SYSTEM_PROCESSOR Cortex-M3)
project(iddfw ASM C)

set(CMAKE_VERBOSE_MAKEFILE ON)

string(APPEND CMAKE_C_FLAGS " -std=gnu11 -Wall -Wno-nonnull-compare")
string(APPEND CMAKE_C_FLAGS " -g3 -DDEBUG")
string(APPEND CMAKE_C_FLAGS " -Ofast")
string(APPEND CMAKE_C_FLAGS " -flto -fno-fat-lto-objects")
#string(APPEND CMAKE_C_FLAGS_DEBUG " -O0 -g3 -DDEBUG")
#string(APPEND CMAKE_C_FLAGS_DEBUG " -O1 -g3 -DDEBUG")
#string(APPEND CMAKE_C_FLAGS_RELEASE " -Ofast -flto -fno-fat-lto-objects")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage")

add_definitions(-DSTM32L1 -DSTM32L152xC)

include(stm32cubemx)
set(cubemx_dir "stm32cubemx/iddfw")
set(halrepo_dir "third_party/STM32Cube_FW_L1")

cubemx_get_linker_script(LINKER_SCRIPT "${cubemx_dir}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -T ${LINKER_SCRIPT}")

cubemx_add_gen_cubemx_header_target(mx_h ${CMAKE_BINARY_DIR}/src/app/machine/_mx.h "${cubemx_dir}")
include_directories(AFTER ${CMAKE_BINARY_DIR}/src/app)

cubemx_add_hal_library(hal OBJECT mx_inc "${cubemx_dir}" "${halrepo_dir}")
include_directories(AFTER ${mx_inc})

include_directories(BEFORE src/kamillibc src/kamillibcstm32 src/app)
file(GLOB_RECURSE src FOLLOW_SYMLINKS src/*.cpp src/*.c src/*.s)

add_executable(${PROJECT_NAME} ${src} $<TARGET_OBJECTS:hal>)
set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
add_dependencies(${PROJECT_NAME} mx_h)
target_link_libraries(${PROJECT_NAME} -specs=nosys.specs -specs=nano.specs)

include(cross-compiling-utils)
add_hex_target(${PROJECT_NAME})

