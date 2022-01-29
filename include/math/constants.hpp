#include <numbers>
#include <concepts>

namespace math
{
template <std::floating_point T>
constexpr T pi = std::numbers::pi_v<T>;

template <std::floating_point T>
constexpr T sqrt2 = std::numbers::sqrt2_v<T>;

template <std::floating_point T>
constexpr T sqrt3 = std::numbers::sqrt3_v<T>;
} // namespace math
