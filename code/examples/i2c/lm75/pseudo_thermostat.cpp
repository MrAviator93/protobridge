
// Include I2C library files
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>
#include <utils/Timer.hpp>
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
#include <functional>
#include <string_view>

#define unwrap( p ) p.first, p.second

namespace PBL::I2C
{

namespace
{

class ThermostatController
{
public:
	ThermostatController( PBL::I2C::BusController& ) { }

	std::expected< void, std::string > adjust( float value )
	{
		std::cout << std::format( "Adjust: {}", value ) << std::endl;
		return {};
	}
};

class ADCController
{
public:
	ADCController( PBL::I2C::BusController& ) { }

	std::expected< float, std::string > readDesiredTemp()
	{
		// You would get it using I2C from
		return 25.0f;
	}
};

} // namespace

} // namespace PBL::I2C

namespace
{

template < typename... Ts >
class CheckActive
{
	template < typename... Args >
	CheckActive( Args... args )
		: m_args{ std::ref( args )... }
	{ }

	bool isActive() const
	{
		return std::apply(
			[ & ]( const auto&... args ) {
				auto isActive = []< typename T >( const T& arg ) -> bool {
					if constexpr( requires { arg.isActive(); } )
					{
						return arg.isActive();
					}

					return true;
				};

				return ( ... && isActive( args.get() ) );
			},
			m_args );
	}

private:
	std::tuple< std::reference_wrapper< Ts >... > m_args;
};

class Thermostat
{
public:
	Thermostat( PBL::I2C::BusController& busController )
		: m_pid{ 0.5, 0.2, 0.25 }
		, m_adc{ busController }
		, m_lm75{ busController }
		, m_thermostat{ busController }

	{ }

	std::expected< void, std::string > update( float dt )
	{
		return m_adc.readDesiredTemp()
			.and_then( [ this ]( float desiredTemp ) -> std::expected< std::pair< float, float >, std::string > {
				auto currTemp = m_lm75.getTemperatureC();
				if( !currTemp )
				{
					return std::unexpected( currTemp.error() );
				}

				return std::pair{ desiredTemp, *currTemp };
			} )
			.and_then( [ this, dt ]( std::pair< float, float > values ) -> std::expected< float, std::string > {
				return ( m_pid.update( dt, unwrap( values ) ) | PBL::Math::Cap{ 0.0f, 10.0f } | PBL::Math::Pow2{} );
			} )
			.and_then( [ this ]( float controlSignal ) { return m_thermostat.adjust( controlSignal ); } )
			.or_else( []( const std::string& error ) -> std::expected< void, std::string > {
				return std::unexpected< std::string >( error );
			} );
	}

private:
	PBL::Math::PIDController< float > m_pid;
	PBL::I2C::ADCController m_adc;
	PBL::I2C::LM75Controller m_lm75;
	PBL::I2C::ThermostatController m_thermostat;
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
	// if( !busController.isOpen() )
	// {
	// 	std::cerr << "Failed to open I2C device" << std::endl;
	// 	return 1;
	// }

	Thermostat thermostat{ busController };
	PBL::Utils::Timer timer;
	timer.start();
	float dt{ 0.0001 };

	while( true )
	{
		timer.reset();
		auto rslt = thermostat.update( dt );

		// if( !rslt )
		// {
		// 	std::cerr << rslt.error() << std::endl;
		// 	break;
		// }

		// std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

		auto dt = static_cast< float >( timer.elapsedTimeS() );
		std::cout << std::format( "{:12f}", dt ) << std::endl;
	}

	return 0;
}
