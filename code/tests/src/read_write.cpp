#include <format>
#include <algorithm>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/os/filesystem.hpp>

std::string dump(const auto &content) {
	std::string result;
	for (auto byte : content) {
		result += std::format("{:02X} ", byte);
	}

	return result;
}

TEST_CASE("filesystem", "[filesystem][read_write]") {
	REQUIRE_FALSE(gxzn::os::fs::initialize(L"filesystem_tests").has_error());

	static constexpr std::initializer_list<uint8_t> expected_content{
		0x29, 0x60, 0x30, 0xA0, 0x22, 0xAB, 0x4C, 0x88, 0xBB, 0xE9
	};


	SECTION("Read res://test.bin") {
		static constexpr std::wstring_view path{ L"res://test.bin" };

		const auto content{ gxzn::os::fs::read_binary(path) };
		INFO("Read content:     " << dump(content));
		INFO("Expected content: " << dump(expected_content));
		REQUIRE_FALSE(content.empty());
		REQUIRE(content.size() == expected_content.size());
		REQUIRE(std::equal(std::begin(content), std::end(content), std::begin(expected_content)));

		// BENCHMARK("Read res://test.bin") { return gxzn::os::fs::read_binary(path); };
	}

	SECTION("Read res://test.txt") {
		static constexpr std::string_view txt_expected{ "Hello, world!" };
		static constexpr std::wstring_view path{ L"res://test.txt" };

		const auto content{ gxzn::os::fs::read_text(path) };
		INFO("Read content:     " << content);
		INFO("Expected content: " << txt_expected);
		REQUIRE_FALSE(content.empty());
		REQUIRE(content.size() == txt_expected.size());
		REQUIRE(content == txt_expected);

		// BENCHMARK("Read res://test.txt") { return gxzn::os::fs::read_binary(path); };
	}

	SECTION("Write user://write.bin") {
		static constexpr std::wstring_view path{ L"user://write.bin" };
		const auto status{ gxzn::os::fs::write_binary(path, expected_content) };
		INFO("Status: " << gxzn::os::fs::to_narrow(status.message));
		REQUIRE_FALSE(status.has_error());

		const auto content{ gxzn::os::fs::read_binary(path) };
		REQUIRE_FALSE(content.empty());
		REQUIRE(content.size() == expected_content.size());
		REQUIRE(std::equal(std::begin(content), std::end(content), std::begin(expected_content)));

		// BENCHMARK("Write user://write.bin") { return gxzn::os::fs::write_binary(path, expected_content); };
	}
}

struct CustomResource {
	explicit CustomResource(std::string &&content) noexcept : text{ std::move(content) } {}
	explicit CustomResource(std::vector<uint8_t> &&content) noexcept : data{ std::move(content) } {}

	std::string text;
	std::vector<uint8_t> data;
};

TEST_CASE("filesystem", "[filesystem][read_write custom]") {
	static constexpr std::string_view expected_text{ "custom" };
	static constexpr std::initializer_list<uint8_t> expected_data{ 'c', 'u', 's', 't', 'o', 'm' };

	SECTION("Construct CustomResource by res://custom.bin") {
		auto res{ gxzn::os::fs::read_binary<CustomResource>("res://custom.bin") };
		INFO("Read content:     " << dump(res.data));
		INFO("Expected content: " << dump(expected_data));
		REQUIRE(res.data.size() == expected_data.size());
		REQUIRE(std::equal(std::begin(res.data), std::end(res.data), std::begin(expected_data)));
	}
	SECTION("Construct CustomResource by res://custom.txt") {
		auto res{ gxzn::os::fs::read_text<CustomResource>("res://custom.txt") };
		INFO("Read content:     " << res.text);
		INFO("Expected content: " << expected_text);
		REQUIRE(res.text.size() == expected_text.size());
		REQUIRE(res.text == expected_text);
	}
	SECTION("Construct std::shared_ptr<CustomResource> by res://custom.bin") {
		auto res{ gxzn::os::fs::read_shared_binary<CustomResource>("res://custom.bin") };
		REQUIRE(res != nullptr);
		INFO("Read content:     " << dump(res->data));
		INFO("Expected content: " << dump(expected_data));
		REQUIRE(res->data.size() == expected_data.size());
		REQUIRE(std::equal(std::begin(res->data), std::end(res->data), std::begin(expected_data)));
	}
	SECTION("Construct std::shared_ptr<CustomResource> by res://custom.txt") {
		auto res{ gxzn::os::fs::read_shared_text<CustomResource>("res://custom.txt") };
		REQUIRE(res != nullptr);
		INFO("Read content:     " << res->text);
		INFO("Expected content: " << expected_text);
		REQUIRE(res->text.size() == expected_text.size());
		REQUIRE(res->text == expected_text);
	}
	SECTION("Construct std::unique_ptr<CustomResource> by res://custom.bin") {
		auto res{ gxzn::os::fs::read_unique_binary<CustomResource>("res://custom.bin") };
		REQUIRE(res != nullptr);
		INFO("Read content:     " << dump(res->data));
		INFO("Expected content: " << dump(expected_data));
		REQUIRE(res->data.size() == expected_data.size());
		REQUIRE(std::equal(std::begin(res->data), std::end(res->data), std::begin(expected_data)));
	}
	SECTION("Construct std::unique_ptr<CustomResource> by res://custom.txt") {
		auto res{ gxzn::os::fs::read_unique_text<CustomResource>("res://custom.txt") };
		REQUIRE(res != nullptr);
		INFO("Read content:     " << res->text);
		INFO("Expected content: " << expected_text);
		REQUIRE(res->text.size() == expected_text.size());
		REQUIRE(res->text == expected_text);
	}
}