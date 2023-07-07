
set(GRM_DEFINITIONS)

if(GRM_DEFINITIONS_INIT)
	set(GRM_DEFINITIONS ${GRM_DEFINITIONS_INIT})
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} upper_build_type)
if(GRM_DEFINITIONS_${upper_build_type})
	list(APPEND GRM_DEFINITIONS ${GRM_DEFINITIONS_${upper_build_type}})
endif()
unset(upper_build_type)

if(GRM_SYSTEM)
	string(TOUPPER ${GRM_SYSTEM} upper_system)
	list(APPEND GRM_DEFINITIONS GRM_SYSTEM_NAME="${GRM_SYSTEM}")
	list(APPEND GRM_DEFINITIONS GRM_${upper_system}=1)
endif()

unset(__me_suffixes)
