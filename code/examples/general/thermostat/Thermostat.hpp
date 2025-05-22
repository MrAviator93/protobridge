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
		: m_pid{ 0.5f, 0.2f, 0.25f }
		, m_adc{ busController }
		, m_lm75{ busController }
		, m_thermostat{ busController }

	{ }

	[[nodiscard]] utils::Result< void > update( float dt )
	{
		using PIDInput = PIDController::Input;

		return m_adc
			.readDesiredTemp()
			// Step 1: Read the desired temperature from the ADC (returns Result<float>)
			.and_then( [ this ]( float desiredTemp ) -> utils::Result< PIDInput > {
				// Step 2: Try to get the current temperature from the LM75 sensor (returns Result<float>)
				// If successful, combine both temperatures into a PIDInput structure
				// If getTemperatureC() fails, the failure propagates automatically
				return m_lm75.getTemperatureC().transform( [ & ]( float currentTemp ) {
					// If successful, produce a PIDInput with both temps
					return PIDInput{ desiredTemp, currentTemp };
				} );
			} )

			// Step 3: Apply the PID controller to compute the control signal
			.transform( [ this, dt ]( PIDInput input ) -> float {
				// The result is post-processed:
				// - Clamped to [0.0, 10.0] using `Cap`
				// - Squared using `Pow2`
				return m_pid( dt, input ) | pbl::math::Cap{ 0.0f, 10.0f } | pbl::math::Pow2{};
			} )

			// Step 4: Use the control signal to adjust the thermostat (returns Result<void>)
			.and_then( [ this ]( float controlSignal ) {
				// If this fails, the error also propagates
				return m_thermostat.adjust( controlSignal );
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
