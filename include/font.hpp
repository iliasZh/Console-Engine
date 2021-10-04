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

		info.cbSize		= sizeof(info);
		info.nFont		= 0;
		info.dwFontSize = { static_cast<SHORT>(width),
							static_cast<SHORT>(height) };
		info.FontFamily = FF_DONTCARE;
		info.FontWeight = FW_NORMAL;

		const std::wstring_view face_name = L"Consolas";
		face_name.copy(static_cast<WCHAR* const>(info.FaceName),
					   std::size(info.FaceName));
	}
	Font(int width, float height_to_width_ratio = default_height_to_width_ratio)
		: Font(width, static_cast<int>(static_cast<float>(width) *
									   height_to_width_ratio))
	{}

	[[nodiscard]] const auto& get() const
	{
		return info;
	}

	[[nodiscard]] auto& get()
	{
		return info;
	}

	static constexpr int   default_width				 = 10;
	static constexpr float default_height_to_width_ratio = 2.0F;

	inline static const std::string field		= "font";
	inline static const std::string width_field = "width";
	inline static const std::string height_to_width_ratio_field =
		"height_to_width_ratio";

private:
	CONSOLE_FONT_INFOEX info = {};
};