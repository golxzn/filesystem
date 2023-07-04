#pragma once

#include <string>
#include <string_view>

namespace golxzn {

class resman {
public:
	resman() = delete;

	static bool initialize(const std::string_view application_name);

private:
	static std::string appname;
};

namespace resources { using manager = resman; }

} // namespace golxzn
