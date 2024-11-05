#include "RaspberryPi5Chip0.hpp"

// Third Party
#include <gpiod.hpp>

namespace pbl::gpio
{

RaspberryPi5Chip0::RaspberryPi5Chip0()
{
	m_pChip = ::gpiod_chip_open_by_name( kChipName.data() );
}

RaspberryPi5Chip0::~RaspberryPi5Chip0()
{
	if( m_pChip )
	{
		::gpiod_chip_close( m_pChip );
		m_pChip = nullptr;
	}
}

bool RaspberryPi5Chip0::isOpen() const noexcept
{
	return m_pChip != nullptr;
}

} // namespace pbl::gpio
