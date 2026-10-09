#include "ICBase.hpp"
#include <array>
#include <vector>
#include <thread>
#include <limits>

namespace pbl::i2c
{

bool v1::ICBase::write( const std::span< const std::uint8_t > data )
{
	return m_busController.write( m_icAddress, data ).has_value();
}

bool v1::ICBase::write( const std::uint8_t reg, const std::uint8_t value )
{
	const std::array< std::uint8_t, 2 > data{ reg, value };
	return write( data );
}

bool v1::ICBase::write( const std::uint8_t reg, const std::span< const std::uint8_t > data )
{
	std::vector< std::uint8_t > request{ reg };
	request.insert( request.end(), data.begin(), data.end() );
	return write( request );
}

bool v1::ICBase::write( const std::uint8_t reg, const std::uint8_t* pData, const std::uint8_t size )
{
	return write( reg, std::span{ pData, size } );
}

std::int16_t v1::ICBase::read( std::span< std::uint8_t > data )
{
	if( data.size() > static_cast< std::size_t >( std::numeric_limits< std::int16_t >::max() ) ) return -1;
	return m_busController.read( m_icAddress, data ) ? static_cast< std::int16_t >( data.size() ) : -1;
}

bool v1::ICBase::read( const std::uint8_t reg, std::uint8_t& result )
{
	return read( reg, &result, 1 ) == 1;
}

std::int16_t v1::ICBase::read( const std::uint8_t reg, std::uint8_t* pData, std::uint16_t size )
{
	if( size > std::numeric_limits< std::int16_t >::max() ) return -1;
	return m_busController.writeRead( m_icAddress, std::span{ &reg, 1 }, std::span{ pData, size } )
			   ? static_cast< std::int16_t >( size )
			   : -1;
}

void v1::ICBase::sleep( const std::chrono::milliseconds sleepTimeMs )
{
	std::this_thread::sleep_for( sleepTimeMs );
}

void v1::ICBase::sleep( const std::chrono::microseconds sleepTimeUs )
{
	std::this_thread::sleep_for( sleepTimeUs );
}

} // namespace pbl::i2c
