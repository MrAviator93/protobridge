#ifndef I2C_MATH_MATRIX_HPP__
#define I2C_MATH_MATRIX_HPP__

#include "MatrixBase.hpp"

// C
extern "C" {
#include <emmintrin.h>
}

namespace pbl::math
{

/**
 * @brief Generic Matrix class
 * 
 * @tparam T The type of the elements in the matrix.
 * @tparam Rows The number of rows in the matrix.
 * @tparam Columns The number of columns in the matrix.
 */
template < typename T, std::size_t Rows, std::size_t Columns >
class Matrix : MatrixBase< T, Rows, Columns >
{
	using Parent = MatrixBase< T, Rows, Columns >;

public:
	using Parent::MatrixBase;

	/// Constructs a matrix with all elements initialized to the same value
	explicit constexpr Matrix( T v ) noexcept { std::ranges::fill( this->m_data, v ); }
};

} // namespace pbl::math
#endif // I2C_MATH_MATRIX_HPP__
