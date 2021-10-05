#include "console.hpp"
#include "exception.hpp"

void Console::set_cursor(const Cursor cursor)
{
	if (SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor.get()) ==
		FALSE)
	{
		THROW_EXCEPTION("failed to set console cursor");
	}
}

void Console::set_font(Font font)
{
	if (SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE,
								&font.get()) == FALSE)
	{
		THROW_EXCEPTION("failed to set console font");
	}
	Sleep(time_to_apply_changes_ms);
}

void Console::set_screen_buffer(ScreenBuffer screen_buffer)
{
	// at any time console window size must not exceed the size of console
	// screen buffer for that reason set window size to minimum so that it is
	// possible to set screen buffer size w/o problem
	auto set_console_window_size = [](COORD lower_right_corner) {
		SMALL_RECT r;
		r.Left	 = 0;
		r.Top	 = 0;
		r.Right	 = lower_right_corner.X - 1;
		r.Bottom = lower_right_corner.Y - 1;

		if (auto* std_out = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleWindowInfo(std_out, TRUE, &r) == FALSE)
		{
			THROW_EXCEPTION(
				"failed to temporarily set console window size to 1 * 1");
		}
	};

	set_console_window_size({ 1, 1 });

	if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
								   screen_buffer.size()) == FALSE)
	{
		THROW_EXCEPTION("failed to set requested console screen buffer size");
	}

	set_console_window_size(screen_buffer.size());
}