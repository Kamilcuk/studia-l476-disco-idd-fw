
function(cubemx_get_c_includes mx_dir hal_repo_dir out)
	execute_process(
		COMMAND /bin/bash -c "make -s -f Makefile -f <(echo -e 'printvar-%: ; $(info $($*))') printvar-C_INCLUDES"
		WORKING_DIRECTORY "${mx_dir}"
		OUTPUT_VARIABLE mx_inc
	)
	string(REPLACE "-I" "" mx_inc "${mx_inc}")
	string(REPLACE " " ";" mx_inc "${mx_inc}")
	list(REVERSE mx_inc)
	set(tmp "")
	foreach(i ${mx_inc})
		if (${i} MATCHES ".*/STM32Cube_FW_*")
			string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+" "${hal_repo_dir}/" i "${i}")
			string(REGEX REPLACE "\n$" "" i "${i}")
			set(i "${CMAKE_CURRENT_SOURCE_DIR}/${i}")
			if (NOT EXISTS ${i}) 
				MESSAGE(FATAL_ERROR ${i} does not exists)
  			endif()
  			list(APPEND tmp "${i}")
		endif()
	endforeach()
	set(mx_inc "${tmp}")
	set(${out} "${mx_inc}" PARENT_SCOPE)
endfunction()

function(cubemx_get_c_sources mx_dir hal_repo_dir out)
	execute_process(
		COMMAND /bin/bash -c "make -s -f Makefile -f <(echo -e 'printvar-%: ; $(info $($*))') printvar-C_SOURCES"
		WORKING_DIRECTORY ${mx_dir}
		OUTPUT_VARIABLE mx_src
	)
	string(REPLACE " " ";" mx_src "${mx_src}")
	set(tmp "")
	foreach(i ${mx_src})
		if (${i} MATCHES ".*/STM32Cube_FW_*")
			string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+/" "${hal_repo_dir}/" i "${i}")
		else()
			string(REGEX REPLACE "^Src" "${mx_dir}/Src" i "${i}")
		endif()
		string(REGEX REPLACE "\n$" "" i "${i}")
		set(i "${CMAKE_CURRENT_SOURCE_DIR}/${i}")
		if (NOT EXISTS ${i}) 
			MESSAGE(FATAL_ERROR "${i} does not exists")
  		endif()
		list(APPEND tmp ${i})
	endforeach()
	set(mx_src "${tmp}")
	set(${out} "${mx_src}" PARENT_SCOPE)
endfunction()

function(add_target_generate_mx_h target mx_dir out)
	file(GLOB mx_headers "${mx_dir}/Inc/*.h")
	get_filename_component(out_dir "${out}" DIRECTORY)
	file(GLOB mx_headers_relative RELATIVE "${out_dir}" "${mx_dir}/Inc/*.h")
	add_custom_command(
		OUTPUT ${out}
		COMMAND echo "/* this file is autogonerated by ${CMAKE_CURRENT_LIST_FILE} cmake script */" > "${out}" 
		COMMAND printf "#include \"%s\"\\n" ${mx_headers_relative} >> "${out}"
		DEPENDS ${mx_headers} 
		COMMENT "Generate ${out} from cubemx header" VERBATIM 
	)
	add_custom_target(${target} DEPENDS ${out})
endfunction()

