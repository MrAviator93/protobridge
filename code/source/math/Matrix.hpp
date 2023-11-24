
#ifndef I2C_MATH_MATRIX_HPP__
#define I2C_MATH_MATRIX_HPP__

#include "Math.hpp"
#include "Linear.hpp"

// C++
#include <array>
#include <optional>
#include <algorithm>
#include <type_traits>

namespace PBL::Math
{

// TODO: Actually matrixes could be also booleans,
// integers, etc, etc.

template < typename T, std::size_t Rows, std::size_t Columns >
class MatrixBase
{
	inline constexpr std::size_t Size = Rows * Columns;

public:
	using ValueType = T;

	static constexpr std::size_t size() const noexcept { return Size; }
	static constexpr std::size_t rows() const noexcept { return Rows; }
	static constexpr std::size_t columns() const noexcept { return Columns; }

protected:
	std::array< T, Size > m_data;
};

template < typename T >
	requires std::is_floating_point_v< T >
class Matrix2x2
{
public:
	Matrix2x2() = default;

	explicit constexpr Matrix2x2( T v ) noexcept
		: m_data{ v, v, v, v }
	{ }

	constexpr Matrix2x2( T m00, T m01, T m10, T m11 ) noexcept
		: m_data{ { m00, m01, m10, m11 } }
	{ }

	constexpr std::optional< T > at( std::size_t idx ) const noexcept
	{
		if( idx < size() )
		{
			return data[ idx ];
		}

		return {};
	}

	constexpr std::optional< T > at( std::size_t column, std::size_t row ) const noexcept { return at( column * row ); }

	constexpr auto& data() noexcept { return m_data; }
	constexpr auto& data() const noexcept { return m_data; }

	static constexpr std::size_t size() const noexcept { return 4; }
	static constexpr std::size_t rows() const noexcept { return 2; }
	static constexpr std::size_t columns() const noexcept { return 2; }

private:
	std::array< T, 4 > m_data;
};

template < typename T >
	requires std::is_floating_point_v< T >
class Matrix3x3
{
public:
	Matrix3x3() noexcept = default;

	explicit constexpr Matrix3x3( T v ) noexcept
		: m_data{ v, v, v, v, v, v, v, v, v }
	{ }

	constexpr Matrix3x3( T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22 ) noexcept
		: m_data{ m00, m01, m02, m10, m11, m12, m20, m21, m22 }
	{ }

	constexpr auto& data() noexcept { return m_data; }
	constexpr auto& data() const noexcept { return m_data; }

	static constexpr std::size_t size() const noexcept { return 9; }
	static constexpr std::size_t rows() const noexcept { return 3; }
	static constexpr std::size_t columns() const noexcept { return 3; }

private:
	std::array< T, 9 > m_data;
};

template < typename T >
	requires std::is_floating_point_v< T >
class Matrix4x4
{
public:
	Matrix3x3() noexcept = default;

	explicit constexpr Matrix4x4( T v ) noexcept
		: m_data{ v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v }
	{ }

	constexpr Matrix4x4( T m00,
						 T m01,
						 T m02,
						 T m03,
						 T m10,
						 T m11,
						 T m12,
						 T m13,
						 T m20,
						 T m21,
						 T m22,
						 T m23,
						 T m30,
						 T m31,
						 T m32,
						 T m33 ) noexcept
		: m_data{ m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 }
	{ }

	constexpr auto& data() noexcept { return m_data; }
	constexpr auto& data() const noexcept { return m_data; }

	static constexpr std::size_t size() const noexcept { return 16; }
	static constexpr std::size_t rows() const noexcept { return 4; }
	static constexpr std::size_t columns() const noexcept { return 4; }

private:
	std::array< T, 16 > m_data;
};

template < typename T, std::size_t Rows, std::size_t Columns >
	requires std::is_floating_point_v< T >
class Matrix
{
	inline constexpr std::size_t Size = Rows * Columns;

public:
	Matrix() noexcept = default;

	explicit constexpr Matrix( T v ) noexcept { std::ranges::fill( m_data, v ); }

	static constexpr std::size_t size() const noexcept { return Size; }
	static constexpr std::size_t rows() const noexcept { return Rows; }
	static constexpr std::size_t columns() const noexcept { return Columns; }

private:
	std::array< T, Size > m_data;
};

} // namespace PBL::Math
#endif //I2C_MATH_MATRIX_HPP__
