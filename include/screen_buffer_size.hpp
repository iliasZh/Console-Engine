#pragma once

#include "exception.hpp"
#include "util.hpp"
#include "windows.hpp"

#include <algorithm>
#include <string>

class ScreenBufferSize
{
public:
	ScreenBufferSize(const int width, const int height)
	{
		auto [min_w, min_h] = min_size();
		auto [max_w, max_h] = max_size();

		if (min_w > max_w || min_h > max_h) {
			THROW_EXCEPTION("screen buffer's minimum size exceeds maximum possible size");
		}

		m_size.X = std::clamp(static_cast<SHORT>(width), min_w, max_w);
		m_size.Y = std::clamp(static_cast<SHORT>(height), min_h, max_h);
	}

	[[nodiscard]] auto get() const noexcept
	{
		return m_size;
	}

	static constexpr int min_width	= 40;
	static constexpr int min_height = 10;
private:
	static COORD max_size() noexcept
	{
		return GetLargestConsoleWindowSize(util::std_out());
	}

	constexpr static COORD min_size() noexcept
	{
		return { static_cast<SHORT>(min_width), static_cast<SHORT>(min_height) };
	}

	COORD m_size{ min_size() };
};
