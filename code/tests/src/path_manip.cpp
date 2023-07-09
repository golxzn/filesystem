
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>


TEST_CASE("resman", "[resman][path manipulation]") {
	REQUIRE_FALSE(golxzn::resman::initialize("resman_tests").has_error());

	SECTION("join") {
		REQUIRE(gxzn::resman::join("a", "/")   == "a");
		REQUIRE(gxzn::resman::join("/", "a")   == "a");
		REQUIRE(gxzn::resman::join("a", "")    == "a");
		REQUIRE(gxzn::resman::join("", "b")    == "b");
		REQUIRE(gxzn::resman::join("a", "b")   == "a/b");
		REQUIRE(gxzn::resman::join("a/", "b")  == "a/b");
		REQUIRE(gxzn::resman::join("a\\", "b") == "a/b");
		REQUIRE(gxzn::resman::join("a", "/b")  == "a/b");
		REQUIRE(gxzn::resman::join("a", "\\b") == "a/b");
		REQUIRE(gxzn::resman::join("a/", "/b") == "a/b");
	} // SECTION("join")

	SECTION("parent_directory") {
		REQUIRE(gxzn::resman::parent_directory("a")       == "");
		REQUIRE(gxzn::resman::parent_directory("a/b")     == "a");
		REQUIRE(gxzn::resman::parent_directory("a/b/c")   == "a/b");
		REQUIRE(gxzn::resman::parent_directory("a\\b")    == "a");
		REQUIRE(gxzn::resman::parent_directory("a\\b\\c") == "a\\b");
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
			INFO("Converting from '" << gxzn::resman::to_narrow(from) <<
				"' to '" << gxzn::resman::to_narrow(to) << "'");
			INFO("Actual result: '" << gxzn::resman::to_narrow(golxzn::resman::normalize(from)) << "'");
			REQUIRE(golxzn::resman::normalize(from) == to);
		}
	} // SECTION("normalize")
}