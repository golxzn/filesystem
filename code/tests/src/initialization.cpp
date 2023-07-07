#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>

#include "common.hpp"

TEST_CASE("resman::initialize", "[resman][initialization]") {
	const auto error{ golxzn::resman::initialize("resman_tests") };
	SECTION("initialize") {
		INFO("golxzn::resman::initialize: " << error.message);
		REQUIRE_FALSE(error.has_error());
	}

	SECTION("User data directory") {
		const auto user_data_dir{ golxzn::resman::user_data_directory() };
		INFO("golxzn::resman::user_data_directory: " << fortest::wstring2string(user_data_dir));
		REQUIRE_FALSE(user_data_dir.empty());
#if defined(GRM_WINDOWS)
		REQUIRE(user_data_dir.starts_with(L"C:/Users/"));
		REQUIRE(user_data_dir.ends_with(L"/AppData/Roaming/resman_tests"));
#elif defined(GRM_LINUX)
		REQUIRE(user_data_dir.starts_with(L"/home/"));
		REQUIRE(user_data_dir.ends_with(L"/.local/resman_tests"));
#elif defined(GRM_MACOS)
		REQUIRE(user_data_dir.starts_with(L"/Users/"));
		REQUIRE(user_data_dir.ends_with(L"/Library/Application Support/resman_tests"));
#endif
	}
}
