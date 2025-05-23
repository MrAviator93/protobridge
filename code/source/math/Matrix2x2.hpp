#ifndef I2C_MATH_MATRIX_2X2_HPP__
#define I2C_MATH_MATRIX_2X2_HPP__

#include "MatrixBase.hpp"

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

	/// Adds another matrix to this matrix in place
	constexpr Matrix2x2& operator+=( const Matrix2x2& other ) noexcept;

	/// Adds a scalar to each element of the matrix
	constexpr Matrix2x2 operator+( const T scalar ) const noexcept;

	/// Adds a scalar to each element of this matrix in place
	constexpr Matrix2x2& operator+=( const T scalar ) noexcept;

	/// Performs matrix by matrix substraction
	constexpr Matrix2x2 operator-( const Matrix2x2& other ) const noexcept;

	/// Subtracts a scalar from each element of the matrix
	constexpr Matrix2x2 operator-( const T scalar ) const noexcept;

	/// Subtracts a scalar from each element of this matrix in place
	constexpr Matrix2x2& operator-=( const T scalar ) noexcept;

	/// Performs matrix by matrix multiplication
	constexpr Matrix2x2 operator*( const Matrix2x2& other ) const noexcept;

	/// Multiplies this matrix by another matrix in place
	constexpr Matrix2x2& operator*=( const Matrix2x2& other ) noexcept;

	/// Multiplies each element of the matrix by a scalar
	constexpr Matrix2x2 operator*( const T scalar ) const noexcept;

	/// Multiplies each element of this matrix by a scalar in place
	constexpr Matrix2x2& operator*=( const T scalar ) noexcept;
};

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator+( const Matrix2x2< T >& other ) const noexcept
{
	return Matrix2x2( this->m_data[ 0 ] + other.m_data[ 0 ],
					  this->m_data[ 1 ] + other.m_data[ 1 ],
					  this->m_data[ 2 ] + other.m_data[ 2 ],
					  this->m_data[ 3 ] + other.m_data[ 3 ] );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator+=( const Matrix2x2& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] + other.m_data[ 0 ];
	self.m_data[ 1 ] + other.m_data[ 1 ];
	self.m_data[ 2 ] + other.m_data[ 2 ];
	self.m_data[ 3 ] + other.m_data[ 3 ];
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
constexpr Matrix2x2< T >& Matrix2x2< T >::operator+=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] += scalar;
	self.m_data[ 1 ] += scalar;
	self.m_data[ 2 ] += scalar;
	self.m_data[ 3 ] += scalar;
	return self;
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
constexpr Matrix2x2< T >& Matrix2x2< T >::operator-=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] -= scalar;
	self.m_data[ 1 ] -= scalar;
	self.m_data[ 2 ] -= scalar;
	self.m_data[ 3 ] -= scalar;
	return self;
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
constexpr Matrix2x2< T >& Matrix2x2< T >::operator*=( const Matrix2x2< T >& other ) noexcept
{
	auto& self = *this;
	const auto& sdata = self.m_data;
	const auto& mdata = other.m_data;

	// Temporary variables to hold the results
	const T m0 = sdata[ 0 ] * mdata[ 0 ] + sdata[ 1 ] * mdata[ 2 ];
	const T m1 = sdata[ 0 ] * mdata[ 1 ] + sdata[ 1 ] * mdata[ 3 ];
	const T m2 = sdata[ 2 ] * mdata[ 0 ] + sdata[ 3 ] * mdata[ 2 ];
	const T m3 = sdata[ 2 ] * mdata[ 1 ] + sdata[ 3 ] * mdata[ 3 ];

	// Assign the results back to self
	self.m_data[ 0 ] = m0;
	self.m_data[ 1 ] = m1;
	self.m_data[ 2 ] = m2;
	self.m_data[ 3 ] = m3;

	return self;
}

template < typename T >
constexpr Matrix2x2< T > Matrix2x2< T >::operator*( T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix2x2(
		self.m_data[ 0 ] * scalar, self.m_data[ 1 ] * scalar, self.m_data[ 2 ] * scalar, self.m_data[ 3 ] * scalar );
}

template < typename T >
constexpr Matrix2x2< T >& Matrix2x2< T >::operator*=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] *= scalar;
	self.m_data[ 1 ] *= scalar;
	self.m_data[ 2 ] *= scalar;
	self.m_data[ 3 ] *= scalar;
	return self;
}

using Matrix2x2i = Matrix2x2< int >;
using Matrix2x2f = Matrix2x2< float >;
using Matrix2x2d = Matrix2x2< double >;

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_2X2_HPP__
