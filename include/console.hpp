#pragma once

#include "windows.hpp"

#include "cursor.hpp"
#include "font.hpp"
#include "screen_buffer.hpp"

class Console
{
public:
	static void set_cursor(Cursor);
	static void set_font(Font);
	static void set_screen_buffer(ScreenBuffer);

	static constexpr int time_to_apply_changes_ms = 50;

private:
	// HWND   window_handle  = nullptr;
};
