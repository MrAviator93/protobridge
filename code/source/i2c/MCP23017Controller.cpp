#include "MCP23017Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

namespace
{

// MCP23017 Register Addresses
constexpr std::uint8_t kIodirARegister{ 0x00 }; //!< PORT A: Configures I/O direction (1=input, 0=output).
constexpr std::uint8_t kIodirBRegister{ 0x01 }; //!< PORT B: Configures I/O direction (1=input, 0=output).
constexpr std::uint8_t kIpolaARegister{ 0x02 }; //!< PORT A: Input polarity (1=inverts input, 0=default).
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

MCP23017ControllerV2::MCP23017ControllerV2( BusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
	, m_portA{ *this, Port::Address::PORT_A, PortTag{} }
	, m_portB{ *this, Port::Address::PORT_B, PortTag{} }
{
	// TODO
}

auto MCP23017ControllerV2::Port::pin( Pins pin ) -> Pin
{
	// TODO: This will get more complicated
	return Pin{ *this, pin, PinTag{} };
}

// auto MCP23017ControllerV2::Port::Pin::mode() const -> PinMode
// {
// 	return PinMode::OUTPUT;
// }

// auto MCP23017ControllerV2::Port::Pin::setMode( const PinMode mode ) -> Result< void >
// {
// 	// // Convert the enum-based pin to its bitmask value
// 	// auto pinMask = static_cast< std::uint8_t >( m_pin );

// 	// if( mode == PinMode::INPUT )
// 	// {
// 	// 	m_port.m_iodir |= pinMask; // Set the bit to 1 for input mode
// 	// }
// 	// else
// 	// {
// 	// 	m_port.m_iodir &= ~pinMask; // Clear the bit to 0 for output mode
// 	// }

// 	// // Write the updated IODIR register to the MCP23017
// 	// if( auto result = m_port.writeRegister( static_cast< uint8_t >( m_port.m_address ), m_port.m_iodir.to_ulong() );
// 	// 	!result )
// 	// {
// 	// 	return std::unexpected( "Failed to write to IODIR register" );
// 	// }

// 	return std::unexpected( "Not implemented ..." );
// }

// auto MCP23017ControllerV2::Port::Pin::pinState() -> Result< PinState >
// {
// 	return std::unexpected( "Not implemented ..." );
// }

// auto MCP23017ControllerV2::Port::Pin::setPinState( const PinState state ) -> Result< void >
// {
// 	if( !isOutput() )
// 	{
// 		return std::unexpected( "Can't set pin state, as pin not configured as an output pin ..." );
// 	}

// 	// TODO: Implement

// 	return std::unexpected( "Not implemented ..." );
// }

} // namespace pbl::i2c
