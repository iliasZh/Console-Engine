#pragma once

#include "windows.hpp"
#include <exception.hpp>

#include <algorithm>
#include <string>

class ScreenBuffer
{
public:
	ScreenBuffer(int width, int height)
	{
		auto [max_width, max_height] = get_largest_size();

		if (static_cast<SHORT>(min_width) > max_width ||
			static_cast<SHORT>(min_height) > max_height)
		{
			THROW_EXCEPTION("screen buffer's minimum size is too big");
		}

		m_size.X = std::clamp(static_cast<SHORT>(width),
							  static_cast<SHORT>(min_width), max_width);
		m_size.Y = std::clamp(static_cast<SHORT>(height),
							  static_cast<SHORT>(min_height), max_height);
	}

	[[nodiscard]] auto size() const noexcept
	{
		return m_size;
	}

	static constexpr int min_width	= 20;
	static constexpr int min_height = 5;

	static constexpr int default_width	= 80;
	static constexpr int default_height = 25;

	inline static const std::string field		 = "console_dimensions";
	inline static const std::string width_field	 = "width";
	inline static const std::string height_field = "height";

private:
	static COORD get_largest_size() noexcept
	{
		return GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	COORD m_size{ static_cast<SHORT>(min_width),
				  static_cast<SHORT>(min_height) };
};