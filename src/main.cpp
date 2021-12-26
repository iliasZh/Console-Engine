#include "character.hpp"
#include "console.hpp"
#include "exception.hpp"
#include "screen_buffer.hpp"
#include "win_utils.hpp"

#include <exception>

using namespace character;

int main()
{
	constexpr auto msg_box_appearance = MB_OK | MB_ICONERROR;

	try {
		ScreenBuffer s{ 400, 200 };
		console::set_screen_buffer_size(s.size());
		console::set_cursor_info({ 1, false });
		console::set_font_info_ex({ 30 });

		console::set_quick_edit_mode(false);

		console::set_title(L"console engine by Ilias");

		HWND console_handle = console::get_handle_to_window();

		win_utils::set_window_resizing(console_handle, false);
		win_utils::set_maximize_button(console_handle, false);

		win_utils::center_window(console_handle);

		s.fill([](size_t x, size_t y) {
			if (x < 5U && y < 5U) {
				return char_info_from(L'\u2591');
			}

			if (x < 10 && y < 10) {
				return char_info_from(L'\u2592');
			}

			if (x < 15 && y < 15) {
				return char_info_from(L' ', color_pair{ color::white, color::red });
			}

			return char_info_from(L' ', color_pair{ color::white, color::white });
		});

		console::draw_screen_buffer(s);
	}
	catch (const Exception& e) {
		MessageBoxW(nullptr, e.what().c_str(), L"Application error", msg_box_appearance);
	}
	catch (const std::exception& e) {
		// Is it fine to use A and W versions together? I don't know. It probably is.
		MessageBoxA(nullptr, e.what(), "C++ error", msg_box_appearance);
	}
	catch (...) {
		MessageBoxW(nullptr, L"No details available", L"Unknown error", msg_box_appearance);
	}
	return 0;
}
