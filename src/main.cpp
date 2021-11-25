#include "console.hpp"
#include "exception.hpp"
#include "util.hpp"

#include <exception>

int main()
{
	try {
		console::set_cursor({ 1, true });
		console::set_font({ 15 });
		console::set_screen_buffer({ 400, 100 });

		console::set_text_selection(false);

		console::set_title("console engine by IliasZ");

		auto* console_handle = console::get_handle_to_window();

		util::set_window_resizing(console_handle, false);
		util::set_maximize_button(console_handle, false);
	}
	catch (Exception& e) {
		MessageBoxA(nullptr, e.what().c_str(), "Application error", MB_OK | MB_ICONERROR);
	}
	catch (std::exception& e) {
		MessageBoxA(nullptr, e.what(), "C++ error", MB_OK | MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(nullptr, "No details available", "Unknown error", MB_OK | MB_ICONERROR);
	}
	return 0;
}
