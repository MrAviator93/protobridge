
#ifndef I2C_MATH_MATRIX_HPP__
#define I2C_MATH_MATRIX_HPP__

#include "Math.hpp"
#include "Linear.hpp"

// C++
#include <array>
#include <optional>
#include <algorithm>
#include <type_traits>

// C++
extern "C" {
#include <emmintrin.h>
}

namespace PBL::Math
{

template < typename T >
concept IsIntFloatOrDouble = std::is_same_v< T, int > || std::is_same_v< T, float > || std::is_same_v< T, double >;

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
	constexpr bool isAligned() const noexcept
	{
		auto ptr = reinterpret_cast< std::uintptr_t >( m_data.data() );
		return ptr % Alignment == 0;
	}

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
	constexpr Matrix2x2 operator*( const Matrix2x2& other ) const noexcept;
};

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*( const Matrix2x2< T >& other ) const noexcept
{
	auto& self = *this;

	if constexpr( isAligned() && IsIntFloatOrDouble< T > )
	{
		Matrix2x2 result;

		if constexpr( std::is_same_v< T, int > )
		{
			__m128i row1 = _mm_load_si128( reinterpret_cast< const __m128i* >( self.m_data ) );
			__m128i row2 = _mm_load_si128( reinterpret_cast< const __m128i* >( self.m_data + 2 ) );

			for( int i = 0; i < 2; ++i )
			{
				__m128i brod1 = _mm_set1_epi32( other.m_data[ i * 2 ] );
				__m128i brod2 = _mm_set1_epi32( other.m_data[ i * 2 + 1 ] );

				// Perform element-wise multiplication and sum
				__m128i mul1 = _mm_mullo_epi32( brod1, row1 );
				__m128i mul2 = _mm_mullo_epi32( brod2, row2 );
				__m128i row = _mm_add_epi32( mul1, mul2 );

				_mm_store_si128( reinterpret_cast< __m128i* >( result.m_data + i * 2 ), row );
			}
		}
		else if constexpr( std::is_same_v< T, float > )
		{
			__m128 row1 = _mm_load_ps( self.m_data );
			__m128 row2 = _mm_load_ps( self.m_data + 2 );

			for( int i{}; i < 2; ++i )
			{
				__m128 brod1 = _mm_set1_ps( other.m_data[ i * 2 ] );
				__m128 brod2 = _mm_set1_ps( other.m_data[ i * 2 + 1 ] );
				__m128 row = _mm_add_ps( _mm_mul_ps( brod1, row1 ), _mm_mul_ps( brod2, row2 ) );
				_mm_store_ps( result.m_data + i * 2, row );
			}

			return result;
		}
		else if constexpr( std::is_same_v< T, double > )
		{

			__m128d row1 = _mm_load_pd( self.m_data );
			__m128d row2 = _mm_load_pd( self.m_data + 2 );

			for( int i = 0; i < 2; ++i )
			{
				__m128d brod1 = _mm_set1_pd( other.m_data[ i * 2 ] );
				__m128d brod2 = _mm_set1_pd( other.m_data[ i * 2 + 1 ] );
				__m128d row = _mm_add_pd( _mm_mul_pd( brod1, row1 ), _mm_mul_pd( brod2, row2 ) );
				_mm_store_pd( result.m_data + i * 2, row );
			}
		}

		return result;
	}
	else
	{
		return Matrix2x2( self.m_data[ 0 ] * other.m_data[ 0 ] + self.m_data[ 1 ] * other.m_data[ 2 ],
						  self.m_data[ 0 ] * other.m_data[ 1 ] + self.m_data[ 1 ] * other.m_data[ 3 ],
						  self.m_data[ 2 ] * other.m_data[ 0 ] + self.m_data[ 3 ] * other.m_data[ 2 ],
						  self.m_data[ 2 ] * other.m_data[ 1 ] + self.m_data[ 3 ] * other.m_data[ 3 ] );
	}
}

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
