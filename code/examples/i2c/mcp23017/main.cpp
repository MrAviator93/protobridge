// MCP23017 Blinking Led Example

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

	// Create an LM75 controller, attached to the bus controller
	pbl::i2c::MCP23017ControllerV2 mcp23017{ busController };
	auto pin = mcp23017.portA().pin( pbl::i2c::MCP23017ControllerV2::Port::Pins::PIN_1 );

	// This API allows us to configure individual pins
	if( !pin.setMode( pbl::i2c::MCP23017ControllerV2::Port::PinMode::OUTPUT ) )
	{
		std::println( stderr, "Error occurred when setting pin mode..." );
		// return 0;
	}

	// More ideas
	// PinsMode pinsMode{ .pin1 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::OUTPUT,
	// 				   .pin2 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::OUTPUT,
	// 				   .pin3 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::OUTPUT,
	// 				   .pin4 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::OUTPUT,
	// 				   .pin5 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::INPUT,
	// 				   .pin6 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::INPUT,
	// 				   .pin7 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::INPUT,
	// 				   .pin8 = pbl::i2c::MCP23017ControllerV2::Port::PinMode::INPUT };

	// if (!mcp23017.portA().setPinsMode(pinsMode))
	// {
	// 	std::println(stderr, "Failed to set port A pins mode");
	// }

	// PinsState pinsState{ .pin1 = pbl::i2c::MCP23017ControllerV2::Port::PinState::ON,
	// 					 .pin2 = pbl::i2c::MCP23017ControllerV2::Port::PinState::OFF,
	// 					 .pin3 = pbl::i2c::MCP23017ControllerV2::Port::PinState::OFF,
	// 					 .pin4 = pbl::i2c::MCP23017ControllerV2::Port::PinState::OFF,
	// 					 .pin5 = pbl::i2c::MCP23017ControllerV2::Port::PinState::OFF,
	// 					 .pin6 = pbl::i2c::MCP23017ControllerV2::Port::PinState::OFF,
	// 					 .pin7 = pbl::i2c::MCP23017ControllerV2::Port::PinState::OFF,
	// 					 .pin8 = pbl::i2c::MCP23017ControllerV2::Port::PinState::ON };

	// if( !mcp23017.portA().setPinsState( pinsState ) )
	// {
	// 	std::println( stderr, "Failed to set port A pins state" );
	// }


	// Also we should be able to itterate through pins:
	// for (auto&& pin : mcp23017.portA())
	// {

	// }

	// Also we should be able to set interrupts, pull up resistors, etc. ...


	// We could also develop struct PortConfig<PortATag> or PortConfig<PortBTag>, so each
	// port would have it's own specific config and it would be really hard to misuse them.
	



	pbl::utils::Timer timer{ std::chrono::seconds( 2 ) };

	while( true )
	{
		if( timer.hasElapsed() )
		{
			using Port = pbl::i2c::MCP23017ControllerV2::Port;

			auto pin = mcp23017.portA().pin( Port::Pins::PIN_1 );

			// Good thing to do, is to check if the pin is set as output
			if( pin.isOutput() )
			{
				if( !pin.switchPinState() )
				{
					std::println( "Failed to set pin state to ON..." );
				}
			}

			// Reset the timer
			timer.set();
		}
	}

	return 0;
}