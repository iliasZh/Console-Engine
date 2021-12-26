#pragma once

#include "character.hpp"
#include "screen_buffer_size.hpp"

#include <algorithm>
#include <concepts>
#include <vector>

template <class F>
concept shader_func = std::invocable<F, size_t, size_t> && requires(F f)
{
	// clang-format off
	{ f(0U, 0U) } -> std::same_as<CHAR_INFO>;
	// clang-format on
};

class ScreenBuffer
{
	using color = character::color;
public:
	[[nodiscard]] explicit ScreenBuffer(const int width, const int height,
										const color bg = color::black)
		: m_size{ width, height }
		, m_data{ linear_size(), character::char_info_from(L' ', { color::white, bg }) }
	{}

	ScreenBuffer(const ScreenBuffer&)		   = delete;
	[[nodiscard]] ScreenBuffer(ScreenBuffer&&) = default;

	ScreenBuffer& operator=(const ScreenBuffer&) = delete;
	ScreenBuffer& operator=(ScreenBuffer&&) = delete;

	~ScreenBuffer() noexcept = default;

	void clear(const color bg = color::black) noexcept
	{
		std::fill(m_data.begin(), m_data.end(),
				  character::char_info_from(L' ', { color::white, bg }));
	}

	void fill(shader_func auto shader)
	{
		for (auto i = 0U; i < linear_size(); ++i) {
			m_data[i] = shader(i % width(), i / width());
		}
	}

	[[nodiscard]] ScreenBufferSize size() const noexcept
	{
		return m_size;
	}

	[[nodiscard]] constexpr size_t width() const noexcept
	{
		return m_size.width();
	}

	[[nodiscard]] constexpr size_t height() const noexcept
	{
		return m_size.height();
	}

	[[nodiscard]] constexpr size_t linear_size() const noexcept
	{
		return width() * height();
	}

	[[nodiscard]] constexpr const CHAR_INFO* buffer_view() const noexcept
	{
		return m_data.data();
	}

	[[nodiscard]] constexpr CHAR_INFO* buffer() noexcept
	{
		return m_data.data();
	}
private:
	const ScreenBufferSize m_size;
	std::vector<CHAR_INFO> m_data{};
};
