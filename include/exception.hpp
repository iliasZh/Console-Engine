#pragma once

#include <string_view>

#include <fmt/format.h>

class Exception
{
public:
	// The warning is that easily swappable params are bugprone.
	// But Exception is instantiated only in the THROW_EXCEPTION macro,
	// which only takes an error message.
	Exception(std::string_view desc, std::string_view file_name, // NOLINT
			  int line) noexcept
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


// The warnings are about function-like macro usage.
// But I found no way to write these in a function.

#define THROW_EXCEPTION(error_msg) /* NOLINT */ \
	throw Exception                             \
	{                                           \
		(error_msg), __FILE__, __LINE__         \
	}

/// Many WinAPI functions signal failure by returning zero.
#define THROW_IF_ZERO(func_call, error_msg) /* NOLINT */ \
	if ((func_call) == 0)                                \
	THROW_EXCEPTION(error_msg)
