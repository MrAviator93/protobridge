#include "PCA9685Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

namespace
{
// PCA9685 Registers
constexpr std::uint8_t MODE1 = 0x00; // Mode register 1
constexpr std::uint8_t MODE2 = 0x01; // Mode register 2
constexpr std::uint8_t PRESCALE = 0xFE; // Prescale register for PWM frequency
constexpr std::uint8_t LED0_ON_L = 0x06; // LED0 output and PWM control, 4 registers per channel

// MODE1 register bit definitions
constexpr std::uint8_t MODE1_RESTART = 0x80; // Restart bit
constexpr std::uint8_t MODE1_SLEEP = 0x10; // Sleep bit
constexpr std::uint8_t MODE1_AI = 0x20; // Auto increment bit

} // namespace

v1::PCA9685Controller::PCA9685Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

auto v1::PCA9685Controller::setPWMFrequency( std::uint16_t frequency ) -> Result< void >
{
	if( frequency < 24 || frequency > 1526 )
	{
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT,
								 "PWM frequency must be between 24 Hz and 1526 Hz." );
	}

	// Calculate prescale value
	std::uint8_t prescale = static_cast< std::uint8_t >( 25000000.0 / ( 4096 * frequency ) - 1 );

	// Put the device in sleep mode before setting prescale
	std::ignore = write( MODE1, MODE1_SLEEP );

	// Write the prescale value to the prescale register
	std::ignore = write( PRESCALE, prescale );

	// Restart the device and enable auto-increment
	std::ignore = write( MODE1, MODE1_RESTART | MODE1_AI );

	return utils::MakeError( utils::ErrorCode::NOT_IMPLEMENTED );
}

// // Set the PWM duty cycle for a specific channel
// void v1::PCA9685Controller::setPWM( Channel channel, PWMState on, PWMState off )
// {
// 	std::uint8_t regBase = LED0_ON_L + 4 * static_cast< std::uint8_t >( channel );

// 	write( regBase, static_cast< std::uint8_t >( static_cast< std::uint16_t >( on ) & 0xFF ) ); // ON_L
// 	write( regBase + 1, static_cast< std::uint8_t >( static_cast< std::uint16_t >( on ) >> 8 ) ); // ON_H
// 	write( regBase + 2, static_cast< std::uint8_t >( static_cast< std::uint16_t >( off ) & 0xFF ) ); // OFF_L
// 	write( regBase + 3, static_cast< std::uint8_t >( static_cast< std::uint16_t >( off ) >> 8 ) ); // OFF_H
// }

// // Enable or disable sleep mode
// void v1::PCA9685Controller::sleep( bool enable )
// {
// 	std::uint8_t mode1;
// 	if( !read( MODE1, mode1 ) )
// 	{
// 		throw std::runtime_error( "Failed to read MODE1 register." );
// 	}
// 	if( enable )
// 	{
// 		mode1 |= MODE1_SLEEP; // Set sleep bit
// 	}
// 	else
// 	{
// 		mode1 &= ~MODE1_SLEEP; // Clear sleep bit
// 	}
// 	write( MODE1, mode1 );
// }

} // namespace pbl::i2c
