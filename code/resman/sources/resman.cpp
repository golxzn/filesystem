#include <format>
#include <ranges>
#include <vector>
#include <locale>
#include <cstdlib>
#include <numeric>
#include <algorithm>

#include "golxzn/resman.hpp"

#if defined(GRM_WINDOWS)

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif // !defined(WIN32_LEAN_AND_MEAN)

#include <windows.h>
#include <winerror.h>
#include <stringapiset.h>
#include <shlobj.h>

#else

#include <pwd.h>
#include <unistd.h>

namespace {

static std::string unix_get_home() {
	std::string res;
	const int uid{ getuid() };

	if (const auto home{ std::getenv("HOME") }; uid != 0 && home != nullptr) {
		return std::string{ home };
	}

	const sysconf_out{ sysconf(_SC_GETPW_R_SIZE_MAX) };
	const size_t length{ sysconf_out > 0 ? static_cast<size_t>(sysconf_out) : 16384lu };

	std::string buffer(length, '\0');

	struct passwd* pw = nullptr;
	struct passwd pwd;
	if (auto err{ getpwuid_r(uid, &pwd, buffer.data(), buffer.size(), &pw) }; err != 0) {
		return "";
	}

	if (const auto dir{ pw->pw_dir }; dir != nullptr) {
		return std::string{ dir };
	}
	return "";
}

} // anonymous namespace

#endif // defined(GRM_WINDOWS)


namespace golxzn {

std::string resman::appname{ resman::default_application_name };
std::wstring resman::user_data_dir{};
std::deque<std::string> resman::assets_dirs{ };

bool resman::error::has_error() const noexcept { return !message.empty(); }
resman::error::operator bool() const noexcept { return !has_error(); }

resman::error resman::initialize(const std::string_view application_name, const string_view_list assets_names) {
	set_application_name(application_name);

	error err{};
	if (!setup_assets_directories(assets_names)) {
		err.message = "Failed to setup assets directories";
	}
	if (!setup_user_data_directory()) {
		err.message += std::format("{} '{}' {}",
			(err.has_error() ? " and" : "Failed to setup"), appname, "user data directory");
	}

	return err;
}

void resman::set_application_name(const std::string_view application_name) noexcept {
	appname = application_name;
	if (appname.empty()) {
		appname = default_application_name;
	}
}

std::string_view resman::application_name() noexcept {
	return appname;
}

std::wstring_view resman::user_data_directory() noexcept {
	return user_data_dir;
}


std::wstring resman::normalize(const std::string_view str) {
	const std::wstring temp{ std::begin(str), std::end(str) };
	return normalize(temp);
}

std::wstring resman::normalize(std::wstring_view str) {
	while(!str.empty() && str.front() == L' ') str.remove_prefix(1);
	while(!str.empty() && str.back() == L' ') str.remove_suffix(1);
	if (str.empty()) return L"";

	static constexpr std::wstring_view slash{ L"\\/" };
	static constexpr std::wstring_view prev_dir{ L".." };
	static constexpr std::wstring_view curr_dir{ L"." };

	std::wstring prefix;
	if (str.find(L':') == 1) {
		prefix.reserve(3);
		prefix = str.substr(0, 2);
		prefix += L"/";
		str.remove_prefix(2);
	} else {
		prefix = L"/";
	}

	std::vector<std::wstring_view> parts;
	parts.reserve(std::ranges::count_if(str, [](const auto &c){ return slash.find(c) != std::wstring_view::npos; }) + 1lu);
	size_t prev_slash{};
	size_t curr_slash{};
	size_t next_slash{};

	for(; next_slash != std::wstring_view::npos; prev_slash = std::exchange(curr_slash, next_slash + 1)) {
		next_slash = str.find_first_of(slash, curr_slash);
		const auto substr{ str.substr(curr_slash, next_slash - curr_slash) };

		if (substr.empty() || substr == L" ") continue;
		if (substr == curr_dir) [[unlikely]] continue;
		if (substr == prev_dir) [[unlikely]] {
			if (parts.empty() || parts.back().find(L':') != std::wstring_view::npos) [[unlikely]] {
				return prefix;
			}
			parts.pop_back();
			continue;
		}
		parts.emplace_back(substr);
	}

	const size_t length{ std::accumulate(
		std::begin(parts), std::end(parts), size_t{},
		[](size_t accum, const auto &str) { return accum + 1 + str.size(); }
	)};

	std::wstring result{ std::move(prefix) };
	result.reserve(length);

	for (auto &&part : parts) {
		if (part.empty()) continue;
		result += std::format(L"{}/", std::move(part));
	}

	return result;
}

bool resman::setup_assets_directories(const string_view_list assets_names) {
	return true;
}

bool resman::setup_user_data_directory() {
#if defined(GRM_WINDOWS)

	LPWSTR path{ nullptr };
	const auto result{ SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &path) };
	if (SUCCEEDED(result)) {
		const size_t length{ std::wcslen(path) };
		user_data_dir = std::format(L"{}/{}", path, std::wstring{ std::begin(appname), std::end(appname) });
		normalize(user_data_dir);
		// user_data_dir.replace(L'\\', static_cast<wchar_t>(separator));
	}
	CoTaskMemFree(path);

#elif defined(GRM_LINUX)

	if (const auto home{ std::getenv("XDG_CONFIG_HOME") }; home != nullptr) {
		user_data_dir = std::format("{}{}{}", home, separator, appname);
		return true;
	}

	if (auto home{ unix_get_home(error) }; !home.empty()) {
		user_data_dir = std::format(L"{}{}{}",
			std::wstring{ std::begin(home), std::end(home) }, L"/.config/", appname
		);
		return true;
	}

#elif defined(GRM_MAC)

	if (auto home{ unix_get_home(error) }; !home.empty()) {
		std::wstring path{ std::begin(home), std::end(home) };
		user_data_dir = std::format(L"{}{}{}",
			std::move(path), L"/Library/Application Support/", appname
		);
		return true;
	}

#endif
	return !user_data_dir.empty();
}

void resman::normalize(std::wstring &path) noexcept {
	std::ranges::replace(path, L'\\', L'/');
}

} // namespace golxzn
