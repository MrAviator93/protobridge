/**
 * @author MrAviator93
 * @date 3 November 2024
 * @brief MCP23017 Example program for detecting button press using polling with a pull-down resistor.
 * 
 * This example demonstrates how to detect a button press on the MCP23017 I/O expander by 
 * configuring a pin as an input with a pull-down resistor and polling the pin's state at regular 
 * intervals.
 * 
 * ## Summary
 * The code configures `Port A, Pin 2` on the MCP23017 as an input with a pull-down resistor 
 * enabled. The pull-down resistor keeps the pin in a stable LOW state (logic 0) until the 
 * button is pressed, which pulls the pin HIGH (logic 1). The program continuously polls the 
 * pin state and logs the status every 500 ms.
 * 
 * ## Use Case
 * This setup is commonly used when a default low (`LOW`) state is desired, and pressing the 
 * button temporarily connects the pin to a high (`HIGH`) state. It is suitable for applications 
 * where a stable low state is necessary when the button is not pressed.
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
	MCP23017 mcp23017{ busController };

	// // Set up Port A, Pin 2 as input with pull-down resistor enabled
	// MCP23017::Port::PinModes pinModes{ .pin2 = MCP23017::Port::PinMode::INPUT };
	// if( !mcp23017.portA().setPinModes( pinModes ) )
	// {
	// 	std::println( stderr, "Failed to set Port A, Pin 2 mode to INPUT" );
	// 	return 1;
	// }

    // TODO: The interface needs to be implemented
	// // Enable pull-down resistor on Pin 2 to keep the pin at LOW by default
	// MCP23017::Port::PinPullDowns pinPullDowns{ .pin2 = true };
	// if( !mcp23017.portA().setPinPullDowns( pinPullDowns ) )
	// {
	// 	std::println( stderr, "Failed to enable pull-down resistor on Port A, Pin 2" );
	// 	return 1;
	// }

	// // Polling loop to check the button state
	// pbl::utils::Timer timer{ std::chrono::milliseconds( 500 ) };
	// while( true )
	// {
	// 	if( timer.hasElapsed() )
	// 	{
	// 		const auto pinStateResult = mcp23017.portA().pinState( MCP23017::Port::Pins::PIN_2 );
	// 		if( pinStateResult )
	// 		{
	// 			const auto pinState = *pinStateResult;
	// 			std::println( "Port A, Pin 2 state: {}", pinState == MCP23017::Port::PinState:HIGH ? "HIGH" : "LOW" );
	// 		}
	// 		else
	// 		{
	// 			std::println( "Failed to read pin state on Port A, Pin 2" );
	// 		}
	// 		timer.set();
	// 	}
	// }
}