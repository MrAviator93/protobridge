
#ifndef I2C_UTILS_MATRIX_HPP__
#define I2C_UTILS_MATRIX_HPP__

#include "Math.hpp"
#include "Linear.hpp"

// C++
#include <array>
#include <type_traits>

namespace PBL::Utils
{

template < typename T >
	requires std::is_floating_point_v< T >
class Matrix2x2
{
public:
	Matrix2x2() = default;

	constexpr Matrix2x2( T v ) noexcept
		: m_data{ v, v, v, v }
	{ }

	constexpr Matrix2x2( T m00, T m01, T m10, T m11 ) noexcept
		: m_data{ { m00, m01, m10, m11 } }
	{ }

	constexpr auto& data() noexcept { return m_data; }
	constexpr auto& data() const noexcept { return m_data; }

private:
	std::array< T, 4 > m_data;
};

} // namespace PBL::Utils
#endif //I2C_UTILS_MATRIX_HPP__
