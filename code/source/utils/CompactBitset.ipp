#ifndef PBL_UTILS_COMPACT_BITSET_IPP__
#define PBL_UTILS_COMPACT_BITSET_IPP__

namespace pbl::utils
{

template < std::size_t N >
constexpr std::size_t CompactBitset< N >::storageSize() noexcept
{
	if constexpr( kIsArray )
	{
		return ( N + 7 ) / 8 * sizeof( std::uint8_t );
	}
	else
	{
		return sizeof( std::uint8_t );
	}
}

template < std::size_t N >
constexpr void CompactBitset< N >::set( std::size_t bit, bool value ) noexcept
{
	// TODO: Add bounds checking

	if constexpr( kIsArray )
	{
		const std::size_t byteIndex = bit / 8;
		const std::size_t bitIndex = bit % 8;

		if( value )
		{
			m_storage[ byteIndex ] |= ( 1u << bitIndex );
		}
		else
		{
			m_storage[ byteIndex ] &= ~( 1u << bitIndex );
		}
	}
	else
	{
		if( value )
		{
			m_storage |= ( 1u << bit );
		}
		else
		{
			m_storage &= static_cast< std::uint8_t >( ~( 1u << bit ) );
		}
	}
}

template < std::size_t N >
constexpr bool CompactBitset< N >::test( std::size_t bit ) noexcept
{
	// TODO: Add bounds checking

	if constexpr( kIsArray )
	{
		const std::size_t byteIndex = bit / 8;
		const std::size_t bitIndex = bit % 8;

		return m_storage[ byteIndex ] & ( 1u << bitIndex );
	}
	else
	{
		return m_storage & ( 1u << bit );
	}
}

template < std::size_t N >
constexpr void CompactBitset< N >::reset() noexcept
{
	if constexpr( kIsArray )
	{
		std::ranges::for_each( m_storage, []( auto& byte ) { byte = {}; } );
	}
	else
	{
		m_storage = {};
	}
}

template < std::size_t N >
constexpr void CompactBitset< N >::reset( std::size_t bit ) noexcept
{
	// TODO: Add bounds checking

	if constexpr( kIsArray )
	{
		const std::size_t byteIndex = bit / 8;
		const std::size_t bitIndex = bit % 8;

		m_storage[ byteIndex ] &= ~( 1u << bitIndex );
	}
	else
	{
		m_storage &= static_cast< std::uint8_t >( ~( 1u << bit ) );
	}
}

template < std::size_t N >
constexpr void CompactBitset< N >::flip( std::size_t bit ) noexcept
{
	// TODO: Add bounds checking

	if constexpr( kIsArray )
	{
		const std::size_t byteIndex = bit / 8;
		const std::size_t bitIndex = bit % 8;

		m_storage[ byteIndex ] ^= ( 1u << bitIndex );
	}
	else
	{
		m_storage ^= ( 1u << bit );
	}
}

template < std::size_t N >
constexpr bool CompactBitset< N >::all() const noexcept
{
	if constexpr( kIsArray )
	{
		auto allBitsSet = []( auto byte ) { return byte == static_cast< std::uint8_t >( 0xFF ); };
		std::ranges::all_of( m_storage, allBitsSet );
	}
	else
	{
		return m_storage == static_cast< std::uint8_t >( ( 1 << N ) - 1 );
	}
}

template < std::size_t N >
constexpr bool CompactBitset< N >::any() const noexcept
{
	if constexpr( kIsArray )
	{
		return std::ranges::any_of( m_storage, []( auto byte ) { return byte != 0; } );
	}
	else
	{
		return m_storage != 0;
	}
}

template < std::size_t N >
constexpr std::size_t CompactBitset< N >::count() const noexcept
{
	if constexpr( kIsArray )
	{
		auto counter = []( std::size_t sum, auto byte ) { return sum + countBitsInByte( byte ); };
		return std::accumulate( m_storage.begin(), m_storage.end(), std::size_t{}, counter );
	}
	else
	{
		return countBitsInByte( m_storage );
	}
}

template < std::size_t N >
bool CompactBitset< N >::operator==( const CompactBitset& other ) const noexcept
{
	if constexpr( kIsArray )
	{
		return std::ranges::equal( m_storage, other.m_storage );
	}
	else
	{
		return m_storage == other.m_storage;
	}
}

template < std::size_t N >
constexpr std::size_t CompactBitset< N >::countBitsInByte( std::uint8_t byte ) noexcept
{
	if( byte == 0 )
	{
		return 0;
	}

	std::size_t count{};
	while( byte )
	{
		byte &= ( byte - 1 );
		++count;
	}

	return count;
}

} // namespace pbl::utils
#endif // PBL_UTILS_COMPACT_BITSET_IPP__
