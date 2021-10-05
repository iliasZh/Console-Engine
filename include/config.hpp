#pragma once

#include "cursor.hpp"
#include "exception.hpp"
#include "font.hpp"
#include "screen_buffer.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <yaml-cpp/yaml.h>

class Config
{
public:
	Config()
		: m_config(YAML::LoadFile(
			  (m_relative_path_to_config / m_config_file).generic_string()))
	{}

	[[nodiscard]] Cursor load_cursor()
	{
		auto cursor_node = m_config[Cursor::field];

		int size = fetch(cursor_node[Cursor::size_field], Cursor::default_size);
		bool visibility = fetch(cursor_node[Cursor::visibility_field],
								Cursor::default_visibility);

		return Cursor{ size, visibility };
	}

	[[nodiscard]] Font load_font()
	{
		auto font_node = m_config[Font::field];

		int	  width = fetch(font_node[Font::width_field], Font::default_width);
		float ratio = fetch(font_node[Font::height_to_width_ratio_field],
							Font::default_height_to_width_ratio);

		return Font{ width, ratio };
	}

	[[nodiscard]] ScreenBuffer load_screen_buffer()
	{
		auto screen_buffer_node = m_config[ScreenBuffer::field];

		int width  = fetch(screen_buffer_node[ScreenBuffer::width_field],
						   ScreenBuffer::default_width);
		int height = fetch(screen_buffer_node[ScreenBuffer::height_field],
						   ScreenBuffer::default_height);

		return ScreenBuffer{ width, height };
	}

private:
	template <class T>
	static T fetch(YAML::Node node, T default_value)
	{
		if (node) {
			return node.as<T>();
		}
		return default_value;
	}

	inline static const std::filesystem::path m_relative_path_to_config =
		"../config";
	inline static const std::filesystem::path m_config_file = "settings.yaml";

	YAML::Node m_config{};
};