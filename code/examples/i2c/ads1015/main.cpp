
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
		// return 1;
	}

	// Create an ADS1015 controller, attached to the bus controller
	pbl::i2c::ADS1015Controller ads{ busController };
	pbl::utils::Timer timer{ std::chrono::milliseconds( 500 ) };

	auto gainResult = ads.setGain( pbl::i2c::ADS1015Controller::Gain::FS_2_048V );
	if( !gainResult )
	{
		std::println( stderr, "Failed to configure ADS1015 gain: {}", gainResult.error().description() );
		return 1;
	}

	auto readerResult = ads.startContinuous( pbl::i2c::ADS1015Controller::Channel::CH0 );
	if( !readerResult )
	{
		std::println( stderr, "Failed to start ADS1015 continuous session: {}",
					  readerResult.error().description() );
		return 1;
	}

	auto reader = std::move( readerResult ).value();
	while( true )
	{
		if( timer.hasElapsed() )
		{
			auto value = reader.read();
			if( !value )
			{
				std::println( stderr, "Failed to read ADS1015 value: {}", value.error().description() );
			}
			else
			{
				std::println( "{}", *value );
			}

			// Reset the timer
			timer.set();
		}
	}

	return 0;
}
