#include "win_utils.hpp"

#include "exception.hpp"
#include "windows.hpp"

#include <cassert>
#include <winuser.h>

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

[[nodiscard]] auto monitor_handle_for_window(HWND h_wnd)
{
	assert(h_wnd != nullptr);

	return MonitorFromWindow(h_wnd, MONITOR_DEFAULTTOPRIMARY);
}

[[nodiscard]] auto get_work_area_rectangle(HMONITOR h_mon)
{
	assert(h_mon != nullptr);

	MONITORINFO mon_info{};
	mon_info.cbSize = sizeof(mon_info);

	THROW_IF_ZERO(GetMonitorInfoW(h_mon, &mon_info), L"failed to get monitor info");

	return mon_info.rcMonitor;
}

[[nodiscard]] auto get_window_rectangle(HWND h_wnd)
{
	assert(h_wnd != nullptr);

	WINDOWINFO win_info{};
	win_info.cbSize = sizeof(win_info);

	THROW_IF_ZERO(GetWindowInfo(h_wnd, &win_info), L"failed to get window info");

	return win_info.rcWindow;
}

[[nodiscard]] auto get_window_style(HWND h_wnd)
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

	const auto current_style = get_window_style(h_wnd);

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
} // namespace win_utils
