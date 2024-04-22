#ifndef I2C_MATH_LINEAR_FORMAT_HPP__
#define I2C_MATH_LINEAR_FORMAT_HPP__

#include "Linear.hpp"

// C++
#include <format>

template <>
struct std::formatter< pbl::math::Vector2f >
{
	// Parses format specifications of the form ['f' | 'e']
	constexpr auto parse( std::format_parse_context& ctx ) { return ctx.begin(); }

	// Formats the point p using the parsed format
	template < typename FormatContext >
	auto format( const pbl::math::Vector2f& mt, FormatContext& ctx ) const
	{
		return format_to(
			ctx.out(), "Vector2f [{}, {}, {}] ", Vector2f.x, Vector2f.y, Vector2f.z );
	}
};

#endif // I2C_MATH_LINEAR_FORMAT_HPP__
