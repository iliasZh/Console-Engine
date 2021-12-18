#include "console.hpp"
#include "exception.hpp"
#include "win_utils.hpp"

#include <exception>

int main()
{
	constexpr auto msg_box_appearance = MB_OK | MB_ICONERROR;

	try {
		console::set_cursor_info({ 1, false });
		console::set_font_info_ex({ 30 });
		console::set_screen_buffer_size({ 400, 100 });

		console::set_quick_edit_mode(false);

		console::set_title(L"console engine by Ilias");

		HWND console_handle = console::get_handle_to_window();

		win_utils::set_window_resizing(console_handle, false);
		win_utils::set_maximize_button(console_handle, false);

		win_utils::center_window(console_handle);
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
