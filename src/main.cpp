#include "character.hpp"
#include "console.hpp"
#include "exception.hpp"
#include "matrix.hpp"
#include "screen_buffer.hpp"
#include "vector.hpp"
#include "win_utils.hpp"

#include <exception>

using namespace character;

int main()
{
	constexpr auto msg_box_appearance = MB_OK | MB_ICONERROR;

	try {
		console::set_font_info_ex({ 16 });

		ScreenBuffer s{ 400, 200 };

		console::set_screen_buffer_size(s.size());
		console::set_cursor_info({ 1, false });
		console::set_quick_edit_mode(true);
		console::set_title(L"console engine by Ilias");

		HWND console_handle = console::get_handle_to_window();

		win_utils::set_window_resizing(console_handle, false);
		win_utils::set_maximize_button(console_handle, false);
		win_utils::center_window(console_handle);

		const auto star		= character::char_info_from('*');
		const auto red_star = character::char_info_from('*', ColorPair{ Color::red });

		const math::Vector2i p1 = { 50, 25 };
		const math::Vector2i p2 = { 100, 5 };
		const math::Vector2i p3 = { 75, 35 };

		s.draw_line(p1, p2, star);
		s.draw_line(p2, p3, star);
		s.draw_line(p3, p1, star);

		s.put_char(p1, red_star);
		s.put_char(p2, red_star);
		s.put_char(p3, red_star);

		// s.draw_line({ 10, 10 }, { 15, 12 }, star);

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
