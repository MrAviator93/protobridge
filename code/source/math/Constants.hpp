#ifndef PBL_MATH_CONSTANTS_HPP__
#define PBL_MATH_CONSTANTS_HPP__

// C++
#include <type_traits>

namespace pbl::math::constants
{

namespace detail
{

template < typename T >
struct Values;

template <>
struct Values< float >
{
	static constexpr float pi = 3.1415927f;
	static constexpr float e = 2.7182818f;
};

template <>
struct Values< double >
{
	static constexpr double pi = 3.14159265358979323846;
	static constexpr double e = 2.71828182845904523536;
};

template <>
struct Values< long double >
{
	static constexpr long double pi = 3.141592653589793238462643383279502884L;
	static constexpr long double e = 2.718281828459045235360287471352662497L;
};

} // namespace detail

// Generic getter (optional convenience layer)
template < typename T >
constexpr T Pi = detail::Values< T >::pi;

template < typename T >
constexpr T E = detail::Values< T >::e;

} // namespace pbl::math::constants
#endif // PBL_MATH_CONSTANTS_HPP__