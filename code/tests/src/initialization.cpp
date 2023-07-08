#include <ranges>
#include <algorithm>
#include <filesystem>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>

namespace {

std::wstring get_current_path() {
	auto current_path{ std::filesystem::current_path().wstring() };
	std::ranges::replace(current_path, L'\\', gxzn::resman::separator);
	return current_path + gxzn::resman::separator;
}

} // anonymous namespace

TEST_CASE("resman::initialize", "[resman][initialization]") {
	const auto current_path{ get_current_path() };

	const auto error{ golxzn::resman::initialize(L"resman_tests") };
	SECTION("Initialization") {
		INFO("golxzn::resman::initialize: " << gxzn::resman::to_narrow(error.message));
		REQUIRE_FALSE(error.has_error());
	}

	SECTION("Assets directory") {

		if (const auto assets_dir{ golxzn::resman::assets_directory() }; true) {
			const auto default_assets_dir{ current_path
				+ std::wstring{ gxzn::resman::default_assets_directory_name }
				+ gxzn::resman::separator
			};

			INFO("golxzn::resman::assets_directory (default): " << gxzn::resman::to_narrow(assets_dir));
			REQUIRE_FALSE(assets_dir.empty());
			REQUIRE(assets_dir == default_assets_dir);
		}

		static constexpr std::wstring_view absolute_res_dir{
#if defined(GRM_WINDOWS)
			L"G:/resources/"
#else // defined(GRM_WINDOWS)
			L"/resources/"
#endif // defined(GRM_WINDOWS)
		};

		REQUIRE_FALSE(gxzn::resman::initialize(L"resman_tests", absolute_res_dir).has_error());
		if (const auto resources_dir{ golxzn::resman::assets_directory() }; true) {
			INFO("golxzn::resman::assets_directory (absolute): " << gxzn::resman::to_narrow(resources_dir));
			REQUIRE_FALSE(resources_dir.empty());
			REQUIRE(resources_dir == absolute_res_dir);
		}

		REQUIRE_FALSE(gxzn::resman::initialize(L"resman_tests", L"../../res").has_error());
		if (const auto res_dir{ golxzn::resman::assets_directory() }; true) {
			const auto expected_res_dir{ gxzn::resman::normalize(current_path + L"../../res") };

			INFO("golxzn::resman::assets_directory (../../res): " << gxzn::resman::to_narrow(res_dir));
			INFO("Expected:                                     " << gxzn::resman::to_narrow(expected_res_dir));
			REQUIRE_FALSE(res_dir.empty());
			REQUIRE(res_dir == expected_res_dir);
		}
	}

	SECTION("Current directory") {
		const auto current_dir{ golxzn::resman::current_directory() };
		INFO("golxzn::resman::current_directory: " << gxzn::resman::to_narrow(current_dir));
		INFO("current path:                      " << gxzn::resman::to_narrow(current_path));
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
