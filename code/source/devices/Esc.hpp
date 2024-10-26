#ifndef PBL_DEVICES_ESC_HPP__
#define PBL_DEVICES_ESC_HPP__

#include <math/Math.hpp>

// C++
#include <memory>
#include <cstdint>
#include <string>
#include <utility>
#include <expected>

namespace pbl::devices
{

template < typename PWMController, typename ArmingStrategy >
class Esc
{
public:
	Esc( PWMController& pwmController,
		 ArmingStrategy&& armingStrategy,
		 std::uint8_t channel,
		 float minThrottle = 0.0f,
		 float maxThrottle = 100.0f )
		: m_pwmController{ pwmController }
		, m_armingStrategy{ std::move( armingStrategy ) }
		, m_channel{ channel }
		, m_minThrottle{ minThrottle }
		, m_maxThrottle{ maxThrottle }
	{ }

	// Arm the ESC using the arming strategy
	std::expected< void, std::string > arm() { return m_armingStrategy( m_pwmController, m_channel ); }

	// Set the throttle percentage
	std::expected< void, std::string > setThrottle( float percentage )
	{
		if( percentage < m_minThrottle || percentage > m_maxThrottle )
		{
			return std::unexpected( "Throttle out of bounds" );
		}

		// Map the throttle percentage to pulse width
		const float pulseWidth =
			math::map( percentage, m_minThrottle, m_maxThrottle, m_minPulseWidth, m_maxPulseWidth );

		// Convert the pulse width to PWM steps (0-4095)
		const std::uint16_t offStep = m_pwmController.pulseWidthToSteps( pulseWidth );

		// Send the PWM signal using the controller
		if( auto rslt = m_pwmController.setPWM( m_channel, 0, offStep ); !rslt )
		{
			return std::unexpected( "Failed to set PWM signal" );
		}

		return {};
	}

	// Optional: Set limits for the pulse width (in microseconds) for fine-tuning
	void setPulseWidthRange( float minPulseWidth, float maxPulseWidth )
	{
		m_minPulseWidth = minPulseWidth;
		m_maxPulseWidth = maxPulseWidth;
	}

private:
	PWMController& m_pwmController;
	ArmingStrategy m_armingStrategy;
	std::uint8_t m_channel; // Channel on the PWM controller
	float m_minThrottle; // Minimum throttle percentage (0%)
	float m_maxThrottle; // Maximum throttle percentage (100%)
	float m_minPulseWidth{ 1000.0f }; // Minimum pulse width in microseconds (e.g., 1000us)
	float m_maxPulseWidth{ 2000.0f }; // Maximum pulse width in microseconds (e.g., 2000us)
};

} // namespace pbl::devices
#endif // PBL_DEVICES_ESC_HPP__