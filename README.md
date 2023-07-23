<h1 align="center">golxzn::resman</h1>

`golxzn::resman` is a simple resource manager which provides basic access to application resources
and user directory.
`golxzn::resman` is dependenceless. There's only STL usage and platform specific headers.

## Platform support

✅ __Windows__ (tested); <br>
✅️ __Linux__ (tested); <br>
⚠️ __MacOS__ (in progress); <br>

I'm planning to add support for __Android__ and __iOS__ in the future.

## How does it work?

When you initialize this library using `golxzn::resman::initialize` method, library does 2 thing:

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
git clone https://github.com/golxzn/resman.git
```

### 2. Add subdirectory:

```cmake
add_subdirectory(resman)

target_link_libraries(YourProjectName PUBLIC golxzn::resman)
```

### 3. Include header & initialize:

```cpp
#include <golxzn/resman.hpp>

int main() {
	golxzn::resman::initialize("your_app_name");

	if (auto content{ gxzn::resman::load("res://config.bin") }; !content.empty()) {
		SomeClass sc{ std::move(content) };
	}
}

```

You could use either a `golxzn::resman` namespace or just `gxzn::resman` or `golxzn::resources::manager`:

```cpp
golxzn::resources::manager::initialize("your_app_name");
gxzn::resman::initialize("your_app_name");
```

