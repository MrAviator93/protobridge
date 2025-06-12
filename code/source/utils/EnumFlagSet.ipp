#ifndef PBL_UTILS_ENUM_FLAG_SET_IPP__
#define PBL_UTILS_ENUM_FLAG_SET_IPP__

namespace pbl::utils
{

/// Enable bitwise OR for strongly typed enums
template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E operator|( E lhs, E rhs ) noexcept
{
	using U = std::underlying_type_t< E >;
	return static_cast< E >( static_cast< U >( lhs ) | static_cast< U >( rhs ) );
}

template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E operator&( E lhs, E rhs ) noexcept
{
	using U = std::underlying_type_t< E >;
	return static_cast< E >( static_cast< U >( lhs ) & static_cast< U >( rhs ) );
}

template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E operator^( E lhs, E rhs ) noexcept
{
	using U = std::underlying_type_t< E >;
	return static_cast< E >( static_cast< U >( lhs ) ^ static_cast< U >( rhs ) );
}

template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E operator~( E value ) noexcept
{
	using U = std::underlying_type_t< E >;
	return static_cast< E >( ~static_cast< U >( value ) );
}

template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E& operator|=( E& lhs, E rhs ) noexcept
{
	lhs = lhs | rhs;
	return lhs;
}

template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E& operator&=( E& lhs, E rhs ) noexcept
{
	lhs = lhs & rhs;
	return lhs;
}

template < typename E >
	requires std::is_enum_v< E >
[[nodiscard]] constexpr E& operator^=( E& lhs, E rhs ) noexcept
{
	lhs = lhs ^ rhs;
	return lhs;
}

} // namespace pbl::utils
#endif // PBL_UTILS_ENUM_FLAG_SET_IPP__
