
// Include I2C library files
#include <utils/Timer.hpp>
#include <i2c/Controllers.hpp>

// C++
#include <print>
#include <thread>
#include <algorithm>

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

	// Create an BMP180 controller, attached to the bus controller
	pbl::i2c::BMP180Controller bmp180{ busController, pbl::i2c::BMP180Controller::DEFAULT };
	pbl::utils::Timer timer{ std::chrono::milliseconds{ 500 } };

	while( true )
	{
		// A little bit more advanced example than LM75, rather than continously
		// looping, here we check if the timer hasn't elapsed, if not, we temporarily
		// yield control with a short sleep to avoid unnecessary CPU usage, then
		// continue to the next loop iteration to recheck the timer.
		if( !timer.hasElapsed() )
		{
			// Pause briefly to yield the thread, allowing other processes to execute
			std::this_thread::sleep_for( std::chrono::milliseconds{ 250 } );
			continue;
		}

		const auto temp = bmp180.getTrueTemperatureC();
		if( temp.has_value() )
		{
			std::println( "True temp: {} C", temp.value() );
		}

		const auto truePress = bmp180.getTruePressurePa();
		if( truePress.has_value() )
		{
			std::println( "True pressure: {} Pa", truePress.value() );
		}

		const auto alt = bmp180.getAbsoluteAltitude();
		if( alt.has_value() )
		{
			std::println( "Absolute altitude: {} m", alt.value() );
		}

		// Reset the timer
		timer.set();
	}

	return 0;
}