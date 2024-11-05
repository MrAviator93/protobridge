#include "Rpi5Chip0.hpp"

// Third Party
#include <gpiod.hpp>

namespace pbl::gpio
{

Rpi5Chip0::Rpi5Chip0()
{
	m_pChip = ::gpiod_chip_open_by_name( kChipName.data() );
}

Rpi5Chip0::~Rpi5Chip0()
{
	if( m_pChip )
	{
		::gpiod_chip_close( m_pChip );
		m_pChip = nullptr;
	}
}

bool Rpi5Chip0::isOpen() const noexcept
{
	return m_pChip != nullptr;
}

} // namespace pbl::gpio
