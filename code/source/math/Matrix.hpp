
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

template < typename T, std::size_t Rows, std::size_t Columns >
	requires std::is_arithmetic_v< T >
class MatrixBase
{
	static constexpr std::size_t Size = Rows * Columns;
	static constexpr std::size_t Alignment = AlignmentSelectorV< T >;

public:
	using ValueType = T;

	constexpr MatrixBase() noexcept = default;

	template < typename... Args >
		requires( sizeof...( Args ) == Size )
	constexpr MatrixBase( Args&&... args ) noexcept
		: m_data{ std::forward< Args >( args )... }
	{ }

	constexpr std::optional< T > at( std::size_t idx ) const noexcept
	{
		if( idx < Size )
		{
			return data[ idx ];
		}

		return {};
	}

	constexpr std::optional< T > at( std::size_t column, std::size_t row ) const noexcept { return at( column * row ); }

	constexpr auto& data() noexcept { return m_data; }
	constexpr auto& data() const noexcept { return m_data; }

	static constexpr std::size_t size() noexcept { return Size; }
	static constexpr std::size_t rows() noexcept { return Rows; }
	static constexpr std::size_t columns() noexcept { return Columns; }

protected:
	alignas( Alignment ) std::array< T, Size > m_data;
};

template < typename T >
class Matrix2x2 : public MatrixBase< T, 2u, 2u >
{
	using Parent = MatrixBase< T, 2u, 2u >;

public:
	using Parent::Parent;

	/// Constructs a 2x2 matrix with all elements initialized to the same value
	explicit constexpr Matrix2x2( T v ) noexcept
		: Parent{ v, v, v, v }
	{ }

	/// Performs matrix by matrix multiplication
	constexpr Matrix2x2 operator*( const Matrix2x2& other ) const noexcept
	{
		auto& self = *this; // Reference to the current matrix instance

		return Matrix2x2( self.m_data[ 0 ] * other.m_data[ 0 ] + self.m_data[ 1 ] * other.m_data[ 2 ],
						  self.m_data[ 0 ] * other.m_data[ 1 ] + self.m_data[ 1 ] * other.m_data[ 3 ],
						  self.m_data[ 2 ] * other.m_data[ 0 ] + self.m_data[ 3 ] * other.m_data[ 2 ],
						  self.m_data[ 2 ] * other.m_data[ 1 ] + self.m_data[ 3 ] * other.m_data[ 3 ] );
	}
};

template < typename T >
class Matrix3x3 : public MatrixBase< T, 3u, 3u >
{
	using Parent = MatrixBase< T, 3u, 3u >;

public:
	using Parent::Parent;

	/// Constructs a 3x3 matrix with all elements initialized to the same value
	explicit constexpr Matrix3x3( T v ) noexcept
		: Parent{ v, v, v, v, v, v, v, v, v }
	{ }
};

template < typename T >
class Matrix4x4 : public MatrixBase< T, 4u, 4u >
{
	using Parent = MatrixBase< T, 4u, 4u >;

public:
	using Parent::Parent;

	/// Constructs a 4x4 matrix with all elements initialized to the same value
	explicit constexpr Matrix4x4( T v ) noexcept
		: Parent{ v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v }
	{ }
};

/**
 * @brief Generic Matrix class
 * 
 * @tparam T The type of the elements in the matrix.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Columns The number of columns in the matrix.
 */
template < typename T, std::size_t Rows, std::size_t Columns >
class Matrix : MatrixBase< T, Rows, Columns >
{
	using Parent = MatrixBase< T, Rows, Columns >;

public:
	using Parent::MatrixBase;

	/// Constructs a matrix with all elements initialized to the same value
	explicit constexpr Matrix( T v ) noexcept { std::ranges::fill( this->m_data, v ); }
};

} // namespace PBL::Math
#endif //I2C_MATH_MATRIX_HPP__
