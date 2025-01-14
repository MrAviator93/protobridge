#ifndef PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__
#define PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__

#include <math/PID.hpp>
#include <utils/ErrorCode.hpp>
#include <i2c/BusController.hpp>
#include <i2c/LM75Controller.hpp>

// C++
#include <print>
#include <expected>

namespace pbl
{

namespace i2c
{

class ThermostatController
{
public:
	ThermostatController( pbl::i2c::BusController& ) { }

	[[nodiscard]] std::expected< void, pbl::utils::ErrorCode > adjust( float value )
	{
		std::println( "Adjust: {}", value );
		return {};
	}
};

class ADCController
{
public:
	ADCController( pbl::i2c::BusController& ) { }

	[[nodiscard]] std::expected< float, pbl::utils::ErrorCode > readDesiredTemp()
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
	using PIDController = pbl::math::PIDController< float >;

public:
	Thermostat( pbl::i2c::BusController& busController )
		: m_pid{ 0.5, 0.2, 0.25 }
		, m_adc{ busController }
		, m_lm75{ busController }
		, m_thermostat{ busController }

	{ }

	[[nodiscard]] std::expected< void, pbl::utils::ErrorCode > update( float dt )
	{
		using PIDInput = PIDController::Input;

		return m_adc.readDesiredTemp()
			.and_then( [ this ]( float desiredTemp ) -> std::expected< PIDInput, pbl::utils::ErrorCode > {
				const auto currTemp = m_lm75.getTemperatureC();
				if( !currTemp )
				{
					return std::unexpected( currTemp.error() );
				}

				return PIDInput{ desiredTemp, *currTemp };
			} )
			.and_then( [ this, dt ]( PIDInput values ) -> std::expected< float, pbl::utils::ErrorCode > {
				return ( m_pid( dt, values ) | pbl::math::Cap{ 0.0f, 10.0f } | pbl::math::Pow2{} );
			} )
			.and_then( [ this ]( float controlSignal ) { return m_thermostat.adjust( controlSignal ); } )
			.or_else( []( const auto error ) -> std::expected< void, pbl::utils::ErrorCode > {
				return std::unexpected( error );
			} );
	}

private:
	PIDController m_pid;
	pbl::i2c::ADCController m_adc;
	pbl::i2c::LM75Controller m_lm75;
	pbl::i2c::ThermostatController m_thermostat;
};

} // namespace examples
} // namespace pbl
#endif // PBL_EXAMPLES_GENERAL_THERMOSTAT_HPP__
