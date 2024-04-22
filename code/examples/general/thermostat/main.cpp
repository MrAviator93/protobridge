
// Include I2C library files
#include "Thermostat.hpp"
#include <utils/Timer.hpp>

// Output
#include <vector>
#include <thread>
#include <chrono>
#include <format>

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

	pbl::examples::Thermostat thermostat{ busController };
	pbl::utils::Timer timer;
	timer.start();
	float dt{ 0.0001 };

	while( true )
	{
		timer.reset();

		// Sleep simulates some work
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

		dt = static_cast< float >( timer.elapsedTimeS() );
		auto rslt = thermostat.update( dt );

		std::cout << std::format( "{:12f}", dt ) << std::endl;

		if( !rslt )
		{
			std::cerr << rslt.error() << std::endl;
			break;
		}
	}

	return 0;
}
