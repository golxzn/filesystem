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

static std::wstring unix_get_home() {
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


resman::associations_type resman::associations_map{};
std::wstring resman::appname{ resman::default_application_name };


//========================================= resman::error ========================================//


bool resman::error::has_error() const noexcept { return !message.empty(); }

resman::error::operator bool() const noexcept { return !has_error(); }


//======================================== resman::public ========================================//


resman::error resman::initialize(const std::wstring_view app_name, const std::wstring_view assets_path) {
	set_application_name(app_name);

	error err{};
	if (auto assets_dir{ setup_assets_directories(assets_path) }; !assets_dir.empty()) [[likely]] {
		associate("res://", std::move(assets_dir));
	} else {
		err.message = L"Failed to setup assets directories";
	}

	if (auto user_dir{ setup_user_data_directory() }; !user_dir.empty()) [[likely]] {
		associate("user://", std::move(user_dir));
	} else {
		err.message += std::format(L"{} '{}' {}",
			(err.has_error() ? L" and" : L"Failed to setup"), appname, L"user data directory");
	}

	return err;
}

void resman::set_application_name(const std::wstring_view application_name) noexcept {
	appname = application_name;
	if (appname.empty()) [[unlikely]] {
		appname = default_application_name;
	}
}

void resman::associate(const std::string_view protocol_view, std::wstring &&prefix) noexcept {
	if (protocol_view.empty()) [[unlikely]] return;

	std::string protocol{ protocol_view };
	if (!protocol_view.ends_with(protocol_separator)) [[unlikely]] {
		protocol += protocol_separator;
	}

	associations_map.insert_or_assign(std::move(protocol), std::move(prefix));
}

std::wstring_view resman::get_association(const std::string_view protocol) noexcept {
	if (protocol.empty()) [[unlikely]] return none;

	if (const auto found{ associations_map.find(protocol) };
			found != std::cend(associations_map)) [[likely]] {
		return found->second;
	}
	return none;
}

std::wstring_view resman::application_name() noexcept {
	return appname;
}

std::wstring_view resman::user_data_directory() noexcept {
	return get_association("user://");
}

std::wstring_view resman::assets_directory() noexcept {
	return get_association("res://");
}

const resman::associations_type &resman::associations() noexcept {
	return associations_map;
}

std::wstring resman::normalize(std::wstring_view str) {
	while(!str.empty() && str.front() == L' ') str.remove_prefix(1);
	while(!str.empty() && str.back() == L' ') str.remove_suffix(1);
	if (str.empty()) [[unlikely]] return L"";

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

std::wstring resman::current_directory() {

#if defined(GRM_WINDOWS)

	std::wstring path(MAX_PATH, L'\0');
	if (const size_t length{ GetCurrentDirectoryW(path.size(), path.data()) }; length != 0) {
		path.resize(length);
		return normalize(path);
	}

#else

	if (std::string path(MAX_PATH, '\0'); getcwd(path.data(), path.size()) != nullptr) {
		return normalize(path);
	}

#endif
	return L"./";
};

std::wstring resman::to_wide(const std::string_view str) noexcept {
	return std::wstring{ std::begin(str), std::end(str) };
}

std::string resman::to_narrow(const std::wstring_view str) noexcept {
	static constexpr uint16_t bits_per_char{ 8 };
	static constexpr size_t difference{
		sizeof(std::wstring_view::value_type) / sizeof(std::string::value_type)
	};

	std::string result;
	result.reserve(str.size() * difference);

	for (const auto c : str) {
		const auto int_c{ static_cast<uint16_t>(c) };
		const char left_part{ static_cast<char>(int_c >> bits_per_char) };
		const char right_part{ static_cast<char>(int_c & 0x00FFu) };

		if (left_part != uint16_t{}) [[unlikely]] {
			result += left_part;
		}

		result += right_part;
	}
	return result;
}


//======================================== resman::aliases =======================================//


resman::error resman::initialize(const std::string_view app, const std::string_view asset) {
	return initialize(to_wide(app), to_wide(asset));
}

void resman::set_application_name(const std::string_view application_name) noexcept {
	set_application_name(to_wide(application_name));
}

std::wstring resman::normalize(const std::string_view str) {
	return normalize(to_wide(str));
}


//======================================== resman::private =======================================//


std::wstring resman::setup_assets_directories(const std::wstring_view assets_path) {
	if (assets_path.starts_with(separator) || assets_path.find(L":") == 1) {
		return normalize(assets_path);
	}

	return normalize(current_directory() + std::wstring{ assets_path });
}

std::wstring resman::setup_user_data_directory() {
	std::wstring user_data_dir;
#if defined(GRM_WINDOWS)

	LPWSTR path{ nullptr };
	const auto result{ SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &path) };
	if (SUCCEEDED(result)) {
		const size_t length{ std::wcslen(path) };
		user_data_dir = std::format(L"{}/{}", path, appname);
	}
	CoTaskMemFree(path);

#elif defined(GRM_LINUX)

	if (const auto home{ std::getenv("XDG_CONFIG_HOME") }; home != nullptr) {
		std::wstring wide_home{ std::begin(home), std::end(home) };
		user_data_dir = std::format(L"{}/{}", std::move(home), appname);
	} else if (auto home{ unix_get_home(error) }; !home.empty()) {
		user_data_dir = std::format(L"{}/{}", std::move(home), L"/.config/", appname);
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
	return normalize(user_data_dir);
}


} // namespace golxzn
