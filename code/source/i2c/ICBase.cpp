#include "ICBase.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

bool ICBase::write( std::uint8_t reg, std::uint8_t value )
{
	return m_busController.write( m_icAddress, reg, value );
}

bool ICBase::write( std::uint8_t reg, std::span< std::uint8_t > data )
{
	return m_busController.write( m_icAddress, reg, data );
}

bool ICBase::write( std::uint8_t reg, std::uint8_t* pData, std::uint8_t size )
{
	return m_busController.write( m_icAddress, reg, pData, size );
}

bool ICBase::read( std::uint8_t reg, std::uint8_t& result )
{
	return m_busController.read( m_icAddress, reg, result );
}

std::int16_t ICBase::read( std::uint8_t reg, std::uint8_t* pData, std::uint16_t size )
{
	return m_busController.read( m_icAddress, reg, pData, size );
}

void ICBase::sleep( std::chrono::milliseconds sleepTimeMs )
{
	m_busController.sleep( sleepTimeMs );
}

void ICBase::sleep( std::chrono::microseconds sleepTimeUs )
{
	m_busController.sleep( sleepTimeUs );
}

} // namespace pbl::i2c
