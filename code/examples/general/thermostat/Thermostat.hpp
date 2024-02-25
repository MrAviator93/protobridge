#ifndef PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__
#define PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__

#include <math/PID.hpp>
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>

// C++
#include <iostream>
#include <expected>

#define unwrap( p ) p.first, p.second

namespace PBL
{

namespace I2C
{

class ThermostatController
{
public:
	ThermostatController( PBL::I2C::BusController& ) { }

	[[nodiscard]] std::expected< void, std::string > adjust( float value )
	{
		std::cout << std::format( "Adjust: {}", value ) << std::endl;
		return {};
	}
};

class ADCController
{
public:
	ADCController( PBL::I2C::BusController& ) { }

	[[nodiscard]] std::expected< float, std::string > readDesiredTemp()
	{
		// You would get it using I2C from
		return 25.0f;
	}
};

} // namespace I2C

namespace Examples
{

class Thermostat
{
public:
	Thermostat( PBL::I2C::BusController& busController )
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

} // namespace Examples

} // namespace PBL
#endif // PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__
