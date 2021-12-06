#pragma once

#include "cursor_info.hpp"
#include "font_info_ex.hpp"
#include "screen_buffer_size.hpp"
#include "windows.hpp"

namespace console
{
void set_cursor_info(CursorInfo);
void set_font_info_ex(FontInfoEx);
void set_screen_buffer_size(ScreenBufferSize);
void set_title(std::wstring_view);

[[nodiscard]] CursorInfo	   get_cursor_info();
[[nodiscard]] FontInfoEx	   get_font_info_ex();
[[nodiscard]] ScreenBufferSize get_screen_buffer_size();
[[nodiscard]] std::wstring	   get_title();

void set_quick_edit_mode(bool);
bool get_quick_edit_mode();

[[nodiscard]] HWND get_handle_to_window();
} // namespace console
