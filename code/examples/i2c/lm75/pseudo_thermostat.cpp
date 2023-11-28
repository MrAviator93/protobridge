
// Include I2C library files
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>
#include <math/PID.hpp>

// Output
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>
#include <format>
#include <utility>
#include <concepts>
#include <iostream>
#include <string_view>

#define unwrap( p ) p.first, p.second

namespace
{

class ThermostatController
{
public:
	std::expected< void, std::string > adjust( float value )
	{
		std::cout << "Adjust " << value << std::endl;
		return {};
	}
};

class Thermostat
{
public:
	Thermostat( PBL::I2C::BusController& bus )
		: m_lm75{ bus }
		, m_pid{ 0.5, 0.2, 0.25 }
	{ }

	std::expected< void, std::string > update( float dt )
	{
		return readDesiredTemp()
			.and_then( [ this ]( float desiredTemp ) -> std::expected< std::pair< float, float >, std::string > {
				auto currTemp = m_lm75.getTemperatureC();
				if( !currTemp )
				{
					return std::unexpected( currTemp.error() );
				}

				return std::pair{ desiredTemp, *currTemp };
			} )
			.and_then( [ this, dt ]( std::pair< float, float > values ) -> std::expected< float, std::string > {
				return ( m_pid.update( dt, unwrap( values ) ) | PBL::Math::Cap{ 0.0f, 10.0f } | PBL::Math::Sqr{} );
			} )
			.and_then( [ this ]( float controlSignal ) { return m_thermostat.adjust( controlSignal ); } )
			.or_else( []( const std::string& error ) -> std::expected< void, std::string > {
				return std::unexpected< std::string >( error );
			} );
	}

private:
	std::expected< float, std::string > readDesiredTemp()
	{
		// You would get it using I2C from
		return 25.0f;
	}

private:
	PBL::I2C::LM75Controller m_lm75;
	PBL::Math::PIDController< float > m_pid;
	ThermostatController m_thermostat;
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

	// This requires all controllers to implement std::expected<bool, std::string> isActive()
	// if(auto check = CheckActive{ lm75, pidController, thermostat }, !check )
	// {
	// 	std::cerr << check.error() << std::endl;
	// 	return 1;
	// }

	// The CheckActive object could be even more complicated
	// template < typename... Args >
	// struct CheckActive
	// {
	// 	CheckActive( Args&... args )
	// 		: m_objects( std::ref( args )... )
	// 	{ }

	// 	// Something like this:
	// 	// bool check() const
	// 	// {
	// 	// 	// Implement the logic to check each object.
	// 	// 	// This is just a placeholder for demonstration.
	// 	// 	return std::apply( []( auto&&... args ) { return ( ... && args.get().isActive() ); }, m_objects );
	// 	// }

	// 	std::tuple< std::reference_wrapper< Args >... > m_objects;
	// };

	Thermostat thermostat{ busController };

	while( true )
	{
		auto rslt = thermostat.update( 0.1 );

		if( !rslt )
		{
			std::cerr << rslt.error() << std::endl;
			return 1;
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	return 0;
}
