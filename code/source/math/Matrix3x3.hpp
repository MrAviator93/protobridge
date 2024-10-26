#ifndef I2C_MATH_MATRIX_3X3_HPP__
#define I2C_MATH_MATRIX_3X3_HPP__

#include "MatrixBase.hpp"

namespace pbl::math
{

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

	/// Performs matrix by matrix summation
	constexpr Matrix3x3 operator+( const Matrix3x3& other ) const noexcept;

	/// Adds another matrix to this matrix in place
	constexpr Matrix3x3& operator+=( const Matrix3x3& other ) noexcept;

	/// Adds a scalar to each element of the matrix
	constexpr Matrix3x3 operator+( const T scalar ) const noexcept;

	/// Adds a scalar to each element of this matrix in place
	constexpr Matrix3x3& operator+=( const T scalar ) noexcept;

	/// Performs matrix by matrix subtraction
	constexpr Matrix3x3 operator-( const Matrix3x3& other ) const noexcept;

	/// Subtracts a scalar from each element of the matrix
	constexpr Matrix3x3 operator-( const T scalar ) const noexcept;

	/// Subtracts a scalar from each element of this matrix in place
	constexpr Matrix3x3& operator-=( const T scalar ) noexcept;

	/// Performs matrix by matrix multiplication
	constexpr Matrix3x3 operator*( const Matrix3x3& other ) const noexcept;

	/// Multiplies this matrix by another matrix in place
	constexpr Matrix3x3& operator*=( const Matrix3x3& other ) noexcept;

	/// Multiplies each element of the matrix by a scalar
	constexpr Matrix3x3 operator*( const T scalar ) const noexcept;

	/// Multiplies each element of this matrix by a scalar in place
	constexpr Matrix3x3& operator*=( const T scalar ) noexcept;
};

using Matrix3x3f = Matrix3x3< float >;
using Matrix3x3d = Matrix3x3< double >;

template < typename T >
constexpr Matrix3x3< T > Matrix3x3< T >::operator+( const Matrix3x3< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix3x3{ self.m_data[ 0 ] + other.m_data[ 0 ],
					  self.m_data[ 1 ] + other.m_data[ 1 ],
					  self.m_data[ 2 ] + other.m_data[ 2 ],
					  self.m_data[ 3 ] + other.m_data[ 3 ],
					  self.m_data[ 4 ] + other.m_data[ 4 ],
					  self.m_data[ 5 ] + other.m_data[ 5 ],
					  self.m_data[ 6 ] + other.m_data[ 6 ],
					  self.m_data[ 7 ] + other.m_data[ 7 ],
					  self.m_data[ 8 ] + other.m_data[ 8 ] };
}

template < typename T >
constexpr Matrix3x3< T >& Matrix3x3< T >::operator+=( const Matrix3x3< T >& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] += other.m_data[ 0 ];
	self.m_data[ 1 ] += other.m_data[ 1 ];
	self.m_data[ 2 ] += other.m_data[ 2 ];
	self.m_data[ 3 ] += other.m_data[ 3 ];
	self.m_data[ 4 ] += other.m_data[ 4 ];
	self.m_data[ 5 ] += other.m_data[ 5 ];
	self.m_data[ 6 ] += other.m_data[ 6 ];
	self.m_data[ 7 ] += other.m_data[ 7 ];
	self.m_data[ 8 ] += other.m_data[ 8 ];
	return *this;
}

template < typename T >
constexpr Matrix3x3< T > Matrix3x3< T >::operator+( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix3x3{ self.m_data[ 0 ] + scalar,
					  self.m_data[ 1 ] + scalar,
					  self.m_data[ 2 ] + scalar,
					  self.m_data[ 3 ] + scalar,
					  self.m_data[ 4 ] + scalar,
					  self.m_data[ 5 ] + scalar,
					  self.m_data[ 6 ] + scalar,
					  self.m_data[ 7 ] + scalar,
					  self.m_data[ 8 ] + scalar };
}

template < typename T >
constexpr Matrix3x3< T >& Matrix3x3< T >::operator+=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] += scalar;
	self.m_data[ 1 ] += scalar;
	self.m_data[ 2 ] += scalar;
	self.m_data[ 3 ] += scalar;
	self.m_data[ 4 ] += scalar;
	self.m_data[ 5 ] += scalar;
	self.m_data[ 6 ] += scalar;
	self.m_data[ 7 ] += scalar;
	self.m_data[ 8 ] += scalar;
	return *this;
}

template < typename T >
constexpr Matrix3x3< T > Matrix3x3< T >::operator-( const Matrix3x3< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix3x3{ self.m_data[ 0 ] - other.m_data[ 0 ],
					  self.m_data[ 1 ] - other.m_data[ 1 ],
					  self.m_data[ 2 ] - other.m_data[ 2 ],
					  self.m_data[ 3 ] - other.m_data[ 3 ],
					  self.m_data[ 4 ] - other.m_data[ 4 ],
					  self.m_data[ 5 ] - other.m_data[ 5 ],
					  self.m_data[ 6 ] - other.m_data[ 6 ],
					  self.m_data[ 7 ] - other.m_data[ 7 ],
					  self.m_data[ 8 ] - other.m_data[ 8 ] };
}

template < typename T >
constexpr Matrix3x3< T > Matrix3x3< T >::operator-( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix3x3{ self.m_data[ 0 ] - scalar,
					  self.m_data[ 1 ] - scalar,
					  self.m_data[ 2 ] - scalar,
					  self.m_data[ 3 ] - scalar,
					  self.m_data[ 4 ] - scalar,
					  self.m_data[ 5 ] - scalar,
					  self.m_data[ 6 ] - scalar,
					  self.m_data[ 7 ] - scalar,
					  self.m_data[ 8 ] - scalar };
}

template < typename T >
constexpr Matrix3x3< T >& Matrix3x3< T >::operator-=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] -= scalar;
	self.m_data[ 1 ] -= scalar;
	self.m_data[ 2 ] -= scalar;
	self.m_data[ 3 ] -= scalar;
	self.m_data[ 4 ] -= scalar;
	self.m_data[ 5 ] -= scalar;
	self.m_data[ 6 ] -= scalar;
	self.m_data[ 7 ] -= scalar;
	self.m_data[ 8 ] -= scalar;
	return *this;
}

template < typename T >
constexpr Matrix3x3< T > Matrix3x3< T >::operator*( const Matrix3x3< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix3x3{ self.m_data[ 0 ] * other.m_data[ 0 ],
					  self.m_data[ 1 ] * other.m_data[ 1 ],
					  self.m_data[ 2 ] * other.m_data[ 2 ],
					  self.m_data[ 3 ] * other.m_data[ 3 ],
					  self.m_data[ 4 ] * other.m_data[ 4 ],
					  self.m_data[ 5 ] * other.m_data[ 5 ],
					  self.m_data[ 6 ] * other.m_data[ 6 ],
					  self.m_data[ 7 ] * other.m_data[ 7 ],
					  self.m_data[ 8 ] * other.m_data[ 8 ] };
}

template < typename T >
constexpr Matrix3x3< T >& Matrix3x3< T >::operator*=( const Matrix3x3< T >& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] *= other.m_data[ 0 ];
	self.m_data[ 1 ] *= other.m_data[ 1 ];
	self.m_data[ 2 ] *= other.m_data[ 2 ];
	self.m_data[ 3 ] *= other.m_data[ 3 ];
	self.m_data[ 4 ] *= other.m_data[ 4 ];
	self.m_data[ 5 ] *= other.m_data[ 5 ];
	self.m_data[ 6 ] *= other.m_data[ 6 ];
	self.m_data[ 7 ] *= other.m_data[ 7 ];
	self.m_data[ 8 ] *= other.m_data[ 8 ];
	return *this;
}

template < typename T >
constexpr Matrix3x3< T > Matrix3x3< T >::operator*( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix3x3{ self.m_data[ 0 ] * scalar,
					  self.m_data[ 1 ] * scalar,
					  self.m_data[ 2 ] * scalar,
					  self.m_data[ 3 ] * scalar,
					  self.m_data[ 4 ] * scalar,
					  self.m_data[ 5 ] * scalar,
					  self.m_data[ 6 ] * scalar,
					  self.m_data[ 7 ] * scalar,
					  self.m_data[ 8 ] * scalar };
}

template < typename T >
constexpr Matrix3x3< T >& Matrix3x3< T >::operator*=( const T scalar ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] *= scalar;
	self.m_data[ 1 ] *= scalar;
	self.m_data[ 2 ] *= scalar;
	self.m_data[ 3 ] *= scalar;
	self.m_data[ 4 ] *= scalar;
	self.m_data[ 5 ] *= scalar;
	self.m_data[ 6 ] *= scalar;
	self.m_data[ 7 ] *= scalar;
	self.m_data[ 8 ] *= scalar;
	return *this;
}

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_3X3_HPP__