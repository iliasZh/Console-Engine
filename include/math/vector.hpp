#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <execution>
#include <functional>
#include <numeric>

#include <fmt/ranges.h>

namespace math
{
template <class T>
concept int_least32 = std::same_as<T, int32_t> || std::same_as<T, int64_t>;

template <class T>
concept signed_number = std::floating_point<T> || int_least32<T>;

template <signed_number Elem, size_t Dims>
class Vector
{
private: // definitions && static asserts
	static_assert(2u <= Dims && Dims <= 4u, "only 2, 3 and 4-dimensional vectors are supported");

	static constexpr Elem zero = static_cast<Elem>(0);
	static constexpr Elem one  = static_cast<Elem>(1);
public: // methods
	using elem_list = std::array<Elem, Dims>;

	[[nodiscard]] constexpr Vector(const elem_list elems) noexcept
		: m_elems{ elems }
	{}

	[[nodiscard]] constexpr Vector(const Elem x, const Elem y) noexcept
		: m_elems{ x, y }
	{
		static_assert(Dims == 2u, "wrong number of components");
	}

	[[nodiscard]] constexpr Vector(const Elem x, const Elem y, const Elem z) noexcept
		: m_elems{ x, y, z }
	{
		static_assert(Dims == 3u, "wrong number of components");
	}

	[[nodiscard]] constexpr Vector(const Elem x, const Elem y, const Elem z, const Elem w) noexcept
		: m_elems{ x, y, z, w }
	{
		static_assert(Dims == 4u, "wrong number of components");
	}

	[[nodiscard]] constexpr Elem dot(const Vector& other) const noexcept
	{
		return std::transform_reduce(std::execution::unseq, m_elems.cbegin(), m_elems.cend(),
									 other.m_elems.cbegin(), zero);
	}

	[[nodiscard]] constexpr Vector cross(const Vector& other) const noexcept
	{
		static_assert(Dims == 3u, "cross product is defined for 3-dimensional vectors only");

		const auto& [ax, ay, az] = m_elems;
		const auto& [bx, by, bz] = other.m_elems;

		return { { ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx } };
	}

	[[nodiscard]] constexpr Elem length_squared() const noexcept
	{
		return dot(*this);
	}

	[[nodiscard]] Elem length() const noexcept
	{
		static_assert(std::floating_point<Elem>,
					  "length method is disabled for non-floating point vectors");

		return std::sqrt(length_squared());
	}

	Vector& normalize() noexcept
	{
		static_assert(std::floating_point<Elem>,
					  "normalize method is disabled for non-floating point vectors");

		return operator/=(length());
	}

	[[nodiscard]] constexpr Vector operator-() const noexcept
	{
		elem_list elems{};
		std::transform(std::execution::unseq, m_elems.cbegin(), m_elems.cend(), elems.begin(),
					   std::negate{});

		return Vector{ elems };
	}

	constexpr Vector& operator+=(const Vector& other) noexcept
	{
		unsequenced_transform(std::plus{}, other);

		return *this;
	}

	[[nodiscard]] constexpr Vector operator+(const Vector& other) const noexcept
	{
		Vector result{ *this };
		result += other;

		return result;
	}

	constexpr Vector& operator-=(const Vector& other) noexcept
	{
		unsequenced_transform(std::minus{}, other);

		return *this;
	}

	[[nodiscard]] constexpr Vector operator-(const Vector& other) const noexcept
	{
		Vector result{ *this };
		result -= other;

		return result;
	}

	constexpr Vector& operator*=(const Elem scalar) noexcept
	{
		unsequenced_transform([scalar](const Elem& e) {
			return e * scalar;
		});

		return *this;
	}

	[[nodiscard]] constexpr Vector operator*(const Elem scalar) const noexcept
	{
		Vector result{ *this };
		result *= scalar;

		return result;
	}

	constexpr Vector& operator/=(const Elem scalar) noexcept
	{
		static_assert(std::floating_point<Elem>,
					  "division is disabled for non-floating point vectors");

		assert(scalar != zero);
		const Elem inverse = one / scalar;

		return operator*=(inverse);
	}

	[[nodiscard]] constexpr Vector operator/(const Elem scalar) const noexcept
	{
		static_assert(std::floating_point<Elem>,
					  "division is disabled for non-floating point vectors");

		Vector result{ *this };
		result /= scalar;

		return result;
	}

	constexpr const elem_list& elements() const noexcept
	{
		return m_elems;
	}

	[[nodiscard]] constexpr Elem& operator[](const size_t i) noexcept
	{
		return m_elems[i];
	}

	[[nodiscard]] constexpr const Elem& operator[](const size_t i) const noexcept
	{
		return m_elems[i];
	}

	[[nodiscard]] constexpr const Elem& x() const noexcept
	{
		return m_elems[0];
	}

	[[nodiscard]] constexpr const Elem& y() const noexcept
	{
		return m_elems[1];
	}

	[[nodiscard]] constexpr const Elem& z() const noexcept
	{
		static_assert(Dims == 3u || Dims == 4u,
					  "there is no z-component in a 2-dimensional vector");

		return m_elems[2];
	}

	[[nodiscard]] constexpr const Elem& w() const noexcept
	{
		static_assert(Dims == 4u, "there is no w-component in a 2- or 3-dimensional vector");

		return m_elems[3];
	}

	[[nodiscard]] static constexpr Vector null() noexcept
	{
		elem_list nulls{};
		nulls.fill(zero);

		return { nulls };
	}

	[[nodiscard]] static constexpr Vector x_versor() noexcept
	{
		auto v = null();
		v[0]   = one;

		return v;
	}

	[[nodiscard]] static constexpr Vector y_versor() noexcept
	{
		auto v = null();
		v[1]   = one;

		return v;
	}

	[[nodiscard]] static constexpr Vector z_versor() noexcept
	{
		auto v = null();
		v[2]   = one;

		return v;
	}

	[[nodiscard]] static constexpr Vector w_versor() noexcept
	{
		auto v = null();
		v[3]   = one;

		return v;
	}

	[[nodiscard]] std::string to_string(const std::string_view delim = ", ") const
	{
		return fmt::format("({})", fmt::join(m_elems, delim));
	}

	/// Can be used only on floating point vectors.
	/// Returns an integer vector with every element rounded using `std::floor`.
	template <int_least32 IntElem>
	[[nodiscard]] Vector<IntElem, Dims> floor() const
	{
		static_assert(std::floating_point<Elem>,
					  "cast to integer vector is defined for floating point vectors only");

		typename Vector<IntElem, Dims>::elem_list elems{};

		for (auto i = 0u; i < Dims; ++i) {
			elems[i] = static_cast<IntElem>(std::floor(m_elems[i]));
		}

		return { elems };
	}
private: // methods
	/// Unsequenced is equivalent to sequenced for now, but maybe it will be beneficial in the
	/// future.
	constexpr void unsequenced_transform(auto binary_op, const Vector& rhs)
	{
		std::transform(std::execution::unseq, m_elems.cbegin(), m_elems.cend(),
					   rhs.m_elems.cbegin(), m_elems.begin(), binary_op);
	}

	constexpr void unsequenced_transform(auto unary_op)
	{
		std::transform(std::execution::unseq, m_elems.cbegin(), m_elems.cend(), m_elems.begin(),
					   unary_op);
	}
private: // data
	elem_list m_elems{};
};

using Vector2f = Vector<float, 2u>;
using Vector3f = Vector<float, 3u>;
using Vector4f = Vector<float, 4u>;

using Vector2d = Vector<double, 2u>;
using Vector3d = Vector<double, 3u>;
using Vector4d = Vector<double, 4u>;

using Vector2i = Vector<int32_t, 2u>;
using Vector3i = Vector<int32_t, 3u>;
using Vector4i = Vector<int32_t, 4u>;

using Vector2i64 = Vector<int64_t, 2u>;
using Vector3i64 = Vector<int64_t, 3u>;
using Vector4i64 = Vector<int64_t, 4u>;


template <signed_number Elem>
[[nodiscard]] constexpr inline Vector<Elem, 3u>
cross_product(const Vector<Elem, 3u>& a, // NOLINT
			  const Vector<Elem, 3u>& b) noexcept // NOLINT
{
	return a.cross(b);
}

template <signed_number Elem, size_t Dims>
[[nodiscard]] constexpr inline Elem dot_product(const Vector<Elem, Dims>& a, // NOLINT
												const Vector<Elem, Dims>& b) noexcept // NOLINT
{
	return a.dot(b);
}
} // namespace math
