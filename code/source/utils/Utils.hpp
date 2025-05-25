#ifndef PBL_UTILS_UTILS_HPP__
#define PBL_UTILS_UTILS_HPP__

namespace pbl::utils
{

template < typename T >
constexpr T toggle( const T& current, const T& a, const T& b )
{
	return ( current == a ) ? b : a;
}

template < typename T >
struct Toggle
{
	constexpr Toggle( T ta, T tb ) noexcept
		: a{ ta }
		, b{ tb }
	{ }

	T a;
	T b;

	[[nodiscard]] constexpr T operator()( const T& current ) const noexcept { return toggle< T >( current, a, b ); }
};

template < typename T >
[[nodiscard]] constexpr T select( const bool condition, const T& a, const T& b )
{
	return condition ? a : b;
}

template < typename T >
struct Select
{
	constexpr Select( T ta, T tb )
		: a{ ta }
		, b{ tb }
	{ }

	T a;
	T b;

	[[nodiscard]] constexpr T operator()( const bool condition ) const noexcept
	{
		return select< T >( condition, a, b );
	}
};

} // namespace pbl::utils
#endif // PBL_UTILS_UTILS_HPP__
