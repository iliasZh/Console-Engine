#pragma once

#include <concepts>

namespace math
{
template <std::integral T>
[[nodiscard]] inline constexpr T sign(T number) noexcept
{
	constexpr T zero = static_cast<T>(0);

	return static_cast<T>(number > zero) - static_cast<T>(number < zero);
}
} // namespace math
