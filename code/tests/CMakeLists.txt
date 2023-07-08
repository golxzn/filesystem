
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(tests ${sources} ${headers})
target_link_libraries(tests PRIVATE
	Catch2::Catch2WithMain
	golxzn::resman
)
