#ifndef PBL_UTILS_ENUM_FLAG_SET_HPP__
#define PBL_UTILS_ENUM_FLAG_SET_HPP__

// C++
#include <cstdint>
#include <type_traits>

namespace pbl::utils
{

template < typename I, typename E >
	requires( std::is_integral_v< I > && std::is_enum_v< E > && sizeof( I ) >= sizeof( E ) )
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
		requires( sizeof...( Bits ) > 1 &&
				  ( ( std::is_same_v< Bits, IntType > || std::is_same_v< Bits, EnumType > ) && ... ) )
	constexpr EnumFlagSet( Bits... bits ) noexcept
		: m_value{ ( ( std::is_same_v< Bits, EnumType > ? static_cast< IntType >( bits ) : bits ) | ... ) }
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
		requires( sizeof...( Bits ) > 1 &&
				  ( ( std::is_same_v< Bits, IntType > || std::is_same_v< Bits, EnumType > ) && ... ) )
	constexpr void set( Bits... bits ) noexcept
	{
		( ( m_value |= ( std::is_same_v< Bits, EnumType > ? static_cast< IntType >( bits ) : bits ) ), ... );
	}

	/// Clears a specific bit.
	constexpr void clear( EnumType bit ) noexcept { m_value &= ~static_cast< IntType >( bit ); }

	///  Variadic clear function to clear multiple bits.
	template < typename... Bits >
		requires( sizeof...( Bits ) > 1 &&
				  ( ( std::is_same_v< Bits, IntType > || std::is_same_v< Bits, EnumType > ) && ... ) )
	constexpr void clear( Bits... bits ) noexcept
	{
		( ( m_value &= ~( std::is_same_v< Bits, EnumType > ? static_cast< IntType >( bits ) : bits ) ), ... );
	}

	/// Flips a specific bit.
	constexpr void flip( EnumType bit ) noexcept { m_value ^= static_cast< IntType >( bit ); }

	/// Variadic toggle function to flip multiple bits.
	template < typename... Bits >
		requires( sizeof...( Bits ) > 1 &&
				  ( ( std::is_same_v< Bits, IntType > || std::is_same_v< Bits, EnumType > ) && ... ) )
	constexpr void flip( Bits... bits ) noexcept
	{
		( ( m_value ^= ( std::is_same_v< Bits, EnumType > ? static_cast< IntType >( bits ) : bits ) ), ... );
	}

	/// Tests if a specific bit is set.
	[[nodiscard]] constexpr bool test( EnumType bit ) const noexcept
	{
		return ( m_value & static_cast< IntType >( bit ) ) != 0;
	}

	/// Variadic test function to test multiple bits.
	template < typename... Bits >
		requires( sizeof...( Bits ) > 1 &&
				  ( ( std::is_same_v< Bits, IntType > || std::is_same_v< Bits, EnumType > ) && ... ) )
	constexpr void test( Bits... bits ) noexcept
	{
		return ( ( m_value & ( std::is_same_v< Bits, EnumType > ? static_cast< IntType >( bits ) : bits ) ) && ... );
	}

	/// Checks if all bits are set.
	[[nodiscard]] constexpr bool all() const noexcept
	{
		if( m_value <= 0 ) return false;
		
		return m_value & ( m_value - 1 ) == 0;
	}

	/// Checks if any bit is set.
	[[nodiscard]] constexpr bool any() const noexcept { return m_value != 0; }

	/// Checks if no bits are set.
	[[nodiscard]] constexpr bool none() const noexcept { return m_value == 0; }

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
#endif // PBL_UTILS_ENUM_FLAG_SET_HPP__
