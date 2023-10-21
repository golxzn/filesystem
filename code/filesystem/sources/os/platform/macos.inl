
#include "unix.inl"

namespace golxzn::os::details {

std::wstring appdata_directory() {
	if (auto home{ __unix_get_home() }; !home.empty()) {
		std::wstring path{ std::begin(home), std::end(home) };
		return std::format(L"{}{}",
			std::move(path), L"/Library/Application Support"
		);
	}
	return L"~/Library/Application Support";
}

// Implemented in platform/unix.inl
// std::wstring cwd() { }

// Implemented in platform/unix.inl
// bool is_directory(const std::wstring_view path) {}

} // namespace golxzn::os::details
