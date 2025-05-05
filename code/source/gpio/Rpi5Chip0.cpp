#include "Rpi5Chip0.hpp"

// Third Party
#include <gpiod.hpp>

namespace pbl::gpio
{

v1::Rpi5Chip0::Rpi5Chip0()
{
	m_pChip = ::gpiod_chip_open_by_name( kChipName.data() );
}

v1::Rpi5Chip0::~Rpi5Chip0()
{
	if( m_pChip )
	{
		::gpiod_chip_close( m_pChip );
		m_pChip = nullptr;
	}
}

bool v1::Rpi5Chip0::isReady() const noexcept
{
	return m_pChip != nullptr;
}

// auto v1::Rpi5Chip0::line( Pin pin ) -> Result< std::reference_wrapper< GpioLine > >
// {

// 	if( !m_pChip ) [[unlikely]]
// 	{
// 		return std::unexpected( utils::ErrorCode::HARDWARE_NOT_AVAILABLE );
// 	}

// 	const std::uint8_t lineNumber = static_cast< std::uint8_t >( pin );
// 	const std::size_t storageIndex = lineNumber - static_cast< std::uint8_t >( Pin::GPIO2 );

// 	if( storageIndex >= m_lines.size() ) [[unlikely]]
// 	{
// 		return std::unexpected( utils::ErrorCode::INVALID_ARGUMENT );
// 	}

// 	if( !m_lines[ storageIndex ].has_value() )
// 	{
// 		GpioLine gpioLine{ static_cast< std::int32_t >( lineNumber ), m_pChip };
// 		if( !gpioLine.isValid() ) [[unlikely]]
// 		{
// 			return std::unexpected( utils::ErrorCode::HARDWARE_FAILURE );
// 		}

// 		m_lines[ storageIndex ].emplace( std::move( gpioLine ) );
// 	}

// 	return std::ref( m_lines[ storageIndex ].value() );
// }

} // namespace pbl::gpio
