#pragma once

#include "windows.hpp"

#include <cassert>
#include <string>

class Font
{
public:
	Font(int width, int height)
	{
		assert(width > 0 && height > 0);

		m_info.cbSize	  = sizeof(m_info);
		m_info.nFont	  = 0;
		m_info.dwFontSize = { static_cast<SHORT>(width),
							  static_cast<SHORT>(height) };
		m_info.FontFamily = FF_DONTCARE;
		m_info.FontWeight = FW_NORMAL;

		const std::wstring_view face_name = L"Consolas";
		face_name.copy(static_cast<WCHAR* const>(m_info.FaceName),
					   std::size(m_info.FaceName));
	}
	Font(int width, float height_to_width_ratio = default_height_to_width_ratio)
		: Font(width, static_cast<int>(static_cast<float>(width) *
									   height_to_width_ratio))
	{}

	[[nodiscard]] const auto& get() const
	{
		return m_info;
	}

	[[nodiscard]] auto& get()
	{
		return m_info;
	}

	static constexpr int   default_width				 = 10;
	static constexpr float default_height_to_width_ratio = 2.0F;

	inline static const std::string field		= "font";
	inline static const std::string width_field = "width";
	inline static const std::string height_to_width_ratio_field =
		"height_to_width_ratio";

private:
	CONSOLE_FONT_INFOEX m_info = {};
};