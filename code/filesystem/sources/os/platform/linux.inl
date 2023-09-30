
#include "unix.inl"

namespace golxzn::os::details {

std::wstring appdata_directory() {
	if (const auto home{ std::getenv("XDG_CONFIG_HOME") }; home != nullptr) {
		return filesystem::to_wide(home);
	} else if (auto home{ __unix_get_home() }; !home.empty()) {
		return std::format(L"{}/{}", std::move(home), L"/.config");
	}
	return L"~/.config";
}

// Implemented in platform/unix.inl
// std::wstring cwd() { }

// Implemented in platform/unix.inl
// bool is_directory(const std::wstring_view path) {}

} // namespace golxzn::os::details
