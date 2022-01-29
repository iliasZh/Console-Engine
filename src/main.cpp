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

		auto v1 = math::Vector3f{ 1.0f, 1.0f, 1.0f };

		auto v2 = math::Vector3f{ 2.0f, 0.5f, 1.5f };

		v1 += math::Vector3f::x_versor();

		v1 *= -1.0f;

		fmt::print("v1: ({}, {}, {})\n", v1.x(), v1.y(), v1.z());
		fmt::print("v2: ({}, {}, {})\n", v2.x(), v2.y(), v2.z());

		fmt::print("len sq: {}\n", v1.length_squared());

		fmt::print("screen buffer size: {} * {}\n", s.width(), s.height());


		// s.test();

		// console::draw_screen_buffer(s);
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
