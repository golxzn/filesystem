
function(find_visual_studio_root path)
	set(VS_INSTALLER_PATH "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/Installer")

	include(CMakeParseArguments)
	cmake_parse_arguments(_fvsr "" "VS_INSTALLER_PATH" "" ${ARGN})

	if (NOT _fvsr_VS_INSTALLER_PATH)
		message(STATUS "No VS_INSTALLER_PATH supplied. Provide default value: (\"${VS_INSTALLER_PATH}\")")
		set(_fvsr_VS_INSTALLER_PATH ${VS_INSTALLER_PATH})
	endif()

	execute_process(
		COMMAND "${_fvsr_VS_INSTALLER_PATH}/vswhere.exe" -latest -property installationPath
		OUTPUT_VARIABLE _vs_install_loc_out
		RESULT_VARIABLE _vs_where_exitcode
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	if (NOT _vs_where_exitcode EQUAL 0)
		message(FATAL_ERROR "vswhere failed with code: ${_vs_where_exitcode}")
	endif()

	file(TO_CMAKE_PATH "${_vs_install_loc_out}" _vs_install_loc)
	set(${path} ${_vs_install_loc} PARENT_SCOPE)
endfunction()

function(get_msvc_version out_version)
	include(CMakeParseArguments)
	cmake_parse_arguments(_gmv "" "VS_ROOT" "" ${ARGN})

	if(NOT _gmv_VS_ROOT)
		message(CHECK_START "VS_ROOT not provided. Attempting to find Visual Studio root")
		find_vsroot(_gmv_VS_ROOT)
		if(NOT _gmv_VS_ROOT)
			message(CHECK_FAIL "VS_ROOT not found")
			return()
		else()
			message(CHECK_PASS "VS_ROOT found at ${_gmv_VS_ROOT}")
		endif()
	endif()

	set(versions_path ${_gmv_VS_ROOT}/VC/Tools/MSVC)
	file(GLOB msvc_versions LIST_DIRECTORIES true RELATIVE ${versions_path} "${versions_path}/*")
	set(latest_version 0)
	foreach(version IN LISTS msvc_versions)
		if (latest_version VERSION_LESS ${version})
			set(latest_version ${version})
		endif()
	endforeach()

	set(${out_version} ${latest_version} PARENT_SCOPE)
endfunction()