#ifndef PBL_UTILS_UTILS_HPP__
#define PBL_UTILS_UTILS_HPP__

namespace pbl::utils
{

/// Selects between two values based on a boolean condition.
/// Returns 'a' if condition is true, otherwise returns 'b'.
template < typename T >
[[nodiscard]] constexpr T select( const bool condition, const T& a, const T& b )
{
	return condition ? a : b;
}

/// Functor version of 'select'. Stores two values (a, b) and chooses one based on a boolean condition.
template < typename T >
struct Select
{
	constexpr Select( T ta, T tb )
		: a{ ta }
		, b{ tb }
	{ }

	[[nodiscard]] constexpr T operator()( const bool condition ) const noexcept
	{
		return select< T >( condition, a, b );
	}

	T a;
	T b;
};

/// Returns the toggled value between 'a' and 'b', depending on 'current'.
/// If current == a, returns b; otherwise, returns a.
/// Note: This is an asymmetric toggle — if current matches neither, it still returns a.
template < typename T >
constexpr T toggle( const T& current, const T& a, const T& b )
{
	return ( current == a ) ? b : a;
}

/// Functor version of 'toggle'. Stores two values and toggles between them.
template < typename T >
struct Toggle
{
	constexpr Toggle( T ta, T tb ) noexcept
		: a{ ta }
		, b{ tb }
	{ }

	[[nodiscard]] constexpr T operator()( const T& current ) const noexcept { return toggle< T >( current, a, b ); }

	T a;
	T b;
};

} // namespace pbl::utils
#endif // PBL_UTILS_UTILS_HPP__
