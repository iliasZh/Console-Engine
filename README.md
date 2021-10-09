# console-engine

A 2d console game engine in progress (64-bit Windows only)

### How to build
First of all, you need 64-bit Windows and an MSVC or Clang C++17 supporting compiler.
I didn\'t manage to make dependencies compile with MinGW GCC, so no GCC unfortunately.

This project uses CMake (3.21 minimum) and vcpkg. Assuming you have CMake installed and working:
* Install vcpkg (`C:\dev\vcpkg` or `C:\src\vcpkg` path is recommended) and run the bootstrap script:

	`git clone https://github.com/microsoft/vcpkg.git`

	`.\vcpkg\bootstrap-vcpkg.bat`
* Install project\'s dependencies (all libraries and MSVC runtime are linked statically, hence the `-static`\'s):

	`vcpkg install fmt:x64-windows-static`

	`vcpkg install yaml-cpp:x64-windows-static`

	I *could* use vcpkg\'s manifest mode, and I tried, but for some reason it does not support selecting static versions of packages.

If you are using CMake from the command line, make sure you're passing these flags:

`-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake`

`-DVCPKG_TARGET_TRIPLET=x64-windows-static`


If you are using the CMake Tools extension for VSCode (like me), add these options to your `settings.json`:
```json
"cmake.configureSettings": {
	"CMAKE_TOOLCHAIN_FILE": "C:\\dev\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake",
	"VCPKG_TARGET_TRIPLET": "x64-windows-static",
},
"cmake.debugConfig": {
	"console": "externalTerminal"
}
```
Then you can launch the project by clicking the debug button of the CMake Tools extension.
