#ifndef PBL_UTILS_PIN_CONFIG_HPP__
#define PBL_UTILS_PIN_CONFIG_HPP__

// C++
#include <bitset>
#include <type_traits>

namespace pbl::utils
{

/**
 * @brief Configuration for an 8-bit pin set.
 * 
 * Provides a configurable interface to manage 8 digital pins with compile-time safety.
 * 
 * @todo Consider adding size_t to define bitset size or PinCount, or leave
 * as 8, as it's the default one.
 * 
 * @tparam T Must be either `bool` or an `enum` with exactly two states.
 * @tparam Default Default value to initialize the pin configuration.
 * @tparam ToBool Callable to convert `T` to `bool`.
 * @tparam FromBool Callable to convert `bool` back to `T`
 */
template < typename T,
		   T Default,
		   typename ToBool = decltype( []( T v ) { return v; } ),
		   typename FromBool = decltype( []( T v ) { return v; } ) >
	requires( std::same_as< T, bool > || std::is_enum_v< T > ) &&
			std::same_as< std::invoke_result_t< ToBool, T >, bool > &&
			std::same_as< std::invoke_result_t< FromBool, bool >, T >
struct PinConfig final
{
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
	constexpr PinConfig( Pins... pins ) noexcept
	{
		static_assert( sizeof...( Pins ) <= 8, "Too many pins provided, maximum is 8" );

		constexpr auto pinTuple = std::tuple{ pins... }; // Store pins in a tuple

		constexpr auto setPins = [ this, &pinTuple ]( auto... indices ) {
			( ( bitset[ indices ] = ToBool{}( std::get< indices >( pinTuple ) ) ), ... );
		};

		setPins( std::make_index_sequence< sizeof...( Pins ) >{} );
	}

	/// Access individual pin value using pin<0>() interface.
	template < size_t Index >
	[[nodiscard]] constexpr T pin() const noexcept
	{
		static_assert( Index < 8, "Index out of bounds" );
		return FromBool{}( bitset[ Index ] );
	}

    /// Converts the PinConfig object to its underlying bitset representation.
	[[nodiscard]] constexpr operator auto() const noexcept { return bitset; }

    /// Comparison operator.
	constexpr auto operator<=>( const PinConfig& ) const = default;

    /// Bitset representing the state of the 8 pins.
	std::bitset< 8 > bitset{};
};

} // namespace pbl::utils
#endif // PBL_UTILS_PIN_CONFIG_HPP__
