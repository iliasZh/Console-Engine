#pragma once

#include <string_view>

#include <fmt/format.h>

class Exception
{
public:
	using err_string	  = std::string;
	using err_string_view = std::string_view;

	Exception(err_string_view description_, err_string_view file_name_,
			  int line_) noexcept
		: description(description_)
		, file_name(file_name_)
		, line(line_)
	{}

	[[nodiscard]] err_string what() const noexcept
	{
		return fmt::format("Error in file \"{}\", line {}: {}", file_name, line,
						   description);
	}

private:
	const err_string_view description;
	const err_string_view file_name;

	const int line;
};

#define THROW_EXCEPTION(error_msg) /* NOLINT */ \
	throw Exception{ error_msg, __FILE__, __LINE__ };
