
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif // !defined(WIN32_LEAN_AND_MEAN)

#include <windows.h>
#include <winerror.h>
#include <stringapiset.h>
#include <shlobj.h>

namespace golxzn::details {

std::wstring appdata_directory() {
	struct clean_path final {
		~clean_path() { CoTaskMemFree(ptr); }
		LPWSTR ptr{ nullptr };
	};

	clean_path path;
	const auto result{
		SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &path.ptr)
	};
	if (SUCCEEDED(result)) {
		const size_t length{ std::wcslen(path.ptr) };
		return std::wstring{ path.ptr, length };
	}
	return std::wstring{};
}

std::wstring cwd() {
	std::wstring path(MAX_PATH, L'\0');
	if (const size_t length{ GetCurrentDirectoryW(path.size(), path.data()) }; length != 0) {
		path.resize(length);
		return path;
	}
	return L"./";
}

bool exists(const std::wstring_view path) {
	const auto attributes{ GetFileAttributesW(path.data()) };
	return attributes != INVALID_FILE_ATTRIBUTES;
}

bool is_file(const std::wstring_view path) {
	const auto attributes{ GetFileAttributesW(path.data()) };
	return (attributes != INVALID_FILE_ATTRIBUTES)
		&& ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

bool is_directory(const std::wstring_view path) {
	const auto attributes{ GetFileAttributesW(path.data()) };
	return (attributes != INVALID_FILE_ATTRIBUTES)
		&& ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

std::vector<std::wstring> ls(const std::wstring_view path) {
	static constexpr std::wstring_view current_directory{ L"." };
	static constexpr std::wstring_view parent_directory{ L".." };


	std::vector<std::wstring> entries;

	const auto pattern{ std::format(L"{}\\*", path) };
	WIN32_FIND_DATAW found;
	HANDLE iterator{ FindFirstFileW(pattern.data(), &found) };

	if (iterator != INVALID_HANDLE_VALUE) {
		do {
			const std::wstring_view name{ found.cFileName };

			// Exclude "." and ".." entries
			if (name != current_directory && name != parent_directory) {
				entries.emplace_back(name);
			}
		} while (FindNextFileW(iterator, &found));

		FindClose(iterator);
	}
	return entries;
}

bool mkdir(const std::wstring_view path) {
	return CreateDirectoryW(path.data(), nullptr) != FALSE;
}

bool rmdir(const std::wstring_view path) {
	return RemoveDirectoryW(path.data()) != FALSE;
}

bool rmfile(const std::wstring_view path) {
	return DeleteFileW(path.data()) != FALSE;
}

} // namespace golxzn::details
