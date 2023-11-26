#ifndef I2C_MATH_MATRIX_4X4_HPP__
#define I2C_MATH_MATRIX_4X4_HPP__

#include "MatrixBase.hpp"

// C
extern "C" {
#include <emmintrin.h>
}

namespace PBL::Math
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
};

using Matrix4x4f = Matrix3x3< float >;
using Matrix4x4d = Matrix3x3< double >;

} // namespace PBL::Math
#endif // I2C_MATH_MATRIX_4X4_HPP__