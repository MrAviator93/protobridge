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

using Matrix3x3i = Matrix3x3< int >;
using Matrix3x3f = Matrix3x3< float >;
using Matrix3x3d = Matrix3x3< double >;

} // namespace pbl::math

#include "Matrix3x3.ipp"

#endif // I2C_MATH_MATRIX_3X3_HPP__