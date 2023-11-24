#ifndef I2C_MATH_MATRIX_FORMATER_HPP__
#define I2C_MATH_MATRIX_FORMATER_HPP__

#include "Matrix.hpp"

// C++
#include <format>

template <>
struct std::formatter< PBL::Math::Matrix2x2 >
{
	// Parses format specifications of the form ['f' | 'e']
	constexpr auto parse( std::format_parse_context& ctx ) { return ctx.begin(); }

	// Formats the point p using the parsed format
	template < typename FormatContext >
	auto format( const PBL::Math::Matrix2x2& mt, FormatContext& ctx ) const
	{
		return format_to(
			ctx.out(), "Matrix2x2 [{}, {}, {}, {}] ", mt.data[ 0 ], mt.data[ 1 ], mt.data[ 2 ], mt.data[ 3 ] );
	}
};

#endif // I2C_MATH_MATRIX_FORMATER_HPP__
