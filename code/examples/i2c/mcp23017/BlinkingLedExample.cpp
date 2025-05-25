/**
 *  @brief MCP23017 Example program for blinking an LED connected to a GPIO pin.
 *  @details This example demonstrates how to use the MCP23017 I/O expander to control an LED 
 *           by toggling a GPIO pin on and off at regular intervals. The LED is connected to 
 *           Port A, Pin 1 of the MCP23017, which is configured as an output pin.
 * 
 *  @author MrAviator93
 *  @date 3 November 2024
 *
 *  ## Requirements
 *  - MCP23017 I/O expander connected to the I2C bus.
 *  - LED connected to Port A, Pin 1 of the MCP23017 with an appropriate current-limiting resistor.
 *
 *  ## Description
 *  - This program initializes the MCP23017, sets up Port A, Pin 1 as an output pin, and toggles 
 *    its state every 2 seconds to create a blinking LED effect.
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
#include <i2c/BusController.hpp>
#include <i2c/MCP23017Controller.hpp>

// Output
#include <print>

int main( int, char** )
{
	// Default name of i2c bus on RPI 4
	std::string deviceName{ "/dev/i2c-1" };

	// Create a bus controller for the I2C bus
	pbl::i2c::BusController busController{ deviceName };

	// Check if the I2C bus is open and accessible
	if( !busController.isOpen() )
	{
		std::println( stderr, "Failed to open I2C device" );
		return 1;
	}

	// Create an MCP23017 controller, attached to the bus controller
	pbl::i2c::MCP23017Controller mcp{ busController };
	auto pin = mcp.portA().pin( pbl::i2c::MCP23017Controller::Port::Pins::PIN_1 );

	// This API allows us to configure individual pins
	if( !pin.setMode( pbl::i2c::MCP23017Controller::Port::PinMode::OUTPUT ) )
	{
		std::println( stderr, "Error occurred when setting pin mode..." );
		return 1;
	}

	pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };

	while( true )
	{
		timer.onTick( [ &pin ]( [[maybe_unused]] auto dt ) -> void {
			if( !pin.switchPinState() )
			{
				std::println( "Failed to set pin state to ON..." );
			}
		} );
	}

	return 0;
}