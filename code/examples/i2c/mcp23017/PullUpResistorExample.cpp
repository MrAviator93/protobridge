/**
 * @author MrAviator93
 * @date 3 November 2024
 * @brief MCP23017 Example program for detecting button press using polling with a pull-up resistor.
 * 
 * For license details, see the LICENSE file in the project root.
 * 
 * This example demonstrates how to detect a button press on the MCP23017 I/O expander by 
 * configuring a pin as an input with a pull-up resistor and periodically polling the pin's 
 * state to check for button presses.
 * 
 * ## Summary
 * The code configures `Port A, Pin 2` on the MCP23017 as an input with a pull-up resistor 
 * enabled. The pull-up resistor keeps the pin in a stable HIGH state (logic 1) until the 
 * button is pressed, which pulls the pin LOW (logic 0). The program continuously polls the 
 * pin state and logs the status every 500 ms. This method is straightforward and works well 
 * for simple applications but may miss quick button presses or use more CPU resources due to 
 * constant polling.
 * 
 * ## Comparison with Interrupt-Based Detection
 * ### Polling with Pull-Up Resistor (Current Approach)
 * - **Method**: Periodically polls the pin state to detect button presses.
 * - **Responsiveness**: Can miss quick button presses depending on the polling interval.
 * - **CPU Usage**: Higher CPU usage since the program continuously checks the pin state.
 * - **Power Efficiency**: Less efficient, as polling requires the CPU to stay active.
 * - **Implementation Complexity**: Simple and straightforward to implement, ideal for applications 
 *   where high responsiveness or power efficiency is not critical.
 * 
 * ### Interrupt-Triggered Detection with Polling
 * - **Method**: Configures an interrupt to indicate state changes, with periodic polling to 
 *   check and handle the interrupt details.
 * - **Responsiveness**: Generally more accurate than direct polling, as the interrupt flag helps 
 *   identify when an event has occurred, reducing the chance of missing button presses.
 * - **CPU Usage**: Similar to direct polling, as the program still checks the interrupt status at 
 *   regular intervals.
 * - **Power Efficiency**: Similar to direct polling since polling is still required, though the 
 *   interrupt flag adds efficiency by narrowing when I2C reads are needed.
 * - **Implementation Complexity**: Moderate, as it requires both interrupt setup on the MCP23017 
 *   and polling-based handling in the program.
 * 
 * ## Use Case Recommendations
 * - **Use Polling with Pull-Up**: Suitable for simple applications where high timing accuracy 
 *   or responsiveness is not required, or where button presses are expected to be infrequent.
 * - **Use Interrupt-Triggered Polling**: For applications needing more accurate detection of 
 *   button presses without a fully event-driven approach, or where some CPU savings can be achieved 
 *   by using the interrupt signal to indicate state changes.
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

    // TODO: Implement the interface
	// // Configure a pin port A pin 2 as input with pull-up resistor enabled
	// auto pin = mcp23017.portA().pin( MCP23017::Port::Pins::PIN_2 );

	// // Check if the pin is set as an input
	// if( !pin.setMode( MCP23017::Port::PinMode::INPUT ) )
	// {
	// 	std::println( "Failed to set Port A, Pin 2 as INPUT" );
	// 	return 1;
	// }

	// // Enable the pull-up resistor for port A pin 2
	// if( !pin.setPullUpResistor( true ) )
	// {
	// 	std::println( "Failed to enable pull-up resistor on Port A, Pin 2" );
	// 	return 1;
	// }

	// // Create a timer to check the button status periodically
	// pbl::utils::Timer timer{ std::chrono::milliseconds( 500 ) };

	// while( true )
	// {
	// 	// Check the pin state every 500 ms
	// 	if( timer.hasElapsed() )
	// 	{
	// 		// Read the current state of the pin
	// 		const auto pinStateResult = pin.pinState();
	// 		if( pinStateResult )
	// 		{
	// 			const auto pinState = *pinStateResult;
	// 			std::println( "Port A, Pin 2 state: {}", pinState == MCP23017::Port::PinState::ON ? "HIGH" : "LOW" );
	// 		}
	// 		else
	// 		{
	// 			std::println( "Failed to read pin state on Port A, Pin 2" );
	// 		}

	// 		// Reset the timer
	// 		timer.set();
	// 	}
	// }

	return 0;
}
