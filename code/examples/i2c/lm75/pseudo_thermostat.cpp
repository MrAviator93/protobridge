
// Include I2C library files
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>

// Output
#include <vector>
#include <thread>
#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

namespace
{

class PIDController
{
public:
	[[nodiscard]] float update() noexcept { return 1; }
};

class ThermostatController
{
public:
	std::expected< void, std::string > adjust( [[maybe_unused]] float value )
	{
		//
	}
};

} // namespace

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
	PBL::I2C::BusController busController{ deviceName };

	// Check if the I2C bus is open and accessible
	if( !busController.isOpen() )
	{
		std::cerr << "Failed to open I2C device" << std::endl;
		return 1;
	}

	// Create an LM75 controller, attached to the bus controller
	PBL::I2C::LM75Controller lm75{ busController };

	PIDController pidController;
	ThermostatController thermostat;

	while( true )
	{
		auto rslt =
			lm75.getTemperatureC()
				.and_then( [ &pidController ]( float temp ) {
					std::cout << std::format( "Temperature: {}°C", temp ) << std::endl;
					float controlOutput = pidController.update();
					return std::expected< float, std::string >{ controlOutput };
				} )
				.and_then( [ &thermostat ]( float controlOutput ) { return thermostat.adjust( controlOutput ); } )
				.or_else( []( const std::string& error ) {
					// Do something with error?

					// Propagate the error
					return std::expected< void, std::string >{ std::unexpect, error };
				} );

		if( !rslt )
		{
			std::cerr << rslt.error() << std::endl;
			return 1;
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	return 0;
}