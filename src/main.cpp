#include "console.hpp"
#include "exception.hpp"

#include <exception>

int main()
{
	try {
		console::set_cursor({ 100, true });
		console::set_font({ 15 });
		console::set_screen_buffer({ 400, 100 });

		fmt::print("abcdefghijklmnopqrstuvwxyz\n");
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
