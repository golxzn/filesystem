
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

message(CHECK_START "Fetching Catch2...")
FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)
message(CHECK_PASS "Catch2 fetched")

include(CTest)
include(Catch)

enable_testing()

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_executable(tests ${sources} ${headers})
target_link_libraries(tests PRIVATE
	Catch2::Catch2WithMain
	golxzn::resman
)

catch_discover_tests(tests
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
