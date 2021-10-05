#include "config.hpp"
#include "console.hpp"
#include "exception.hpp"

#include <exception>

int main()
{
	try {
		Config config;

		Console::set_cursor(config.load_cursor());
		Console::set_font(config.load_font());
		Console::set_screen_buffer(config.load_screen_buffer());
	}
	catch (Exception& e) {
		MessageBoxA(nullptr, e.what().c_str(), "Application error",
					MB_OK | MB_ICONERROR);
	}
	catch (std::exception& e) {
		MessageBoxA(nullptr, e.what(), "C++ error", MB_OK | MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(nullptr, "No details available", "Unknown error",
					MB_OK | MB_ICONERROR);
	}
	return 0;
}
