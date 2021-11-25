#pragma once

#include "windows.hpp"

namespace util
{
[[nodiscard]] HANDLE std_out();
[[nodiscard]] HANDLE std_in();

[[nodiscard]] auto monitor_handle_for_window(HWND h_wnd);
[[nodiscard]] auto get_work_area_rectangle(HMONITOR h_mon);
[[nodiscard]] auto get_window_rectangle(HWND h_wnd);

void set_window_resizing(HWND h_wnd, bool enable);
void set_maximize_button(HWND h_wnd, bool enable);
} // namespace util
