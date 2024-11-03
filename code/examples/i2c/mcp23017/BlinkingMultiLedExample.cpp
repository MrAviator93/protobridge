/**
 *  @brief MCP23017 Example program for blinking multiple LEDs connected to individual GPIO pins.
 *  @details This example demonstrates how to use the MCP23017 I/O expander to control an multiple
 *           LEDs by toggling a GPIO pin son and off at regular intervals.
 * 
 *  @author MrAviator93
 *  @date 3 November 2024
 *
 *  ## Requirements
 *  - MCP23017 I/O expander connected to the I2C bus.
 *  - LED 1 connected to Port A, Pin 1 of the MCP23017 with an appropriate current-limiting resistor.
 *  - LED 2 connected to Port A, Pin 2 of the MCP23017 with an appropriate current-limiting resistor.
 *
 *  ## Description
 *  - This program initializes the MCP23017, sets up Port A, Pin 1 & 2 as an output pins, and toggles 
 *    their states every 2 seconds to create a blinking LED effect.
 *  - The LED will turn on and off in response to the toggling of the output pin.
 *  - The program continuously checks the timer, and every time it elapses, it switches the state 
 *    of the pin to create the blink.
 * 
 *  ## Usage
 *  - Connect an LED to Port A, Pin 1 of the MCP23017. Ensure a suitable resistor is in series 
 *    to prevent overcurrent through the LED.
 *  - Compile and run the program on a device with access to the I2C bus.
 *  - Optionally, pass a different I2C bus device name as an argument if using something other 
 *    than the default `/dev/i2c-1`.
 *
 *  ## Example Output
 *  - If the LED is connected correctly, it should blink on and off every 2 seconds.
 *
 *  ## Error Handling
 *  - The program checks if the I2C bus is open and accessible, and it verifies that the pin 
 *    mode is set successfully. Errors in setting the pin mode or switching the pin state are 
 *    reported to standard error output.
 *
 *  For license details, see the LICENSE file in the project root.
 */

// Include I2C library files
#include <utils/Timer.hpp>
#include <i2c/Controllers.hpp>

// Output
#include <print>

int main( const int argc, const char* const* const argv )
{
	const std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );

	// Default name of i2c bus on RPI 4
	std::string deviceName{ "/dev/i2c-1" };

	if( args.size() >= 2 )
	{
		deviceName = args[ 1 ];
	}

	// Create a bus controller for the I2C bus
	pbl::i2c::BusController busController{ deviceName };

	// Check if the I2C bus is open and accessible
	if( !busController.isOpen() )
	{
		std::println( stderr, "Failed to open I2C device" );
		return 1;
	}

	using MCP23017 = pbl::i2c::MCP23017ControllerV2;

	// Create an LM75 controller, attached to the bus controller
	MCP23017 mcp23017{ busController };

	MCP23017::Port::PinModes pinModes{ .pin1 = MCP23017::Port::PinMode::OUTPUT,
									   .pin2 = MCP23017::Port::PinMode::OUTPUT };

	// Validate that we were able to set them correctly
	if( !mcp23017.portA().setPinModes( pinModes ) )
	{
		std::println( stderr, "Failed to set port A pins mode" );
		return 1;
	}

	pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };

	while( true )
	{
		if( timer.hasElapsed() )
		{
            // TODO


			// Reset the timer
			timer.set();
		}
	}

	return 0;
}