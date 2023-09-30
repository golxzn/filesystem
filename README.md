<h1 align="center">golxzn::os::filesystem</h1>
<div align="center">

![Tests](https://github.com/golxzn/filesystem/actions/workflows/ci-test-static-lib.yml/badge.svg)

</div>

`golxzn::filesystem` is a simple resource manager which provides basic access to application resources
and user directory.
`golxzn::filesystem` is dependenceless. There's only STL usage and platform specific headers.

## Platform support

✅ __Windows__ (tested); <br>
✅️ __Linux__ (tested); <br>
⚠️ __MacOS__ (in progress); <br>

I'm planning to add support for __Android__ and __iOS__ in the future.

## How does it work?

When you initialize this library using `golxzn::filesystem::initialize` method, library does 2 thing:

 1. Looks for `assets` or `res` directory starting from current application directory;
 2. Creates application directory

### Look for assets or res

This library goes up to the root directory and checks `assets` or `res` directory to set read-only
resources directory up

### Creates application directory

This library creates application directory for data which could be modified at:

* For Linux: `/home/you/.config/your_app_name`;
* For Windows: `%APPDATA%/your_app_name`;
* For MacOS: `/Users/you/Library/Application Support/your_app_name`;

## CMake integration

### 1. Clone this repository:

```bash
git clone https://github.com/golxzn/filesystem.git
```

### 2. Add subdirectory:

```cmake
add_subdirectory(filesystem)

target_link_libraries(YourProjectName PUBLIC golxzn::filesystem)
```

### 3. Include header & initialize:

```cpp
#include <golxzn/os/filesystem.hpp>

namespace your {

class Image {
public:
    explicit YourImage(std::vector<uint8_t> &&raw_image) {
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

