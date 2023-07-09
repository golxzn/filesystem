
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace golxzn::details {

std::wstring __unix_get_home() {
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

std::wstring current_directory() {
	if (std::string path(MAX_PATH, '\0'); getcwd(path.data(), path.size()) != nullptr) {
		return std::wstring{ std::begin(path), std::end(path) };
	}
	return L"./"
}

bool exists(const std::wstring_view path) {
	struct stat st;
	return stat(to_narrow(path).c_str(), &st) == 0;
}

bool is_file(const std::wstring_view path) {
	const auto narrow_path{ to_narrow(path) };
	if (struct stat st; stat(narrow_path.c_str(), &st) == 0) {
		return S_ISREG(st.st_mode);
	}
	return false;
}

bool is_directory(const std::wstring_view path) {
	const auto narrow_path{ to_narrow(path) };
	if (struct stat st; stat(narrow_path.c_str(), &st) == 0) {
		return S_ISDIR(st.st_mode);
	}
	return false;
}

std::vector<std::wstring> ls(const std::wstring_view path) {
	std::vector<std::wstring> entries;

	const auto dir_path{ resman::to_narrow(path) };
	if (auto dir{ opendir(dir_path.c_str()) }; dir != nullptr) {
		dirent* entry{ nullptr };
		while ((entry = readdir(dir)) != nullptr) {
			const std::string_view name{ entry->d_name };

			if (name != "." && name != "..") {
				entries.emplace_back(resman::to_wide(name));
			}
		}
		closedir(dir);
	}
	return entries;
}

bool mkdir(const std::wstring_view path) {
	return mkdir(resman::to_narrow(path).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

bool rmdir(const std::wstring_view path) {
	return rmdir(resman::to_narrow(path).c_str()) == 0;
}

bool rmfile(const std::wstring_view path) {
	return unlink(resman::to_narrow(path).c_str()) == 0;
}

} // namespace golxzn::details

