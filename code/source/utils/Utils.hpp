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
	T a;
	T b;
	const T operator()( const T& current ) { return toggle< T >( current, a, b ); }
};

} // namespace pbl::utils
#endif // PBL_UTILS_UTILS_HPP__
