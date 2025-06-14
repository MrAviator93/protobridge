#ifndef PBL_UTILS_ENUM_FLAG_SET_HPP__
#define PBL_UTILS_ENUM_FLAG_SET_HPP__

// C++
#include <limits>
#include <cstdint>
#include <type_traits>

namespace pbl::utils
{

namespace detail
{

template < typename I, typename E >
concept EnumFlagCompatible = std::is_integral_v< I > && std::is_enum_v< E > && ( sizeof( I ) >= sizeof( E ) );

template < typename IntT, typename EnumT, typename... Bits >
concept EnumFlagVariadicArgs =
	( sizeof...( Bits ) > 1 ) && ( ( std::is_same_v< Bits, IntT > || std::is_same_v< Bits, EnumT > ) && ... );

} // namespace detail

template < typename I, typename E >
	requires detail::EnumFlagCompatible< I, E >
class EnumFlagSet final
{
public:
	using IntType = I;
	using EnumType = E;

	/// Default constructor.
	constexpr EnumFlagSet() noexcept = default;

	/// Constructor from an integral value.
	explicit constexpr EnumFlagSet( IntType v ) noexcept
		: m_value{ v }
	{ }

	/// Constructor from an enum value.
	explicit constexpr EnumFlagSet( EnumType e ) noexcept
		: m_value{ static_cast< IntType >( e ) }
	{ }

	/// Variadic constructor from multiple bits, accepts IntType and or EnumType.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	constexpr EnumFlagSet( Bits... bits ) noexcept
		: m_value{ static_cast< IntType >( ( static_cast< int >( bits ) | ... ) ) }
	{ }

	EnumFlagSet( const EnumFlagSet& ) = default;
	EnumFlagSet( EnumFlagSet&& ) = default;
	EnumFlagSet& operator=( const EnumFlagSet& ) = default;
	EnumFlagSet& operator=( EnumFlagSet&& ) = default;

	/// Resets all flags.
	constexpr void reset() noexcept { m_value = {}; }

	/// Sets a specific bit.
	constexpr void set( EnumType bit ) noexcept { m_value |= static_cast< IntType >( bit ); }

	/// Variadic set function to set multiple bits.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	constexpr void set( Bits... bits ) noexcept
	{
		( ( m_value |= static_cast< IntType >( bits ) ), ... );
	}

	/// Clears a specific bit.
	constexpr void clear( EnumType bit ) noexcept { m_value &= ~static_cast< IntType >( bit ); }

	///  Variadic clear function to clear multiple bits.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	constexpr void clear( Bits... bits ) noexcept
	{
		( ( m_value &= ~static_cast< IntType >( bits ) ), ... );
	}

	/// Flips a specific bit.
	constexpr void flip( EnumType bit ) noexcept { m_value ^= static_cast< IntType >( bit ); }

	/// Variadic toggle function to flip multiple bits.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	constexpr void flip( Bits... bits ) noexcept
	{
		( ( m_value ^= static_cast< IntType >( bits ) ), ... );
	}

	/// Tests if a specific bit is set.
	[[nodiscard]] constexpr bool test( EnumType bit ) const noexcept
	{
		return ( m_value & static_cast< IntType >( bit ) ) != 0;
	}

	/// Variadic test function to test multiple bits.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	[[nodiscard]] constexpr bool test( Bits... bits ) const noexcept
	{
		return ( ( m_value & static_cast< IntType >( bits ) ) && ... );
	}

	/**
	 * @brief Checks if all bits are set in the internal value representation.
	 * @note This compares against the maximum value of the underlying type, which may include bits not defined in the enum.
	 */
	[[nodiscard]] constexpr bool full() const noexcept { return m_value == std::numeric_limits< I >::max(); }

	/**
	 * @brief Checks if all specified flags are set.
	 * @tparam Bits One or more enum or integral flag values.
	 * @param bits The flags to check.
	 * @return true if all provided flags are set; false otherwise.
	 */
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	[[nodiscard]] constexpr bool all( Bits... bits ) const noexcept
	{
		const IntType mask = ( static_cast< IntType >( bits ) | ... );
		return ( m_value & mask ) == mask;
	}

	/// Checks if any bit is set.
	[[nodiscard]] constexpr bool any() const noexcept { return m_value != 0; }

	/// @brief Checks if any of the specified flags are set.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	[[nodiscard]] constexpr bool any( Bits... bits ) const noexcept
	{
		const IntType mask = ( static_cast< IntType >( bits ) | ... );
		return ( m_value & mask ) != 0;
	}

	/// Checks if no bits are set.
	[[nodiscard]] constexpr bool none() const noexcept { return m_value == 0; }

	/// @brief Checks if none of the specified flags are set.
	template < typename... Bits >
		requires detail::EnumFlagVariadicArgs< IntType, EnumType, Bits... >
	[[nodiscard]] constexpr bool none( Bits... bits ) const noexcept
	{
		const IntType mask = ( static_cast< IntType >( bits ) | ... );
		return ( m_value & mask ) == 0;
	}

	/// Counts the number of set bits.
	[[nodiscard]] constexpr std::size_t count() const noexcept
	{
		std::size_t count{};
		for( IntType val = m_value; val != 0; val >>= 1 )
		{
			if( val & 1 )
			{
				++count;
			}
		}

		return count;
	}

	[[nodiscard]] constexpr auto value() const noexcept { return m_value; }

	[[nodiscard]] constexpr auto operator<=>( const EnumFlagSet& ) const noexcept = default;

private:
	IntType m_value{};
};

} // namespace pbl::utils

#include "EnumFlagSet.ipp"

#endif // PBL_UTILS_ENUM_FLAG_SET_HPP__
