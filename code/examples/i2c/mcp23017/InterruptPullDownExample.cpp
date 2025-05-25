/**
 * @author MrAviator93
 * @date 3 November 2024
 * @brief MCP23017 Example program for detecting button press using interrupts with a pull-up resistor.
 * 
 * This example demonstrates how to detect a button press on the MCP23017 I/O expander by 
 * configuring a pin as an input with a pull-up resistor and setting up an interrupt to 
 * detect the button press event.
 * 
 * ## Summary
 * The code configures `Port A, Pin 2` on the MCP23017 as an input with a pull-up resistor 
 * enabled. The pull-up resistor keeps the pin in a stable HIGH state (logic 1) until the 
 * button is pressed, which pulls the pin LOW (logic 0) and triggers an interrupt. The program 
 * periodically polls for interrupt events and logs the event details.
 * 
 * ## Use Case
 * This setup is suitable when the default state of the pin should be high (`HIGH`), and pressing 
 * the button momentarily connects the pin to ground, creating a clear transition from high to low 
 * that can trigger an interrupt.
 */

#include <utils/Timer.hpp>
#include <i2c/Controllers.hpp>

// C++
#include <print>

using MCP23017 = pbl::i2c::MCP23017ControllerV2;

template < std::size_t Idx >
using PmPin = MCP23017::Port::PinModes::Pin< Idx, MCP23017::Port::PinMode >;

template < std::size_t Idx >
using PpuPin = MCP23017::Port::PinPullUps::Pin< Idx, bool >;

template < std::size_t Idx >
using PiPin = MCP23017::Port::PinInterrupts::Pin< Idx, bool >;

int main( const int argc, const char* const* const argv )
{

	// Set up I2C bus and MCP23017 controller
	const std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );
	std::string deviceName{ "/dev/i2c-1" }; // Default I2C device name on Raspberry Pi 4

	if( args.size() >= 2 )
	{
		deviceName = args[ 1 ];
	}

	pbl::i2c::BusController busController{ deviceName };
	if( !busController.isOpen() ) [[unlikely]]
	{
		std::println( stderr, "Failed to open I2C device" );
		return 1;
	}

	// Create an LM75 controller, attached to the bus controller
	MCP23017 mcp{ busController };

	// Configure Port A, Pin 2 as input with pull-down resistor enabled
	MCP23017::Port::PinModes pinModes{ PmPin< 2 >{ MCP23017::Port::PinMode::INPUT } };
	if( !mcp.portA().setPinModes( pinModes ) ) [[unlikely]]
	{
		std::println( stderr, "Failed to set Port A, Pin 2 mode to INPUT" );
		return 1;
	}

	[[maybe_unused]] auto pin = mcp.portA().pin( MCP23017::Port::Pins::PIN_2 );

	// if( !pin.enableInterrupt( true, true, MCP23017::Port::PinState::HIGH ) )
	// {
	// 	std::println( stderr, "Failed to enable interrupt on pin 2 on Port A" );
	// 	return 1;
	// }

	// // Enable pull-up resistor on Pin 2
	// MCP23017::Port::PinPullUps pinPullUps{ PpuPin< 2 >{ true } };
	// if( !mcp.portA().setPullUps( pinPullUps ) )
	// {
	// 	std::println( stderr, "Failed to enable pull-up resistor on Port A, Pin 2" );
	// 	return 1;
	// }

	// // Enable interrupts
	// if( !mcp.portA().enableInterrupt( true, true, MCP23017::Port::PinState::HIGH ) )
	// {
	// 	std::println( stderr, "Failed to set interrupt on Port A, Pin 2" );
	// 	return 1;
	// }

	// Periodic polling loop to handle the interrupt
	pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };
	while( true )
	{
		if( timer.hasElapsed() )
		{
			// const auto interrupts = mcp23017.portA().triggeredInterrupts();
			// const auto portCapture = mcp23017.portA().interruptCapture();

			// std::println( "Port A Interrupts: {}", interrupts );
			// std::println( "Port A Interrupt Capture: {}", portCapture );

			// // Check which pins triggered the interrupt
			// if( interrupts.test( MCP23017::Port::Pins::PIN_2 ) )
			// {
			// 	std::println( "Interrupt triggered on Port A, Pin 2" );
			// 	std::println( "Pin 2 state at interrupt time: {}",
			// 				  portCapture.test( MCP23017::Port::Pins::PIN_2 ) ? "HIGH" : "LOW" );
			// }

			// // Clear interrupts on Port A after processing
			// mcp23017.portA().clearInterrupts();

			// Reset the timer
			timer.set();
		}

		return 0;
	}
}
