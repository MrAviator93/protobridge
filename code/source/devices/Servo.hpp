#ifndef PBL_DEVICES_SERVO_HPP__
#define PBL_DEVICES_SERVO_HPP__

#include <math/Math.hpp>

// C++
#include <cstdint>
#include <expected>

namespace pbl::devices
{

template < typename PWMController >
class Servo
{
public:
	using Result = std::expected< void, std::string >;

	Servo( PWMController& pwmController, std::uint8_t channel, float minAngle = 0.0f, float maxAngle = 180.0f )
		: m_pwmController{ pwmController }
		, m_channel{ channel }
		, m_minAngle{ minAngle }
		, m_maxAngle{ maxAngle }
	{ }

	// Set the angle of the servo
	Result setAngle( float angle )
	{
		if( angle < m_minAngle || angle > m_maxAngle )
		{
			return std::unexpected( "Angle out of bounds" );
		}

		// Map the angle to pulse width in microseconds
		const float pulseWidth = math::map( angle, m_minAngle, m_maxAngle, m_minPulseWidth, m_maxPulseWidth );

		// Convert the pulse width to PWM steps (0-4095)
		const std::uint16_t offStep = m_pwmController.pulseWidthToSteps( pulseWidth );

		// Send the PWM signal using the controller
		if( const auto result = m_pwmController.setPWM( m_channel, 0, offStep ); !result )
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
	PWMController& m_pwmController; // Reference to the PWM controller (e.g., PCA9685)
	std::uint8_t m_channel; // Channel on the PWM controller
	float m_minAngle; // Minimum angle (e.g., 0°)
	float m_maxAngle; // Maximum angle (e.g., 180°)
	float m_minPulseWidth{ 1000.0f }; // Minimum pulse width in microseconds (e.g., 1000us)
	float m_maxPulseWidth{ 2000.0f }; // Maximum pulse width in microseconds (e.g., 2000us)
};
} // namespace pbl::devices
#endif // PBL_DEVICES_SERVO_HPP__
