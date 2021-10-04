#pragma once

#include "cursor.hpp"
#include "exception.hpp"
#include "font.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <yaml-cpp/yaml.h>


class Config
{
public:
	Config()
		: config(YAML::LoadFile(
			  (relative_path_to_config / config_file).generic_string()))
	{}

	[[nodiscard]] Cursor load_cursor()
	{
		auto cursor_node = config[Cursor::field];

		int size = fetch(cursor_node[Cursor::size_field], Cursor::default_size);
		bool visibility = fetch(cursor_node[Cursor::visibility_field],
								Cursor::default_visibility);

		return Cursor{ size, visibility };
	}

	[[nodiscard]] Font load_font()
	{
		auto font_node = config[Font::field];

		int	  width = fetch(font_node[Font::width_field], Font::default_width);
		float ratio = fetch(font_node[Font::height_to_width_ratio_field],
							Font::default_height_to_width_ratio);

		return Font{ width, ratio };
	}

private:
	template <class T>
	T fetch(YAML::Node node, T default_value)
	{
		if (node) {
			return node.as<T>();
		}
		return default_value;
	}

	const std::filesystem::path relative_path_to_config = "../config";
	const std::filesystem::path config_file				= "settings.yaml";
	YAML::Node					config{};
};