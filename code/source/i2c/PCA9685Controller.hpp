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

	/// Enum for on/off time ranges (0-4095, represented by ON or OFF state)
	enum class PWMState : std::uint16_t
	{
		MIN = 0,
		MAX = 4095
	};

	using enum Address;
	using enum Channel;

	explicit PCA9685Controller( class BusController& busController, Address address = H40 );

	// Set PWM frequency (applies globally to all channels)
	Result< void > setPWMFrequency( std::uint16_t frequency );

	std::uint16_t pulseWidthToSteps( float pulseWidth )
	{
		// Specific resolution of the PCA9685, which allows 12-bit (4096-step) resolution for PWM.
		constexpr float pcaResolution = 4096.0f;

		// Calculate the period of one cycle in microseconds
		// const float periodUs = 1'000'000.0f / pwmFrequency; // currently set to 20'000

		// 1 step = 4.88 microseconds (20000us / 4096 steps for 50Hz)
		return static_cast< std::uint16_t >( ( pulseWidth / 20'000.0f ) * pcaResolution );
	}

private:
	PCA9685Controller( const PCA9685Controller& ) = delete;
	PCA9685Controller& operator=( const PCA9685Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_PCA9658_CONTROLLER_HPP__
