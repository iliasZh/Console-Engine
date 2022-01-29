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

[[nodiscard]] inline constexpr SMALL_RECT rect_from_coord(const COORD c) noexcept
{
	SMALL_RECT r{};

	r.Left	 = 0;
	r.Top	 = 0;
	r.Right	 = static_cast<SHORT>(c.X - 1);
	r.Bottom = static_cast<SHORT>(c.Y - 1);

	return r;
}
} // namespace win_utils
