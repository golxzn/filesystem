
set(GXZN_OS_FS_DEFINITIONS)

if(GXZN_OS_FS_DEFINITIONS_INIT)
	set(GXZN_OS_FS_DEFINITIONS ${GXZN_OS_FS_DEFINITIONS_INIT})
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} upper_build_type)
if(GXZN_OS_FS_DEFINITIONS_${upper_build_type})
	list(APPEND GXZN_OS_FS_DEFINITIONS ${GXZN_OS_FS_DEFINITIONS_${upper_build_type}})
endif()
unset(upper_build_type)

if(GXZN_OS_FS_SYSTEM)
	string(TOUPPER ${GXZN_OS_FS_SYSTEM} upper_system)
	list(APPEND GXZN_OS_FS_DEFINITIONS GXZN_OS_FS_SYSTEM_NAME="${GXZN_OS_FS_SYSTEM}")
	list(APPEND GXZN_OS_FS_DEFINITIONS GXZN_OS_FS_${upper_system}=1)
endif()

unset(__me_suffixes)
