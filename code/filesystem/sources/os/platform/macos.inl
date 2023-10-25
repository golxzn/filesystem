
#include "unix.inl"

namespace golxzn::os::details {

std::wstring appdata_directory() {
	if (auto home{ __unix_get_home() }; !home.empty()) {
		filesystem::join(home, L"/Library/Application Support");
		return home;
	}
	return L"~/Library/Application Support";
}

// Implemented in platform/unix.inl
// std::wstring cwd() { }

// Implemented in platform/unix.inl
// bool is_directory(const std::wstring_view path) {}

} // namespace golxzn::os::details
