# console-engine

A 2d console game engine in progress (for 64-bit Windows 10)

### How to build
You'll need:
* Windows SDK.

* MSVC 16.11.7 or newer / Clang 13.0.0 or newer. My MinGW GCC 10.3.0 does not support the `cxx_std_23` option (see CMakeLists.txt on why C++23 instead of C++20), and I didn't bother to try a newer version, but it may work.

* CMake 3.21 or newer.

All of the above (except Clang and not sure about CMake) comes with any recent version of Visual Studio 2019/2022.

There are no additional dependencies yet.


If you got everything, just build the project with CMake.


If you are using the CMake Tools extension for VS Code (like me), add these options to your `settings.json`:
```json
"cmake.debugConfig": {
	"console": "externalTerminal"
}
```
Then you can launch the project as intended by clicking the debug button of the CMake Tools extension.

### Misc

Renaming project\'s folder ("console-engine") or creating folders with the same name inside may mess up custom exceptions\' info, but otherwise won't affect anything.
