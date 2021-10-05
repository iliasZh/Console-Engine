#pragma once

#include "windows.hpp"

#include <algorithm>
#include <string>

class Cursor
{
public:
	constexpr Cursor(int size, bool visible) noexcept
	{
		set_size(size);
		set_visibility(visible);
	}

	[[nodiscard]] constexpr const auto& get() const noexcept
	{
		return m_info;
	}

	[[nodiscard]] constexpr auto& get() noexcept
	{
		return m_info;
	}

	static constexpr int min_size = 1;
	static constexpr int max_size = 100;

	inline static const std::string field			 = "cursor";
	inline static const std::string size_field		 = "size";
	inline static const std::string visibility_field = "visible";

	static constexpr int  default_size		 = max_size;
	static constexpr bool default_visibility = true;

private:
	constexpr void set_size(int size) noexcept
	{
		m_info.dwSize =
			static_cast<DWORD>(std::clamp(size, min_size, max_size));
	}
	constexpr void set_visibility(bool visible) noexcept
	{
		m_info.bVisible = static_cast<BOOL>(visible);
	}

	CONSOLE_CURSOR_INFO m_info = { static_cast<DWORD>(min_size),
								   static_cast<BOOL>(true) };
};