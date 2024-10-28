#ifndef I2C_MATH_MATRIX_6X6_HPP__
#define I2C_MATH_MATRIX_6X6_HPP__

#include "MatrixBase.hpp"

namespace pbl::math
{

template < typename T >
class Matrix6x6 : public MatrixBase< T, 6u, 6u >
{
	using Parent = MatrixBase< T, 6u, 6u >;

public:
	using Parent::Parent;

	/// Constructs a 6x6 matrix with all elements initialized to the same value
	explicit constexpr Matrix6x6( T v ) noexcept
		: Parent{ v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v }
	{ }

	/// Performs matrix by matrix summation
	constexpr Matrix6x6 operator+( const Matrix6x6& other ) const noexcept;

	/// Adds another matrix to this matrix in place
	constexpr Matrix6x6& operator+=( const Matrix6x6& other ) noexcept;

	/// Adds a scalar to each element of the matrix
	constexpr Matrix6x6 operator+( const T scalar ) const noexcept;

	/// Adds a scalar to each element of this matrix in place
	constexpr Matrix6x6& operator+=( const T scalar ) noexcept;

	/// Performs matrix by matrix subtraction in place (element-wise)
	constexpr Matrix6x6& operator-=( const Matrix6x6& other ) noexcept;

	/// Performs matrix by matrix subtraction
	constexpr Matrix6x6 operator-( const Matrix6x6& other ) const noexcept;

	/// Subtracts a scalar from each element of the matrix
	constexpr Matrix6x6 operator-( const T scalar ) const noexcept;

	/// Subtracts a scalar from each element of this matrix in place
	constexpr Matrix6x6& operator-=( const T scalar ) noexcept;

	/// Performs matrix by matrix multiplication (element-wise)
	constexpr Matrix6x6 operator*( const Matrix6x6& other ) const noexcept;

	/// Multiplies this matrix by another matrix in place (element-wise)
	constexpr Matrix6x6& operator*=( const Matrix6x6& other ) noexcept;

	/// Multiplies each element of the matrix by a scalar
	constexpr Matrix6x6 operator*( const T scalar ) const noexcept;

	/// Multiplies each element of this matrix by a scalar in place
	constexpr Matrix6x6& operator*=( const T scalar ) noexcept;
};

template < typename T >
constexpr Matrix6x6< T > Matrix6x6< T >::operator+( const Matrix6x6< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix6x6{ self.m_data[ 0 ] + other.m_data[ 0 ],	  self.m_data[ 1 ] + other.m_data[ 1 ],
					  self.m_data[ 2 ] + other.m_data[ 2 ],	  self.m_data[ 3 ] + other.m_data[ 3 ],
					  self.m_data[ 4 ] + other.m_data[ 4 ],	  self.m_data[ 5 ] + other.m_data[ 5 ],
					  self.m_data[ 6 ] + other.m_data[ 6 ],	  self.m_data[ 7 ] + other.m_data[ 7 ],
					  self.m_data[ 8 ] + other.m_data[ 8 ],	  self.m_data[ 9 ] + other.m_data[ 9 ],
					  self.m_data[ 10 ] + other.m_data[ 10 ], self.m_data[ 11 ] + other.m_data[ 11 ],
					  self.m_data[ 12 ] + other.m_data[ 12 ], self.m_data[ 13 ] + other.m_data[ 13 ],
					  self.m_data[ 14 ] + other.m_data[ 14 ], self.m_data[ 15 ] + other.m_data[ 15 ],
					  self.m_data[ 16 ] + other.m_data[ 16 ], self.m_data[ 17 ] + other.m_data[ 17 ],
					  self.m_data[ 18 ] + other.m_data[ 18 ], self.m_data[ 19 ] + other.m_data[ 19 ],
					  self.m_data[ 20 ] + other.m_data[ 20 ], self.m_data[ 21 ] + other.m_data[ 21 ],
					  self.m_data[ 22 ] + other.m_data[ 22 ], self.m_data[ 23 ] + other.m_data[ 23 ],
					  self.m_data[ 24 ] + other.m_data[ 24 ], self.m_data[ 25 ] + other.m_data[ 25 ],
					  self.m_data[ 26 ] + other.m_data[ 26 ], self.m_data[ 27 ] + other.m_data[ 27 ],
					  self.m_data[ 28 ] + other.m_data[ 28 ], self.m_data[ 29 ] + other.m_data[ 29 ],
					  self.m_data[ 30 ] + other.m_data[ 30 ], self.m_data[ 31 ] + other.m_data[ 31 ],
					  self.m_data[ 32 ] + other.m_data[ 32 ], self.m_data[ 33 ] + other.m_data[ 33 ],
					  self.m_data[ 34 ] + other.m_data[ 34 ], self.m_data[ 35 ] + other.m_data[ 35 ] };
}

template < typename T >
constexpr Matrix6x6< T >& Matrix6x6< T >::operator+=( const Matrix6x6< T >& other ) noexcept
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
	self.m_data[ 9 ] += other.m_data[ 9 ];
	self.m_data[ 10 ] += other.m_data[ 10 ];
	self.m_data[ 11 ] += other.m_data[ 11 ];
	self.m_data[ 12 ] += other.m_data[ 12 ];
	self.m_data[ 13 ] += other.m_data[ 13 ];
	self.m_data[ 14 ] += other.m_data[ 14 ];
	self.m_data[ 15 ] += other.m_data[ 15 ];
	self.m_data[ 16 ] += other.m_data[ 16 ];
	self.m_data[ 17 ] += other.m_data[ 17 ];
	self.m_data[ 18 ] += other.m_data[ 18 ];
	self.m_data[ 19 ] += other.m_data[ 19 ];
	self.m_data[ 20 ] += other.m_data[ 20 ];
	self.m_data[ 21 ] += other.m_data[ 21 ];
	self.m_data[ 22 ] += other.m_data[ 22 ];
	self.m_data[ 23 ] += other.m_data[ 23 ];
	self.m_data[ 24 ] += other.m_data[ 24 ];
	self.m_data[ 25 ] += other.m_data[ 25 ];
	self.m_data[ 26 ] += other.m_data[ 26 ];
	self.m_data[ 27 ] += other.m_data[ 27 ];
	self.m_data[ 28 ] += other.m_data[ 28 ];
	self.m_data[ 29 ] += other.m_data[ 29 ];
	self.m_data[ 30 ] += other.m_data[ 30 ];
	self.m_data[ 31 ] += other.m_data[ 31 ];
	self.m_data[ 32 ] += other.m_data[ 32 ];
	self.m_data[ 33 ] += other.m_data[ 33 ];
	self.m_data[ 34 ] += other.m_data[ 34 ];
	self.m_data[ 35 ] += other.m_data[ 35 ];
	return self;
}

template < typename T >
constexpr Matrix6x6< T > Matrix6x6< T >::operator+( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix6x6{ self.m_data[ 0 ] + scalar,  self.m_data[ 1 ] + scalar,  self.m_data[ 2 ] + scalar,
					  self.m_data[ 3 ] + scalar,  self.m_data[ 4 ] + scalar,  self.m_data[ 5 ] + scalar,
					  self.m_data[ 6 ] + scalar,  self.m_data[ 7 ] + scalar,  self.m_data[ 8 ] + scalar,
					  self.m_data[ 9 ] + scalar,  self.m_data[ 10 ] + scalar, self.m_data[ 11 ] + scalar,
					  self.m_data[ 12 ] + scalar, self.m_data[ 13 ] + scalar, self.m_data[ 14 ] + scalar,
					  self.m_data[ 15 ] + scalar, self.m_data[ 16 ] + scalar, self.m_data[ 17 ] + scalar,
					  self.m_data[ 18 ] + scalar, self.m_data[ 19 ] + scalar, self.m_data[ 20 ] + scalar,
					  self.m_data[ 21 ] + scalar, self.m_data[ 22 ] + scalar, self.m_data[ 23 ] + scalar,
					  self.m_data[ 24 ] + scalar, self.m_data[ 25 ] + scalar, self.m_data[ 26 ] + scalar,
					  self.m_data[ 27 ] + scalar, self.m_data[ 28 ] + scalar, self.m_data[ 29 ] + scalar,
					  self.m_data[ 30 ] + scalar, self.m_data[ 31 ] + scalar, self.m_data[ 32 ] + scalar,
					  self.m_data[ 33 ] + scalar, self.m_data[ 34 ] + scalar, self.m_data[ 35 ] + scalar };
}

template < typename T >
constexpr Matrix6x6< T >& Matrix6x6< T >::operator+=( const T scalar ) noexcept
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
	self.m_data[ 9 ] += scalar;
	self.m_data[ 10 ] += scalar;
	self.m_data[ 11 ] += scalar;
	self.m_data[ 12 ] += scalar;
	self.m_data[ 13 ] += scalar;
	self.m_data[ 14 ] += scalar;
	self.m_data[ 15 ] += scalar;
	self.m_data[ 16 ] += scalar;
	self.m_data[ 17 ] += scalar;
	self.m_data[ 18 ] += scalar;
	self.m_data[ 19 ] += scalar;
	self.m_data[ 20 ] += scalar;
	self.m_data[ 21 ] += scalar;
	self.m_data[ 22 ] += scalar;
	self.m_data[ 23 ] += scalar;
	self.m_data[ 24 ] += scalar;
	self.m_data[ 25 ] += scalar;
	self.m_data[ 26 ] += scalar;
	self.m_data[ 27 ] += scalar;
	self.m_data[ 28 ] += scalar;
	self.m_data[ 29 ] += scalar;
	self.m_data[ 30 ] += scalar;
	self.m_data[ 31 ] += scalar;
	self.m_data[ 32 ] += scalar;
	self.m_data[ 33 ] += scalar;
	self.m_data[ 34 ] += scalar;
	self.m_data[ 35 ] += scalar;
	return self;
}

template < typename T >
constexpr Matrix6x6< T > Matrix6x6< T >::operator-( const Matrix6x6< T >& other ) const noexcept
{
	auto& self = *this;
	return Matrix6x6{ self.m_data[ 0 ] - other.m_data[ 0 ],	  self.m_data[ 1 ] - other.m_data[ 1 ],
					  self.m_data[ 2 ] - other.m_data[ 2 ],	  self.m_data[ 3 ] - other.m_data[ 3 ],
					  self.m_data[ 4 ] - other.m_data[ 4 ],	  self.m_data[ 5 ] - other.m_data[ 5 ],
					  self.m_data[ 6 ] - other.m_data[ 6 ],	  self.m_data[ 7 ] - other.m_data[ 7 ],
					  self.m_data[ 8 ] - other.m_data[ 8 ],	  self.m_data[ 9 ] - other.m_data[ 9 ],
					  self.m_data[ 10 ] - other.m_data[ 10 ], self.m_data[ 11 ] - other.m_data[ 11 ],
					  self.m_data[ 12 ] - other.m_data[ 12 ], self.m_data[ 13 ] - other.m_data[ 13 ],
					  self.m_data[ 14 ] - other.m_data[ 14 ], self.m_data[ 15 ] - other.m_data[ 15 ],
					  self.m_data[ 16 ] - other.m_data[ 16 ], self.m_data[ 17 ] - other.m_data[ 17 ],
					  self.m_data[ 18 ] - other.m_data[ 18 ], self.m_data[ 19 ] - other.m_data[ 19 ],
					  self.m_data[ 20 ] - other.m_data[ 20 ], self.m_data[ 21 ] - other.m_data[ 21 ],
					  self.m_data[ 22 ] - other.m_data[ 22 ], self.m_data[ 23 ] - other.m_data[ 23 ],
					  self.m_data[ 24 ] - other.m_data[ 24 ], self.m_data[ 25 ] - other.m_data[ 25 ],
					  self.m_data[ 26 ] - other.m_data[ 26 ], self.m_data[ 27 ] - other.m_data[ 27 ],
					  self.m_data[ 28 ] - other.m_data[ 28 ], self.m_data[ 29 ] - other.m_data[ 29 ],
					  self.m_data[ 30 ] - other.m_data[ 30 ], self.m_data[ 31 ] - other.m_data[ 31 ],
					  self.m_data[ 32 ] - other.m_data[ 32 ], self.m_data[ 33 ] - other.m_data[ 33 ],
					  self.m_data[ 34 ] - other.m_data[ 34 ], self.m_data[ 35 ] - other.m_data[ 35 ] };
}

template < typename T >
constexpr Matrix6x6< T >& Matrix6x6< T >::operator-=( const Matrix6x6< T >& other ) noexcept
{
	auto& self = *this;
	self.m_data[ 0 ] -= other.m_data[ 0 ];
	self.m_data[ 1 ] -= other.m_data[ 1 ];
	self.m_data[ 2 ] -= other.m_data[ 2 ];
	self.m_data[ 3 ] -= other.m_data[ 3 ];
	self.m_data[ 4 ] -= other.m_data[ 4 ];
	self.m_data[ 5 ] -= other.m_data[ 5 ];
	self.m_data[ 6 ] -= other.m_data[ 6 ];
	self.m_data[ 7 ] -= other.m_data[ 7 ];
	self.m_data[ 8 ] -= other.m_data[ 8 ];
	self.m_data[ 9 ] -= other.m_data[ 9 ];
	self.m_data[ 10 ] -= other.m_data[ 10 ];
	self.m_data[ 11 ] -= other.m_data[ 11 ];
	self.m_data[ 12 ] -= other.m_data[ 12 ];
	self.m_data[ 13 ] -= other.m_data[ 13 ];
	self.m_data[ 14 ] -= other.m_data[ 14 ];
	self.m_data[ 15 ] -= other.m_data[ 15 ];
	self.m_data[ 16 ] -= other.m_data[ 16 ];
	self.m_data[ 17 ] -= other.m_data[ 17 ];
	self.m_data[ 18 ] -= other.m_data[ 18 ];
	self.m_data[ 19 ] -= other.m_data[ 19 ];
	self.m_data[ 20 ] -= other.m_data[ 20 ];
	self.m_data[ 21 ] -= other.m_data[ 21 ];
	self.m_data[ 22 ] -= other.m_data[ 22 ];
	self.m_data[ 23 ] -= other.m_data[ 23 ];
	self.m_data[ 24 ] -= other.m_data[ 24 ];
	self.m_data[ 25 ] -= other.m_data[ 25 ];
	self.m_data[ 26 ] -= other.m_data[ 26 ];
	self.m_data[ 27 ] -= other.m_data[ 27 ];
	self.m_data[ 28 ] -= other.m_data[ 28 ];
	self.m_data[ 29 ] -= other.m_data[ 29 ];
	self.m_data[ 30 ] -= other.m_data[ 30 ];
	self.m_data[ 31 ] -= other.m_data[ 31 ];
	self.m_data[ 32 ] -= other.m_data[ 32 ];
	self.m_data[ 33 ] -= other.m_data[ 33 ];
	self.m_data[ 34 ] -= other.m_data[ 34 ];
	self.m_data[ 35 ] -= other.m_data[ 35 ];
	return self;
}

template < typename T >
constexpr Matrix6x6< T > Matrix6x6< T >::operator-( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix6x6{ self.m_data[ 0 ] - scalar,  self.m_data[ 1 ] - scalar,  self.m_data[ 2 ] - scalar,
					  self.m_data[ 3 ] - scalar,  self.m_data[ 4 ] - scalar,  self.m_data[ 5 ] - scalar,
					  self.m_data[ 6 ] - scalar,  self.m_data[ 7 ] - scalar,  self.m_data[ 8 ] - scalar,
					  self.m_data[ 9 ] - scalar,  self.m_data[ 10 ] - scalar, self.m_data[ 11 ] - scalar,
					  self.m_data[ 12 ] - scalar, self.m_data[ 13 ] - scalar, self.m_data[ 14 ] - scalar,
					  self.m_data[ 15 ] - scalar, self.m_data[ 16 ] - scalar, self.m_data[ 17 ] - scalar,
					  self.m_data[ 18 ] - scalar, self.m_data[ 19 ] - scalar, self.m_data[ 20 ] - scalar,
					  self.m_data[ 21 ] - scalar, self.m_data[ 22 ] - scalar, self.m_data[ 23 ] - scalar,
					  self.m_data[ 24 ] - scalar, self.m_data[ 25 ] - scalar, self.m_data[ 26 ] - scalar,
					  self.m_data[ 27 ] - scalar, self.m_data[ 28 ] - scalar, self.m_data[ 29 ] - scalar,
					  self.m_data[ 30 ] - scalar, self.m_data[ 31 ] - scalar, self.m_data[ 32 ] - scalar,
					  self.m_data[ 33 ] - scalar, self.m_data[ 34 ] - scalar, self.m_data[ 35 ] - scalar };
}

template < typename T >
constexpr Matrix6x6< T >& Matrix6x6< T >::operator-=( const T scalar ) noexcept
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
	self.m_data[ 9 ] -= scalar;
	self.m_data[ 10 ] -= scalar;
	self.m_data[ 11 ] -= scalar;
	self.m_data[ 12 ] -= scalar;
	self.m_data[ 13 ] -= scalar;
	self.m_data[ 14 ] -= scalar;
	self.m_data[ 15 ] -= scalar;
	self.m_data[ 16 ] -= scalar;
	self.m_data[ 17 ] -= scalar;
	self.m_data[ 18 ] -= scalar;
	self.m_data[ 19 ] -= scalar;
	self.m_data[ 20 ] -= scalar;
	self.m_data[ 21 ] -= scalar;
	self.m_data[ 22 ] -= scalar;
	self.m_data[ 23 ] -= scalar;
	self.m_data[ 24 ] -= scalar;
	self.m_data[ 25 ] -= scalar;
	self.m_data[ 26 ] -= scalar;
	self.m_data[ 27 ] -= scalar;
	self.m_data[ 28 ] -= scalar;
	self.m_data[ 29 ] -= scalar;
	self.m_data[ 30 ] -= scalar;
	self.m_data[ 31 ] -= scalar;
	self.m_data[ 32 ] -= scalar;
	self.m_data[ 33 ] -= scalar;
	self.m_data[ 34 ] -= scalar;
	self.m_data[ 35 ] -= scalar;
	return self;
}

template < typename T >
constexpr Matrix6x6< T > Matrix6x6< T >::operator*( const Matrix6x6< T >& other ) const noexcept
{
	const auto& self = *this;
	const auto& sdata = self.m_data;
	const auto& mdata = other.m_data;

	return Matrix6x6{ // First row
					  sdata[ 0 ] * mdata[ 0 ] + sdata[ 1 ] * mdata[ 6 ] + sdata[ 2 ] * mdata[ 12 ] +
						  sdata[ 3 ] * mdata[ 18 ] + sdata[ 4 ] * mdata[ 24 ] + sdata[ 5 ] * mdata[ 30 ],
					  sdata[ 0 ] * mdata[ 1 ] + sdata[ 1 ] * mdata[ 7 ] + sdata[ 2 ] * mdata[ 13 ] +
						  sdata[ 3 ] * mdata[ 19 ] + sdata[ 4 ] * mdata[ 25 ] + sdata[ 5 ] * mdata[ 31 ],
					  sdata[ 0 ] * mdata[ 2 ] + sdata[ 1 ] * mdata[ 8 ] + sdata[ 2 ] * mdata[ 14 ] +
						  sdata[ 3 ] * mdata[ 20 ] + sdata[ 4 ] * mdata[ 26 ] + sdata[ 5 ] * mdata[ 32 ],
					  sdata[ 0 ] * mdata[ 3 ] + sdata[ 1 ] * mdata[ 9 ] + sdata[ 2 ] * mdata[ 15 ] +
						  sdata[ 3 ] * mdata[ 21 ] + sdata[ 4 ] * mdata[ 27 ] + sdata[ 5 ] * mdata[ 33 ],
					  sdata[ 0 ] * mdata[ 4 ] + sdata[ 1 ] * mdata[ 10 ] + sdata[ 2 ] * mdata[ 16 ] +
						  sdata[ 3 ] * mdata[ 22 ] + sdata[ 4 ] * mdata[ 28 ] + sdata[ 5 ] * mdata[ 34 ],
					  sdata[ 0 ] * mdata[ 5 ] + sdata[ 1 ] * mdata[ 11 ] + sdata[ 2 ] * mdata[ 17 ] +
						  sdata[ 3 ] * mdata[ 23 ] + sdata[ 4 ] * mdata[ 29 ] + sdata[ 5 ] * mdata[ 35 ],

					  // Second row
					  sdata[ 6 ] * mdata[ 0 ] + sdata[ 7 ] * mdata[ 6 ] + sdata[ 8 ] * mdata[ 12 ] +
						  sdata[ 9 ] * mdata[ 18 ] + sdata[ 10 ] * mdata[ 24 ] + sdata[ 11 ] * mdata[ 30 ],
					  sdata[ 6 ] * mdata[ 1 ] + sdata[ 7 ] * mdata[ 7 ] + sdata[ 8 ] * mdata[ 13 ] +
						  sdata[ 9 ] * mdata[ 19 ] + sdata[ 10 ] * mdata[ 25 ] + sdata[ 11 ] * mdata[ 31 ],
					  sdata[ 6 ] * mdata[ 2 ] + sdata[ 7 ] * mdata[ 8 ] + sdata[ 8 ] * mdata[ 14 ] +
						  sdata[ 9 ] * mdata[ 20 ] + sdata[ 10 ] * mdata[ 26 ] + sdata[ 11 ] * mdata[ 32 ],
					  sdata[ 6 ] * mdata[ 3 ] + sdata[ 7 ] * mdata[ 9 ] + sdata[ 8 ] * mdata[ 15 ] +
						  sdata[ 9 ] * mdata[ 21 ] + sdata[ 10 ] * mdata[ 27 ] + sdata[ 11 ] * mdata[ 33 ],
					  sdata[ 6 ] * mdata[ 4 ] + sdata[ 7 ] * mdata[ 10 ] + sdata[ 8 ] * mdata[ 16 ] +
						  sdata[ 9 ] * mdata[ 22 ] + sdata[ 10 ] * mdata[ 28 ] + sdata[ 11 ] * mdata[ 34 ],
					  sdata[ 6 ] * mdata[ 5 ] + sdata[ 7 ] * mdata[ 11 ] + sdata[ 8 ] * mdata[ 17 ] +
						  sdata[ 9 ] * mdata[ 23 ] + sdata[ 10 ] * mdata[ 29 ] + sdata[ 11 ] * mdata[ 35 ],

					  // Third row
					  sdata[ 12 ] * mdata[ 0 ] + sdata[ 13 ] * mdata[ 6 ] + sdata[ 14 ] * mdata[ 12 ] +
						  sdata[ 15 ] * mdata[ 18 ] + sdata[ 16 ] * mdata[ 24 ] + sdata[ 17 ] * mdata[ 30 ],
					  sdata[ 12 ] * mdata[ 1 ] + sdata[ 13 ] * mdata[ 7 ] + sdata[ 14 ] * mdata[ 13 ] +
						  sdata[ 15 ] * mdata[ 19 ] + sdata[ 16 ] * mdata[ 25 ] + sdata[ 17 ] * mdata[ 31 ],
					  sdata[ 12 ] * mdata[ 2 ] + sdata[ 13 ] * mdata[ 8 ] + sdata[ 14 ] * mdata[ 14 ] +
						  sdata[ 15 ] * mdata[ 20 ] + sdata[ 16 ] * mdata[ 26 ] + sdata[ 17 ] * mdata[ 32 ],
					  sdata[ 12 ] * mdata[ 3 ] + sdata[ 13 ] * mdata[ 9 ] + sdata[ 14 ] * mdata[ 15 ] +
						  sdata[ 15 ] * mdata[ 21 ] + sdata[ 16 ] * mdata[ 27 ] + sdata[ 17 ] * mdata[ 33 ],
					  sdata[ 12 ] * mdata[ 4 ] + sdata[ 13 ] * mdata[ 10 ] + sdata[ 14 ] * mdata[ 16 ] +
						  sdata[ 15 ] * mdata[ 22 ] + sdata[ 16 ] * mdata[ 28 ] + sdata[ 17 ] * mdata[ 34 ],
					  sdata[ 12 ] * mdata[ 5 ] + sdata[ 13 ] * mdata[ 11 ] + sdata[ 14 ] * mdata[ 17 ] +
						  sdata[ 15 ] * mdata[ 23 ] + sdata[ 16 ] * mdata[ 29 ] + sdata[ 17 ] * mdata[ 35 ],

					  // Fourth row
					  sdata[ 18 ] * mdata[ 0 ] + sdata[ 19 ] * mdata[ 6 ] + sdata[ 20 ] * mdata[ 12 ] +
						  sdata[ 21 ] * mdata[ 18 ] + sdata[ 22 ] * mdata[ 24 ] + sdata[ 23 ] * mdata[ 30 ],
					  sdata[ 18 ] * mdata[ 1 ] + sdata[ 19 ] * mdata[ 7 ] + sdata[ 20 ] * mdata[ 13 ] +
						  sdata[ 21 ] * mdata[ 19 ] + sdata[ 22 ] * mdata[ 25 ] + sdata[ 23 ] * mdata[ 31 ],
					  sdata[ 18 ] * mdata[ 2 ] + sdata[ 19 ] * mdata[ 8 ] + sdata[ 20 ] * mdata[ 14 ] +
						  sdata[ 21 ] * mdata[ 20 ] + sdata[ 22 ] * mdata[ 26 ] + sdata[ 23 ] * mdata[ 32 ],
					  sdata[ 18 ] * mdata[ 3 ] + sdata[ 19 ] * mdata[ 9 ] + sdata[ 20 ] * mdata[ 15 ] +
						  sdata[ 21 ] * mdata[ 21 ] + sdata[ 22 ] * mdata[ 27 ] + sdata[ 23 ] * mdata[ 33 ],
					  sdata[ 18 ] * mdata[ 4 ] + sdata[ 19 ] * mdata[ 10 ] + sdata[ 20 ] * mdata[ 16 ] +
						  sdata[ 21 ] * mdata[ 22 ] + sdata[ 22 ] * mdata[ 28 ] + sdata[ 23 ] * mdata[ 34 ],
					  sdata[ 18 ] * mdata[ 5 ] + sdata[ 19 ] * mdata[ 11 ] + sdata[ 20 ] * mdata[ 17 ] +
						  sdata[ 21 ] * mdata[ 23 ] + sdata[ 22 ] * mdata[ 29 ] + sdata[ 23 ] * mdata[ 35 ],

					  // Fifth row
					  sdata[ 24 ] * mdata[ 0 ] + sdata[ 25 ] * mdata[ 6 ] + sdata[ 26 ] * mdata[ 12 ] +
						  sdata[ 27 ] * mdata[ 18 ] + sdata[ 28 ] * mdata[ 24 ] + sdata[ 29 ] * mdata[ 30 ],
					  sdata[ 24 ] * mdata[ 1 ] + sdata[ 25 ] * mdata[ 7 ] + sdata[ 26 ] * mdata[ 13 ] +
						  sdata[ 27 ] * mdata[ 19 ] + sdata[ 28 ] * mdata[ 25 ] + sdata[ 29 ] * mdata[ 31 ],
					  sdata[ 24 ] * mdata[ 2 ] + sdata[ 25 ] * mdata[ 8 ] + sdata[ 26 ] * mdata[ 14 ] +
						  sdata[ 27 ] * mdata[ 20 ] + sdata[ 28 ] * mdata[ 26 ] + sdata[ 29 ] * mdata[ 32 ],
					  sdata[ 24 ] * mdata[ 3 ] + sdata[ 25 ] * mdata[ 9 ] + sdata[ 26 ] * mdata[ 15 ] +
						  sdata[ 27 ] * mdata[ 21 ] + sdata[ 28 ] * mdata[ 27 ] + sdata[ 29 ] * mdata[ 33 ],
					  sdata[ 24 ] * mdata[ 4 ] + sdata[ 25 ] * mdata[ 10 ] + sdata[ 26 ] * mdata[ 16 ] +
						  sdata[ 27 ] * mdata[ 22 ] + sdata[ 28 ] * mdata[ 28 ] + sdata[ 29 ] * mdata[ 34 ],
					  sdata[ 24 ] * mdata[ 5 ] + sdata[ 25 ] * mdata[ 11 ] + sdata[ 26 ] * mdata[ 17 ] +
						  sdata[ 27 ] * mdata[ 23 ] + sdata[ 28 ] * mdata[ 29 ] + sdata[ 29 ] * mdata[ 35 ],

					  // Sixth row
					  sdata[ 30 ] * mdata[ 0 ] + sdata[ 31 ] * mdata[ 6 ] + sdata[ 32 ] * mdata[ 12 ] +
						  sdata[ 33 ] * mdata[ 18 ] + sdata[ 34 ] * mdata[ 24 ] + sdata[ 35 ] * mdata[ 30 ],
					  sdata[ 30 ] * mdata[ 1 ] + sdata[ 31 ] * mdata[ 7 ] + sdata[ 32 ] * mdata[ 13 ] +
						  sdata[ 33 ] * mdata[ 19 ] + sdata[ 34 ] * mdata[ 25 ] + sdata[ 35 ] * mdata[ 31 ],
					  sdata[ 30 ] * mdata[ 2 ] + sdata[ 31 ] * mdata[ 8 ] + sdata[ 32 ] * mdata[ 14 ] +
						  sdata[ 33 ] * mdata[ 20 ] + sdata[ 34 ] * mdata[ 26 ] + sdata[ 35 ] * mdata[ 32 ],
					  sdata[ 30 ] * mdata[ 3 ] + sdata[ 31 ] * mdata[ 9 ] + sdata[ 32 ] * mdata[ 15 ] +
						  sdata[ 33 ] * mdata[ 21 ] + sdata[ 34 ] * mdata[ 27 ] + sdata[ 35 ] * mdata[ 33 ],
					  sdata[ 30 ] * mdata[ 4 ] + sdata[ 31 ] * mdata[ 10 ] + sdata[ 32 ] * mdata[ 16 ] +
						  sdata[ 33 ] * mdata[ 22 ] + sdata[ 34 ] * mdata[ 28 ] + sdata[ 35 ] * mdata[ 34 ],
					  sdata[ 30 ] * mdata[ 5 ] + sdata[ 31 ] * mdata[ 11 ] + sdata[ 32 ] * mdata[ 17 ] +
						  sdata[ 33 ] * mdata[ 23 ] + sdata[ 34 ] * mdata[ 29 ] + sdata[ 35 ] * mdata[ 35 ] };
}

template < typename T >
constexpr Matrix6x6< T >& Matrix6x6< T >::operator*=( const Matrix6x6< T >& other ) noexcept
{
	auto& self = *this;
	const auto& sdata = self.m_data;
	const auto& m_data = other.m_data;

	// Temporary variables for the result
	const T m0 = sdata[ 0 ] * m_data[ 0 ] + sdata[ 1 ] * m_data[ 6 ] + sdata[ 2 ] * m_data[ 12 ] +
				 sdata[ 3 ] * m_data[ 18 ] + sdata[ 4 ] * m_data[ 24 ] + sdata[ 5 ] * m_data[ 30 ];
	const T m1 = sdata[ 0 ] * m_data[ 1 ] + sdata[ 1 ] * m_data[ 7 ] + sdata[ 2 ] * m_data[ 13 ] +
				 sdata[ 3 ] * m_data[ 19 ] + sdata[ 4 ] * m_data[ 25 ] + sdata[ 5 ] * m_data[ 31 ];
	const T m2 = sdata[ 0 ] * m_data[ 2 ] + sdata[ 1 ] * m_data[ 8 ] + sdata[ 2 ] * m_data[ 14 ] +
				 sdata[ 3 ] * m_data[ 20 ] + sdata[ 4 ] * m_data[ 26 ] + sdata[ 5 ] * m_data[ 32 ];
	const T m3 = sdata[ 0 ] * m_data[ 3 ] + sdata[ 1 ] * m_data[ 9 ] + sdata[ 2 ] * m_data[ 15 ] +
				 sdata[ 3 ] * m_data[ 21 ] + sdata[ 4 ] * m_data[ 27 ] + sdata[ 5 ] * m_data[ 33 ];
	const T m4 = sdata[ 0 ] * m_data[ 4 ] + sdata[ 1 ] * m_data[ 10 ] + sdata[ 2 ] * m_data[ 16 ] +
				 sdata[ 3 ] * m_data[ 22 ] + sdata[ 4 ] * m_data[ 28 ] + sdata[ 5 ] * m_data[ 34 ];
	const T m5 = sdata[ 0 ] * m_data[ 5 ] + sdata[ 1 ] * m_data[ 11 ] + sdata[ 2 ] * m_data[ 17 ] +
				 sdata[ 3 ] * m_data[ 23 ] + sdata[ 4 ] * m_data[ 29 ] + sdata[ 5 ] * m_data[ 35 ];

	const T m6 = sdata[ 6 ] * m_data[ 0 ] + sdata[ 7 ] * m_data[ 6 ] + sdata[ 8 ] * m_data[ 12 ] +
				 sdata[ 9 ] * m_data[ 18 ] + sdata[ 10 ] * m_data[ 24 ] + sdata[ 11 ] * m_data[ 30 ];
	const T m7 = sdata[ 6 ] * m_data[ 1 ] + sdata[ 7 ] * m_data[ 7 ] + sdata[ 8 ] * m_data[ 13 ] +
				 sdata[ 9 ] * m_data[ 19 ] + sdata[ 10 ] * m_data[ 25 ] + sdata[ 11 ] * m_data[ 31 ];
	const T m8 = sdata[ 6 ] * m_data[ 2 ] + sdata[ 7 ] * m_data[ 8 ] + sdata[ 8 ] * m_data[ 14 ] +
				 sdata[ 9 ] * m_data[ 20 ] + sdata[ 10 ] * m_data[ 26 ] + sdata[ 11 ] * m_data[ 32 ];
	const T m9 = sdata[ 6 ] * m_data[ 3 ] + sdata[ 7 ] * m_data[ 9 ] + sdata[ 8 ] * m_data[ 15 ] +
				 sdata[ 9 ] * m_data[ 21 ] + sdata[ 10 ] * m_data[ 27 ] + sdata[ 11 ] * m_data[ 33 ];
	const T m10 = sdata[ 6 ] * m_data[ 4 ] + sdata[ 7 ] * m_data[ 10 ] + sdata[ 8 ] * m_data[ 16 ] +
				  sdata[ 9 ] * m_data[ 22 ] + sdata[ 10 ] * m_data[ 28 ] + sdata[ 11 ] * m_data[ 34 ];
	const T m11 = sdata[ 6 ] * m_data[ 5 ] + sdata[ 7 ] * m_data[ 11 ] + sdata[ 8 ] * m_data[ 17 ] +
				  sdata[ 9 ] * m_data[ 23 ] + sdata[ 10 ] * m_data[ 29 ] + sdata[ 11 ] * m_data[ 35 ];

	const T m12 = sdata[ 12 ] * m_data[ 0 ] + sdata[ 13 ] * m_data[ 6 ] + sdata[ 14 ] * m_data[ 12 ] +
				  sdata[ 15 ] * m_data[ 18 ] + sdata[ 16 ] * m_data[ 24 ] + sdata[ 17 ] * m_data[ 30 ];
	const T m13 = sdata[ 12 ] * m_data[ 1 ] + sdata[ 13 ] * m_data[ 7 ] + sdata[ 14 ] * m_data[ 13 ] +
				  sdata[ 15 ] * m_data[ 19 ] + sdata[ 16 ] * m_data[ 25 ] + sdata[ 17 ] * m_data[ 31 ];
	const T m14 = sdata[ 12 ] * m_data[ 2 ] + sdata[ 13 ] * m_data[ 8 ] + sdata[ 14 ] * m_data[ 14 ] +
				  sdata[ 15 ] * m_data[ 20 ] + sdata[ 16 ] * m_data[ 26 ] + sdata[ 17 ] * m_data[ 32 ];
	const T m15 = sdata[ 12 ] * m_data[ 3 ] + sdata[ 13 ] * m_data[ 9 ] + sdata[ 14 ] * m_data[ 15 ] +
				  sdata[ 15 ] * m_data[ 21 ] + sdata[ 16 ] * m_data[ 27 ] + sdata[ 17 ] * m_data[ 33 ];
	const T m16 = sdata[ 12 ] * m_data[ 4 ] + sdata[ 13 ] * m_data[ 10 ] + sdata[ 14 ] * m_data[ 16 ] +
				  sdata[ 15 ] * m_data[ 22 ] + sdata[ 16 ] * m_data[ 28 ] + sdata[ 17 ] * m_data[ 34 ];
	const T m17 = sdata[ 12 ] * m_data[ 5 ] + sdata[ 13 ] * m_data[ 11 ] + sdata[ 14 ] * m_data[ 17 ] +
				  sdata[ 15 ] * m_data[ 23 ] + sdata[ 16 ] * m_data[ 29 ] + sdata[ 17 ] * m_data[ 35 ];

	const T m18 = sdata[ 18 ] * m_data[ 0 ] + sdata[ 19 ] * m_data[ 6 ] + sdata[ 20 ] * m_data[ 12 ] +
				  sdata[ 21 ] * m_data[ 18 ] + sdata[ 22 ] * m_data[ 24 ] + sdata[ 23 ] * m_data[ 30 ];
	const T m19 = sdata[ 18 ] * m_data[ 1 ] + sdata[ 19 ] * m_data[ 7 ] + sdata[ 20 ] * m_data[ 13 ] +
				  sdata[ 21 ] * m_data[ 19 ] + sdata[ 22 ] * m_data[ 25 ] + sdata[ 23 ] * m_data[ 31 ];
	const T m20 = sdata[ 18 ] * m_data[ 2 ] + sdata[ 19 ] * m_data[ 8 ] + sdata[ 20 ] * m_data[ 14 ] +
				  sdata[ 21 ] * m_data[ 20 ] + sdata[ 22 ] * m_data[ 26 ] + sdata[ 23 ] * m_data[ 32 ];
	const T m21 = sdata[ 18 ] * m_data[ 3 ] + sdata[ 19 ] * m_data[ 9 ] + sdata[ 20 ] * m_data[ 15 ] +
				  sdata[ 21 ] * m_data[ 21 ] + sdata[ 22 ] * m_data[ 27 ] + sdata[ 23 ] * m_data[ 33 ];
	const T m22 = sdata[ 18 ] * m_data[ 4 ] + sdata[ 19 ] * m_data[ 10 ] + sdata[ 20 ] * m_data[ 16 ] +
				  sdata[ 21 ] * m_data[ 22 ] + sdata[ 22 ] * m_data[ 28 ] + sdata[ 23 ] * m_data[ 34 ];
	const T m23 = sdata[ 18 ] * m_data[ 5 ] + sdata[ 19 ] * m_data[ 11 ] + sdata[ 20 ] * m_data[ 17 ] +
				  sdata[ 21 ] * m_data[ 23 ] + sdata[ 22 ] * m_data[ 29 ] + sdata[ 23 ] * m_data[ 35 ];

	const T m24 = sdata[ 24 ] * m_data[ 0 ] + sdata[ 25 ] * m_data[ 6 ] + sdata[ 26 ] * m_data[ 12 ] +
				  sdata[ 27 ] * m_data[ 18 ] + sdata[ 28 ] * m_data[ 24 ] + sdata[ 29 ] * m_data[ 30 ];
	const T m25 = sdata[ 24 ] * m_data[ 1 ] + sdata[ 25 ] * m_data[ 7 ] + sdata[ 26 ] * m_data[ 13 ] +
				  sdata[ 27 ] * m_data[ 19 ] + sdata[ 28 ] * m_data[ 25 ] + sdata[ 29 ] * m_data[ 31 ];
	const T m26 = sdata[ 24 ] * m_data[ 2 ] + sdata[ 25 ] * m_data[ 8 ] + sdata[ 26 ] * m_data[ 14 ] +
				  sdata[ 27 ] * m_data[ 20 ] + sdata[ 28 ] * m_data[ 26 ] + sdata[ 29 ] * m_data[ 32 ];
	const T m27 = sdata[ 24 ] * m_data[ 3 ] + sdata[ 25 ] * m_data[ 9 ] + sdata[ 26 ] * m_data[ 15 ] +
				  sdata[ 27 ] * m_data[ 21 ] + sdata[ 28 ] * m_data[ 27 ] + sdata[ 29 ] * m_data[ 33 ];
	const T m28 = sdata[ 24 ] * m_data[ 4 ] + sdata[ 25 ] * m_data[ 10 ] + sdata[ 26 ] * m_data[ 16 ] +
				  sdata[ 27 ] * m_data[ 22 ] + sdata[ 28 ] * m_data[ 28 ] + sdata[ 29 ] * m_data[ 34 ];
	const T m29 = sdata[ 24 ] * m_data[ 5 ] + sdata[ 25 ] * m_data[ 11 ] + sdata[ 26 ] * m_data[ 17 ] +
				  sdata[ 27 ] * m_data[ 23 ] + sdata[ 28 ] * m_data[ 29 ] + sdata[ 29 ] * m_data[ 35 ];

	const T m30 = sdata[ 30 ] * m_data[ 0 ] + sdata[ 31 ] * m_data[ 6 ] + sdata[ 32 ] * m_data[ 12 ] +
				  sdata[ 33 ] * m_data[ 18 ] + sdata[ 34 ] * m_data[ 24 ] + sdata[ 35 ] * m_data[ 30 ];
	const T m31 = sdata[ 30 ] * m_data[ 1 ] + sdata[ 31 ] * m_data[ 7 ] + sdata[ 32 ] * m_data[ 13 ] +
				  sdata[ 33 ] * m_data[ 19 ] + sdata[ 34 ] * m_data[ 25 ] + sdata[ 35 ] * m_data[ 31 ];
	const T m32 = sdata[ 30 ] * m_data[ 2 ] + sdata[ 31 ] * m_data[ 8 ] + sdata[ 32 ] * m_data[ 14 ] +
				  sdata[ 33 ] * m_data[ 20 ] + sdata[ 34 ] * m_data[ 26 ] + sdata[ 35 ] * m_data[ 32 ];
	const T m33 = sdata[ 30 ] * m_data[ 3 ] + sdata[ 31 ] * m_data[ 9 ] + sdata[ 32 ] * m_data[ 15 ] +
				  sdata[ 33 ] * m_data[ 21 ] + sdata[ 34 ] * m_data[ 27 ] + sdata[ 35 ] * m_data[ 33 ];
	const T m34 = sdata[ 30 ] * m_data[ 4 ] + sdata[ 31 ] * m_data[ 10 ] + sdata[ 32 ] * m_data[ 16 ] +
				  sdata[ 33 ] * m_data[ 22 ] + sdata[ 34 ] * m_data[ 28 ] + sdata[ 35 ] * m_data[ 34 ];
	const T m35 = sdata[ 30 ] * m_data[ 5 ] + sdata[ 31 ] * m_data[ 11 ] + sdata[ 32 ] * m_data[ 17 ] +
				  sdata[ 33 ] * m_data[ 23 ] + sdata[ 34 ] * m_data[ 29 ] + sdata[ 35 ] * m_data[ 35 ];

	// Assign results back to self
	self.m_data[ 0 ] = m0;
	self.m_data[ 1 ] = m1;
	self.m_data[ 2 ] = m2;
	self.m_data[ 3 ] = m3;
	self.m_data[ 4 ] = m4;
	self.m_data[ 5 ] = m5;
	self.m_data[ 6 ] = m6;
	self.m_data[ 7 ] = m7;
	self.m_data[ 8 ] = m8;
	self.m_data[ 9 ] = m9;
	self.m_data[ 10 ] = m10;
	self.m_data[ 11 ] = m11;
	self.m_data[ 12 ] = m12;
	self.m_data[ 13 ] = m13;
	self.m_data[ 14 ] = m14;
	self.m_data[ 15 ] = m15;
	self.m_data[ 16 ] = m16;
	self.m_data[ 17 ] = m17;
	self.m_data[ 18 ] = m18;
	self.m_data[ 19 ] = m19;
	self.m_data[ 20 ] = m20;
	self.m_data[ 21 ] = m21;
	self.m_data[ 22 ] = m22;
	self.m_data[ 23 ] = m23;
	self.m_data[ 24 ] = m24;
	self.m_data[ 25 ] = m25;
	self.m_data[ 26 ] = m26;
	self.m_data[ 27 ] = m27;
	self.m_data[ 28 ] = m28;
	self.m_data[ 29 ] = m29;
	self.m_data[ 30 ] = m30;
	self.m_data[ 31 ] = m31;
	self.m_data[ 32 ] = m32;
	self.m_data[ 33 ] = m33;
	self.m_data[ 34 ] = m34;
	self.m_data[ 35 ] = m35;

	return self;
}

template < typename T >
constexpr Matrix6x6< T > Matrix6x6< T >::operator*( const T scalar ) const noexcept
{
	auto& self = *this;
	return Matrix6x6{ self.m_data[ 0 ] * scalar,  self.m_data[ 1 ] * scalar,  self.m_data[ 2 ] * scalar,
					  self.m_data[ 3 ] * scalar,  self.m_data[ 4 ] * scalar,  self.m_data[ 5 ] * scalar,
					  self.m_data[ 6 ] * scalar,  self.m_data[ 7 ] * scalar,  self.m_data[ 8 ] * scalar,
					  self.m_data[ 9 ] * scalar,  self.m_data[ 10 ] * scalar, self.m_data[ 11 ] * scalar,
					  self.m_data[ 12 ] * scalar, self.m_data[ 13 ] * scalar, self.m_data[ 14 ] * scalar,
					  self.m_data[ 15 ] * scalar, self.m_data[ 16 ] * scalar, self.m_data[ 17 ] * scalar,
					  self.m_data[ 18 ] * scalar, self.m_data[ 19 ] * scalar, self.m_data[ 20 ] * scalar,
					  self.m_data[ 21 ] * scalar, self.m_data[ 22 ] * scalar, self.m_data[ 23 ] * scalar,
					  self.m_data[ 24 ] * scalar, self.m_data[ 25 ] * scalar, self.m_data[ 26 ] * scalar,
					  self.m_data[ 27 ] * scalar, self.m_data[ 28 ] * scalar, self.m_data[ 29 ] * scalar,
					  self.m_data[ 30 ] * scalar, self.m_data[ 31 ] * scalar, self.m_data[ 32 ] * scalar,
					  self.m_data[ 33 ] * scalar, self.m_data[ 34 ] * scalar, self.m_data[ 35 ] * scalar };
}

template < typename T >
constexpr Matrix6x6< T >& Matrix6x6< T >::operator*=( const T scalar ) noexcept
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
	self.m_data[ 9 ] *= scalar;
	self.m_data[ 10 ] *= scalar;
	self.m_data[ 11 ] *= scalar;
	self.m_data[ 12 ] *= scalar;
	self.m_data[ 13 ] *= scalar;
	self.m_data[ 14 ] *= scalar;
	self.m_data[ 15 ] *= scalar;
	self.m_data[ 16 ] *= scalar;
	self.m_data[ 17 ] *= scalar;
	self.m_data[ 18 ] *= scalar;
	self.m_data[ 19 ] *= scalar;
	self.m_data[ 20 ] *= scalar;
	self.m_data[ 21 ] *= scalar;
	self.m_data[ 22 ] *= scalar;
	self.m_data[ 23 ] *= scalar;
	self.m_data[ 24 ] *= scalar;
	self.m_data[ 25 ] *= scalar;
	self.m_data[ 26 ] *= scalar;
	self.m_data[ 27 ] *= scalar;
	self.m_data[ 28 ] *= scalar;
	self.m_data[ 29 ] *= scalar;
	self.m_data[ 30 ] *= scalar;
	self.m_data[ 31 ] *= scalar;
	self.m_data[ 32 ] *= scalar;
	self.m_data[ 33 ] *= scalar;
	self.m_data[ 34 ] *= scalar;
	self.m_data[ 35 ] *= scalar;
	return self;
}

using Matrix6x6i = Matrix6x6< int >;
using Matrix6x6f = Matrix6x6< float >;
using Matrix6x6d = Matrix6x6< double >;

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_6X6_HPP__
