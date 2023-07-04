
macro(setup_windows_environment arch)

	set(CMAKE_HOST_SYSTEM_PROCESSOR ${arch} CACHE STRING "Host system processor")
	set(CMAKE_SYSTEM_PROCESSOR ${arch} CACHE STRING "System processor")
	set(CMAKE_SYSTEM_NAME Windows CACHE STRING "System name")
	mark_as_advanced(CMAKE_HOST_SYSTEM_PROCESSOR CMAKE_SYSTEM_PROCESSOR CMAKE_SYSTEM_NAME)

	if(NOT CMAKE_SYSTEM_VERSION)
		include(FindWindowsSDK)
		find_windows_sdk(latest_version kits_directory)
		set(CMAKE_SYSTEM_VERSION ${latest_version} CACHE STRING "Windows SDK version" FORCE)
		set(CMAKE_KITS_DIRECTORY ${kits_directory} CACHE STRING "Windows SDK path" FORCE)
		message(STATUS "Using Windows SDK version ${CMAKE_SYSTEM_VERSION} in \"${CMAKE_KITS_DIRECTORY}\"")
	endif()

	#======================================= Looking for Visual Studio =======================================#
	if(NOT CMAKE_VS_ROOT)
		include(FindVisualStudioRoot)
		find_visual_studio_root(_vsroot)
		set(CMAKE_VS_ROOT ${_vsroot} CACHE STRING "Visual Studio Root" FORCE)
		message(STATUS "Using Visual Studio Root: \"${CMAKE_VS_ROOT}\"")
	endif()

	if(NOT CMAKE_MSVC_VERSION)
		get_msvc_version(ver VS_ROOT ${CMAKE_VS_ROOT})
		set(CMAKE_MSVC_VERSION ${ver} CACHE STRING "MSVC Version" FORCE)
		message(STATUS "MSVC Version: ${CMAKE_MSVC_VERSION}")
	endif()

	set(sdk_include ${CMAKE_KITS_DIRECTORY}/Include/${CMAKE_SYSTEM_VERSION}/)
	include_directories(
		${sdk_include}/cppwinrt
		${sdk_include}/shared
		${sdk_include}/ucrt
		${sdk_include}/um
		${sdk_include}/winrt
		${CMAKE_VS_ROOT}/VC/Tools/MSVC/${CMAKE_MSVC_VERSION}/include
	)
	link_directories(
		${CMAKE_KITS_DIRECTORY}/bin/${CMAKE_SYSTEM_VERSION}/${arch}/
		${CMAKE_KITS_DIRECTORY}/Lib/${CMAKE_SYSTEM_VERSION}/um/${arch}
		${CMAKE_KITS_DIRECTORY}/Lib/${CMAKE_SYSTEM_VERSION}/ucrt/${arch}
		${CMAKE_VS_ROOT}/VC/Tools/MSVC/${CMAKE_MSVC_VERSION}/lib/${arch}
	)

	list(APPEND CMAKE_PROGRAM_PATH
		${CMAKE_KITS_DIRECTORY}/bin/${CMAKE_SYSTEM_VERSION}/${arch}/
		${CMAKE_KITS_DIRECTORY}/Lib/${CMAKE_SYSTEM_VERSION}/um/${arch}
		${CMAKE_KITS_DIRECTORY}/Lib/${CMAKE_SYSTEM_VERSION}/ucrt/${arch}
		${CMAKE_VS_ROOT}/VC/Tools/MSVC/${CMAKE_MSVC_VERSION}/lib/${arch}
	)

endmacro()
