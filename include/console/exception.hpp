#pragma once

#include <string>

#include <fmt/xchar.h>

class Exception
{
public:
	// The warning is that easily swappable params are bugprone.
	// But Exception is instantiated only in the THROW_EXCEPTION macro,
	// which only takes an error message.
	explicit constexpr Exception(const std::wstring_view desc, // NOLINT
								 const std::wstring_view file_name, // NOLINT
								 const int				 line) noexcept
		: m_description(desc)
		, m_file_name(trim_to_root(file_name))
		, m_line(line)
	{}

	[[nodiscard]] std::wstring what() const noexcept
	{
		return fmt::format(L"Error in \"{}\", line {}:\n {}", m_file_name,
						   m_line, m_description);
	}
private:
	[[nodiscard]] constexpr static std::wstring_view
	trim_to_root(const std::wstring_view file_name) noexcept
	{
		const auto root_pos = file_name.rfind(L"/console-engine/");

		// Failed to find the sequence.
		if (root_pos == std::wstring_view::npos) {
			return file_name;
		}

		return file_name.substr(root_pos + 1);
	}

	const std::wstring_view m_description;
	const std::wstring_view m_file_name;

	const int m_line;
};


// The warnings are about function-like macro usage.
// I know about <source_location>, but it breaks my clangd and clang-tidy :(

#define WIDESTR_FILE_HELPER_2(x) L##x // NOLINT
#define WIDESTR_FILE_HELPER_1(x) WIDESTR_FILE_HELPER_2(x) // NOLINT
#define WIDESTR_FILE			 WIDESTR_FILE_HELPER_1(__FILE__) // NOLINT

#define THROW_EXCEPTION(error_msg) /* NOLINT */ \
	throw Exception                             \
	{                                           \
		(error_msg), WIDESTR_FILE, __LINE__     \
	}

/// Many WinAPI functions signal failure by returning zero.
#define THROW_IF_ZERO(func_call, error_msg) /* NOLINT */ \
	if ((func_call) == 0)                                \
	THROW_EXCEPTION(error_msg)
