
// Include I2C library files
#include <i2c/BusController.hpp>
#include <i2c/MPU9250Controller.hpp>

// Output
#include <vector>
#include <thread>
#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

int main( const int argc, const char* const* const argv )
{
	std::vector< std::string_view > args( argv, std::next( argv, static_cast< std::ptrdiff_t >( argc ) ) );

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
		std::cerr << "Failed to open I2C device" << std::endl;
		return 1;
	}

	// Create an MPU9250 controller, attached to the bus controller, using the default device address
	pbl::i2c::MPU9250Controller mpu{ busController };

	while( true )
	{
        // TODO

		// Sleep for half a second
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	return 0;
}
