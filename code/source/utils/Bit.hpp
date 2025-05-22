#ifndef PBL_UTILS_BIT_HPP__
#define PBL_UTILS_BIT_HPP__

// C++
#include <cstdint>
#include <type_traits>

namespace pbl::utils
{

template < typename T, std::size_t Shift >
[[nodiscard]] constexpr T bit() noexcept
{
	static_assert( std::is_integral_v< T >, "T must be an integral type." );
	static_assert( Shift > 0, "Bit shift must be greater than 0 for 1-based indexing." );
	static_assert( Shift <= sizeof( T ) * 8, "Bit shift can't exceed bit width of T." );

	return static_cast< T >( 1 ) << ( Shift - 1 );
}

} // namespace pbl::utils
#endif // PBL_UTILS_BIT_HPP__
