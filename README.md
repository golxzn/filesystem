<h1 align="center">golxzn::os::filesystem</h1>
<div align="center">

[![Windows](https://github.com/golxzn/filesystem/actions/workflows/ci-static-lib-windows.yml/badge.svg)][ci-windows]
[![Ubuntu](https://github.com/golxzn/filesystem/actions/workflows/ci-static-lib-ubuntu.yml/badge.svg)][ci-ubuntu]
[![MacOS](https://github.com/golxzn/filesystem/actions/workflows/ci-static-lib-macos.yml/badge.svg)][ci-macos]

</div>

`golxzn::os::filesystem` is a simple resource manager which provides basic access to application resources
and user directory.
`golxzn::os::filesystem` is dependenceless. There's only STL usage and platform specific headers.

<h2><b>How does it work?</b></h2>

When you initialize this library using `golxzn::os::filesystem::initialize` method, library does 2 thing:

 1. Looks for `assets` or `res` directory starting from current application directory;
 2. Creates application directory

<h3>Look for assets or res</h3>

This library goes up to the root directory and checks `assets` or `res` directory to set read-only
resources directory up

<h3>Creates application directory</h3>

This library creates application directory for data which could be modified at:

* For Linux: `/home/you/.config/your_app_name`;
* For Windows: `%APPDATA%/your_app_name`;
* For MacOS: `/Users/you/Library/Application Support/your_app_name`;

<h2><b>CMake integration</b><h2>

<h3>1. Clone this repository</h3>

```bash
git clone https://github.com/golxzn/filesystem.git
```

<h3>2. Add subdirectory</h3>

```cmake
add_subdirectory(filesystem)

target_link_libraries(YourProjectName PUBLIC golxzn::os::filesystem)
```

<h3>3. Include header & initialize</h3>

```cpp
#include <golxzn/os/filesystem.hpp>

namespace your {

class Image {
public:
    explicit YourImage(std::vector<gxzn::os::byte> &&raw_image) {
        // Parse your image
    }
    ...
};

class Settings {
public:
    explicit YourSettings(std::string &&content) {
        // Parse your settings
    }
    ...
};

} // namespace your

int main() {
    golxzn::os::filesystem::initialize("your_app_name");

    auto app_settings{ gxzn::os::fs::load_text<your::Settings>("res://settings/application.ini") };

    auto splash_screen{ ///< std::shared_ptr<your::Image>
        gxzn::os::fs::load_shared_binary<your::Image>(app_settings.get_splash_screen_path())
    };
}

```

You could use either a `golxzn::os::filesystem` namespace or just `gxzn::os::fs`:

```cpp
golxzn::os::filesystem::initialize("your_app_name");
gxzn::os::fs::initialize("your_app_name");
```

<hr>

<h2>Thanks</h2>

Documentations is powered by:

- [💪 Doxygen][doxygen-link]
- [💘 doxygen-awesome-css][doxygen-awesome-css-link].

[ci-windows]: https://github.com/golxzn/filesystem/actions/workflows/ci-static-lib-windows.yml
[ci-ubuntu]: https://github.com/golxzn/filesystem/actions/workflows/ci-static-lib-ubuntu.yml
[ci-macos]: https://github.com/golxzn/filesystem/actions/workflows/ci-static-lib-macos.yml

[doxygen-link]: https://www.doxygen.nl/
[doxygen-awesome-css-link]: https://github.com/jothepro/doxygen-awesome-css

