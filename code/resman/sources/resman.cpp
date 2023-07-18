#include <deque>
#include <format>
#include <ranges>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <fstream>
#include <algorithm>

#include "golxzn/resman.hpp"


#if defined(GRM_WINDOWS)
# include "platform/win.inl"
#elif defined(GRM_LINUX)
# include "platform/linux.inl"
#elif defined(GRM_MACOS)
# include "platform/macos.inl"
#else
# error "Unsupported platform"
#endif // defined(GRM_WINDOWS)


namespace golxzn {

namespace details {

using binary_ofstream = std::basic_ofstream<uint8_t>;
using binary_ifstream = std::basic_ifstream<uint8_t>;

resman::error write_binary_data(const std::wstring_view path,
		const uint8_t *data, const size_t len,
		const std::ios::openmode mode = std::ios::out) noexcept {

	if (len == 0) [[unlikely]] { return resman::OK; }

	try {

		if (binary_ofstream file{ path.data(), mode | std::ios::binary }; file.is_open()) {
			file.write(data, len);
			if (file.good()) [[likely]] {
				return resman::OK;
			}

			return resman::error{ std::format(L"Failed to write to file '{}'", path) };
		}

	} catch(const std::exception &ex) {
		return resman::error{ std::format(L"Failed to write to file '{}' due to exception '{}'",
			path, resman::to_wide(ex.what())
		) };
	} catch(...) {
		return resman::error{ std::format(L"Failed to write to file '{}' due to unknown exception",
			path
		) };
	}

	return resman::error{ std::format(L"Failed to open file '{}' for writing", path) };
}

} // namespace details

resman::associations_type resman::associations_map{};
std::wstring resman::appname{ resman::default_application_name };


//========================================= resman::error ========================================//


bool resman::error::has_error() const noexcept { return !message.empty(); }

resman::error::operator bool() const noexcept { return !has_error(); }


//======================================== resman::public ========================================//


resman::error resman::initialize(const std::wstring_view app_name, const std::wstring_view assets_path) {
	set_application_name(app_name);

	error err{ OK };
	if (auto assets_dir{ setup_assets_directories(assets_path) }; !assets_dir.empty()) [[likely]] {
		associate(L"res://", std::wstring{ assets_dir });
		associate(L"assets://", std::move(assets_dir));
	} else {
		err.message = L"Failed to setup assets directories";
	}

	if (auto user_dir{ setup_user_data_directory() }; !user_dir.empty()) [[likely]] {
		associate(L"user://", std::wstring{ user_dir });
		associate(L"temp://", std::move(user_dir) + L"/temp");
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

void resman::associate(const std::wstring_view protocol_view, std::wstring &&prefix) noexcept {
	if (protocol_view.empty()) [[unlikely]] return;

	std::wstring protocol{ protocol_view };
	if (!protocol_view.ends_with(protocol_separator)) [[unlikely]] {
		protocol += protocol_separator;
	}

	associations_map.insert_or_assign(std::move(protocol), std::move(prefix));
}

std::vector<uint8_t> resman::read_binary(const std::wstring_view path) {
	if (path.find(protocol_separator) == std::wstring_view::npos) [[unlikely]] {
		throw std::invalid_argument{ std::format(
			"[resman::read_binary] Protocol prefix expected in the path: '{}'", to_narrow(path)
		) };
	}

	using bifstream = std::basic_ifstream<uint8_t, std::char_traits<uint8_t>>;

	const auto wide_path{ replace_association_prefix(path) };
	if (bifstream file{ wide_path, std::ios::binary | std::ios::ate }; file.is_open()) [[likely]] {
		std::vector<uint8_t> content(file.tellg());
		file.seekg(std::ios::beg);
		file.read(content.data(), content.size());
		return content;
	}

	return {};
}

resman::error resman::write_binary(const std::wstring_view path, const std::span<uint8_t> &data) {
	if (path.find(protocol_separator) == std::wstring_view::npos) [[unlikely]] {
		return error{
			std::format(L"[resman::write_binary] Protocol prefix expected in the path: '{}'", path)
		};
	}

	if (auto status{ make_directory(parent_directory(path)) }; status.has_error()) {
		status.message += std::format(L" (During creating parent directory for writing file '{}')",
			path
		);
		return status;
	}

	return details::write_binary_data(replace_association_prefix(path), data.data(), data.size());
}

resman::error resman::write_binary(const std::wstring_view path, const std::initializer_list<uint8_t> data) {
	if (path.find(protocol_separator) == std::wstring_view::npos) [[unlikely]] {
		return error{
			std::format(L"[resman::write_binary] Protocol prefix expected in the path: '{}'", path)
		};
	}

	if (auto status{ make_directory(parent_directory(path)) }; status.has_error()) {
		status.message += std::format(L" (During creating parent directory for writing file '{}')",
			path
		);
		return status;
	}

	return details::write_binary_data(replace_association_prefix(path), data.begin(), data.size());
}

resman::error resman::append_binary(const std::wstring_view path, const std::span<uint8_t> &data) {
	if (path.find(protocol_separator) == std::wstring_view::npos) [[unlikely]] {
		return error{
			std::format(L"[resman::write_binary] Protocol prefix expected in the path: '{}'", path)
		};
	}

	if (auto status{ make_directory(parent_directory(path)) }; status.has_error()) {
		status.message += std::format(L" (During creating parent directory for writing file '{}')",
			path
		);
		return status;
	}

	return details::write_binary_data(replace_association_prefix(path), data.data(), data.size(),
		std::ios::app);
}

resman::error resman::append_binary(const std::wstring_view path, const std::initializer_list<uint8_t> data) {
	if (path.find(protocol_separator) == std::wstring_view::npos) [[unlikely]] {
		return error{
			std::format(L"[resman::write_binary] Protocol prefix expected in the path: '{}'", path)
		};
	}

	if (auto status{ make_directory(parent_directory(path)) }; status.has_error()) {
		status.message += std::format(L" (During creating parent directory for writing file '{}')",
			path
		);
		return status;
	}

	return details::write_binary_data(replace_association_prefix(path), data.begin(), data.size(),
		std::ios::app);
}

std::wstring_view resman::get_association(const std::wstring_view protocol) noexcept {
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

void resman::join(std::wstring &left, std::wstring_view right) noexcept {
	if (left.empty() || right.empty()) [[unlikely]] return;
	if (const auto last{ left.back() }; last != separator || last == L'\\') {
		if (last == L'\\') [[unlikely]] left.pop_back();
		left += separator;
	}
	if (right.front() == separator || right.front() == L'\\') [[unlikely]] {
		right.remove_prefix(1);
	}

	if (right.empty()) [[unlikely]] return;

	left.reserve(left.size() + right.size());
	left += right;
}

std::wstring resman::join(std::wstring_view left, std::wstring_view right) noexcept {
	static const auto is_separator = [](const auto c) noexcept {
		if (c.size() > 1) return false;
		return c.front() == separator || c.front() == L'\\';
	};

	if (right.empty() || is_separator(right)) [[unlikely]] return std::wstring{ left };
	if (left.empty() || is_separator(left)) [[unlikely]] return std::wstring{ right };

	if (left.back()   == separator || left.back()   == L'\\') left.remove_suffix(1);
	if (right.front() == separator || right.front() == L'\\') right.remove_prefix(1);

	return std::format(L"{}/{}", left, right);
}

void resman::parent_directory(std::wstring &path) noexcept {
	if (const auto last_slash{ path.find_last_of(L"/\\") }; last_slash != std::string::npos) [[likely]] {
		path.resize(last_slash + 1);
	} else {
		path.clear();
	}
}

std::wstring resman::parent_directory(const std::wstring_view path) noexcept {
	if (const auto last_slash{ path.find_last_of(L"/\\") }; last_slash != std::wstring::npos) {
		return std::wstring{ path.substr(0, last_slash + 1) };
	}
	return std::wstring{};
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
		prefix += separator;
		str.remove_prefix(2);
	} else {
		prefix = separator;
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
		if (!part.empty()) [[likely]] {
			join(result, std::move(part));
		}
	}

	return result;
}

bool resman::exists(const std::wstring_view path) noexcept {
	if (path.empty()) return false;

	return details::exists(replace_association_prefix(path));
}

bool resman::is_file(const std::wstring_view path) {
	if (path.empty()) return false;

	return details::is_file(replace_association_prefix(path));
}

bool resman::is_directory(const std::wstring_view path) {
	if (path.empty()) return false;

	return details::is_directory(replace_association_prefix(path));
}

resman::error resman::make_directory(const std::wstring_view path) {
	if (path.empty()) {
		return error{ L"Empty path" };
	}
	if (is_directory(path)) return OK;
	if (is_file(path)) {
		return error{ std::format(L"Path is a file: '{}'", path) };
	}

	const auto full_path{ replace_association_prefix(path) };

	std::deque<std::wstring> parts;

	std::wstring root{ full_path };
	while (!root.empty() && !details::is_directory(root)) {
		parts.emplace_front(root.substr(root.find_last_of(separator) + 1));
		parent_directory(root);
	}

	for (auto &&part : parts) {
		if (join(root, part); !details::mkdir(root)) {
			return error{ L"Cannot create directory: " + root };
		}
	}

	return OK;
}

resman::error resman::remove_directory(const std::wstring_view path) {
	if (path.empty()) {
		return error{ L"Empty path" };
	}
	if (!exists(path)) return OK;

	if (!is_directory(path)) {
		return error{ std::format(L"Not a directory: '{}'", path) };
	}

	const auto full_path{ replace_association_prefix(path) };

	for (auto &&entry : details::ls(full_path)) {
		const auto entry_path{ join(path, entry) };
		error status{ OK };
		if (is_directory(entry_path)) {
			status = remove_directory(entry_path);
		} else {
			status = remove_file(entry_path);
		}

		if (status.has_error()) [[unlikely]] { return status; }
	}
	if (!details::rmdir(full_path)) {
		return error{ std::format(L"Cannot remove directory: '{}'", path) };
	}

	return OK;
}

resman::error resman::remove_file(const std::wstring_view path) {
	if (path.empty()) {
		return error{ L"Empty path" };
	}
	if (!is_file(path)) return OK;

	const auto full_path{ replace_association_prefix(path) };

	if (!details::rmfile(full_path)) {
		return error{ std::format(L"Cannot remove file: '{}'", path) };
	}

	return OK;
}

resman::error resman::remove(const std::wstring_view path) {
	if (path.empty()) {
		return error{ L"Empty path" };
	}

	if (!exists(path)) return OK;

	if (is_directory(path)) {
		return remove_directory(path);
	}

	return remove_file(path);
}

std::wstring resman::current_directory() {
	return normalize(details::cwd());
};

std::vector<std::wstring> resman::entries(const std::wstring_view path) {
	if (!is_directory(path)) return {};

	const auto full_path{ replace_association_prefix(path) };
	const auto association{ get_association(get_protocol(path)) };

	auto paths{ details::ls(full_path) };
	for (auto &path : paths) {
		path = join(association, path);
	}
	return paths;
}

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

void resman::associate(const std::string_view protocol_view, const std::string_view prefix) noexcept {
	associate(to_wide(protocol_view), to_wide(prefix));
}

std::vector<uint8_t> resman::read_binary(const std::string_view path) {
	return read_binary(to_wide(path));
}

resman::error resman::write_binary(const std::string_view path, const std::span<uint8_t> &data) {
	return write_binary(to_wide(path), data);
}

resman::error resman::write_binary(const std::string_view path, const std::initializer_list<uint8_t> data) {
	return write_binary(to_wide(path), data);
}

resman::error resman::append_binary(const std::string_view path, const std::span<uint8_t> &data) {
	return append_binary(to_wide(path), data);
}

resman::error resman::append_binary(const std::string_view path, const std::initializer_list<uint8_t> data) {
	return append_binary(to_wide(path), data);
}


std::wstring_view resman::get_association(const std::string_view protocol) noexcept {
	return get_association(to_wide(protocol));
}

void resman::join(std::string &left, std::string_view right) noexcept {
	if (const auto last{ left.back() }; last != '/' || last == '\\') {
		if (last == '\\') [[unlikely]] left.pop_back();
		left += '/';
	}
	if (right.front() == '/' || right.front() == '\\') [[unlikely]] {
		right.remove_prefix(1);
	}

	if (right.empty()) [[unlikely]] return;

	left.reserve(left.size() + right.size());
	left += right;
}

std::string resman::join(std::string_view left, std::string_view right) noexcept {
	static const auto is_separator = [](const auto c) noexcept {
		if (c.size() > 1) return false;
		return c.front() == separator_narrow || c.front() == '\\';
	};

	if (right.empty() || is_separator(right)) [[unlikely]] return std::string{ left };
	if (left.empty() || is_separator(left)) [[unlikely]] return std::string{ right };

	if (left.back()   == separator_narrow || left.back()   == '\\') left.remove_suffix(1);
	if (right.front() == separator_narrow || right.front() == '\\') right.remove_prefix(1);

	return std::format("{}/{}", left, right);
}

void resman::parent_directory(std::string &path) noexcept {
	if (const auto last_slash{ path.find_last_of("/\\") }; last_slash != std::string::npos) [[likely]] {
		path.resize(last_slash);
	} else {
		path.clear();
	}
}

std::string resman::parent_directory(const std::string_view path) noexcept {
	if (const auto last_slash{ path.find_last_of("/\\") }; last_slash != std::string::npos) [[likely]] {
		return std::string{ path.substr(0, last_slash) };
	}
	return std::string{};
}

std::wstring resman::normalize(const std::string_view str) {
	return normalize(to_wide(str));
}

bool resman::exists(const std::string_view path) noexcept {
	return exists(to_wide(path));
}

bool resman::is_file(const std::string_view path) {
	return is_file(to_wide(path));
}

bool resman::is_directory(const std::string_view path) {
	return is_directory(to_wide(path));
}

resman::error resman::make_directory(const std::string_view path) {
	return make_directory(to_wide(path));
}

resman::error resman::remove_directory(const std::string_view path) {
	return remove_directory(to_wide(path));
}

resman::error resman::remove_file(const std::string_view path) {
	return remove_file(to_wide(path));
}

resman::error resman::remove(const std::string_view path) {
	return remove(to_wide(path));
}

std::vector<std::string> resman::entries(const std::string_view path) {
	if (!is_directory(path)) return {};

	const auto wide_path{ to_wide(path) };
	const auto full_path{ replace_association_prefix(wide_path) };
	const auto association{ get_association(get_protocol(wide_path)) };

	std::vector<std::string> result;
	const auto paths{ details::ls(full_path) };
	result.reserve(paths.size());
	for (const auto &path : paths) {
		result.emplace_back(to_narrow(join(association, path)));
	}
	return result;
}


//======================================== resman::private =======================================//


std::wstring_view resman::get_protocol(const std::wstring_view path) noexcept {
	if (auto found{ path.find(protocol_separator) }; found != std::wstring_view::npos) {
		return path.substr(0, found + protocol_separator.size());
	}
	return L"";
}

std::wstring resman::replace_association_prefix(std::wstring_view path) noexcept {
	if (const auto protocol{ get_protocol(path) }; !protocol.empty()) {
		if (protocol == path) return std::wstring{ get_association(protocol) };

		if (const auto prefix{ get_association(protocol) }; prefix != none) {
			path.remove_prefix(protocol.size());
			return normalize(join(prefix, path));
		}
	}

	return normalize(path);
}

std::wstring resman::setup_assets_directories(const std::wstring_view assets_path) {
	if (assets_path.starts_with(separator) || assets_path.find(L":") == 1) {
		return normalize(assets_path);
	}

	const auto cwd{ details::cwd() };
	return normalize(join(cwd, assets_path));
}

std::wstring resman::setup_user_data_directory() {
	if (auto dir{ details::appdata_directory() }; !dir.empty()) {
		return normalize(join(std::wstring_view{ dir }, application_name()));
	}
	return appname;
}


} // namespace golxzn
