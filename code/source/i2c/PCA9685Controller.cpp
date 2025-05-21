#include "PCA9685Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

namespace
{
// PCA9685 Registers
constexpr std::uint8_t kMode1 = 0x00; // Mode register 1
constexpr std::uint8_t kMode2 = 0x01; // Mode register 2
constexpr std::uint8_t kPrescale = 0xFE; // Prescale register for PWM frequency
constexpr std::uint8_t kLed0OnL = 0x06; // LED0 output and PWM control, 4 registers per channel

// kMode1 register bit definitions
constexpr std::uint8_t kMode1Restart = 0x80; // Restart bit
constexpr std::uint8_t kMode1Sleep = 0x10; // Sleep bit
constexpr std::uint8_t kMode1Ai = 0x20; // Auto increment bit

} // namespace

v1::PCA9685Controller::PCA9685Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

auto v1::PCA9685Controller::setPWMFrequency( std::uint16_t frequency ) -> Result< void >
{
	constexpr std::uint16_t kMinFrequency{ 24 };
	constexpr std::uint16_t kMaxFrequency{ 1526 };

	if( frequency < kMinFrequency || frequency > kMaxFrequency )
	{
		std::string msg = "PWM frequency must be between 24 Hz and 1526 Hz.";
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, std::move( msg ) );
	}

	// Calculate prescale value
	const std::uint8_t prescale = static_cast< std::uint8_t >( 25000000.0 / ( 4096 * frequency ) - 1 );

	// Put the device in sleep mode before setting prescale
	std::ignore = write( kMode1, kMode1Sleep );

	// Write the prescale value to the prescale register
	std::ignore = write( kPrescale, prescale );

	// Restart the device and enable auto-increment
	std::ignore = write( kMode1, kMode1Restart | kMode1Ai );

	return utils::MakeError( utils::ErrorCode::NOT_IMPLEMENTED );
}

// // Set the PWM duty cycle for a specific channel
// auto v1::PCA9685Controller::setPWM( Channel channel, PWMState on, PWMState off ) -> Result< void >
// {
// 	std::uint8_t regBase = kLed0OnL + 4 * static_cast< std::uint8_t >( channel );

// 	std::ignore = write( regBase, static_cast< std::uint8_t >( static_cast< std::uint16_t >( on ) & 0xFF ) ); // ON_L
// 	std::ignore = write( regBase + 1, static_cast< std::uint8_t >( static_cast< std::uint16_t >( on ) >> 8 ) ); // ON_H
// 	std::ignore = write( regBase + 2, static_cast< std::uint8_t >( static_cast< std::uint16_t >( off ) & 0xFF ) ); // OFF_L
// 	std::ignore = write( regBase + 3, static_cast< std::uint8_t >( static_cast< std::uint16_t >( off ) >> 8 ) ); // OFF_H
//  return utils::MakeSuccess();
// }

// // Enable or disable sleep mode
// auto v1::PCA9685Controller::sleep( bool enable ) -> Result< void >
// {
// 	std::uint8_t mode1;
// 	if( !read( kMode1, mode1 ) )
// 	{
// 		throw std::runtime_error( "Failed to read kMode1 register." );
// 	}
//
// 	if( enable )
// 	{
// 		mode1 |= MODE1_SLEEP; // Set sleep bit
// 	}
// 	else
// 	{
// 		mode1 &= ~MODE1_SLEEP; // Clear sleep bit
// 	}
//
// 	std::ignore = write( kMode1, mode1 );
//
//  return utils::MakeSuccess();
// }

} // namespace pbl::i2c
