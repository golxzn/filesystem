#include <algorithm>
#include <filesystem>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/os/filesystem.hpp>

namespace {

std::wstring get_current_path() {
	auto current_path{ std::filesystem::current_path().wstring() };
	std::replace(std::begin(current_path), std::end(current_path), L'\\', gxzn::os::fs::separator);
	return current_path;
}

bool ends_with(const std::wstring_view str, const std::wstring_view end) {
	if (str.size() < end.size()) {
		return false;
	}
	auto rbegin{ std::rbegin(str) };
	return std::equal(rbegin, std::next(rbegin, end.size()), std::rbegin(end));
}

} // anonymous namespace

TEST_CASE("filesystem", "[filesystem][initialization]") {
	const auto current_path{ get_current_path() };

	const auto error{ gxzn::os::fs::initialize(L"filesystem_tests") };
	SECTION("Initialization") {
		INFO("gxzn::os::fs::initialize: " << gxzn::os::fs::to_narrow(error.message));
		REQUIRE_FALSE(error.has_error());
	} // SECTION("Initialization")

	SECTION("Assets directory") {

		if (const auto assets_dir{ gxzn::os::fs::assets_directory() }; true) {
			const auto default_assets_dir{
				gxzn::os::fs::join(current_path, gxzn::os::fs::default_assets_directory_name)
			};

			INFO("gxzn::os::fs::assets_directory (default): " << gxzn::os::fs::to_narrow(assets_dir));
			REQUIRE_FALSE(assets_dir.empty());
			REQUIRE(assets_dir == default_assets_dir);
		}

		static constexpr std::wstring_view absolute_res_dir{
#if defined(GXZN_OS_FS_WINDOWS)
			L"G:/resources"
#else // defined(GXZN_OS_FS_WINDOWS)
			L"/resources"
#endif // defined(GXZN_OS_FS_WINDOWS)
		};

		if (const auto status{ gxzn::os::fs::initialize(L"filesystem_tests", absolute_res_dir) }; status) {
			INFO("gxzn::os::fs::initialize(L\"filesystem_tests\", " \
				<< gxzn::os::fs::to_narrow(absolute_res_dir) << "): " \
				<< gxzn::os::fs::to_narrow(status.message));
			REQUIRE_FALSE(status.has_error());
		}
		if (const auto resources_dir{ gxzn::os::fs::assets_directory() }; true) {
			INFO("gxzn::os::fs::assets_directory (absolute): " << gxzn::os::fs::to_narrow(resources_dir));
			REQUIRE_FALSE(resources_dir.empty());
			REQUIRE(resources_dir == absolute_res_dir);
		}

		if (const auto status{ gxzn::os::fs::initialize(L"filesystem_tests", L"../../res") }; status) {
			INFO("gxzn::os::fs::initialize(L\"filesystem_tests\", L\"../../res\"): " \
				<< gxzn::os::fs::to_narrow(status.message));
			REQUIRE_FALSE(status.has_error());
		}
		if (const auto res_dir{ gxzn::os::fs::assets_directory() }; true) {
			const auto expected_res_dir{ gxzn::os::fs::normalize(current_path + L"/../../res") };

			INFO("gxzn::os::fs::assets_directory (../../res): " << gxzn::os::fs::to_narrow(res_dir));
			INFO("Expected:                                     " << gxzn::os::fs::to_narrow(expected_res_dir));
			REQUIRE_FALSE(res_dir.empty());
			REQUIRE(res_dir == expected_res_dir);
		}
	} // SECTION("Assets directory")

	SECTION("Current directory") {
		const auto current_dir{ gxzn::os::fs::current_directory() };
		INFO("gxzn::os::fs::current_directory: " << gxzn::os::fs::to_narrow(current_dir));
		INFO("current path:                      " << gxzn::os::fs::to_narrow(current_path));
		REQUIRE_FALSE(current_dir.empty());
		REQUIRE(current_dir.rfind(current_path, 0) == 0);
	} // SECTION("Current directory")

	SECTION("User data directory") {
		const auto user_data_dir{ gxzn::os::fs::user_data_directory() };
		INFO("gxzn::os::fs::user_data_directory: " << gxzn::os::fs::to_narrow(user_data_dir));
		REQUIRE_FALSE(user_data_dir.empty());
#if defined(GXZN_OS_FS_WINDOWS)
		REQUIRE(user_data_dir.rfind(L"C:/Users/", 0) == 0);
		REQUIRE(ends_with(user_data_dir, L"/AppData/Roaming/filesystem_tests"));
#elif defined(GXZN_OS_FS_LINUX)
		REQUIRE(user_data_dir.rfind(L"/home/", 0) == 0);
		REQUIRE(ends_with(user_data_dir, L"/.config/filesystem_tests"));
#elif defined(GXZN_OS_FS_MACOS)
		REQUIRE(user_data_dir.rfind(L"/Users/", 0) == 0);
		REQUIRE(ends_with(user_data_dir, L"/Library/Application Support/filesystem_tests"));
#endif
	} // SECTION("User data directory")
}
