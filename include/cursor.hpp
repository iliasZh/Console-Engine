#pragma once

#include "windows.hpp"

#include <algorithm>
#include <string>

class Cursor
{
public:
	enum class Preset
	{
		Invisible,
		Underline,
		FullBlock
	};

	constexpr Cursor(Preset preset) noexcept
	{
		switch (preset) {
		case Preset::Invisible:
			set_size(min_size);
			set_visibility(false);
			break;
		case Preset::Underline:
			set_size(min_size);
			set_visibility(true);
			break;
		case Preset::FullBlock:
			set_size(max_size);
			set_visibility(true);
			break;
		default:
			break;
		}
	}
	constexpr Cursor(int size, bool visible) noexcept
	{
		set_size(size);
		set_visibility(visible);
	}

	[[nodiscard]] constexpr const auto& get() const noexcept
	{
		return info;
	}

	[[nodiscard]] constexpr auto& get() noexcept
	{
		return info;
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
		info.dwSize = static_cast<DWORD>(std::clamp(size, min_size, max_size));
	}
	constexpr void set_visibility(bool visible) noexcept
	{
		info.bVisible = static_cast<BOOL>(visible);
	}

	CONSOLE_CURSOR_INFO info = { static_cast<DWORD>(min_size),
								 static_cast<BOOL>(true) };
};