#pragma once

#include <string_view>

#include <fmt/format.h>

class Exception
{
public:
	Exception(std::string_view desc, std::string_view file_name, int line) noexcept // NOLINT
		: m_description(desc)
		, m_file_name(file_name)
		, m_line(line)
	{}

	[[nodiscard]] std::string what() const noexcept
	{
		return fmt::format("Error in \"{}\":{}:\n {}", m_file_name, m_line, m_description);
	}
private:
	const std::string_view m_description;
	const std::string_view m_file_name;

	const int m_line;
};

#define THROW_EXCEPTION(error_msg) throw Exception{ error_msg, __FILE__, __LINE__ }; // NOLINT
