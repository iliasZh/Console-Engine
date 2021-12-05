# console-engine

A 2d console game engine in progress (for 64-bit Windows 10)

### How to build
You'll need:
* Windows SDK.

* MSVC / Clang with support for C++20 flag (MinGW doesn't work with fmt for some reason - I only tried 10.3.0)

* CMake 3.21 or newer.

* vcpkg

All of the above except vcpkg comes with any recent version of Visual Studio 2022.

I build with latest Clang (from LLVM not from VS installation) and latest MSVC from VS2022.

Install vcpkg and then install fmt with
`vcpkg install fmt:x64-windows-static`

If you're using VS, just build the project with CMake, no configuration needed.

If you're running CMake manually, run it with `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake` AND `-DVCPKG_TARGET_TRIPLET=x64-windows-static`

If you are using the CMake Tools extension for VS Code, add these options to your `settings.json`:
```json
"cmake.debugConfig": {
	"console": "externalTerminal"
}
"cmake.configureSettings": {
		"CMAKE_TOOLCHAIN_FILE": "C:\\dev\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake",
		"VCPKG_TARGET_TRIPLET": "x64-windows-static",
	},
```
Then you can launch the project by clicking the debug button of the CMake Tools extension.

### Misc

Renaming project\'s folder ("console-engine") or creating folders with the same name inside may mess up custom exceptions\' info, but otherwise won't affect anything.
