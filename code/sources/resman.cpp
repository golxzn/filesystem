#include "golxzn/resman.hpp"

namespace golxzn {

std::string resman::appname{ "unknown_app" };

bool resman::initialize(const std::string_view application_name) {
	return true;
}


} // namespace golxzn
