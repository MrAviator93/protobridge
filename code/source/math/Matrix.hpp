
#ifndef I2C_UTILS_MATRIX_HPP__
#define I2C_UTILS_MATRIX_HPP__

#include "Math.hpp"
#include "Linear.hpp"

// C++
#include <array>
#include <format>
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

template <>
struct std::formatter< PBL::Utils::Matrix2x2 >
{
	// Parses format specifications of the form ['f' | 'e']
	constexpr auto parse( std::format_parse_context& ctx ) { return ctx.begin(); }

	// Formats the point p using the parsed format
	template < typename FormatContext >
	auto format( const PBL::Utils::Matrix2x2& mt, FormatContext& ctx ) const
	{
		return format_to(
			ctx.out(), "Matrix2x2 [{}, {}, {}, {}] ", mt.data[ 0 ], mt.data[ 1 ], mt.data[ 2 ], mt.data[ 3 ] );
	}
};

#endif //I2C_UTILS_MATRIX_HPP__
