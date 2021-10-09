#pragma once

#include "windows.hpp"

#include <algorithm>
#include <cassert>
#include <string>

class FontInfo
{
public:
	FontInfo(const int width)
	{
		auto w = std::clamp(width, min_width, max_width);
		auto h = w * 2;

		m_info.cbSize	  = sizeof(m_info);
		m_info.nFont	  = 0;
		m_info.dwFontSize = { static_cast<SHORT>(w), static_cast<SHORT>(h) };
		m_info.FontFamily = FF_DONTCARE;
		m_info.FontWeight = FW_NORMAL;

		const std::wstring_view face_name = L"Consolas";
		face_name.copy(static_cast<WCHAR* const>(m_info.FaceName), std::size(m_info.FaceName));
	}

	[[nodiscard]] const auto& cref() const noexcept
	{
		return m_info;
	}

	[[nodiscard]] auto& ref() noexcept
	{
		return m_info;
	}

	static constexpr int min_width = 7;
	static constexpr int max_width = 36;
private:
	CONSOLE_FONT_INFOEX m_info = {};
};
