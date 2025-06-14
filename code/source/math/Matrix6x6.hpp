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


using Matrix6x6i = Matrix6x6< int >;
using Matrix6x6f = Matrix6x6< float >;
using Matrix6x6d = Matrix6x6< double >;

} // namespace pbl::math

#include "Matrix6x6.ipp"

#endif // I2C_MATH_MATRIX_6X6_HPP__
