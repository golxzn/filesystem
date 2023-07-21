cmake_minimum_required(VERSION 3.23)

set(local_root ${CMAKE_CURRENT_SOURCE_DIR})

if (GRM_GENERATE_INFO_HEADER)
	include(TargetInfoHeader)
	make_target_info_header(
		TARGET golxzn_resman
		OUTPUT_PATH ${local_root}/headers/golxzn
		FILENAME resman_about
		NAMESPACE golxzn::about
		PARAMETERS
			APP_NAME "golxzn::resman"
			APP_VERSION "${PROJECT_VERSION}"
			APP_DESCRIPTION "${PROJECT_DESCRIPTION}"
	)
endif()

file(GLOB_RECURSE headers RELATIVE ${local_root} CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE sources RELATIVE ${local_root} CONFIGURE_DEPENDS "*.cpp")

source_group(TREE ${local_root}/headers FILES ${headers})
source_group(TREE ${local_root}/sources FILES ${sources})

add_library(golxzn_resman ${headers} ${sources})
add_library(golxzn::resman ALIAS golxzn_resman)

target_include_directories(golxzn_resman PUBLIC ${local_root}/headers)
target_compile_definitions(golxzn_resman PUBLIC ${GRM_DEFINITIONS})
set_target_properties(golxzn_resman PROPERTIES
	FOLDER "golxzn"
)
