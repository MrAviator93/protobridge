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

	constexpr EnumFlagSet() noexcept = default;

	explicit constexpr EnumFlagSet( IntType v ) noexcept
		: m_value{ v }
	{ }

	explicit constexpr EnumFlagSet( EnumType e ) noexcept
		: m_value{ static_cast< IntType >( e ) }
	{ }

	template < typename... Bits >
		requires( ( std::is_same_v< Bits, IntType > || std::is_same_v< Bits, EnumType > ) && ... )
	constexpr EnumFlagSet( Bits... bits ) noexcept
		: m_value{ ( ( std::is_same_v< Bits, EnumType > ? static_cast< IntType >( bits ) : bits ) | ... ) }
	{ }

	EnumFlagSet( const EnumFlagSet& ) = default;
	EnumFlagSet( EnumFlagSet&& ) = default;
	EnumFlagSet& operator=( const EnumFlagSet& ) = default;
	EnumFlagSet& operator=( EnumFlagSet&& ) = default;

	constexpr void reset() noexcept { m_value = {}; }

	constexpr void set( EnumType bit ) noexcept { m_value |= static_cast< IntType >( bit ); }

	constexpr void clear( EnumType bit ) noexcept { m_value &= ~static_cast< IntType >( bit ); }

	constexpr void toggle( EnumType bit ) noexcept { m_value ^= static_cast< IntType >( bit ); }

	[[nodiscard]] constexpr bool test( EnumType bit ) const noexcept
	{
		return ( m_value & static_cast< IntType >( bit ) ) != 0;
	}

	[[nodiscard]] constexpr bool any() const noexcept { return m_value != 0; }

	[[nodiscard]] constexpr bool none() const noexcept { return m_value == 0; }

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

private:
	IntType m_value{};
};

} // namespace pbl::utils
#endif // PBL_UTILS_ENUM_FLAG_SET_HPP__
