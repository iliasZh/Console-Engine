#pragma once

#include <concepts>

namespace math
{
template <class T>
concept int_least32 = std::same_as<T, int32_t> || std::same_as<T, int64_t>;

template <class T>
concept uint_least32 = std::same_as<T, uint32_t> || std::same_as<T, uint64_t>;

static_assert(uint_least32<std::size_t>, "std::size_t should be 32 or 64 bits");

template <class T>
concept integral_least32 = int_least32<T> || uint_least32<T>;

template <class T>
concept signed_number = std::floating_point<T> || int_least32<T>;
} // namespace math
