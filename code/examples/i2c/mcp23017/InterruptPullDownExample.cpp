/**
 * @author MrAviator93
 * @date 3 November 2024
 * @brief MCP23017 Example program for detecting button press using interrupts with a pull-down resistor.
 * 
 * This example demonstrates how to detect a button press on the MCP23017 I/O expander by 
 * configuring a pin as an input with a pull-down resistor and setting up an interrupt to 
 * detect the button press event.
 * 
 * ## Summary
 * The code configures `Port A, Pin 2` on the MCP23017 as an input with a pull-down resistor 
 * enabled. The pull-down resistor keeps the pin in a stable LOW state (logic 0) until the 
 * button is pressed, which pulls the pin HIGH (logic 1) and triggers an interrupt. The program 
 * periodically polls for interrupt events and logs the event details.
 * 
 * ## Use Case
 * This setup is suitable when the default state of the pin should be low (`LOW`), and pressing 
 * the button momentarily connects the pin to a high (`HIGH`) state, creating a clear transition 
 * from low to high that can trigger an interrupt.
 */

#include <utils/Timer.hpp>
#include <i2c/Controllers.hpp>

// C++
#include <print>

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
	if( !busController.isOpen() )
	{
		std::println( stderr, "Failed to open I2C device" );
		return 1;
	}

	using MCP23017 = pbl::i2c::MCP23017ControllerV2;

	// Configure Port A, Pin 2 as input with pull-down resistor enabled
	// MCP23017::Port::PinModes pinModes{ .pin2 = MCP23017::Port::PinMode::INPUT };
	// if( !mcp23017.portA().setPinModes( pinModes ) )
	// {
	// 	std::println( stderr, "Failed to set Port A, Pin 2 mode to INPUT" );
	// 	return 1;
	// }

	// // Enable pull-down resistor on Pin 2
	// MCP23017::Port::PinPullDowns pinPullDowns{ .pin2 = true };
	// if( !mcp23017.portA().setPinPullDowns( pinPullDowns ) )
	// {
	// 	std::println( stderr, "Failed to enable pull-down resistor on Port A, Pin 2" );
	// 	return 1;
	// }

	// // Enable interrupt for Pin 2 on Port A
	// MCP23017::Port::PinInterrupts pinInterrupts{ .pin2 = true };
	// if( !mcp23017.portA().setPinInterrupts( pinInterrupts ) )
	// {
	// 	std::println( stderr, "Failed to set interrupt on Port A, Pin 2" );
	// 	return 1;
	// }

	// // Periodic polling loop to handle the interrupt
	// pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };
	// while( true )
	// {
	// 	if( timer.hasElapsed() )
	// 	{
	// 		const auto interrupts = mcp23017.portA().triggeredInterrupts();
	// 		const auto portCapture = mcp23017.portA().interruptCapture();

	// 		std::println( "Port A Interrupts: {}", interrupts );
	// 		std::println( "Port A Interrupt Capture: {}", portCapture );

	// 		// Check which pins triggered the interrupt
	// 		if( interrupts.test( MCP23017::Port::Pins::PIN_2 ) )
	// 		{
	// 			std::println( "Interrupt triggered on Port A, Pin 2" );
	// 			std::println( "Pin 2 state at interrupt time: {}",
	// 						  portCapture.test( MCP23017::Port::Pins::PIN_2 ) ? "HIGH" : "LOW" );
	// 		}

	// 		// Clear interrupts on Port A after processing
	// 		mcp23017.portA().clearInterrupts();

	// 		// Reset the timer
	// 		timer.set();
	// 	}

	return 0;
}
