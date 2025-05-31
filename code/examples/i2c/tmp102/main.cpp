// PBL
#include <utils/Timer.hpp>
#include <i2c/BusController.hpp>
#include <i2c/TMP102Controller.hpp>

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

	// Create an TMP102 controller, attached to the bus controller
	pbl::i2c::TMP102Controller tmp102{ busController };
	pbl::utils::Timer timer{ std::chrono::milliseconds( 500 ) };

	while( true )
	{
		if( timer.hasElapsed() )
		{

			// Reset the timer
			timer.set();
		}
	}
}