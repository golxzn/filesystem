#pragma once

#include <deque>
#include <string>
#include <string_view>

namespace golxzn {

class resman final {
	using string_view_list = std::initializer_list<std::string_view>;
public:
	static constexpr char separator{ '/' };
	static constexpr std::string_view default_application_name{ "unknown_application" };
	static constexpr string_view_list default_assets_directory_names{
		"assets",
		"resources"
		"res",
	};

	struct error {
		std::string message;

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
	 * @details This method does two thing:
	 * 1. Looks for the provided assets directories.
	 * 2. Creates a directory for the application user data.
	 *
	 * @param application_name Your application name or the name of the user assets directory
	 * @param assets_names List of the assets directories names to search for
	 * @return error The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error initialize(const std::string_view application_name,
		const string_view_list assets_names = default_assets_directory_names);

	/**
	 * @brief Initialize the Resource Manager
	 * @details @see initialize(std::string_view application_name, string_view_list assets_names)
	 *
	 * @param application_name Your application name or the name of the user assets directory
	 * @param assets_name The name of the assets directory to search for
	 * @return error The error message or an empty string if there's no error
	 */
	[[nodiscard]] static error initialize(const std::string_view application_name,
		const std::string_view assets_name);
	/**
	 * @brief Sets the application name object
	 * @details Sets the name of the application which is used in the directory path for user data.
	 * If the name is empty, the default name will be used.
	 * @warning This method doesn't change the name of the user data directory!
	 * @param application_name application name
	 */
	static void set_application_name(const std::string_view application_name) noexcept;

	/**
	 * @brief Get the application name
	 *
	 * @return std::string_view application name
	 */
	[[nodiscard]] static std::string_view application_name() noexcept;

	/**
	 * @brief Get the user data directory object
	 * @details Gets the full path of the user data directory:
	 *  - Windows: `%USERPROFILE%/AppData/Roaming/<application name>`
	 *  - MacOS: `$HOME/Library/Application Support/<application name>`
	 *  - Linux: `$XDG_CONFIG_HOME/<application name>` or `$HOME/.config/<application name>`
	 * @return std::string user data directory path
	 */
	[[nodiscard]] static std::wstring_view user_data_directory() noexcept;

	/**
	 * @brief Get the assets directories
	 *
	 * @return std::vector<std::string> Assets directories paths
	 */
	[[nodiscard]] static const std::deque<std::string> &assets_directory() noexcept;

	/**
	 * @brief Fix the path separators to the '/' and remove the trailing slash.
	 *
	 * @warning This method copies the path to the internal buffer!
	 * If you need to use it many times, it is better to use the
	 * @see golxzn::resman::normalize(std::wstring_view path) method.
	 *
	 * @param path std::string_view path
	 * @return std::wstring canonical path
	 */
	static std::wstring normalize(const std::string_view path);

	/**
	 * @brief Fix the path separators to the '/' and remove the trailing slash.
	 *
	 * @param path std::wstring_view path
	 * @return std::wstring canonical path
	 */
	static std::wstring normalize(std::wstring_view path);

private:
	static std::string appname;
	static std::wstring user_data_dir;
	static std::deque<std::string> assets_dirs;

	static bool setup_assets_directories(const string_view_list assets_names);
	static bool setup_user_data_directory();

	static void normalize(std::wstring &path) noexcept;
};

namespace resources { using manager = resman; }

} // namespace golxzn

namespace gxzn = golxzn;
