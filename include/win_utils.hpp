#pragma once

#include "windows.hpp"

namespace win_utils
{
[[nodiscard]] HANDLE std_out();
[[nodiscard]] HANDLE std_in();

[[nodiscard]] HMONITOR primary_monitor_handle();
[[nodiscard]] RECT	   primary_monitor_work_area();
[[nodiscard]] RECT	   window_rectangle(HWND);

void set_window_resizing(HWND h_wnd, bool enable);
void set_maximize_button(HWND h_wnd, bool enable);

void center_window(HWND);
} // namespace win_utils
