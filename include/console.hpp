#pragma once

#include "windows.hpp"

#include "cursor_info.hpp"
#include "font_info_ex.hpp"
#include "screen_buffer_size.hpp"

namespace console
{
void					 set_cursor_info(CursorInfo);
[[nodiscard]] CursorInfo get_cursor_info();

void set_font_info_ex(FontInfoEx);
// [[nodiscard]] FontInfoEx get_font_info_ex();

void set_screen_buffer_size(ScreenBufferSize);
// [[nodiscard]] ScreenBufferSize get_screen_buffer_size();

void					   set_title(std::wstring_view);
[[nodiscard]] std::wstring get_title();

void set_quick_edit_mode(bool);
bool get_quick_edit_mode();

[[nodiscard]] HWND get_handle_to_window();
} // namespace console
