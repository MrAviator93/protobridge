#include "Rpi5Chip0.hpp"
#include "Gpio.hpp"

// C++
#include <array>
#include <optional>

namespace pbl::gpio
{

 

struct v1::Rpi5Chip0::Impl
{
	Impl( std::string_view chipName )
		: pChip{ MakeGpioChip( chipName ) }
	{ }

	std::unique_ptr< gpiod_chip, GpiodChipDeleter > pChip;

	// Using optional here, allows us to lazy initialize the lines,
	// we don't need to use all the gpio lines always
	std::array< std::optional< GpioLine >, kGpioLineCount > lines;
};

v1::Rpi5Chip0::Rpi5Chip0()
	: m_pImpl{ std::make_unique< Impl >( kChipName ) }
{ }

v1::Rpi5Chip0::~Rpi5Chip0() = default;

bool v1::Rpi5Chip0::isReady() const noexcept
{
	return m_pImpl->pChip != nullptr;
}

auto v1::Rpi5Chip0::line( Pin pin, [[maybe_unused]] GpioLine::Direction direction ) -> RefResult< GpioLine >
{

	if( !m_pImpl->pChip ) [[unlikely]]
	{
		return utils::MakeError< std::reference_wrapper< GpioLine > >( utils::ErrorCode::HARDWARE_NOT_AVAILABLE );
	}

	const std::uint8_t lineNumber = static_cast< std::uint8_t >( pin );
	const std::size_t storageIndex = lineNumber - static_cast< std::uint8_t >( Pin::GPIO2 );

	auto& lines = m_pImpl->lines;
	if( storageIndex >= lines.size() ) [[unlikely]]
	{
		return utils::MakeError< std::reference_wrapper< GpioLine > >( utils::ErrorCode::HARDWARE_NOT_AVAILABLE );
	}

	if( !lines[ storageIndex ].has_value() )
	{
		// TODO (MrAviator93): Implement

		// GpioLine gpioLine{ static_cast< std::int32_t >( lineNumber ), m_pChip };
		// if( !gpioLine.isValid() ) [[unlikely]]
		// {
		// 	return std::unexpected( utils::ErrorCode::HARDWARE_FAILURE );
		// }

		// lines[ storageIndex ].emplace( std::move( gpioLine ) );

		return utils::MakeError< std::reference_wrapper< GpioLine > >( utils::ErrorCode::HARDWARE_NOT_AVAILABLE );
	}

	return std::ref( lines[ storageIndex ].value() );
}

} // namespace pbl::gpio
