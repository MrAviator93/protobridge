#ifndef I2C_MATH_LINEAR_BASE_HPP__
#define I2C_MATH_LINEAR_BASE_HPP__

#include "Math.hpp"

// C++
#include <array>
#include <type_traits>

namespace PBL::Math
{

template < typename T >
struct AlignmentSelector;

// Specialization for large types
template < typename T >
	requires( sizeof( T ) == 8 )
struct AlignmentSelector< T >
{
	static constexpr std::size_t value{ 32 };
};

// Specialization for small types
template < typename T >
	requires( sizeof( T ) == 4 )
struct AlignmentSelector< T >
{
	static constexpr std::size_t value{ 16 };
};

template < typename T >
	requires( sizeof( T ) == 2 )
struct AlignmentSelector< T >
{
	static constexpr std::size_t value{ 8 };
};

template < typename T >
	requires( sizeof( T ) == 1 )
struct AlignmentSelector< T >
{
	static constexpr std::size_t value{ 4 };
};

template < typename T >
constexpr std::size_t AlignmentSelectorV = AlignmentSelector< T >::value;

template < typename T >
concept IsIntFloatOrDouble = std::is_same_v< T, int > || std::is_same_v< T, float > || std::is_same_v< T, double >;

template < typename T, std::size_t Size >
class VectorBase
{
	static constexpr std::size_t Alignment = AlignmentSelectorV< T >;

public:
	using ValueType = T;

	constexpr VectorBase() noexcept = default;

	template < typename... Args >
		requires( sizeof...( Args ) == Size )
	constexpr VectorBase( Args&&... args ) noexcept
		: m_data{ std::forward< Args >( args )... }
	{ }

	constexpr auto& data() noexcept { return m_data; }
	constexpr auto& data() const noexcept { return m_data; }

protected:
	alignas( Alignment ) std::array< T, Size > m_data;
};

} // namespace PBL::Math
#endif I2C_MATH_LINEAR_BASE_HPP__
