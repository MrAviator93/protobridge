#include "ICBase.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

bool v1::ICBase::write( const std::span< const std::uint8_t > data )
{
	return m_busController.write( m_icAddress, data );
}

bool v1::ICBase::write( const std::uint8_t reg, const std::uint8_t value )
{
	return m_busController.write( m_icAddress, reg, value );
}

bool v1::ICBase::write( const std::uint8_t reg, const std::span< const std::uint8_t > data )
{
	return m_busController.write( m_icAddress, reg, data );
}

bool v1::ICBase::write( const std::uint8_t reg, const std::uint8_t* pData, const std::uint8_t size )
{
	return m_busController.write( m_icAddress, reg, pData, size );
}

std::int16_t v1::ICBase::read( std::span< std::uint8_t > data )
{
	return m_busController.read( m_icAddress, data );
}

bool v1::ICBase::read( const std::uint8_t reg, std::uint8_t& result )
{
	return m_busController.read( m_icAddress, reg, result );
}

std::int16_t v1::ICBase::read( const std::uint8_t reg, std::uint8_t* pData, std::uint16_t size )
{
	return m_busController.read( m_icAddress, reg, pData, size );
}

void v1::ICBase::sleep( const std::chrono::milliseconds sleepTimeMs )
{
	m_busController.sleep( sleepTimeMs );
}

void v1::ICBase::sleep( const std::chrono::microseconds sleepTimeUs )
{
	m_busController.sleep( sleepTimeUs );
}

} // namespace pbl::i2c
