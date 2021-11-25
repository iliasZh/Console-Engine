#pragma once

#include "windows.hpp"

#include "cursor_info.hpp"
#include "font_info.hpp"
#include "screen_buffer_size.hpp"

namespace console
{
void set_cursor(CursorInfo);
void set_font(FontInfo);
void set_screen_buffer(ScreenBufferSize);
void set_title(std::string_view);
void set_text_selection(bool);

[[nodiscard]] std::string get_title();
[[nodiscard]] HWND		  get_handle_to_window();
} // namespace console
