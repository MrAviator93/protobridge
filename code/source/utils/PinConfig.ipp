#ifndef PBL_UTILS_PIN_CONFIG_IPP__
#define PBL_UTILS_PIN_CONFIG_IPP__

namespace pbl::utils
{

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
template < typename... Pins >
constexpr PinConfig< T, Default, ToBool, FromBool >::PinConfig( Pins... pins ) noexcept
{
	static_assert( sizeof...( Pins ) <= kMaxPins, "Too many pins provided, maximum is 8" );

	auto pinTuple = std::make_tuple( pins... );

	auto setPins = [ this, &pinTuple ]< std::size_t... Is >( std::index_sequence< Is... > ) {
		( ( bitset[ Is ] = ToBool{}( std::get< Is >( pinTuple ) ) ), ... );
	};

	setPins( std::make_index_sequence< sizeof...( Pins ) >{} );
}

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
template < size_t Index >
constexpr T PinConfig< T, Default, ToBool, FromBool >::pin() const noexcept
{
	static_assert( Index < kMaxPins, "Index out of bounds" );
	return FromBool{}( bitset[ Index ] );
}

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
constexpr std::optional< T > PinConfig< T, Default, ToBool, FromBool >::pin( std::size_t index ) const noexcept
{
	if( index >= kMaxPins )
	{
		return std::nullopt;
	}

	return FromBool{}( bitset[ index ] );
}

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
constexpr std::optional< T > PinConfig< T, Default, ToBool, FromBool >::operator[]( std::size_t index ) const noexcept
{
	if( index >= kMaxPins )
	{
		return std::nullopt;
	}
	return FromBool{}( bitset[ index ] );
}

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
template < size_t Index >
constexpr void PinConfig< T, Default, ToBool, FromBool >::setPin( T value ) noexcept
{
	static_assert( Index < kMaxPins );
	bitset[ Index ] = ToBool{}( value );
}

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
constexpr bool PinConfig< T, Default, ToBool, FromBool >::setPin( std::size_t index, T value ) noexcept
{
	if( index >= kMaxPins )
	{
		return false;
	}

	bitset[ index ] = ToBool{}( value );

	return true;
}

template < typename T, T Default, typename ToBool, typename FromBool >
	requires detail::PinCompatible< T, ToBool, FromBool >
template < typename Func >
constexpr void PinConfig< T, Default, ToBool, FromBool >::forEachPin( Func&& fn ) const
{
	for( std::size_t i{}; i < kMaxPins; ++i )
	{
		fn( i, FromBool{}( bitset[ i ] ) );
	}
}

} // namespace pbl::utils
#endif // PBL_UTILS_PIN_CONFIG_IPP__
