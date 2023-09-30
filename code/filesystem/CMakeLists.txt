cmake_minimum_required(VERSION 3.23)

if (GXZN_OS_FS_GENERATE_INFO_HEADER)
	include(TargetInfoHeader)
	make_target_info_header(
		TARGET golxzn_os_filesystem
		OUTPUT_PATH ${GXZN_OS_FS_CODE_DIR}/headers/golxzn
		FILENAME filesystem_about
		NAMESPACE golxzn::about
		PARAMETERS
			APP_NAME "golxzn::os::filesystem"
			APP_VERSION "${PROJECT_VERSION}"
			APP_DESCRIPTION "${PROJECT_DESCRIPTION}"
	)
endif()

file(GLOB_RECURSE headers RELATIVE ${GXZN_OS_FS_CODE_DIR} CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE sources RELATIVE ${GXZN_OS_FS_CODE_DIR} CONFIGURE_DEPENDS "*.cpp")

source_group(TREE ${GXZN_OS_FS_CODE_DIR}/headers FILES ${headers})
source_group(TREE ${GXZN_OS_FS_CODE_DIR}/sources FILES ${sources})

add_library(golxzn_os_filesystem ${headers} ${sources})
add_library(golxzn::os::filesystem ALIAS golxzn_os_filesystem)

target_include_directories(golxzn_os_filesystem PUBLIC ${GXZN_OS_FS_CODE_DIR}/headers)
target_compile_definitions(golxzn_os_filesystem PUBLIC ${GXZN_OS_FS_DEFINITIONS})
set_target_properties(golxzn_os_filesystem PROPERTIES
	FOLDER "golxzn/os"
)

if(TARGET golxzn::os::aliases)
	target_link_libraries(golxzn_os_filesystem PUBLIC golxzn::os::aliases)
endif()
