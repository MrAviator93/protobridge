#ifndef I2C_MATH_MATRIX_12X12_HPP__
#define I2C_MATH_MATRIX_12X12_HPP__

#include "MatrixBase.hpp"

namespace PBL::Math
{

template < typename T >
class Matrix12x12 : public MatrixBase< T, 12u, 12u >
{
	using Parent = MatrixBase< T, 12u, 12u >;

public:
	using Parent::Parent;

	/// Constructs a 6x6 matrix with all elements initialized to the same value
	explicit constexpr Matrix12x12( T v ) noexcept
		: Parent{ v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v,
				  v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v }
	{ }
};

using Matrix12x12f = Matrix12x12< float >;
using Matrix12x12d = Matrix12x12< double >;

} // namespace PBL::Math
#endif // I2C_MATH_MATRIX_12X12_HPP__