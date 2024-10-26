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
constexpr Matrix2x2< T >& Matrix2x2< T >::operator*=( const Matrix2x2& other ) noexcept
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
	return Matrix2x2( this->m_data[ 0 ] * scalar,
					  this->m_data[ 1 ] * scalar,
					  this->m_data[ 2 ] * scalar,
					  this->m_data[ 3 ] * scalar );
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

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_2X2_HPP__
