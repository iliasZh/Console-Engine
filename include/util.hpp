#pragma once

#include "windows.hpp"

namespace util
{
[[nodiscard]] inline HANDLE std_out()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}
} // namespace util
