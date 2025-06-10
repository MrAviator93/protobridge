#include "GpioLine.hpp"
#include "Gpio.hpp"

namespace pbl::gpio
{

v1::GpioLine::GpioLine( GpioLine&& other ) noexcept
	: m_pLine{ other.m_pLine }
	, m_lineNumber{ other.m_lineNumber }
{
	other.m_pLine = nullptr;
	other.m_lineNumber = 0;
}

GpioLine& v1::GpioLine::operator=( GpioLine&& other ) noexcept
{
	if( this != &other )
	{
		// Release current line if any
		if( m_pLine )
		{
			::gpiod_line_release( m_pLine );
		}

		m_pLine = other.m_pLine;
		m_lineNumber = other.m_lineNumber;

		other.m_pLine = nullptr;
		other.m_lineNumber = 0;
	}
	return *this;
}

v1::GpioLine::~GpioLine()
{
	release();
}

void v1::GpioLine::release()
{
	if( m_pLine )
	{
		::gpiod_line_release( m_pLine );
		m_pLine = nullptr;
	}
}

auto v1::GpioLine::open( gpiod_chip* pChip, std::int32_t lineNumber, Direction direction ) -> Result< GpioLine >
{
	auto pLine = ::gpiod_chip_get_line( pChip, lineNumber );
	if( !pLine )
	{
		return utils::MakeError( utils::ErrorCode::HARDWARE_NOT_AVAILABLE );
	}

	int ret{};
	switch( direction )
	{
		case Direction::Input: {
			ret = ::gpiod_line_request_input( pLine, "gpio_input" );
			break;
		}
		case Direction::Output: {
			ret = ::gpiod_line_request_output( pLine, "gpio_output", 0 );
			break;
		}
	}

	if( ret < 0 )
	{
		return utils::MakeError( utils::ErrorCode::HARDWARE_NOT_AVAILABLE );
	}

	return utils::MakeSuccess< GpioLine >( std::in_place, pLine, lineNumber, direction, PrivateTag{} );
}

} // namespace pbl::gpio
