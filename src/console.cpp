#include "console.hpp"
#include "exception.hpp"

void Console::set_cursor(const Cursor cursor)
{
	if (SetConsoleCursorInfo(std_out_handle, &cursor.get()) == NULL) {
		THROW_EXCEPTION("failed to set console cursor");
	}
}

void Console::set_font(Font font)
{
	if (SetCurrentConsoleFontEx(std_out_handle, FALSE, &font.get()) == NULL) {
		THROW_EXCEPTION("failed to set console font");
	}
	Sleep(time_to_apply_changes_ms);
}
