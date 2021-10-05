#pragma once

#include <string_view>

#include <fmt/format.h>

class Exception
{
public:
	using err_string	  = std::string;
	using err_string_view = std::string_view;

	Exception(err_string_view description, err_string_view file_name,
			  int line) noexcept
		: m_description(description)
		, m_file_name(file_name)
		, m_line(line)
	{}

	[[nodiscard]] err_string what() const noexcept
	{
		return fmt::format("Error in file \"{}\", line {}: {}", m_file_name,
						   m_line, m_description);
	}

private:
	const err_string_view m_description;
	const err_string_view m_file_name;

	const int m_line;
};

#define THROW_EXCEPTION(error_msg) /* NOLINT */ \
	throw Exception{ error_msg, __FILE__, __LINE__ };
