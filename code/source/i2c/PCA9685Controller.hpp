#ifndef PBL_I2C_PCA9658_CONTROLLER_HPP__
#define PBL_I2C_PCA9658_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <string>
#include <expected>

namespace pbl::i2c
{

inline namespace v1
{

/**
 * @brief Controller for the PCA9685 16-channel, 12-bit PWM Fm+ I2C-bus LED controller.
 * 
 */
class PCA9685Controller final : public ICBase, public utils::Counter< PCA9685Controller >
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	// I2C addresses determined by hardware address pins A0–A2.
	enum class Address : std::uint8_t
	{
		H40 = 0x40, // A0=0, A1=0, A2=0
		H41 = 0x41, // A0=1, A1=0, A2=0
		H42 = 0x42, // A0=0, A1=1, A2=0
		H43 = 0x43, // A0=1, A1=1, A2=0
		H44 = 0x44, // A0=0, A1=0, A2=1
		H45 = 0x45, // A0=1, A1=0, A2=1
		H46 = 0x46, // A0=0, A1=1, A2=1
		H47 = 0x47 // A0=1, A1=1, A2=1
	};

	/// 16 PWM output channels.
	enum class Channel : std::uint8_t
	{
		CH0 = 0,
		CH1 = 1,
		CH2 = 2,
		CH3 = 3,
		CH4 = 4,
		CH5 = 5,
		CH6 = 6,
		CH7 = 7,
		CH8 = 8,
		CH9 = 9,
		CH10 = 10,
		CH11 = 11,
		CH12 = 12,
		CH13 = 13,
		CH14 = 14,
		CH15 = 15
	};

	/// Represents a PWM step value within the 12-bit range of the PCA9685 (0–4095).
	class PWMState final
	{
		static constexpr std::uint16_t kMin = 0;
		static constexpr std::uint16_t kMax = 4095;

	public:
		/// @brief Constructs a PWMState with a default value of 0 steps.
		constexpr PWMState() noexcept = default;

		/// @brief Constructs a PWMState from an explicit step count (0–4095).
		/// Values above 4095 are clamped to 4095.
		explicit constexpr PWMState( std::uint16_t steps ) noexcept
			: m_steps{ steps > kMax ? kMax : steps }
		{ }

		/// @brief Creates a PWMState from a duty cycle percentage (0.0% to 100.0%).
		/// Values outside the valid range are clamped to 0% or 100%.
		/// @param percent Duty cycle as a float between 0.0f and 100.0f
		[[nodiscard]] static constexpr PWMState fromPercent( float percent ) noexcept
		{
			if( percent <= 0.0f ) return PWMState{ kMin };
			if( percent >= 100.0f ) return PWMState{ kMax };
			return PWMState{ static_cast< std::uint16_t >( kMax * ( percent / 100.0f ) ) };
		}

		/// @brief Creates a PWMState from a pulse width in microseconds.
		/// The period defaults to 20000µs (50 Hz), appropriate for servo motors.
		/// Returns 0 steps if the period is invalid (<= 0).
		/// @param us Pulse width in microseconds
		/// @param periodUs Full PWM cycle period in microseconds
		[[nodiscard]] static constexpr PWMState fromMicroseconds( float us, float periodUs = 20000.0f ) noexcept
		{
			if( periodUs <= 0.0f ) return PWMState{ kMin };
			return PWMState{ static_cast< std::uint16_t >( ( us / periodUs ) * kMax ) };
		}

		/// @brief Returns the raw step count value (0–4095).
		[[nodiscard]] constexpr std::uint16_t steps() const noexcept { return m_steps; }

		/// @brief Implicit conversion to raw step count.
		/// Allows seamless use in contexts requiring uint16_t (e.g. register writing).
		[[nodiscard]] constexpr operator std::uint16_t() const noexcept { return m_steps; }

		/// @brief Returns the minimum possible PWM state (0 steps).
		[[nodiscard]] static constexpr PWMState min() noexcept { return PWMState{ kMin }; }

		/// @brief Returns the maximum possible PWM state (4095 steps).
		[[nodiscard]] static constexpr PWMState max() noexcept { return PWMState{ kMax }; }

	private:
		std::uint16_t m_steps{ kMin };
	};

	using enum Address;
	using enum Channel;

	/// Construct controller with I2C bus and device address.
	explicit PCA9685Controller( class BusController& busController, Address address = H40 );

	/// Set global PWM frequency [Hz] (applies globally to all channels).
	[[nodiscard]] Result< void > setPWMFrequency( std::uint16_t frequency );

	/// Set channel ON/OFF step timings.
	[[nodiscard]] Result< void > setPWM( Channel channel, PWMState on, PWMState off );

	/// Set channel duty cycle as percent (0–100%).
	[[nodiscard]] Result< void > setPWMPercentage( Channel channel, float dutyPercent );

	/// Enable or disable chip low-power sleep mode.
	[[nodiscard]] Result< void > setSleepMode( bool enable );

	/// Set channel full-on mode (bit 4 in LEDn_ON_H).
	[[nodiscard]] Result< void > setFullOn( Channel channel, bool enable );

	/// Set channel full-off mode (bit 4 in LEDn_OFF_H).
	[[nodiscard]] Result< void > setFullOff( Channel channel, bool enable );

	/// Set output mode: true = totem-pole, false = open-drain.
	[[nodiscard]] Result< void > setOutputMode( bool totemPole );

	/// Enable or disable ALL_CALL I2C address.
	[[nodiscard]] Result< void > enableAllCallAddress( bool enable );

	/// Send general call software reset.
	[[nodiscard]] Result< void > softwareReset();

	// /// Convert µs pulse width to PWM steps (based on 20ms period).
	// [[nodiscard]] std::uint16_t pulseWidthToSteps( float pulseWidth )
	// {
	// 	// Specific resolution of the PCA9685, which allows 12-bit (4096-step) resolution for PWM.
	// 	constexpr float pcaResolution = 4096.0f;

	// 	// Calculate the period of one cycle in microseconds
	// 	// const float periodUs = 1'000'000.0f / pwmFrequency; // currently set to 20'000

	// 	// 1 step = 4.88 microseconds (20000us / 4096 steps for 50Hz)
	// 	return static_cast< std::uint16_t >( ( pulseWidth / 20'000.0f ) * pcaResolution );
	// }

private:
	PCA9685Controller( const PCA9685Controller& ) = delete;
	PCA9685Controller& operator=( const PCA9685Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_PCA9658_CONTROLLER_HPP__
