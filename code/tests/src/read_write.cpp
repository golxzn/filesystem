#include <format>
#include <algorithm>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>

std::string dump(const auto &content) {
	std::string result;
	for (auto byte : content) {
		result += std::format("{:02X} ", byte);
	}

	return result;
}

TEST_CASE("resman", "[resman][read_write]") {
	REQUIRE_FALSE(gxzn::resman::initialize(L"resman_tests").has_error());

	static constexpr std::initializer_list<uint8_t> expected_content{
		0x29, 0x60, 0x30, 0xA0, 0x22, 0xAB, 0x4C, 0x88, 0xBB, 0xE9
	};


	SECTION("Read res://test.bin") {
		static constexpr std::wstring_view path{ L"res://test.bin" };

		const auto content{ gxzn::resman::read_binary(path) };
		INFO("Read content:     " << dump(content));
		INFO("Expected content: " << dump(expected_content));
		REQUIRE_FALSE(content.empty());
		REQUIRE(content.size() == expected_content.size());
		REQUIRE(std::equal(std::begin(content), std::end(content), std::begin(expected_content)));

		// BENCHMARK("Read res://test.bin") { return gxzn::resman::read_binary(path); };
	}

	SECTION("Write user://write.bin") {
		static constexpr std::wstring_view path{ L"user://write.bin" };
		const auto status{ gxzn::resman::write_binary(path, expected_content) };
		INFO("Status: " << gxzn::resman::to_narrow(status.message));
		REQUIRE_FALSE(status.has_error());

		const auto content{ gxzn::resman::read_binary(path) };
		REQUIRE_FALSE(content.empty());
		REQUIRE(content.size() == expected_content.size());
		REQUIRE(std::equal(std::begin(content), std::end(content), std::begin(expected_content)));

		// BENCHMARK("Write user://write.bin") { return gxzn::resman::write_binary(path, expected_content); };
	}
}
