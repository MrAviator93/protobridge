#ifndef PBL_MATH_CONSTANTS_HPP__
#define PBL_MATH_CONSTANTS_HPP__

// C++
#include <type_traits>

namespace pbl::math::constants
{

template < typename T >
struct Values;

template <>
struct Values< float >
{
	static constexpr float pi = 3.1415927f; // Accurate to float precision
};

template <>
struct Values< double >
{
	static constexpr double pi = 3.14159265358979323846;
};

template <>
struct Values< long double >
{
	static constexpr long double pi = 3.141592653589793238462643383279502884L;
};

// Generic getter (optional convenience layer)
template < typename T >
constexpr T Pi = Values< T >::pi;

} // namespace pbl::math::constants
#endif // PBL_MATH_CONSTANTS_HPP__