cmake_minimum_required(VERSION 3.23)

if (GRM_GENERATE_INFO_HEADER)
	include(TargetInfoHeader)
	make_target_info_header(
		TARGET golxzn_os_filesystem
		OUTPUT_PATH ${GRM_CODE_DIR}/headers/golxzn
		FILENAME filesystem_about
		NAMESPACE golxzn::about
		PARAMETERS
			APP_NAME "golxzn::os::filesystem"
			APP_VERSION "${PROJECT_VERSION}"
			APP_DESCRIPTION "${PROJECT_DESCRIPTION}"
	)
endif()

file(GLOB_RECURSE headers RELATIVE ${GRM_CODE_DIR} CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE sources RELATIVE ${GRM_CODE_DIR} CONFIGURE_DEPENDS "*.cpp")

source_group(TREE ${GRM_CODE_DIR}/headers FILES ${headers})
source_group(TREE ${GRM_CODE_DIR}/sources FILES ${sources})

add_library(golxzn_os_filesystem ${headers} ${sources})
add_library(golxzn::os::filesystem ALIAS golxzn_os_filesystem)

target_include_directories(golxzn_os_filesystem PUBLIC ${GRM_CODE_DIR}/headers)
target_compile_definitions(golxzn_os_filesystem PUBLIC ${GRM_DEFINITIONS})
set_target_properties(golxzn_os_filesystem PROPERTIES
	FOLDER "golxzn/os"
)
