#include "win_utils.hpp"

#include "exception.hpp"
#include "windows.hpp"

#include <cassert>

namespace win_utils
{
[[nodiscard]] HANDLE std_out()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

[[nodiscard]] HANDLE std_in()
{
	return GetStdHandle(STD_INPUT_HANDLE);
}

/// Returns a handle to the primary display monitor.
[[nodiscard]] HMONITOR primary_monitor_handle()
{
	const POINT zero = { 0, 0 };

	return MonitorFromPoint(zero, MONITOR_DEFAULTTOPRIMARY);
}

/// The work area rectangle of the primary display monitor, expressed in virtual-screen coordinates.
[[nodiscard]] RECT primary_monitor_work_area()
{
	MONITORINFO mon_info{};
	mon_info.cbSize = sizeof(mon_info);

	THROW_IF_ZERO(GetMonitorInfoW(primary_monitor_handle(), &mon_info),
				  L"failed to get monitor info");

	return mon_info.rcWork;
}

/// Returns the coordinates of the window.
[[nodiscard]] RECT window_rectangle(HWND h_wnd)
{
	assert(h_wnd != nullptr);

	WINDOWINFO win_info{};
	win_info.cbSize = sizeof(win_info);

	THROW_IF_ZERO(GetWindowInfo(h_wnd, &win_info), L"failed to get window info");

	return win_info.rcWindow;
}

[[nodiscard]] auto window_style(HWND h_wnd)
{
	assert(h_wnd != nullptr);

	auto style = GetWindowLongPtrW(h_wnd, GWL_STYLE);

	if (style == 0) {
		THROW_EXCEPTION(L"failed to get window style");
	}

	return style;
}

void set_window_style(HWND h_wnd, const LONG_PTR style)
{
	assert(h_wnd != nullptr);
	assert(style != 0);

	THROW_IF_ZERO(SetWindowLongPtrW(h_wnd, GWL_STYLE, style), L"failed to set window style");

	// MSDN: Certain window data is cached, so changes you make using SetWindowLongPtr
	// will not take effect until you call the SetWindowPos function.

	// This call does nothing except updating the styles.
	THROW_IF_ZERO(SetWindowPos(h_wnd, HWND_TOP, 0, 0, 0, 0,
							   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED),
				  L"failed to update window style");
}

void set_button(HWND h_wnd, const LONG_PTR button_mask, const bool enable)
{
	assert(h_wnd != nullptr);

	const auto current_style = window_style(h_wnd);

	const bool button_enabled = (current_style & button_mask) != 0;

	if (button_enabled != enable) {
		set_window_style(h_wnd, current_style ^ button_mask);
	}
}

void set_window_resizing(HWND h_wnd, const bool enable)
{
	assert(h_wnd != nullptr);

	set_button(h_wnd, WS_SIZEBOX, enable);
}

void set_maximize_button(HWND h_wnd, const bool enable)
{
	assert(h_wnd != nullptr);

	set_button(h_wnd, WS_MAXIMIZEBOX, enable);
}

/// Centers the window to the primary monitor.
void center_window(HWND h_wnd)
{
	assert(h_wnd != nullptr);

	const auto work_area = primary_monitor_work_area();
	const auto win_rect	 = window_rectangle(h_wnd);

	const auto width_of = [](const RECT r) {
		return r.right - r.left;
	};

	const auto height_of = [](const RECT r) {
		return r.bottom - r.top;
	};

	const auto centered_pos_x = (width_of(work_area) - width_of(win_rect)) / 2;
	const auto centered_pos_y = (height_of(work_area) - height_of(win_rect)) / 2;

	THROW_IF_ZERO(SetWindowPos(h_wnd, HWND_TOP, centered_pos_x, centered_pos_y, 0, 0,
							   SWP_NOSIZE /* do not change the size of the window */),
				  L"failed to center the window");
}
} // namespace win_utils
