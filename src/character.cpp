#include "character.hpp"

namespace character
{
constexpr CHAR_INFO char_info_from(const wchar_t symbol, const color_pair cp) noexcept
{
	CHAR_INFO ch{};

	// Can't change CHAR_INFO to use std::variant :(
	ch.Char.UnicodeChar = symbol; // NOLINT
	ch.Attributes		= cp.attributes();

	return ch;
}

constexpr wchar_t char_from(const CHAR_INFO ch) noexcept
{
	// Can't change CHAR_INFO to use std::variant :(
	return ch.Char.UnicodeChar; // NOLINT
}

constexpr color_pair colors_from(const CHAR_INFO ch) noexcept
{
	return color_pair{ ch.Attributes };
}
} // namespace character
