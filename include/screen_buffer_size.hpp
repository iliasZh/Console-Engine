#pragma once

#include "exception.hpp"
#include "win_utils.hpp"
#include "windows.hpp"

#include <algorithm>
#include <string>

#include <fmt/format.h>
#include <fmt/xchar.h>

class ScreenBufferSize
{
public:
	/// Clamps the size between minimum hardcoded size and maximum possible dynamic size given by
	/// GetLargestConsoleWindowSize.
	explicit ScreenBufferSize(const COORD size)
	{
		auto [min_w, min_h] = min_size();
		auto [max_w, max_h] = max_size();

		if (min_w > max_w || min_h > max_h) {
			THROW_EXCEPTION(fmt::format(L"screen buffer's maximum possible size ({}*{}) is smaller "
										L"than hardcoded minimum size ({}*{})",
										max_w, max_h, min_w, min_h));
		}

		m_size = { std::clamp(size.X, min_w, max_w), std::clamp(size.Y, min_h, max_h) };
	}

	/// Clamps the size between minimum hardcoded size and maximum possible dynamic size given by
	/// GetLargestConsoleWindowSize.
	ScreenBufferSize(const int width, const int height)
		: ScreenBufferSize{ COORD{ static_cast<SHORT>(width), static_cast<SHORT>(height) } }
	{}

	[[nodiscard]] auto get() const noexcept
	{
		return m_size;
	}

	// These are arbitrary, but I think adequate.
	static constexpr int min_width	= 40;
	static constexpr int min_height = 10;
private:
	static COORD max_size() noexcept
	{
		return GetLargestConsoleWindowSize(win_utils::std_out());
	}

	constexpr static COORD min_size() noexcept
	{
		return { static_cast<SHORT>(min_width), static_cast<SHORT>(min_height) };
	}

	COORD m_size{ min_size() };
};
