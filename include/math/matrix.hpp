#pragma once

#include "vector.hpp"
#include <execution>
#include <numbers>

namespace math
{
template <std::floating_point Elem, size_t Dims>
class Matrix
{
private: // definitions & static asserts
	static_assert(2u <= Dims && Dims <= 4u, "only 2x2, 3x3 and 4x4 matrices are supported");

	static constexpr Elem zero = static_cast<Elem>(0.0f);
	static constexpr Elem one  = static_cast<Elem>(1.0f);

	static constexpr size_t number_of_elements = Dims * Dims;

	using VectorDims = Vector<Elem, Dims>;
	using Vector3	 = Vector<Elem, 3u>;
public: // methods
	using elem_list = std::array<Elem, number_of_elements>;

	[[nodiscard]] constexpr Matrix(const elem_list elems) noexcept
		: m_elems{ elems }
	{}

	[[nodiscard]] constexpr Matrix operator*(const Matrix& other)
	{
		Matrix product = null();

		// triple!
		for (size_t i = 0u; i < Dims; ++i) {
			for (size_t j = 0u; j < Dims; ++j) {
				for (size_t k = 0u; k < Dims; ++k) {
					product.elem_at(i, j) += elem_at(i, k) * other.elem_at(k, j);
				}
			}
		}

		return product;
	}

	constexpr Matrix& operator+=(const Matrix& other) noexcept
	{
		unsequenced_transform(std::plus{}, other);
		return *this;
	}

	[[nodiscard]] constexpr Matrix operator+(const Matrix& other) const noexcept
	{
		Matrix sum{ *this };
		sum += other;

		return sum;
	}

	constexpr Matrix& operator-=(const Matrix& other) noexcept
	{
		unsequenced_transform(std::minus{}, other);
		return *this;
	}

	[[nodiscard]] constexpr Matrix operator-(const Matrix& other) const noexcept
	{
		Matrix difference{ *this };
		difference -= other;

		return difference;
	}

	constexpr Matrix& operator*=(const Elem scalar) noexcept
	{
		unsequenced_transform([scalar](const Elem& e) {
			return e * scalar;
		});

		return *this;
	}

	[[nodiscard]] constexpr Matrix operator*(const Elem scalar) const noexcept
	{
		Matrix product{ *this };
		product *= scalar;

		return product;
	}

	constexpr Matrix& operator/=(const Elem scalar) noexcept
	{
		assert(scalar != zero);
		const Elem inverse = one / scalar;

		return operator*=(inverse);
	}

	[[nodiscard]] constexpr Matrix operator/(const Elem scalar) const noexcept
	{
		Matrix quotient{ *this };
		quotient /= scalar;

		return quotient;
	}

	[[nodiscard]] constexpr VectorDims operator*(const VectorDims& vec) noexcept
	{
		typename VectorDims::elem_list product{};

		for (size_t i = 0u; i < Dims; ++i) {
			const auto row_begin = std::next(m_elems.cbegin(), Dims * i);
			const auto row_end	 = std::next(row_begin, Dims);

			product[i] = std::transform_reduce(std::execution::unseq, row_begin, row_end,
											   vec.elements().cbegin(), zero);
		}

		return { product };
	}

	[[nodiscard]] static constexpr Matrix null() noexcept
	{
		elem_list nulls{};
		nulls.fill(zero);

		return { nulls };
	}

	// A super ugly section starts here, enjoy!

	[[nodiscard]] static constexpr Matrix identity() noexcept
	{
		// Simple approach :)

		// clang-format off
		if constexpr (Dims == 2u) {
			return { {
				one,	zero,
				zero,	one,
			} };
		} else if constexpr (Dims == 3u) {
			return { {
				one,	zero,	zero,
				zero,	one,	zero,
				zero,	zero,	one,
			} };
		} else {
			return { {
				one,	zero,	zero,	zero,
				zero,	one,	zero,	zero,
				zero,	zero,	one,	zero,
				zero,	zero,	zero,	one,
			} };
		}
		// clang-format on
	}

	[[nodiscard]] static Matrix rotation_z(const Elem theta) noexcept
	{
		// Simple approach :)

		const Elem sin = std::sin(theta);
		const Elem cos = std::cos(theta);

		// clang-format off
		if constexpr (Dims == 2u) {
			return { {
				cos,	-sin,
				sin,	cos,
			} };
		} else if constexpr (Dims == 3u) {
			return { {
				cos,	-sin,	zero,
				sin,	cos,	zero,
				zero,	zero,	one,
			} };
		} else {
			return { {
				cos,	-sin,	zero,	zero,
				sin,	cos,	zero,	zero,
				zero,	zero,	one,	zero,
				zero,	zero,	zero,	one,
			} };
		}
		// clang-format on
	}

	[[nodiscard]] static Matrix rotation_y(const Elem theta) noexcept
	{
		static_assert(Dims != 2u, "this rotation is undefined for 2 dimensions");

		// Simple approach :)

		const Elem sin = std::sin(theta);
		const Elem cos = std::cos(theta);

		// clang-format off
		if constexpr (Dims == 3u) {
			return { {
				cos,	zero,	sin,
				zero,	one,	zero,
				-sin,	zero,	cos,
			} };
		} else {
			return { {
				cos,	zero,	sin,	zero,
				zero,	one,	zero,	zero,
				-sin,	zero,	cos,	zero,
				zero,	zero,	zero,	one,
			} };
		}
		// clang-format on
	}

	[[nodiscard]] static Matrix rotation_x(const Elem theta) noexcept
	{
		static_assert(Dims != 2u, "this rotation is undefined for 2 dimensions");

		// Simple approach :)

		const Elem sin = std::sin(theta);
		const Elem cos = std::cos(theta);

		// clang-format off
		if constexpr (Dims == 3u) {
			return { {
				one,	zero,	zero,
				zero,	cos,	-sin,
				zero,	sin,	cos,
			} };
		} else {
			return { {
				one,	zero,	zero,	zero,
				zero,	cos,	-sin,	zero,
				zero,	sin,	cos,	zero,
				zero,	zero,	zero,	one,
			} };
		}
		// clang-format on
	}

	[[nodiscard]] static constexpr Matrix translation(const Vector3& t) noexcept
	{
		static_assert(Dims == 4u, "translation matrix exists only in 4 dimensions");

		// clang-format off
		return { {
				one,	zero,	zero,	t.x(),
				zero,	one,	zero,	t.y(),
				zero,	zero,	one,	t.z(),
				zero,	zero,	zero,	one,
		} };
		// clang-format on
	}
private: // methods
	[[nodiscard]] constexpr Elem& elem_at(size_t row, size_t col) noexcept
	{
		return m_elems[Dims * row + col];
	}

	[[nodiscard]] constexpr const Elem& elem_at(size_t row, size_t col) const noexcept
	{
		return m_elems[Dims * row + col];
	}

	/// Unsequenced is equivalent to sequenced for now, but maybe it will be beneficial in the
	/// future.
	constexpr void unsequenced_transform(auto binary_op, const Matrix& other)
	{
		std::transform(std::execution::unseq, m_elems.cbegin(), m_elems.cend(),
					   other.m_elems.cbegin(), m_elems.begin(), binary_op);
	}

	constexpr void unsequenced_transform(auto unary_op)
	{
		std::transform(std::execution::unseq, m_elems.cbegin(), m_elems.cend(), m_elems.begin(),
					   unary_op);
	}
private: // data
	elem_list m_elems;
};

using Matrix2f = Matrix<float, 2u>;
using Matrix3f = Matrix<float, 3u>;
using Matrix4f = Matrix<float, 4u>;

using Matrix2d = Matrix<double, 2u>;
using Matrix3d = Matrix<double, 3u>;
using Matrix4d = Matrix<double, 4u>;
} // namespace math
