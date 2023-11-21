#include "MCP23017Controller.hpp"
#include "I2CBusController.hpp"

namespace PBL::I2C
{

/**
 * Note:
 * To configure pull up resistor (enable do the following) you need to enable them through configuration
 * of registers GPPUA (address either 0x06 or 0x0C, see below) and GPPUB (address either 0x16 or 0x0D, 
 * see below) according to this pattern (from the datasheet):
 * 
 * 0x20, 0x0C, 0b00000001 we enable pull up resistor for pin 1 on port A
 * 0x20, 0x0D, 0b00000001 we enable pull up resistor for pin 1 on port B
 */

namespace
{

// MCP23017 Register Addresses
constexpr std::uint8_t kIodirARegister{ 0x00 }; //!< PORT A: Configures I/O direction (1=input, 0=output).
constexpr std::uint8_t kIodirBRegister{ 0x01 }; //!< PORT B: Configures I/O direction (1=input, 0=output).
constexpr std::uint8_t kIpolaRegister{ 0x02 }; //!< PORT A: Input polarity (1=inverts input, 0=default).
constexpr std::uint8_t kIpolaBRegister{ 0x03 }; //!< PORT B: Input polarity (1=inverts input, 0=default).
constexpr std::uint8_t kGpintenARegister{ 0x04 }; //!< PORT A: Interrupt-on-change enable (1=enabled).
constexpr std::uint8_t kGpintenBRegister{ 0x05 }; //!< PORT B: Interrupt-on-change enable (1=enabled).
constexpr std::uint8_t kDefvalARegister{ 0x06 }; //!< PORT A: Default comparison value for interrupts.
constexpr std::uint8_t kDefvalBRegister{ 0x07 }; //!< PORT B: Default comparison value for interrupts.
constexpr std::uint8_t kIntconARegister{ 0x08 }; //!< PORT A: Interrupt control (determines compare value).
constexpr std::uint8_t kIntconBRegister{ 0x09 }; //!< PORT B: Interrupt control (determines compare value).
constexpr std::uint8_t kIoconRegister{ 0x0A }; //!< Configuration register (modifies device behavior).
constexpr std::uint8_t kGppuARegister{ 0x0C }; //!< PORT A: Controls pull-up resistors (1=enabled).
constexpr std::uint8_t kGppuBRegister{ 0x0D }; //!< PORT B: Controls pull-up resistors (1=enabled).
constexpr std::uint8_t kIntfARegister{ 0x0E }; //!< PORT A: Indicates interrupt occurrence on pins.
constexpr std::uint8_t kIntfBRegister{ 0x0F }; //!< PORT B: Indicates interrupt occurrence on pins.
constexpr std::uint8_t kIntcapARegister{ 0x10 }; //!< PORT A: Captures value during interrupt, resets after read.
constexpr std::uint8_t kIntcapBRegister{ 0x11 }; //!< PORT B: Captures value during interrupt, resets after read.
constexpr std::uint8_t kGpioARegister{ 0x12 }; //!< PORT A: Represents logic levels, can be modified.
constexpr std::uint8_t kGpioBRegister{ 0x13 }; //!< PORT B: Represents logic levels, can be modified.
constexpr std::uint8_t kOlatARegister{ 0x14 }; //!< PORT A: Output latch register, affects output pins.
constexpr std::uint8_t kOlatBRegister{ 0x15 }; //!< PORT B: Output latch register, affects output pins.

// Note: There are several other configuration bits within some of these registers that control the behavior of the MCP23017.
// Please refer to the MCP23017 datasheet for detailed information about each register's function.

} // namespace

MCP23017Controller::MCP23017Controller( I2CBusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
{
	configure();
}

MCP23017Controller::MCP23017Controller( I2CBusController& busController,
										std::uint8_t portAConfig,
										std::uint8_t portBConfig,
										Address address ) noexcept
	: ICBase{ busController, address }
	, m_portAConfiguration{ portAConfig }
	, m_portBConfiguration{ portBConfig }
{
	configure();
}

bool MCP23017Controller::setPortConfig( Port port, std::uint8_t config )
{
	bool rslt{};

	if( port == Port::PORT_A )
	{
		if( m_portAConfiguration != config )
		{
			m_portAConfiguration = config;
			rslt = configure();
		}
	}

	if( port == Port::PORT_B )
	{
		if( m_portBConfiguration != config )
		{
			m_portBConfiguration = config;
			rslt = configure();
		}
	}

	return rslt;
}

bool MCP23017Controller::setOnPortA( Pins pin )
{
	// Check if the pin is configured to be an output
	if( ( m_portAConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
	{
		m_portAPinStates &= ~static_cast< std::uint8_t >( pin );
		return write( 0x14, m_portAPinStates );
	}

	return false;
}

bool MCP23017Controller::setOffPortA( Pins pin )
{
	// Check if pin is configured to be an output
	if( ( m_portAConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
	{
		m_portAPinStates |= static_cast< std::uint8_t >( pin );
		return write( 0x14, m_portAPinStates );
	}

	return false;
}

bool MCP23017Controller::setOnPortB( Pins pin )
{
	// Check if the pin is configured to be an output
	if( ( m_portBConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
	{
		m_portBPinStates &= ~static_cast< std::uint8_t >( pin );
		return write( 0x15, m_portBPinStates );
	}

	return false;
}

bool MCP23017Controller::setOffPortB( Pins pin )
{
	// Check if the pin is configured to be an output
	if( ( m_portBConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
	{
		m_portBPinStates &= ~static_cast< std::uint8_t >( pin );
		return write( 0x15, m_portBPinStates );
	}

	return false;
}

bool MCP23017Controller::configure()
{
	bool rslt{};
	rslt = write( kIodirARegister, m_portAConfiguration );
	rslt = rslt && write( kIodirBRegister, m_portBConfiguration );
	return rslt && retrieve();
}

bool MCP23017Controller::retrieve()
{
	bool rslt{};
	rslt = read( 0x12, m_portAPinStates );
	rslt = rslt && read( 0x13, m_portBPinStates );
	return rslt;
}

} // namespace PBL::I2C
