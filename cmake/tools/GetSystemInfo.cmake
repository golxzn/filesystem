
macro(_get_arch arch sys)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		if (${sys} MATCHES "Windows")
			set(${arch} "x64" PARENT_SCOPE)
		else()
			set(${arch} "x86_64" PARENT_SCOPE)
		endif()
	else()
		set(${arch} "x86" PARENT_SCOPE)
	endif()
endmacro()

function(get_system_info SYSTEM_NAME ARCHITECTURE)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		set(${SYSTEM_NAME} "Windows" PARENT_SCOPE)
		_get_arch(${ARCHITECTURE} ${SYSTEM_NAME})

	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		set(${SYSTEM_NAME} "Linux" PARENT_SCOPE)
		_get_arch(${ARCHITECTURE} ${SYSTEM_NAME})

	elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		set(${SYSTEM_NAME} "macOS" PARENT_SCOPE)

		execute_process(
			COMMAND "sysctl" "-n" "machdep.cpu.brand_string"
			OUTPUT_VARIABLE CPU_BRAND_STRING
		)
		if (CPU_BRAND_STRING MATCHES "Intel")
			_get_arch(${ARCHITECTURE} ${SYSTEM_NAME})
		elseif()
			set(${ARCHITECTURE} "ARMv8.5-A" PARENT_SCOPE)
		endif()

	else()
		set(${SYSTEM_NAME} "Unknown" PARENT_SCOPE)
		set(${ARCHITECTURE} "Unknown" PARENT_SCOPE)
		message(WARNING "Could not determine system or architecture!")
	endif()
endfunction()
