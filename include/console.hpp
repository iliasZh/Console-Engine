#pragma once

#include "windows.hpp"

#include "cursor.hpp"
#include "font.hpp"

class Console
{
public:
	void set_cursor(Cursor);
	void set_font(Font);

	static constexpr int time_to_apply_changes_ms = 50;

private:
	HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	// HANDLE std_in_handle  = GetStdHandle(STD_INPUT_HANDLE);
	// HWND   window_handle  = nullptr;
};
