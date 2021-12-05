#include "console.hpp"
#include "exception.hpp"
#include "win_utils.hpp"

#include <exception>
#include <iostream>

#include <fmt/format.h>
#include <fmt/xchar.h>

int main()
{
	try {
		console::set_cursor_info({ 1, false });
		console::set_font_info_ex({ 30 });
		console::set_screen_buffer_size({ 400, 100 });

		console::set_quick_edit_mode(false);

		console::set_title(L"console engine by Ilias");

		auto* console_handle = console::get_handle_to_window();

		win_utils::set_window_resizing(console_handle, false);
		win_utils::set_maximize_button(console_handle, false);

		std::wcout << fmt::format(L"Hello, C++{}\n", 20);

		Sleep(5000);
	}
	catch (const Exception& e) {
		MessageBoxW(nullptr, e.what().c_str(), L"Application error", MB_OK | MB_ICONERROR);
	}
	catch (const std::exception& e) {
		// Is it fine to use A and W versions together? I don't know. It probably is.
		MessageBoxA(nullptr, e.what(), "C++ error", MB_OK | MB_ICONERROR);
	}
	catch (...) {
		MessageBoxW(nullptr, L"No details available", L"Unknown error", MB_OK | MB_ICONERROR);
	}
	return 0;
}
