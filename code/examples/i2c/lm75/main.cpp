
// Include I2C library files
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>

// Output
#include <print>
#include <vector>
#include <thread>
#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

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
		std::print( "Failed to open I2C device\n" );
		return 1;
	}

	// Create an LM75 controller, attached to the bus controller
	pbl::i2c::LM75Controller lm75{ busController };

	while( true )
	{
		// Read the temperature in Celsius from the LM75 sensor
		auto temp = lm75.getTemperatureC();

		// Check wether the reading is present (indication of successful read)
		if( !temp.has_value() )
		{
			// With C++ 23 we can retrieve exact error what caused the value not present
			std::print( "{}\n", temp.error() );
			return 1;
		}

		// Output the temperature to the console
		std::print( "Temperature: {}°C\n", temp.value() );

		// Sleep for half a second
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	return 0;
}