#ifndef I2C_MATH_MATRIX_4X4_HPP__
#define I2C_MATH_MATRIX_4X4_HPP__

#include "MatrixBase.hpp"

namespace pbl::math
{

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

	/// Performs matrix by matrix summation
	constexpr Matrix4x4 operator+( const Matrix4x4& other ) const noexcept;

	/// Adds another matrix to this matrix in place
	constexpr Matrix4x4& operator+=( const Matrix4x4& other ) noexcept;

	/// Adds a scalar to each element of the matrix
	constexpr Matrix4x4 operator+( const T scalar ) const noexcept;

	/// Adds a scalar to each element of this matrix in place
	constexpr Matrix4x4& operator+=( const T scalar ) noexcept;

	/// Performs matrix by matrix subtraction
	constexpr Matrix4x4 operator-( const Matrix4x4& other ) const noexcept;

	/// Subtracts a scalar from each element of the matrix
	constexpr Matrix4x4 operator-( const T scalar ) const noexcept;

	/// Subtracts a scalar from each element of this matrix in place
	constexpr Matrix4x4& operator-=( const T scalar ) noexcept;

	/// Performs matrix by matrix multiplication (element-wise)
	constexpr Matrix4x4 operator*( const Matrix4x4& other ) const noexcept;

	/// Multiplies this matrix by another matrix in place (element-wise)
	constexpr Matrix4x4& operator*=( const Matrix4x4& other ) noexcept;

	/// Multiplies each element of the matrix by a scalar
	constexpr Matrix4x4 operator*( const T scalar ) const noexcept;

	/// Multiplies each element of this matrix by a scalar in place
	constexpr Matrix4x4& operator*=( const T scalar ) noexcept;
};

using Matrix4x4i = Matrix4x4< int >;
using Matrix4x4f = Matrix4x4< float >;
using Matrix4x4d = Matrix4x4< double >;

} // namespace pbl::math

#include "Matrix4x4.ipp"

#endif // I2C_MATH_MATRIX_4X4_HPP__
