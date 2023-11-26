#ifndef I2C_MATH_MATRIX_6X6_HPP__
#define I2C_MATH_MATRIX_6X6_HPP__

#include "MatrixBase.hpp"

// C
extern "C" {
#include <emmintrin.h>
}

namespace PBL::Math
{

template < typename T >
class Matrix6x6 : public MatrixBase< T, 6u, 6u >
{
	using Parent = MatrixBase< T, 6u, 6u >;

public:
	using Parent::Parent;

	/// Constructs a 6x6 matrix with all elements initialized to the same value
	explicit constexpr Matrix6x6( T v ) noexcept
		: Parent{ v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v }
	{ }
};

using Matrix6x6f = Matrix6x6< float >;
using Matrix6x6d = Matrix6x6< double >;

} // namespace PBL::Math
#endif // I2C_MATH_MATRIX_6X6_HPP__