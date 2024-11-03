/**
 * @author MrAviator93
 * @date 3 November 2024
 * @brief MCP23017 Example program for detecting button press using interrupts with a pull-up resistor.
 * 
 * For license details, see the LICENSE file in the project root.
 * 
 * This example demonstrates how to detect a button press on the MCP23017 I/O expander by 
 * configuring pins as inputs with pull-up resistors and setting up interrupts to 
 * notify the system of a button press event. Although the MCP23017 automatically generates 
 * an interrupt signal on state changes, this program uses a polling loop to periodically 
 * check the interrupt status and read the state of the pins. At the time of writing, the
 * automatic detection of interrupt via the callback is not implemented, it's planned the
 * next major release.
 * 
 * ## Summary
 * The code configures all pins of Port A on the MCP23017 as inputs with pull-up resistors 
 * enabled, and sets up interrupts for detecting button presses. The pull-up resistors ensure 
 * that each pin is in a stable HIGH state (logic 1) until a button is pressed, pulling the pin 
 * LOW (logic 0) and triggering the interrupt signal on the MCP23017's INT pin. In this example, 
 * the program polls the MCP23017 at regular intervals to check for interrupt signals and 
 * retrieve the pin states at the time of the interrupt.
 * 
 * ## Comparison with Direct Polling-Based Detection
 * ### Interrupt-Triggered Detection with Polling (Current Approach)
 * - **Method**: Configures an interrupt to indicate state changes, with periodic polling to 
 *   check and handle the interrupt details.
 * - **Responsiveness**: Generally more accurate than continuous state polling alone, as the 
 *   interrupt flag helps to identify when an event has occurred, reducing the chance of 
 *   missing brief button presses.
 * - **CPU Usage**: Similar to direct polling, as the program continuously checks the interrupt 
 *   status at regular intervals.
 * - **Power Efficiency**: Similar to direct polling since the CPU is still actively polling.
 * - **Implementation Complexity**: Moderate, as it requires both interrupt setup on the MCP23017 
 *   and polling-based handling in the program.
 * 
 * ### Direct Polling with Pull-Up Resistor (Comparison)
 * - **Method**: Periodically polls each pin state directly, without relying on interrupts.
 * - **Responsiveness**: Less responsive to fast button presses, depending on polling interval.
 * - **CPU Usage**: Higher, as it continuously checks the state of each pin.
 * - **Power Efficiency**: Less power efficient, due to constant polling.
 * - **Implementation Complexity**: Simple and straightforward to implement.
 * 
 * ## Use Case Recommendations
 * - **Use Interrupt-Triggered Polling**: For applications that benefit from the MCP23017 
 *   interrupt pin as an indicator of state changes but where event-driven handling is not feasible.
 * - **Use Direct Polling**: For simpler applications with less frequent button presses or where 
 *   high responsiveness is not required.
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

	// Create an MCP23017 controller, attached to the bus controller
	MCP23017 mcp23017{ busController };

	// Set up each pin on Port A as an input (all pins in this example)
	// Configuring the pins as inputs allows us to detect external signals, such as button presses.
	// Here, each pin is designated for input to illustrate a scenario where multiple buttons or
	// other input devices might be connected to the MCP23017. This configuration is required before
	// enabling interrupts on the pins, as interrupts can only be generated for input pins.
	// MCP23017::Port::PinModes pinModes{ .pin1 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin2 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin3 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin4 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin5 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin6 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin7 = MCP23017::Port::PinMode::INPUT,
	// 								   .pin8 = MCP23017::Port::PinMode::INPUT };

	// // Validate that we were able to set them correctly
	// if( !mcp23017.portA().setPinModes( pinModes ) )
	// {
	// 	std::println( stderr, "Failed to set port A pins mode" );
	// 	return 1;
	// }

	// By enabling interrupts on each input pin, we allow the MCP23017 to notify us immediately
	// when any pin changes state. This is especially useful for detecting button presses or
	// other events in real time. When a state change is detected on any of these pins, the MCP23017
	// will generate an interrupt signal on its INT pin, which we can monitor for event-driven processing.
	// MCP23017::Port::PinInterrupts pinInterrupts{ .pin1 = true,
	// 											 .pin2 = true,
	// 											 .pin3 = true,
	// 											 .pin4 = true,
	// 											 .pin5 = true,
	// 											 .pin6 = true,
	// 											 .pin7 = true,
	// 											 .pin8 = true };

	// if( !mcp23017.portA().setPinInterrupts( pinInterrupts ) )
	// {
	// 	std::println( stderr, "Failed to set port A pins mode" );
	// 	return 1;
	// }

	// Configuring interrupts on all input pins of Port A allows the MCP23017
	// to generate an interrupt signal when any pin state changes, such as a button press.
	// With pull-up resistors enabled, each pin is held at a stable high state by default,
	// and pressing a button pulls the pin low, creating a clear HIGH-to-LOW transition
	// that triggers the interrupt signal on the MCP23017's interrupt pin.
	//
	// Although the MCP23017 generates the interrupt automatically, this example still relies on
	// periodic polling to check the interrupt status over I2C. The polling loop reads the
	// interrupt and capture registers to determine which specific pins triggered the interrupt
	// and their states at that moment. This setup allows for reliable event detection with
	// pull-up resistors and efficient processing of interrupt information in each polling cycle.

	// MCP23017::Port::PinPullUps pinPullUps{ .pin1 = true,
	// 									   .pin2 = true,
	// 									   .pin3 = true,
	// 									   .pin4 = true,
	// 									   .pin5 = true,
	// 									   .pin6 = true,
	// 									   .pin7 = true,
	// 									   .pin8 = true };

	// if( !mcp23017.portA().setPinPullsUps( pinPullUps ) )
	// {
	// 	std::println( stderr, "Failed to set port A pins mode" );
	// 	return 1;
	// }

	// Also we should be able to itterate through pins:
	// for (auto&& pin : mcp23017.portA().pins())
	// {

	// }

	// Also we should be able to set interrupts, pull up resistors, etc. ...

	// We could also develop struct PortConfig<PortATag> or PortConfig<PortBTag>, so each
	// port would have it's own specific config and it would be really hard to misuse them.

	// pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };

	// while( true )
	// {
	// 	if( timer.hasElapsed() )
	// 	{
	// 		// TODO: This interface needs to be implemented
	// 		const auto interrupts = mcp23017.portA().triggeredInterrupts();
	// 		const auto portCapture = mcp23017.portA().interruptCapture();

	// 		std::println( "Port A Interrupts: {}", interrupts );
	// 		std::println( "Port A Interrupt Capture: {}", portCapture );

	// 		// Check if multiple pins triggered the interrupt
	// 		const int interruptCount = interrupts.count();
	// 		if( interruptCount > 1 )
	// 		{
	// 			std::println( "Multiple pins triggered interrupts on Port A ({} pins).", interruptCount );
	// 		}
	// 		else if( interruptCount == 1 )
	// 		{
	// 			std::println( "A single pin triggered an interrupt on Port A." );
	// 		}
	// 		else
	// 		{
	// 			std::println( "No interrupts triggered on Port A." );
	// 		}

	// 		// Determine which pins triggered an interrupt on Port A
	// 		for( int pinNum = 0; pinNum < 8; ++pinNum )
	// 		{
	// 			// If the bit is set, this pin triggered an interrupt
	// 			if( interrupts.test( pinNum ) )
	// 			{
	// 				std::println( "Interrupt triggered on Port A, Pin {}", pinNum + 1 );
	// 				std::println(
	// 					"Pin {} state at interrupt time: {}", pinNum + 1, portCapture.test( pinNum ) ? "HIGH" : "LOW" );
	// 			}
	// 		}

	// 		// Clear interrupts on Port A after processing
	// 		mcp23017.portA().clearInterrupts();

	// 		// Reset the timer
	// 		timer.set();
	// 	}
	// }

	return 0;
}

