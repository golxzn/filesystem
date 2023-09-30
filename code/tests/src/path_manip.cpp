
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/os/filesystem.hpp>


TEST_CASE("filesystem", "[filesystem][path manipulation]") {
	REQUIRE_FALSE(gxzn::os::fs::initialize("filesystem_tests").has_error());

	SECTION("join") {
		REQUIRE(gxzn::os::fs::join("a", "/")   == "a");
		REQUIRE(gxzn::os::fs::join("/", "a")   == "a");
		REQUIRE(gxzn::os::fs::join("a", "")    == "a");
		REQUIRE(gxzn::os::fs::join("", "b")    == "b");
		REQUIRE(gxzn::os::fs::join("a", "b")   == "a/b");
		REQUIRE(gxzn::os::fs::join("a/", "b")  == "a/b");
		REQUIRE(gxzn::os::fs::join("a\\", "b") == "a/b");
		REQUIRE(gxzn::os::fs::join("a", "/b")  == "a/b");
		REQUIRE(gxzn::os::fs::join("a", "\\b") == "a/b");
		REQUIRE(gxzn::os::fs::join("a/", "/b") == "a/b");
	} // SECTION("join")

	SECTION("parent_directory") {
		REQUIRE(gxzn::os::fs::parent_directory("a")       == "");
		REQUIRE(gxzn::os::fs::parent_directory("a/b")     == "a");
		REQUIRE(gxzn::os::fs::parent_directory("a/b/c")   == "a/b");
		REQUIRE(gxzn::os::fs::parent_directory("a\\b")    == "a");
		REQUIRE(gxzn::os::fs::parent_directory("a\\b\\c") == "a\\b");
	} // SECTION("parent_directory")

	SECTION("normalize") {
		using namespace std::string_view_literals;
		static const std::unordered_map<std::wstring_view, std::wstring_view> tests{
			{ L"G:/"sv,                                                   L"G:/"sv            },
			{ L""sv,                                                      L""sv               },
			{ L"    G:\\Hello world/how\\..\\lol\\..\\./.\\.\\.     "sv,  L"G:/Hello world"sv },
			{ L"G:\\Hello world/./how\\..\\lol\\..\\.."sv,                L"G:/"sv },
			{ L"G:\\Hello world/how\\..\\lol\\..\\..\\"sv,                L"G:/"sv },
			{ L"G:\\Hello world/./how\\..\\lol\\..\\.\\../"sv,            L"G:/"sv },
			{ L"G:/././././\\Hello world/how\\..\\lol\\.\\kek/"sv,        L"G:/Hello world/lol/kek"sv },
			{ L"/./"sv,                                                   L"/"sv },
			{ L"/./Hello world/./how\\..\\lol\\..\\..\\../"sv,            L"/"sv },
			{ L"/Hello world/how\\..\\lol\\.\\kek/"sv,                    L"/Hello world/lol/kek"sv },
			{ L"////how\\..\\lol\\.\\kek/"sv,                             L"/lol/kek"sv },
		};

		for (const auto [from, to] : tests) {
			INFO("Converting from '" << gxzn::os::fs::to_narrow(from) <<
				"' to '" << gxzn::os::fs::to_narrow(to) << "'");
			INFO("Actual result: '" << gxzn::os::fs::to_narrow(gxzn::os::fs::normalize(from)) << "'");
			REQUIRE(gxzn::os::fs::normalize(from) == to);
		}
	} // SECTION("normalize")
}