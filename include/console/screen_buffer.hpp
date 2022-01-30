#pragma once

#include "character.hpp"
#include "concepts.hpp"
#include "screen_buffer_size.hpp"
#include "utils.hpp"
#include "vector.hpp"

#include <algorithm>
#include <concepts>
#include <vector>

template <class F>
concept shader_func = std::invocable<F, size_t, size_t> && requires(F f)
{
	// clang-format off
	{ f(0u, 0u) } -> std::same_as<CHAR_INFO>;
	// clang-format on
};

class ScreenBuffer
{
	using Color = character::Color;
public:
	template <math::integral_least32 IntType = std::size_t>
	[[nodiscard]] constexpr IntType width() const noexcept
	{
		if constexpr (std::same_as<IntType, std::size_t>) {
			return m_size.width();
		} else {
			return static_cast<IntType>(m_size.width());
		}
	}

	template <math::integral_least32 IntType = std::size_t>
	[[nodiscard]] constexpr IntType height() const noexcept
	{
		if constexpr (std::same_as<IntType, std::size_t>) {
			return m_size.height();
		}
		return static_cast<IntType>(m_size.height());
	}

	template <math::integral_least32 IntType = std::size_t>
	[[nodiscard]] constexpr IntType linear_size() const noexcept
	{
		if constexpr (std::same_as<IntType, std::size_t>) {
			return width() * height();
		} else {
			return static_cast<IntType>(width() * height());
		}
	}

	[[nodiscard]] explicit ScreenBuffer(const int width, const int height,
										const Color bg = Color::black)
		: m_size{ width, height }
		, m_data{ linear_size(), character::char_info_from(L' ', { Color::white, bg }) }
	{}

	ScreenBuffer(const ScreenBuffer&)		   = delete;
	[[nodiscard]] ScreenBuffer(ScreenBuffer&&) = default;

	ScreenBuffer& operator=(const ScreenBuffer&) = delete;
	ScreenBuffer& operator=(ScreenBuffer&&) = delete;

	~ScreenBuffer() noexcept = default;

	void clear(const Color bg = Color::black) noexcept
	{
		std::fill(m_data.begin(), m_data.end(),
				  character::char_info_from(L' ', { Color::white, bg }));
	}

	void fill(shader_func auto shader)
	{
		for (auto i = 0u; i < linear_size(); ++i) {
			m_data[i] = shader(i % width(), i / width());
		}
	}

	// TODO: remove
	void test()
	{
		std::wstring_view gradient = L".:!/r(l1Z4H9W8$@";

		for (size_t i = 0u; i < 16u; ++i) {
			for (size_t j = 0u; j < 16u; ++j) {
				m_data[i * width() + j] = character::char_info_from(
					gradient[j], character::ColorPair{ character::Color(i) });
			}
		}
	}

	[[nodiscard]] ScreenBufferSize size() const noexcept
	{
		return m_size;
	}

	[[nodiscard]] constexpr const CHAR_INFO* buffer_view() const noexcept
	{
		return m_data.data();
	}

	[[nodiscard]] constexpr CHAR_INFO* buffer() noexcept
	{
		return m_data.data();
	}

	[[nodiscard]] constexpr CHAR_INFO& at(const math::Vector2i p)
	{
		assert(contains(p));

		const auto [x, y] = p.elements();

		return m_data[width() * static_cast<size_t>(y) + static_cast<size_t>(x)];
	}

	void put_char(const math::Vector2i p, const CHAR_INFO ch)
	{
		at(p) = ch;
	}

	void draw_line(const math::Vector2i p1, const math::Vector2i p2, const CHAR_INFO ch)
	{
		// First point is always higher (has lesser y-coord) than the second point.
		if (p1.y() < p2.y()) {
			draw_line_bresenham(p1, p2, ch);
		} else {
			draw_line_bresenham(p2, p1, ch);
		}
	}
private:
	[[nodiscard]] bool contains(const math::Vector2i p) const
	{
		const auto [x, y] = p.elements();

		return (0 <= x && x < width<int>()) && (0 <= y && y < height<int>());
	}

	void draw_line_bresenham(const math::Vector2i p1, const math::Vector2i p2, const CHAR_INFO ch)
	{
		assert(contains(p1) && contains(p2));
		assert(p1.y() <= p2.y());

		const auto [dx, dy] = (p2 - p1).elements();

		const auto dx_sign = math::sign(dx);
		const auto dy_sign = math::sign(dy);

		if (dx == 0 && dy == 0) {
			put_char(p1, ch);
			return;
		}

		if (dx == 0) {
			assert(dy_sign != 0);

			const auto step = math::Vector2i::y_versor() * dy_sign;
			draw_line_with_step(p1, p2, step, ch);

			return;
		}

		if (dy == 0) {
			assert(dx_sign != 0);

			const auto step = math::Vector2i::x_versor() * dx_sign;
			draw_line_with_step(p1, p2, step, ch);

			return;
		}

		const auto dx_abs = std::abs(dx);
		const auto dy_abs = std::abs(dy);

		assert(dx_abs != 0 && dy_abs != 0);

		if (auto error = 0; dx_abs >= dy_abs) {
			for (auto [x, y] = p1.elements(); x != p2.x(); x += dx_sign, error += 2 * dy_abs) {
				if (error >= dx_abs) {
					error -= 2 * dx_abs;
					y += dy_sign;
				}

				put_char({ x, y }, ch);
			}
		} else {
			for (auto [x, y] = p1.elements(); y != p2.y(); y += dy_sign, error += 2 * dx_abs) {
				if (error >= dy_abs) {
					error -= 2 * dy_abs;
					x += dx_sign;
				}

				put_char({ x, y }, ch);
			}
		}

		put_char(p2, ch);
	}

	void draw_line_with_step(const math::Vector2i begin, const math::Vector2i end,
							 const math::Vector2i step, const CHAR_INFO ch)
	{
		for (auto point = begin; point != end; point += step) {
			put_char(point, ch);
		}

		put_char(end, ch);
	}
private:
	const ScreenBufferSize m_size;
	std::vector<CHAR_INFO> m_data{};
};
