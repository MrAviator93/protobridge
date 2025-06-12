#ifndef PBL_UTILS_PIN_CONFIG_HPP__
#define PBL_UTILS_PIN_CONFIG_HPP__

// C++
#include <bitset>
#include <type_traits>

namespace pbl::utils
{

namespace detail
{

template < typename T, typename ToBool, typename FromBool >
concept PinCompatible =
	// T Must be either a boolean or an enumerator
	( std::same_as< T, bool > || std::is_enum_v< T > ) &&
	// ToBool must be nothrow invocable
	std::is_nothrow_invocable_r_v< bool, ToBool, T > &&
	// FromBool must be nothrow invocable
	std::is_nothrow_invocable_r_v< T, FromBool, bool >;

template < typename T >
struct DefaultToBool
{
	[[nodiscard]] constexpr bool operator()( T v ) const noexcept { return static_cast< bool >( v ); }
};

template < typename T >
struct DefaultFromBool
{
	[[nodiscard]] constexpr T operator()( bool v ) const noexcept { return static_cast< T >( v ); }
};

} // namespace detail

/**
 * @brief Configuration for an 8-bit pin set.
 * 
 * Provides a configurable interface to manage 8 digital pins with compile-time safety.
 * 
 * @todo Consider adding size_t to define bitset size or PinCount, or leave
 * as 8, as it's the default one.
 * 
 * @todo Add more unit tests for this class, for improved test coverage.
 * 
 * @tparam T Must be either `bool` or an `enum` with exactly two states (true/false, ON, OFF, etc.).
 * @tparam Default Default value to initialize the pin configuration.
 * @tparam ToBool Callable to convert `T` to `bool`.
 * @tparam FromBool Callable to convert `bool` back to `T`
 */
template < typename T,
		   T Default,
		   typename ToBool = detail::DefaultToBool< T >,
		   typename FromBool = detail::DefaultFromBool< T > >
	requires detail::PinCompatible< T, ToBool, FromBool >
struct PinConfig final
{
	inline static constexpr std::size_t kMaxPins{ 8 };

	template < std::size_t Idx, typename PinType >
	struct Pin
	{
		static constexpr std::size_t Index = Idx;
		PinType value;
	};

	/// Default constructor initializes all pins with the default value.
	constexpr PinConfig() noexcept { bitset.set( ToBool{}( Default ) ); }

	/// Constructor to initialize all pins with a single value.
	constexpr explicit PinConfig( T v ) noexcept { bitset.set( ToBool{}( v ) ); }

	/**
	 * @brief Constructor to initialize pins with individual values.
	 * 
	 * Accepts up to 8 pin values and sets them sequentially.
	 * 
	 * @tparam Pins Variadic template for pin values.
	 * @param pins Pin values to set.
	 */
	template < typename... Pins >
	constexpr PinConfig( Pins... pins ) noexcept;

	/// TBW
	template < typename... Pins >
		requires( ( sizeof...( Pins ) > 0 ) && ... && requires( Pins p ) { p.value; } )
	constexpr explicit PinConfig( Pins... pins ) noexcept
	{
		( bitset.set( Pins::Index, ToBool{}( pins.value ) ), ... );
	}

	/// Access individual pin value using pin<0>() interface.
	template < size_t Index >
	[[nodiscard]] constexpr T pin() const noexcept;

	/// TBW
	[[nodiscard]] constexpr std::optional< T > pin( std::size_t index ) const noexcept;

	/// TBW
	[[nodiscard]] constexpr std::optional< T > operator[]( std::size_t index ) const noexcept;

	/// Mutates pin state
	template < size_t Index >
	constexpr void setPin( T value ) noexcept;

	/// TBW
	[[nodiscard]] constexpr bool setPin( std::size_t index, T value ) noexcept;

	/// For each pin
	template < typename Func >
	constexpr void forEachPin( Func&& fn ) const;

	/// Converts the PinConfig object to its underlying bitset representation.
	[[nodiscard]] constexpr operator auto() const noexcept { return bitset; }

	/// Comparison operator.
	constexpr auto operator<=>( const PinConfig& ) const = default;

	/// Bitset representing the state of the 8 pins.
	std::bitset< kMaxPins > bitset{};
};

} // namespace pbl::utils

#include "PinConfig.ipp"

#endif // PBL_UTILS_PIN_CONFIG_HPP__
