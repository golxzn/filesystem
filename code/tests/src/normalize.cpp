
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>

#include "common.hpp"

TEST_CASE("resman::normalize", "[resman][normalize]") {
	using namespace std::string_view_literals;
	static const std::unordered_map<std::wstring_view, std::wstring_view> tests{
		{ L"G:/"sv,                                                   L"G:/"sv            },
		{ L""sv,                                                      L""sv               },
		{ L"    G:\\Hello world/how\\..\\lol\\..\\./.\\.\\.     "sv,  L"G:/Hello world/"sv },
		{ L"G:\\Hello world/./how\\..\\lol\\..\\.."sv,                L"G:/"sv },
		{ L"G:\\Hello world/how\\..\\lol\\..\\..\\"sv,                L"G:/"sv },
		{ L"G:\\Hello world/./how\\..\\lol\\..\\.\\../"sv,            L"G:/"sv },
		{ L"G:/././././\\Hello world/how\\..\\lol\\.\\kek/"sv,        L"G:/Hello world/lol/kek/"sv },
		{ L"/./"sv,                                                   L"/"sv },
		{ L"/./Hello world/./how\\..\\lol\\..\\..\\../"sv,            L"/"sv },
		{ L"/Hello world/how\\..\\lol\\.\\kek/"sv,                    L"/Hello world/lol/kek/"sv },
		{ L"////how\\..\\lol\\.\\kek/"sv,                             L"/lol/kek/"sv },
	};

	for (const auto [from, to] : tests) {
		INFO("Converting from '" << fortest::wstring2string(from) <<
			"' to '" << fortest::wstring2string(to) << "'");
		INFO("Actual result: '" << fortest::wstring2string(golxzn::resman::normalize(from)) << "'");
		REQUIRE(golxzn::resman::normalize(from) == to);
	}
}
