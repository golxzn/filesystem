#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>

TEST_CASE("resman::initialize", "[resman][initialization]") {
	const std::string_view str_current_path{ __FILE__ };
	const std::wstring current_path{
		std::begin(str_current_path),
		std::prev(std::end(str_current_path), sizeof("code/tests/src/initialization.cpp") - 1)
	};

	const auto error{ golxzn::resman::initialize(L"resman_tests") };
	SECTION("initialize") {
		INFO("golxzn::resman::initialize: " << gxzn::resman::to_narrow(error.message));
		REQUIRE_FALSE(error.has_error());
	}

	SECTION("Current directory") {
		const auto current_dir{ golxzn::resman::current_directory() };
		INFO("golxzn::resman::current_directory: " << gxzn::resman::to_narrow(current_dir));
		REQUIRE_FALSE(current_dir.empty());
		REQUIRE(current_dir.starts_with(current_path));
	}

	SECTION("User data directory") {
		const auto user_data_dir{ golxzn::resman::user_data_directory() };
		INFO("golxzn::resman::user_data_directory: " << gxzn::resman::to_narrow(user_data_dir));
		REQUIRE_FALSE(user_data_dir.empty());
#if defined(GRM_WINDOWS)
		REQUIRE(user_data_dir.starts_with(L"C:/Users/"));
		REQUIRE(user_data_dir.ends_with(L"/AppData/Roaming/resman_tests/"));
#elif defined(GRM_LINUX)
		REQUIRE(user_data_dir.starts_with(L"/home/"));
		REQUIRE(user_data_dir.ends_with(L"/.local/resman_tests/"));
#elif defined(GRM_MACOS)
		REQUIRE(user_data_dir.starts_with(L"/Users/"));
		REQUIRE(user_data_dir.ends_with(L"/Library/Application Support/resman_tests/"));
#endif
	}
}
