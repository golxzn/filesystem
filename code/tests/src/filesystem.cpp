
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <golxzn/resman.hpp>

TEST_CASE("resman", "[resman][filesystem]") {
	REQUIRE_FALSE(gxzn::resman::initialize(L"resman_tests").has_error());

	SECTION("exists") {
		REQUIRE_FALSE(gxzn::resman::exists("user://nonexisfile.txt"));
		REQUIRE_FALSE(gxzn::resman::exists("user://nonexisdir"));

		REQUIRE(gxzn::resman::exists("res://"));
	}

	SECTION("is_file and is_directory") {
		REQUIRE_FALSE(gxzn::resman::is_file("user://nonexisfile.txt"));
		REQUIRE_FALSE(gxzn::resman::is_directory("user://nonexisdir"));

		REQUIRE(gxzn::resman::is_file("res://test.bin"));
		REQUIRE(gxzn::resman::is_directory("res://"));
	}

	SECTION("make_directory & remove_directory & remove_file") {
		static constexpr auto testdir{ "user://testdir" };

		REQUIRE_FALSE(gxzn::resman::remove_directory(testdir).has_error()); // Ensure directory does not exist
		REQUIRE_FALSE(gxzn::resman::exists(testdir));

		const auto make_status{ gxzn::resman::make_directory(testdir) };
		INFO("Make '" << testdir << "' error: " << gxzn::resman::to_narrow(make_status.message));
		REQUIRE_FALSE(make_status.has_error());
		REQUIRE(gxzn::resman::exists(testdir));

		const auto remove_status{ gxzn::resman::remove_directory(testdir) };
		INFO("Remove '" << testdir << "' error: " << gxzn::resman::to_narrow(make_status.message));
		REQUIRE_FALSE(make_status.has_error());
		REQUIRE_FALSE(gxzn::resman::exists(testdir));

		static constexpr std::initializer_list<std::string_view> test_files{
			"user://testdir/test1.txt",
			"user://testdir/test2.txt",
			"user://testdir/test3.txt",
			"user://testdir/lol/test11.txt",
			"user://testdir/kek/test11.txt",
			"user://testdir/WATA/test11.txt",
		};

		static constexpr std::initializer_list<uint8_t> content{
			0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
		};
		for (const auto &file : test_files) {
			const auto status{ gxzn::resman::write_binary(file, content) };
			INFO("Make file '" << file << "' error: " << gxzn::resman::to_narrow(status.message));
			REQUIRE_FALSE(status.has_error());
			REQUIRE(gxzn::resman::exists(file));
		}

		const auto recursively_remove_status{ gxzn::resman::remove(testdir) };
		INFO("Remove recursively '" << testdir << "' error: " \
			<< gxzn::resman::to_narrow(recursively_remove_status.message));
		REQUIRE_FALSE(recursively_remove_status.has_error());
		REQUIRE_FALSE(gxzn::resman::exists(testdir));
	}

	SECTION("entries") {
		const auto entries{ gxzn::resman::entries("res://") };
		REQUIRE_FALSE(entries.empty());
	}

}
