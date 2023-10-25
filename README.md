# console-engine

A console graphics rendering engine - work in progress (for 64-bit Windows 10)

# How to build
You'll need:
* Windows SDK

* MSVC / Clang with support for C++20 flag (MinGW doesn't work with fmt for some reason - I only tried 10.3.0)

* CMake 3.26 or newer

All of the above comes with any recent version of Visual Studio 2022.

I build with latest Clang (from LLVM, not from VS installation) and latest MSVC from VS 2022.

If you're using VS, just build the project with CMake, no configuration needed.

# Misc

Renaming project\'s folder ("console-engine") or creating folders with the same name inside may mess up custom exceptions\' info, but otherwise won't affect anything.
