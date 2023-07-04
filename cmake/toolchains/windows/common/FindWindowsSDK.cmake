
function(find_windows_sdk result_version kits_path)
	include(CMakeParseArguments)
	cmake_parse_arguments(_fwsdk "" "WINDOWS_VERSION;KITS_PATH" "" ${ARGN})

	if (NOT _fwsdk_KITS_PATH)
		message(STATUS "KITS_PATH not provided, using default path (\"C:/Program Files (x86)/Windows Kits\")")
		set(_fwsdk_KITS_PATH "C:/Program Files (x86)/Windows Kits")
	endif()
	if (NOT _fwsdk_WINDOWS_VERSION)
		message(STATUS "WINDOWS_VERSION not provided, using default version (10)")
		set(_fwsdk_WINDOWS_VERSION "10")
	endif()
	string(APPEND _fwsdk_KITS_PATH "/${_fwsdk_WINDOWS_VERSION}")
	set(${kits_path} ${_fwsdk_KITS_PATH} PARENT_SCOPE)

	file(GLOB raw_kit_version_list LIST_DIRECTORIES true "${_fwsdk_KITS_PATH}/Include/*")
	set(kit_version_list)
	foreach(kit_version IN LISTS raw_kit_version_list)
		if(IS_DIRECTORY ${kit_version})
			list(APPEND kit_version_list ${kit_version})
		endif()
	endforeach()

	list(LENGTH kit_version_list kit_version_list_length)
	if (kit_version_list_length EQUAL 0)
		message(FATAL_ERROR
			" +-------------------------------------------------------------------------+\n"
			" | Cannot find Windows 10 SDK. Please, install Windows 10 SDK by this link |\n"
			" | https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/ |\n"
			" +-------------------------------------------------------------------------+\n"
		)
	endif()

	set(latest 0)
	message(VERBOSE "Found Windows SDK versions: ${kit_version}")
	foreach(kit_version IN LISTS kit_version_list)
		string(REPLACE "${_fwsdk_KITS_PATH}/Include/" "" kit_version "${kit_version}")
		message(VERBOSE "    - ${kit_version}")
		if (kit_version VERSION_GREATER latest)
			set(latest ${kit_version})
		endif()
	endforeach()
	message(VERBOSE "Found latest: ${latest}")

	set(${result_version} ${latest} PARENT_SCOPE)
endfunction()