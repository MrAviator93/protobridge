#ifndef I2C_MATH_MATRIX_2X2_HPP__
#define I2C_MATH_MATRIX_2X2_HPP__

#include "MatrixBase.hpp"

// // C
// extern "C" {
// #include <immintrin.h>
// #include <emmintrin.h>
// #include <xmmintrin.h>
// }

namespace pbl::math
{

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

	/// Performs matrix by matrix summation
	constexpr Matrix2x2 operator+( const Matrix2x2& other ) const noexcept;

	/// TBW
	constexpr Matrix2x2 operator+=( const Matrix2x2& other ) noexcept;

	/// TBW
	constexpr Matrix2x2 operator+( const T scalar ) const noexcept;

	/// TBW
	constexpr Matrix2x2 operator+=( const T scalar ) noexcept;

	/// Performs matrix by matrix substraction
	constexpr Matrix2x2 operator-( const Matrix2x2& other ) const noexcept;

	/// TBW
	constexpr Matrix2x2 operator-( const T scalar ) const noexcept;

	/// TBW
	constexpr Matrix2x2 operator-=( const T scalar ) noexcept;

	/// Performs matrix by matrix multiplication
	constexpr Matrix2x2 operator*( const Matrix2x2& other ) const noexcept;

	/// TBW
	constexpr Matrix2x2 operator*=( const Matrix2x2& other ) noexcept;

	/// Performs matrix by scalar multiplication
	constexpr Matrix2x2 operator*( const T scalar ) const noexcept;

	/// TBW
	constexpr Matrix2x2 operator*=( const T scalar ) noexcept;
};

using Matrix2x2f = Matrix2x2< float >;
using Matrix2x2d = Matrix2x2< double >;

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+( const Matrix2x2< T >& other ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] + other.m_data[ 0 ],
					  this->m_data[ 1 ] + other.m_data[ 1 ],
					  this->m_data[ 2 ] + other.m_data[ 2 ],
					  this->m_data[ 3 ] + other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+=( const Matrix2x2& other ) noexcept
{
	this->m_data[ 0 ] + other.m_data[ 0 ];
	this->m_data[ 1 ] + other.m_data[ 1 ];
	this->m_data[ 2 ] + other.m_data[ 2 ];
	this->m_data[ 3 ] + other.m_data[ 3 ];
	return *this;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+( const T scalar ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] + scalar,
					  this->m_data[ 1 ] + scalar,
					  this->m_data[ 2 ] + scalar,
					  this->m_data[ 3 ] + scalar );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+=( const T scalar ) noexcept
{
	this->m_data[ 0 ] + scalar;
	this->m_data[ 1 ] + scalar;
	this->m_data[ 2 ] + scalar;
	this->m_data[ 3 ] + scalar;
	return *this;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator-( const Matrix2x2< T >& other ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] - other.m_data[ 0 ],
					  this->m_data[ 1 ] - other.m_data[ 1 ],
					  this->m_data[ 2 ] - other.m_data[ 2 ],
					  this->m_data[ 3 ] - other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator-( const T scalar ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] - scalar,
					  this->m_data[ 1 ] - scalar,
					  this->m_data[ 2 ] - scalar,
					  this->m_data[ 3 ] - scalar );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator-=( const T scalar ) noexcept
{
	this->m_data[ 0 ] - scalar;
	this->m_data[ 1 ] - scalar;
	this->m_data[ 2 ] - scalar;
	this->m_data[ 3 ] - scalar;
	return *this;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*( const Matrix2x2< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix2x2( self.m_data[ 0 ] * other.m_data[ 0 ] + self.m_data[ 1 ] * other.m_data[ 2 ],
					  self.m_data[ 0 ] * other.m_data[ 1 ] + self.m_data[ 1 ] * other.m_data[ 3 ],
					  self.m_data[ 2 ] * other.m_data[ 0 ] + self.m_data[ 3 ] * other.m_data[ 2 ],
					  self.m_data[ 2 ] * other.m_data[ 1 ] + self.m_data[ 3 ] * other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*=( const Matrix2x2& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] = self.m_data[ 0 ] * other.m_data[ 0 ] + self.m_data[ 1 ] * other.m_data[ 2 ];
	self.m_data[ 1 ] = self.m_data[ 0 ] * other.m_data[ 1 ] + self.m_data[ 1 ] * other.m_data[ 3 ];
	self.m_data[ 2 ] = self.m_data[ 2 ] * other.m_data[ 0 ] + self.m_data[ 3 ] * other.m_data[ 2 ];
	self.m_data[ 3 ] = self.m_data[ 2 ] * other.m_data[ 1 ] + self.m_data[ 3 ] * other.m_data[ 3 ];
	return self;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*( T scalar ) const noexcept
{
	// const auto& self = *this;

	// if( self.isAligned() && IsIntFloatOrDouble< T > )
	// {
	// 	Matrix2x2< float > result{};

	// 	if constexpr( std::is_same_v< T, int > )
	// 	{
	// 		__m128i scalarVector = _mm_set1_epi32( scalar );
	// 		__m128i row1 = _mm_load_si128( reinterpret_cast< const __m128i* >( self.m_data.data() ) );

	// 		__m128i resultVector = _mm_mullo_epi32( row1, scalarVector );
	// 		_mm_store_si128( reinterpret_cast< __m128i* >( result.m_data.data() ), resultVector );
	// 	}
	// 	else if constexpr( std::is_same_v< T, float > )
	// 	{
	// 		__m128 scalarVector = _mm_set1_ps( scalar );
	// 		__m128 row1 = _mm_load_ps( self.m_data.data() );

	// 		__m128 resultVector = _mm_mul_ps( row1, scalarVector );
	// 		_mm_store_ps( reinterpret_cast< float* >( result.m_data.data() ), resultVector );
	// 	}
	// 	else if constexpr( std::is_same_v< T, double > )
	// 	{
	// 		__m128d scalarVector1 = _mm_set1_pd( scalar );
	// 		__m128d row1 = _mm_load_pd( self.m_data.data() );
	// 		__m128d row2 = _mm_load_pd( self.m_data.data() + 2 );

	// 		__m128d resultVector1 = _mm_mul_pd( row1, scalarVector1 );
	// 		__m128d resultVector2 = _mm_mul_pd( row2, scalarVector1 );

	// 		_mm_store_pd( reinterpret_cast< double* >( result.m_data.data() ), resultVector1 );
	// 		_mm_store_pd( reinterpret_cast< double* >( result.m_data.data() + 2 ), resultVector2 );
	// 	}

	// 	return result;
	// }
	// else
	// {
	// 	return Matrix2x2( self.m_data[ 0 ] * scalar,
	// 					  self.m_data[ 1 ] * scalar,
	// 					  self.m_data[ 2 ] * scalar,
	// 					  self.m_data[ 3 ] * scalar );
	// }

	return Matrix2x2( this->m_data[ 0 ] * scalar,
					  this->m_data[ 1 ] * scalar,
					  this->m_data[ 2 ] * scalar,
					  this->m_data[ 3 ] * scalar );
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*=( const T scalar ) noexcept
{
	this->m_data[ 0 ] * scalar;
	this->m_data[ 1 ] * scalar;
	this->m_data[ 2 ] * scalar;
	this->m_data[ 3 ] * scalar;
	return *this;
}

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_2X2_HPP__
