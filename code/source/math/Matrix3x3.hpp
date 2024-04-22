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
};

using Matrix3x3f = Matrix3x3< float >;
using Matrix3x3d = Matrix3x3< double >;

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_3X3_HPP__