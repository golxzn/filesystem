#pragma once

#if !defined(GOLXZN_OS_FILESYSTEM)
#define GOLXZN_OS_FILESYSTEM
#endif // !defined(GOLXZN_OS_FILESYSTEM)

#include <span>
#include <string>
#include <memory>
#include <string_view>
#include <unordered_map>

#if defined(GOLXZN_OS_ALIASES)
#include <golxzn/os/aliases.hpp>
#endif // defined(GOLXZN_OS_ALIASES)

namespace golxzn::os {

#if !defined(GOLXZN_OS_ALIASES)

using u16 = uint16_t;
using byte = std::byte;
using size = std::size_t;

#endif // defined(GOLXZN_OS_ALIASES)

namespace details {

template<class T>
struct transparent_hash {
	using hash_type = std::hash<T>;
	using is_transparent = void;

	size operator()(const T &v) const noexcept { return hash_type{}(v); }
};

template<>
struct transparent_hash<std::wstring> {
	using char_type = std::wstring::value_type;
	using hash_type = std::hash<std::wstring_view>;
	using is_transparent = void;

	size operator()(const std::wstring &v)      const noexcept { return hash_type{}(v); }
	size operator()(const std::wstring_view &v) const noexcept { return hash_type{}(v); }
	size operator()(const char_type *const v)   const noexcept { return hash_type{}(v); }
};

} // namespace

/**
 * @brief Golxzn Resource Manager
 * @details Use this class to load resources from the program's directory.
 * @warning Every methods with string arguments instead of wstring cause a memory allocation.
 */
class filesystem final {
public:
	/** @brief Map of the protocol extensions and their prefixes */
	using associations_type = std::unordered_map<
		std::wstring, std::wstring, details::transparent_hash<std::wstring>, std::equal_to<>
	>;

	static constexpr std::wstring_view none{ L"" }; ///< Empty wide string
	static constexpr std::wstring::value_type separator{ L'/' }; ///< Path separator
	static constexpr std::wstring_view default_application_name{ L"unknown_application" }; ///< Default application name
	static constexpr std::wstring_view default_assets_directory_name{ L"assets" }; ///< Default assets directory name
	static constexpr std::wstring_view protocol_separator{ L"://" }; ///< Protocol separator

	static constexpr std::string_view none_narrow{ "" }; ///< Narrow version of golxzn::os::filesystem::none
	static constexpr std::string::value_type separator_narrow{ '/' }; ///< Narrow version of golxzn::os::filesystem::separator
	static constexpr std::string_view default_application_name_narrow{ "unknown_application" }; ///< Narrow version of golxzn::os::filesystem::default_application_name
	static constexpr std::string_view default_assets_directory_name_narrow{ "assets" }; ///< Narrow version of golxzn::os::filesystem::default_assets_directory_name
	static constexpr std::string_view protocol_separator_narrow{ "://" }; ///< Narrow version of golxzn::os::filesystem::protocol_separator



	/**
	 * @brief Struct returned by some methods to tell if there's an error
	 * @warning This struct has unexplicit conversion to bool! Use carefully!
	 */
	struct error {
		std::wstring message; ///< Error message. If it's golxzn::os::filesystem::none, everything is OK.

		/**
		 * @brief Checks if there's an error
		 *
		 * @return `true` - Something wrong (error message is not empty)
		 * @return `false` - All right (error message is empty)
		 */
		bool has_error() const noexcept;

		/**
		 * @brief Non-implicit conversion to bool
		 *
		 * @return `true` - All right. There's no error
		 * @return `false` - An error occurred
		 */
		operator bool() const noexcept;
	};
	static inline const error OK{ std::wstring{ none } }; ///< OK struct

	filesystem() = delete;

	/** @addtogroup initialization Initialization and setting up
	 * @{
	 */

	/**
	 * @brief Initialize the Resource Manager
	 *
	 * @param application_name Your application name or the name of the user assets directory
	 * @param assets_path The name of the assets directory or the full path to the directory.
	 * If there's [LETTER]:/ or / at the beginning of the string, the path will be used as it is.
	 * Otherwise the path is relative to the program's directory.
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error initialize(const std::wstring_view application_name,
		const std::wstring_view assets_path = default_assets_directory_name);

	/**
	 * @brief Sets the application name object
	 * @details Sets the name of the application which is used in the directory path for user data.
	 * If the name is empty, the default name will be used.
	 * @warning This method doesn't change the name of the user data directory!
	 * @param application_name application name
	 */
	static void set_application_name(const std::wstring_view application_name) noexcept;

	/**
	 * @brief Add the association between the protocol and the prefix. (ex. L"res://" and L"user://")
	 * @details Add custom protocol to your application. For example, you could have a L"ab-test://"
	 * association with your own separated assets directory.
	 * @warning L"res://", L"user://", L"temp://" are reserved and cannot be used.
	 * @param protocol Protocol extension (ex. "res://"). Has to end with "://"
	 * @param prefix Prefix of the path. Has to end with a slash!
	 */
	static void associate(std::wstring_view protocol, std::wstring &&prefix) noexcept;

	/** @} */

	/** @addtogroup read Reading files
	 * @{
	 */

	/**
	 * @brief Read whole binary file
	 *
	 * @warning This method throws an exception `std::invalid_argument` if the path has no protocol!
	 * @param path Path to the file
	 * @return `std::vector<byte>` - The data or an empty vector if there's an reading error.
	 */
	[[nodiscard]] static std::vector<byte> read_binary(const std::wstring_view path);

	/**
	 * @brief Read whole text file
	 *
	 * @warning This method throws an exception `std::invalid_argument` if the path has no protocol!
	 * @param path Path to the file
	 * @return `std::string` - The data or an empty string if there's an reading error.
	 */
	[[nodiscard]] static std::string read_text(const std::wstring_view path);

	/**
	 * @brief Construct @p Custom class by binary data from file
	 *
	 * @tparam Custom Class to construct. Has to have a constructor with std::vector<byte> argument
	 * @param path Path to the file
	 * @return `Custom` - Constructed class
	 */
	template<std::constructible_from<std::vector<byte>> Custom>
	[[nodiscard]] static Custom read_binary(const std::wstring_view path);

	/**
	 * @brief Construct @p Custom class by text data from file
	 *
	 * @tparam Custom Class to construct. Has to have a constructor with std::string argument
	 * @param path Path to the file
	 * @return `Custom` - Constructed class
	 */
	template<std::constructible_from<std::string> Custom>
	[[nodiscard]] static Custom read_text(const std::wstring_view path);

	/**
	 * @brief Construct @p std::shared_ptr<Custom> by binary data from file
	 *
	 * @tparam Custom Class to construct. Has to have a public constructor with std::vector<byte> argument
	 * @param path Path to the file
	 * @return `std::shared_ptr<Custom>` - Constructed shared class
	 */
	template<std::constructible_from<std::vector<byte>> Custom>
	[[nodiscard]] static std::shared_ptr<Custom> read_shared_binary(const std::wstring_view path);

	/**
	 * @brief Construct @p std::shared_ptr<Custom> by binary data from file
	 *
	 * @tparam Custom Class to construct. Has to have a public constructor with std::string argument
	 * @param path Path to the file
	 * @return `std::shared_ptr<Custom>` - Constructed shared class
	 */
	template<std::constructible_from<std::string> Custom>
	[[nodiscard]] static std::shared_ptr<Custom> read_shared_text(const std::wstring_view path);

	/**
	 * @brief Construct @p std::shared_ptr<Custom> by binary data from file
	 *
	 * @tparam Custom Class to construct. Has to have a public constructor with std::vector<byte> argument
	 * @param path Path to the file
	 * @return `std::unique_ptr<Custom>` - Constructed unique class
	 */
	template<std::constructible_from<std::vector<byte>> Custom>
	[[nodiscard]] static std::unique_ptr<Custom> read_unique_binary(const std::wstring_view path);

	/**
	 * @brief Construct @p std::shared_ptr<Custom> by binary data from file
	 *
	 * @tparam Custom Class to construct. Has to have a public constructor with std::string argument
	 * @param path Path to the file
	 * @return `std::unique_ptr<Custom>` - Constructed unique class
	 */
	template<std::constructible_from<std::string> Custom>
	[[nodiscard]] static std::unique_ptr<Custom> read_unique_text(const std::wstring_view path);

	/** @} */

	/** @addtogroup write Writing files
	 * @{
	 */

	/**
	 * @brief Write binary data to a file
	 *
	 * @param path Path to the file
	 * @param data Data to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error write_binary(const std::wstring_view path, const std::span<byte> &data);

	/**
	 * @brief Write binary data to a file
	 *
	 * @param path Path to the file
	 * @param data Data to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error write_binary(const std::wstring_view path, const std::initializer_list<byte> data);

	/**
	 * @brief Append binary data to a file
	 *
	 * @param path Path to the file
	 * @param data Data to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error append_binary(const std::wstring_view path, const std::span<byte> &data);

	/**
	 * @brief Append binary data to a file
	 *
	 * @param path Path to the file
	 * @param data Data to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error append_binary(const std::wstring_view path, const std::initializer_list<byte> data);

	/**
	 * @brief Write text data to a file
	 *
	 * @param path Path to the file
	 * @param text Text to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error write_text(const std::wstring_view path, const std::string_view text);

	/**
	 * @brief Append text data to a file
	 *
	 * @param path Path to the file
	 * @param text Text to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error append_text(const std::wstring_view path, const std::string_view text);

	/**
	 * @brief Write text data to a file
	 *
	 * @param path Path to the file
	 * @param text Text to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error write_text(const std::wstring_view path, const std::wstring_view text);

	/**
	 * @brief Append text data to a file
	 *
	 * @param path Path to the file
	 * @param text Text to write to the file
	 * @return golxzn::os::filesystem::error - The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error append_text(const std::wstring_view path, const std::wstring_view text);

	/** @} */

	/**
	 * @brief Get the association object
	 *
	 * @param protocol The protocol. Has to end with "://". If the prefix is not set, it returns an
	 * golxzn::os::filesystem::none
	 * @return `std::wstring_view` - Prefix or golxzn::os::filesystem::none
	 */
	[[nodiscard]] static std::wstring_view get_association(const std::wstring_view protocol) noexcept;

	/**
	 * @brief Get the application name
	 *
	 * @return `std::string_view` - application name
	 */
	[[nodiscard]] static std::wstring_view application_name() noexcept;

	/**
	 * @brief Get the user data directory object
	 * @details Same as golxzn::os::filesystem::get_association(L"user://")
	 * Gets the full path of the user data directory:
	 *  - __Windows__: `%USERPROFILE%/AppData/Roaming/<application name>`
	 *  - __MacOS__: `$HOME/Library/Application Support/<application name>`
	 *  - __Linux__: `$XDG_CONFIG_HOME/<application name>` or `$HOME/.config/<application name>`
	 *
	 * @return User directory or golxzn::os::filesystem::none if it's not set
	 */
	[[nodiscard]] static std::wstring_view user_data_directory() noexcept;

	/**
	 * @brief Get the assets directories
	 * @details Same as golxzn::os::filesystem::get_association(L"res://")
	 *
	 * @return assets directory or golxzn::os::filesystem::none if it's not set
	 */
	[[nodiscard]] static std::wstring_view assets_directory() noexcept;

	/**
	 * @brief Get the associations
	 *
	 * @return `const associations_type& associations` - Map of the protocol extensions and their prefixes
	 */
	[[nodiscard]] static const associations_type &associations() noexcept;

	/**
	 * @brief Join two paths with a slash.
	 *
	 * @param left Path that will be changed
	 * @param right Path to append
	 */
	static void join(std::wstring &left, std::wstring_view right) noexcept;

	/**
	 * @brief Join two paths with a slash.
	 *
	 * @param left left path
	 * @param right right path
	 * @return `std::wstring` - Joined path
	 */
	[[nodiscard]] static std::wstring join(std::wstring_view left, std::wstring_view right) noexcept;

	/**
	 * @brief Remove the last path component.
	 *
	 * @param path path
	 */
	static void parent_directory(std::wstring &path) noexcept;

	/**
	 * @brief Remove the last path component.
	 *
	 * @param path path
	 * @return `std::wstring` - Parent directory
	 */
	[[nodiscard]] static std::wstring parent_directory(std::wstring_view path) noexcept;

	/**
	 * @brief Fix the path separators to the '/' and remove the trailing slash.
	 *
	 * @param path std::wstring_view path
	 * @return `std::wstring` - Canonical path
	 */
	[[nodiscard]] static std::wstring normalize(std::wstring_view path);

	/** @addtogroup fdmanip Files and directories manipulation
	 * @{
	 */

	/**
	 * @brief Check if an entry exists.
	 *
	 * @param path directory or file path
	 * @return `true` if exists, `false` otherwise
	 */
	[[nodiscard]] static bool exists(const std::wstring_view path) noexcept;

	/**
	 * @brief Check if an entry is a file.
	 *
	 * @param path path to the file entry
	 * @return `true` if is file, `false` otherwise
	 */
	[[nodiscard]] static bool is_file(const std::wstring_view path);

	/**
	 * @brief Check if an entry is a directory.
	 *
	 * @param path path to the directory entry
	 * @return `true` if is directory, `false` otherwise
	 */
	[[nodiscard]] static bool is_directory(const std::wstring_view path);

	/**
	 * @brief Create a directory (recursively).
	 *
	 * @param path path to the directory
	 * @return golxzn::os::filesystem::error - filesystem::OK or the error message
	 */
	[[nodiscard]] static error make_directory(const std::wstring_view path);

	/**
	 * @brief Remove a directory (recursively).
	 *
	 * @param path path to the directory
	 * @return golxzn::os::filesystem::error - filesystem::OK or the error message
	 */
	[[nodiscard]] static error remove_directory(const std::wstring_view path);

	/**
	 * @brief Remove a file.
	 *
	 * @param path path to the file
	 * @return golxzn::os::filesystem::error - filesystem::OK or the error message
	 */
	[[nodiscard]] static error remove_file(const std::wstring_view path);

	/**
	 * @brief List an entry (file or directory).
	 *
	 * @param path path to the entry
	 * @return golxzn::os::filesystem::error - filesystem::OK or the error message
	 */
	[[nodiscard]] static error remove(const std::wstring_view path);

	// [[nodiscard]] static error move_file(const std::wstring_view path, const std::wstring_view destination);

	// [[nodiscard]] static error copy_file(const std::wstring_view path, const std::wstring_view destination);

	/** @} */

	/**
	 * @brief Get the absolute path of this application.
	 * @returns std::wstring Absolute path of the application or, in the failure case, a "./".
	 */
	[[nodiscard]] static std::wstring current_directory();

	/**
	 * @brief Get all entries in a directory.
	 *
	 * @param path path to the directory
	 * @return `std::vector<std::wstring>` - vector of entries
	 */
	[[nodiscard]] static std::vector<std::wstring> entries(const std::wstring_view path);

	/**
	 * @brief Convert a string to wide string
	 *
	 * @param str string to convert
	 * @return `std::wstring` - converted wide string
	 */
	[[nodiscard]] static std::wstring to_wide(const std::string_view str) noexcept;

	/**
	 * @brief Convert a wide string to string
	 *
	 * @param wstr wide string to convert
	 * @return `std::string` - converted string
	 */
	[[nodiscard]] static std::string to_narrow(const std::wstring_view wstr) noexcept;



	/** @addtogroup aliases Narrow string aliases
	 * @warning Methods with a narrow string arguments instead of wide string could cause a memory allocation.
	 * @{
	 */

	/// @brief Narrow string alias for golxzn::os::filesystem::initialize(const std::wstring_view, const std::wstring_view)
	[[nodiscard]] static error initialize(const std::string_view application_name,
		const std::string_view assets_path = default_assets_directory_name_narrow);

	/// @brief Narrow string alias for golxzn::os::filesystem::set_application_name(const std::wstring_view)
	static void set_application_name(const std::string_view application_name) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::associate(const std::wstring_view, const std::wstring_view)
	static void associate(const std::string_view protocol, const std::string_view prefix) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::read_binary(const std::wstring_view path)
	[[nodiscard]] static std::vector<byte> read_binary(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_text(const std::wstring_view path)
	[[nodiscard]] static std::string read_text(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_binary(const std::wstring_view path)
	template<std::constructible_from<std::vector<byte>> Custom>
	[[nodiscard]] static Custom read_binary(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_text(const std::wstring_view path)
	template<std::constructible_from<std::string> Custom>
	[[nodiscard]] static Custom read_text(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_shared_binary(const std::wstring_view path)
	template<std::constructible_from<std::vector<byte>> Custom>
	[[nodiscard]] static std::shared_ptr<Custom> read_shared_binary(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_shared_text(const std::wstring_view path)
	template<std::constructible_from<std::string> Custom>
	[[nodiscard]] static std::shared_ptr<Custom> read_shared_text(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_unique_binary(const std::wstring_view path)
	template<std::constructible_from<std::vector<byte>> Custom>
	[[nodiscard]] static std::unique_ptr<Custom> read_unique_binary(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::read_unique_text(const std::wstring_view path)
	template<std::constructible_from<std::string> Custom>
	[[nodiscard]] static std::unique_ptr<Custom> read_unique_text(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::write_binary(const std::wstring_view path, const std::span<byte> &data)
	[[nodiscard]] static error write_binary(const std::string_view path, const std::span<byte> &data);

	/// @brief Narrow string alias for golxzn::os::filesystem::write_binary(const std::wstring_view path, const std::initializer_list<byte> data)
	[[nodiscard]] static error write_binary(const std::string_view path, const std::initializer_list<byte> data);

	/// @brief Narrow string alias for golxzn::os::filesystem::append_binary(const std::wstring_view path, const std::span<byte> &data)
	[[nodiscard]] static error append_binary(const std::string_view path, const std::span<byte> &data);

	/// @brief Narrow string alias for golxzn::os::filesystem::append_binary(const std::wstring_view path, const std::initializer_list<byte> data)
	[[nodiscard]] static error append_binary(const std::string_view path, const std::initializer_list<byte> data);

	/// @brief Narrow string alias for golxzn::os::filesystem::write_text(const std::wstring_view path, const std::string_view text)
	[[nodiscard]] static error write_text(const std::string_view path, const std::string_view text);

	/// @brief Narrow string alias for golxzn::os::filesystem::append_text(const std::wstring_view path, const std::string_view text)
	[[nodiscard]] static error append_text(const std::string_view path, const std::string_view text);

	/// @brief Narrow string alias for golxzn::os::filesystem::write_text(const std::wstring_view path, const std::wstring_view text)
	[[nodiscard]] static error write_text(const std::string_view path, const std::wstring_view text);

	/// @brief Narrow string alias for golxzn::os::filesystem::append_text(const std::wstring_view path, const std::wstring_view text)
	[[nodiscard]] static error append_text(const std::string_view path, const std::wstring_view text);

	/// @brief Narrow string alias for golxzn::os::filesystem::get_association(const std::wstring_view protocol)
	[[nodiscard]] static std::wstring_view get_association(const std::string_view protocol) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::join(std::wstring &left, std::wstring_view right)
	static void join(std::string &left, std::string_view right) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::join(std::wstring_view left, std::wstring_view right)
	[[nodiscard]] static std::string join(std::string_view left, std::string_view right) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::parent_directory(std::wstring &path)
	static void parent_directory(std::string &path) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::parent_directory(std::wstring_view path)
	[[nodiscard]] static std::string parent_directory(std::string_view path) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::normalize(const std::wstring_view path)
	[[nodiscard]] static std::wstring normalize(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::exists(const std::wstring_view path)
	[[nodiscard]] static bool exists(const std::string_view path) noexcept;

	/// @brief Narrow string alias for golxzn::os::filesystem::is_file(const std::wstring_view path)
	[[nodiscard]] static bool is_file(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::is_directory(const std::wstring_view path)
	[[nodiscard]] static bool is_directory(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::make_directory(const std::wstring_view path)
	[[nodiscard]] static error make_directory(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::remove_directory(const std::wstring_view path)
	[[nodiscard]] static error remove_directory(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::remove_file(const std::wstring_view path)
	[[nodiscard]] static error remove_file(const std::string_view path);

	/// @brief Narrow string alias for golxzn::os::filesystem::remove(const std::wstring_view path)
	[[nodiscard]] static error remove(const std::string_view path);

	/// @brief Narrow stirng alias for golxzn::os::filesystem::entries(const std::wstring_view path)
	[[nodiscard]] static std::vector<std::string> entries(const std::string_view path);


	/** @} */

private:
	static std::wstring appname;
	static associations_type associations_map;

	static std::wstring_view get_protocol(const std::wstring_view path) noexcept;
	static std::wstring replace_association_prefix(std::wstring_view path) noexcept;
	static std::wstring setup_assets_directories(const std::wstring_view assets_path);
	static std::wstring setup_user_data_directory();
};

using fs = filesystem;

//======================================== Implementation ========================================//


template<std::constructible_from<std::vector<byte>> Custom>
Custom filesystem::read_binary(const std::wstring_view path) {
	return Custom{ read_binary(path) };
}
template<std::constructible_from<std::string> Custom>
Custom filesystem::read_text(const std::wstring_view path) {
	return Custom{ read_text(path) };
}

template<std::constructible_from<std::vector<byte>> Custom>
std::shared_ptr<Custom> filesystem::read_shared_binary(const std::wstring_view path) {
	return std::make_shared<Custom>(read_binary(path));
}

template<std::constructible_from<std::string> Custom>
std::shared_ptr<Custom> filesystem::read_shared_text(const std::wstring_view path) {
	return std::make_shared<Custom>(read_text(path));
}

template<std::constructible_from<std::vector<byte>> Custom>
std::unique_ptr<Custom> filesystem::read_unique_binary(const std::wstring_view path) {
	return std::make_unique<Custom>(read_binary(path));
}

template<std::constructible_from<std::string> Custom>
std::unique_ptr<Custom> filesystem::read_unique_text(const std::wstring_view path) {
	return std::make_unique<Custom>(read_text(path));
}


template<std::constructible_from<std::vector<byte>> Custom>
Custom filesystem::read_binary(const std::string_view path) {
	return Custom{ read_binary(path) };
}

template<std::constructible_from<std::string> Custom>
Custom filesystem::read_text(const std::string_view path) {
	return Custom{ read_text(path) };
}

template<std::constructible_from<std::vector<byte>> Custom>
std::shared_ptr<Custom> filesystem::read_shared_binary(const std::string_view path) {
	return std::make_shared<Custom>(read_binary(path));
}

template<std::constructible_from<std::string> Custom>
std::shared_ptr<Custom> filesystem::read_shared_text(const std::string_view path) {
	return std::make_shared<Custom>(read_text(path));
}

template<std::constructible_from<std::vector<byte>> Custom>
std::unique_ptr<Custom> filesystem::read_unique_binary(const std::string_view path) {
	return std::make_unique<Custom>(read_binary(path));
}

template<std::constructible_from<std::string> Custom>
std::unique_ptr<Custom> filesystem::read_unique_text(const std::string_view path) {
	return std::make_unique<Custom>(read_text(path));
}

} // namespace golxzn::os

namespace gxzn = golxzn;
