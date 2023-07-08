#pragma once

#include <deque>
#include <string>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace golxzn {

namespace details {

template<class T>
struct transparent_hash {
	using hash_type = std::hash<T>;
	using is_transparent = void;

	std::size_t operator()(const T &v) const noexcept { return hash_type{}(v); }
};

template<>
struct transparent_hash<std::string> {
	using hash_type = std::hash<std::string_view>;
	using is_transparent = void;

	std::size_t operator()(const std::string &v)      const noexcept { return hash_type{}(v); }
	std::size_t operator()(const std::string_view &v) const noexcept { return hash_type{}(v); }
	std::size_t operator()(const char *const v)       const noexcept { return hash_type{}(v); }
};

} // namespace

/**
 * @brief Golxzn Resource Manager
 * @details Use this class to load resources from the program's directory.
 * @warning Every methods with string arguments instead of wstring cause a memory allocation.
 */
class resman final {
public:
	using associations_type = std::unordered_map<
		std::string, std::wstring, details::transparent_hash<std::string>, std::equal_to<>
	>;

	static constexpr std::wstring_view none{ L"" };
	static constexpr std::wstring::value_type separator{ L'/' };
	static constexpr std::wstring_view default_application_name{ L"unknown_application" };
	static constexpr std::wstring_view default_assets_directory_name{ L"assets" };
	static constexpr std::string_view protocol_separator{ "://" };

	/**
	 * @brief Struct returned by some methods to tell if there's an error
	 * @warning This struct has unexplicit conversion to bool! Use carefully!
	 */
	struct error {
		std::wstring message;

		/**
		 * @brief Checks if there's an error
		 *
		 * @return true Something wrong (error message is not empty)
		 * @return false All right (error message is empty)
		 */
		bool has_error() const noexcept;

		/**
		 * @brief Non-implicit conversion to bool
		 *
		 * @return true All right. There's no error
		 * @return false An error occurred
		 */
		operator bool() const noexcept;
	};

	resman() = delete;

	/**
	 * @brief Initialize the Resource Manager
	 *
	 * @param application_name Your application name or the name of the user assets directory
	 * @param assets_path The name of the assets directory or the full path to the directory.
	 * If there's [LETTER]:/ or / at the beginning of the string, the path will be used as it is.
	 * Otherwise the path is relative to the program's directory.
	 * @return error The error message or an empty string if there's no error
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
	 * @brief Add the association between the protocol and the prefix. (ex. "res://" and "user://")
	 *
	 * @param extension Protocol extension (ex. "res://"). Has to end with "://"
	 * @param prefix Prefix of the path. Has to end with a slash!
	 */
	static void associate(std::string_view protocol, std::wstring &&prefix) noexcept;

	/**
	 * @brief Get the association object
	 *
	 * @param protocol The protocol. Has to end with "://". If the prefix is not set, it returns an
	 * @ref golxzn::resman::none
	 * @return prefix
	 */
	static std::wstring_view get_association(const std::string_view protocol) noexcept;

	/**
	 * @brief Get the application name
	 *
	 * @return std::string_view application name
	 */
	[[nodiscard]] static std::wstring_view application_name() noexcept;

	/**
	 * @brief Get the user data directory object
	 * @details Same as @ref golxzn::resman::get_association("user://")
	 * Gets the full path of the user data directory:
	 *  - Windows: `%USERPROFILE%/AppData/Roaming/<application name>`
	 *  - MacOS: `$HOME/Library/Application Support/<application name>`
	 *  - Linux: `$XDG_CONFIG_HOME/<application name>` or `$HOME/.config/<application name>`
	 *
	 * @return user directory or @ref golxzn::resman::none if it's not set
	 */
	[[nodiscard]] static std::wstring_view user_data_directory() noexcept;

	/**
	 * @brief Get the assets directories
	 * @details Same as @ref golxzn::resman::get_association("res://")
	 *
	 * @return assets directory or @ref golxzn::resman::none if it's not set
	 */
	[[nodiscard]] static std::wstring_view assets_directory() noexcept;

	/**
	 * @brief Get the associations
	 *
	 * @return const associations_type& associations
	 */
	[[nodiscard]] static const associations_type &associations() noexcept;

	/**
	 * @brief Fix the path separators to the '/' and remove the trailing slash.
	 *
	 * @param path std::wstring_view path
	 * @return std::wstring canonical path
	 */
	[[nodiscard]] static std::wstring normalize(std::wstring_view path);

	/**
	 * @brief Get the absolute path of this application.
	 * @returns std::wstring Absolute path of the application or, in the failure case, a "./".
	 */
	[[nodiscard]] static std::wstring current_directory();

	/**
	 * @brief Convert a string to wide string
	 *
	 * @param str string to convert
	 * @return std::wstring converted wide string
	 */
	[[nodiscard]] static std::wstring to_wide(const std::string_view str) noexcept;

	/**
	 * @brief Convert a wide string to string
	 *
	 * @param wstr wide string to convert
	 * @return std::string converted string
	 */
	[[nodiscard]] static std::string to_narrow(const std::wstring_view wstr) noexcept;

	/**
	 * @addtogroup Narrow string aliases
	 * @warning Every methods with string arguments instead of wstring cause a memory allocation.
	 * @{
	 */

	/**
	 * @brief Alias for wide version of golxzn::resman::initialize
	 * @details @see golxzn::resman::initialize(std::wstring_view application_name, std::wstring_view assets_path)
	 *
	 * @param application_name Your application name or the name of the user assets directory
	 * @param assets_path The name of the assets directory to search for
	 * @return error The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error initialize(const std::string_view application_name,
		const std::string_view assets_path);

	/**
	 * @brief Narrow string alias for set_application_name
	 * @details Sets the name of the application which is used in the directory path for user data.
	 * If the name is empty, the default name will be used.
	 * @warning This method doesn't change the name of the user data directory!
	 * @param application_name application name
	 */
	static void set_application_name(const std::string_view application_name) noexcept;

	/**
	 * @brief Fix the path separators to the '/' and remove the trailing slash.
	 * @see golxzn::resman::normalize(std::wstring_view path) method.
	 *
	 * @param path std::string_view path
	 * @return std::wstring canonical path
	 */
	[[nodiscard]] static std::wstring normalize(const std::string_view path);


	/** @} */

private:
	static std::wstring appname;
	static associations_type associations_map;

	static std::wstring setup_assets_directories(const std::wstring_view assets_path);
	static std::wstring setup_user_data_directory();
};

namespace resources { using manager = resman; }

} // namespace golxzn

namespace gxzn = golxzn;
