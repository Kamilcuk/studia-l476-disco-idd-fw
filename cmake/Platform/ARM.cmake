
#message(STATUS ARM.cmake)

function(add_hex_target target)
#	if(NOT CMAKE_OBJCOPY)
#		message(FATAL_ERROR "CMAKE_OBJCOPY not defined")
#	endif()
	add_custom_command(OUTPUT "${target}.hex"
		COMMAND "${CMAKE_OBJCOPY}" -O binary "${target}${CMAKE_EXECUTABLE_SUFFIX}" "${target}.hex"
		DEPENDS "${target}")
    add_custom_target("${target}.hex" ALL
    	DEPENDS "${target}.hex" 
        COMMENT "Generating ${target}.hex")
endfunction()

function(firmware_size target)
#	if(NOT CMAKE_SIZE_UTIL)
#		message(FATAL_ERROR "CMAKE_SIZE_UTIL not defined")
#	endif()
    add_custom_command(TARGET "${target}" POST_BUILD
        COMMAND "${CMAKE_SIZE_UTIL}" -B "${CMAKE_CURRENT_BINARY_DIR}/${target}${CMAKE_EXECUTABLE_SUFFIX}"
        COMMENT "Priting ${target}.elf size information")
endfunction()
