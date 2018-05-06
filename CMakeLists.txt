cmake_minimum_required (VERSION 3.6)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Toolchain/arm-none-eabi-gcc)
set(CMAKE_SYSTEM_NAME ARM)
set(CMAKE_SYSTEM_PROCESSOR Cortex-M0+)
project(disco ASM C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

string(APPEND CMAKE_C_FLAGS " -std=gnu11 -Wall -Wno-nonnull-compare")
#string(APPEND CMAKE_C_FLAGS " -Ofast")
#string(APPEND CMAKE_C_FLAGS " -flto -fno-fat-lto-objects")
file(GLOB LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32cubemx/*.ld)
string(APPEND CMAKE_EXE_LINKER_FLAGS " -T ${LINKER_SCRIPT}")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage")
link_libraries(-specs=nosys.specs -specs=nano.specs)
add_definitions(-DSTM32L1 -DSTM32L152xC)

string(APPEND CMAKE_C_FLAGS_DEBUG " -g3 -DDEBUG")

include(cubemx)
add_target_generate_mx_h(mx_h stm32cubemx ${CMAKE_SOURCE_DIR}/src/app/machine/mx.h)
cubemx_get_c_includes(stm32cubemx third_party/STM32Cube_FW_L1 mx_inc)
include_directories(AFTER ${mx_inc})
cubemx_get_c_sources(stm32cubemx third_party/STM32Cube_FW_L1 mx_src)
file(GLOB mx_asm_src stm32cubemx/*.s)
add_library(hal OBJECT ${mx_asm_src} ${mx_src})
set(hal_objs $<TARGET_OBJECTS:hal>)

include_directories(${CMAKE_SOURCE_DIR}/stm32cubemx/Inc)

include_directories(BEFORE src/app src/kamillibc src/kamillibcstm32)
file(GLOB_RECURSE src FOLLOW_SYMLINKS src/*.c)
add_executable(${PROJECT_NAME} ${hal_objs} ${src})
add_hex_target(${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} mx_h)

