
// Include I2C library files
#include "Thermostat.hpp"
#include <utils/Timer.hpp>

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
		std::println( "Failed to open I2C device" );
		return 1;
	}

	pbl::examples::Thermostat thermostat{ busController };
	pbl::utils::Timer timer{ std::chrono::milliseconds( 100 ) };

	while( true )
	{
		if( timer.hasElapsed() )
		{
			auto dt = timer.elapsedSinceSetInSeconds();
			auto rslt = thermostat.update( dt );

			std::println( "{:12f}", dt );

			if( !rslt )
			{
				std::println( stderr, "{}", pbl::utils::toStringView( rslt.error() ) );
				break;
			}

			timer.set();
		}
	}

	return 0;
}
