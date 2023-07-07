#include "common.hpp"

namespace fortest {

std::string wstring2string(const std::wstring_view str) {
	return std::string{ std::begin(str), std::end(str) };
}

} // namespace fortest