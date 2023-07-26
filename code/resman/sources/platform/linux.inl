
#include "unix.inl"

namespace golxzn::details {

std::wstring appdata_directory() {
	if (const auto home{ std::getenv("XDG_CONFIG_HOME") }; home != nullptr) {
		return resman::to_wide(home);
	} else if (auto home{ __unix_get_home() }; !home.empty()) {
		resman::join(home, L"/.config");
		return home;
	}
	return L"~/.config";
}

// Implemented in platform/unix.inl
// std::wstring cwd() { }

// Implemented in platform/unix.inl
// bool is_directory(const std::wstring_view path) {}

} // namespace golxzn::details
