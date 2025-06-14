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

using Matrix2x2i = Matrix2x2< int >;
using Matrix2x2f = Matrix2x2< float >;
using Matrix2x2d = Matrix2x2< double >;

} // namespace pbl::math

#include "Matrix2x2.ipp"

#endif // I2C_MATH_MATRIX_2X2_HPP__
