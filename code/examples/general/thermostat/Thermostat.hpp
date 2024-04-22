#ifndef PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__
#define PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__

#include <math/PID.hpp>
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>

// C++
#include <iostream>
#include <expected>

#define unwrap( p ) p.first, p.second

namespace pbl
{

namespace i2c
{

class ThermostatController
{
public:
	ThermostatController( pbl::i2c::BusController& ) { }

	[[nodiscard]] std::expected< void, std::string > adjust( float value )
	{
		std::cout << std::format( "Adjust: {}", value ) << std::endl;
		return {};
	}
};

class ADCController
{
public:
	ADCController( pbl::i2c::BusController& ) { }

	[[nodiscard]] std::expected< float, std::string > readDesiredTemp()
	{
		// You would get it using I2C from
		return 25.0f;
	}
};

} // namespace i2c

namespace examples
{

class Thermostat
{
public:
	Thermostat( pbl::i2c::BusController& busController )
		: m_pid{ 0.5, 0.2, 0.25 }
		, m_adc{ busController }
		, m_lm75{ busController }
		, m_thermostat{ busController }

	{ }

	[[nodiscard]] std::expected< void, std::string > update( float dt )
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
				return ( m_pid.update( dt, unwrap( values ) ) | pbl::math::Cap{ 0.0f, 10.0f } | pbl::math::Pow2{} );
			} )
			.and_then( [ this ]( float controlSignal ) { return m_thermostat.adjust( controlSignal ); } )
			.or_else( []( const std::string& error ) -> std::expected< void, std::string > {
				return std::unexpected< std::string >( error );
			} );
	}

private:
	pbl::math::PIDController< float > m_pid;
	pbl::i2c::ADCController m_adc;
	pbl::i2c::LM75Controller m_lm75;
	pbl::i2c::ThermostatController m_thermostat;
};

} // namespace examples
} // namespace pbl
#endif // PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__
