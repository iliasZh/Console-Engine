#pragma once

#include "windows.hpp"

namespace character
{
// clang-format off
// because it formats weirdly with [[maybe_unused]]
enum class [[maybe_unused]] color : WORD
{
	black		 = 0x00,
	dark_blue	 = 0x01,
	dark_green	 = 0x02,
	dark_cyan	 = 0x03,
	dark_red	 = 0x04,
	dark_magenta = 0x05,
	dark_yellow	 = 0x06,
	gray		 = 0x07,
	dark_gray	 = 0x08,
	blue		 = 0x09,
	green		 = 0x0A,
	cyan		 = 0x0B,
	red			 = 0x0C,
	magenta		 = 0x0D,
	yellow		 = 0x0E,
	white		 = 0x0F
};
// clang-format on

/// Represents the foreground and background colors of a character cell.
/// WinAPI represents the colors of a cell with the 'attributes' field of CHAR_INFO struct.
/// It is a 16-bit value (WORD) where the upper 8 bits are unused in my case, and the lower 8 bits
/// represent colors.
/// Of those 8 bits, lower 4 represent foreground color, and upper 4 - background color.
class color_pair
{
public:
	explicit constexpr color_pair(const WORD attributes) noexcept
		: fg{ static_cast<color>(attributes & fg_mask) }
		, bg{ static_cast<color>((attributes & bg_mask) >> color_bit_size) }
	{}

	// I'm just ignoring the warning about easily swappable params lol.
	constexpr color_pair(const color foreground = color::white, /* NOLINT */
						 const color background = color::black /* NOLINT */) noexcept
		: fg{ foreground }
		, bg{ background }
	{}

	[[nodiscard]] constexpr WORD attributes() const noexcept
	{
		return static_cast<WORD>(static_cast<int>(fg) | (static_cast<int>(bg) << color_bit_size));
	}

	static constexpr int color_bit_size = 4;

	static constexpr int fg_mask = 0x0F;
	static constexpr int bg_mask = fg_mask << color_bit_size; // i.e. 0xF0
private:
	color fg = color::white;
	color bg = color::black;

	static constexpr int colors_bits = 0xFF;

	static_assert((fg_mask | bg_mask) == colors_bits);
};

// test cases
static_assert(color_pair{ color::white, color::black }.attributes() == 0x0F);	  // NOLINT
static_assert(color_pair{ color::green, color::dark_blue }.attributes() == 0x1A); // NOLINT


inline constexpr CHAR_INFO char_info_from(wchar_t symbol, color_pair cp = {}) noexcept
{
	CHAR_INFO ch{};

	// Can't change CHAR_INFO to use std::variant :(
	ch.Char.UnicodeChar = symbol; // NOLINT
	ch.Attributes		= cp.attributes();

	return ch;
}

inline constexpr wchar_t char_from(const CHAR_INFO ch) noexcept
{
	// Can't change CHAR_INFO to use std::variant :(
	return ch.Char.UnicodeChar; // NOLINT
}

inline constexpr color_pair colors_from(const CHAR_INFO ch) noexcept
{
	return color_pair{ ch.Attributes };
}
} // namespace character
