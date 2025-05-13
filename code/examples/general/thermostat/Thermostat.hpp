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

	[[nodiscard]] utils::Result< void > adjust( float value )
	{
		std::println( "Adjust: {}", value );
		return {};
	}
};

class ADCController
{
public:
	ADCController( pbl::i2c::BusController& ) { }

	[[nodiscard]] utils::Result< float > readDesiredTemp()
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

	[[nodiscard]] utils::Result< void > update( float dt )
	{
		using PIDInput = PIDController::Input;

		return m_adc.readDesiredTemp()
			.and_then( [ this ]( float desiredTemp ) -> utils::Result< PIDInput > {
				return m_lm75.getTemperatureC().transform(
					[ & ]( float currentTemp ) { return PIDInput{ desiredTemp, currentTemp }; } );
			} )
			.transform( [ this, dt ]( PIDInput input ) -> float {
				return m_pid( dt, input ) | pbl::math::Cap{ 0.0f, 10.0f } | pbl::math::Pow2{};
			} )
			.and_then( [ this ]( float controlSignal ) { return m_thermostat.adjust( controlSignal ); } );
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
