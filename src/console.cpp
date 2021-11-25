#include "console.hpp"
#include "exception.hpp"
#include "util.hpp"

#include <array>

namespace console
{
constexpr int time_to_apply_changes_ms = 50;

void set_cursor(const CursorInfo cursor_info)
{
	THROW_IF_ZERO(SetConsoleCursorInfo(util::std_out(), &cursor_info.cref()),
				  "failed to set console cursor");
}

void set_font(FontInfo font_info)
{
	THROW_IF_ZERO(SetCurrentConsoleFontEx(util::std_out(), FALSE, &font_info.ref()),
				  "failed to set console font");

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

		THROW_IF_ZERO(SetConsoleWindowInfo(util::std_out(), TRUE, &window),
					  fmt::format("failed to set console window size to {}*{}", dims.X, dims.Y));
	};

	// At any time console window size must not exceed the size of console screen buffer.
	// For that reason set window size to minimum so that it is possible to set screen
	// buffer size without a problem.
	set_console_window_size({ 1, 1 });

	THROW_IF_ZERO(SetConsoleScreenBufferSize(util::std_out(), screen_buffer_size.get()),
				  "failed to set requested console screen buffer size");

	set_console_window_size(screen_buffer_size.get());
}

/// It takes some tens of milliseconds to set the title.
void set_title(const std::string_view title)
{
	THROW_IF_ZERO(SetConsoleTitle(title.data()),
				  fmt::format("failed to set console title to \"{}\"", title));

	Sleep(time_to_apply_changes_ms);
}

[[nodiscard]] DWORD get_mode(HANDLE h_buf)
{
	// The warning is about the handle macro using C-style cast - can't do anything about it.
	assert(h_buf != INVALID_HANDLE_VALUE); // NOLINT

	DWORD mode = 0;

	THROW_IF_ZERO(GetConsoleMode(h_buf, &mode), "failed to get console's current i/o mode");

	return mode;
}

void set_mode(HANDLE h_buf, const DWORD mode)
{
	THROW_IF_ZERO(SetConsoleMode(h_buf, mode), "failed to set console mode");
}

void set_text_selection(const bool enable)
{
	HANDLE std_in = util::std_in();

	const auto current_mode = get_mode(std_in);

	const bool selection_enabled = (current_mode & ENABLE_QUICK_EDIT_MODE) != 0;

	if (selection_enabled != enable) {
		set_mode(std_in, ENABLE_EXTENDED_FLAGS | (current_mode ^ ENABLE_QUICK_EDIT_MODE));
	}
}

/// Maximum of 256 characters. Truncates if the title is bigger.
[[nodiscard]] std::string get_title()
{
	constexpr int buf_size = 256;

	std::array<char, buf_size> title_buf{};

	const auto chars_written = GetConsoleTitle(title_buf.data(), buf_size);
	if (chars_written == 0) {
		THROW_EXCEPTION("failed to get console window title");
	}

	return { title_buf.cbegin(), std::next(title_buf.cbegin(), chars_written) };
}

[[nodiscard]] HWND get_handle_to_window()
{
	auto current_title = get_title();

	// Change the title to avoid (unlikely, but) possible collisions.
	std::string_view temp_title = "console engine by Ilias, this is a temporary title";
	set_title(temp_title);

	HWND handle = FindWindow(nullptr, temp_title.data());
	if (handle == nullptr) {
		THROW_EXCEPTION(fmt::format("failed to find window named \"{}\"", temp_title));
	}

	set_title(current_title);

	return handle;
}
} // namespace console
