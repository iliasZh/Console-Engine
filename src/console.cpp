#include "console.hpp"
#include "exception.hpp"
#include "util.hpp"

namespace console
{
constexpr int time_to_apply_changes_ms = 50;

void set_cursor(const CursorInfo cursor_info)
{
	if (SetConsoleCursorInfo(util::std_out(), &cursor_info.cref()) == FALSE) {
		THROW_EXCEPTION("failed to set console cursor");
	}
}

void set_font(FontInfo font_info)
{
	if (SetCurrentConsoleFontEx(util::std_out(), FALSE, &font_info.ref()) == FALSE) {
		THROW_EXCEPTION("failed to set console font");
	}

	Sleep(time_to_apply_changes_ms);
}

void set_screen_buffer(const ScreenBufferSize screen_buffer_size)
{
	auto set_console_window_size = [](const COORD dims) {
		SMALL_RECT window;
		window.Left	  = 0;
		window.Top	  = 0;
		window.Right  = static_cast<SHORT>(dims.X - 1);
		window.Bottom = static_cast<SHORT>(dims.Y - 1);

		if (SetConsoleWindowInfo(util::std_out(), TRUE, &window) == FALSE) {
			THROW_EXCEPTION(
				fmt::format("failed to set console window size to {}*{}", dims.X, dims.Y));
		}
	};

	// At any time console window size must not exceed the size of console screen buffer.
	// For that reason set window size to minimum so that it is possible to set screen
	// buffer size without a problem
	set_console_window_size({ 1, 1 });

	if (SetConsoleScreenBufferSize(util::std_out(), screen_buffer_size.get()) == FALSE) {
		THROW_EXCEPTION("failed to set requested console screen buffer size");
	}

	set_console_window_size(screen_buffer_size.get());
}
} // namespace console
