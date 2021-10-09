#pragma once

#include "windows.hpp"

#include <algorithm>
#include <string>

class CursorInfo
{
public:
	constexpr CursorInfo(const int size, const bool visible) noexcept
	{
		m_info.dwSize	= static_cast<DWORD>(std::clamp(size, min_size, max_size));
		m_info.bVisible = static_cast<BOOL>(visible);
	}

	[[nodiscard]] constexpr const auto& cref() const noexcept
	{
		return m_info;
	}

	[[nodiscard]] constexpr auto& ref() noexcept
	{
		return m_info;
	}

	static constexpr int min_size = 1;
	static constexpr int max_size = 100;
private:
	CONSOLE_CURSOR_INFO m_info = { static_cast<DWORD>(min_size), static_cast<BOOL>(true) };
};
