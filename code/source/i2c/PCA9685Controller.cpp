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
		std::string msg = "Frequency out of range, it must be between 24 Hz and 1526 Hz.";
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, std::move( msg ) );
	}

	// Calculate prescale value
	const auto prescale = static_cast< std::uint8_t >( ( 25'000'000.0 / ( 4'096.0 * frequency ) ) - 1.0 );

	std::uint8_t oldmode{};
	if( !read( kMode1, oldmode ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	// Put the device in sleep mode before setting prescale
	std::uint8_t sleepmode = ( oldmode & 0x7F ) | kMode1Sleep;
	if( !write( kMode1, sleepmode ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	// Write the prescale value to the prescale register
	if( !write( kPrescale, prescale ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	if( !write( kMode1, oldmode ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	sleep( std::chrono::milliseconds( 1 ) );

	if( !write( kMode1, oldmode | kMode1Restart | kMode1Ai ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::setPWM( Channel channel, PWMState on, PWMState off ) -> Result< void >
{
	const auto reg = static_cast< std::uint8_t >( kLed0OnL + 4 * static_cast< int >( channel ) );
	const auto onValue = static_cast< std::uint16_t >( on );
	const auto offValue = static_cast< std::uint16_t >( off );

	if( !write( reg, static_cast< std::uint8_t >( onValue & 0xFF ) ) ||
		!write( reg + 1, static_cast< std::uint8_t >( onValue >> 8 ) ) ||
		!write( reg + 2, static_cast< std::uint8_t >( offValue & 0xFF ) ) ||
		!write( reg + 3, static_cast< std::uint8_t >( offValue >> 8 ) ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::setPWMPercentage( Channel channel, float dutyPercent ) -> Result< void >
{
	if( dutyPercent < 0.0f || dutyPercent > 100.0f )
	{
		std::string msg = "Duty cycle must be between 0 and 100.";
		return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT, std::move( msg ) );
	}

	const auto steps = static_cast< std::uint16_t >( 4095.0f * ( dutyPercent / 100.0f ) );
	return setPWM( channel, PWMState::min(), static_cast< PWMState >( steps ) );
}

auto v1::PCA9685Controller::setSleepMode( bool enable ) -> Result< void >
{
	std::uint8_t mode1{};
	if( !read( kMode1, mode1 ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	if( enable )
	{
		mode1 |= kMode1Sleep;
	}
	else
	{
		mode1 &= ~kMode1Sleep;
	}

	if( !write( kMode1, mode1 ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::setFullOn( Channel channel, bool enable ) -> Result< void >
{
	const auto base = 0x06 + 4 * static_cast< int >( channel );
	const auto reg = static_cast< std::uint8_t >( base + 1 ); // LEDn_ON_H

	std::uint8_t val{};
	if( !read( reg, val ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	val = enable ? ( val | 0x10 ) : ( val & ~0x10 );
	if( !write( reg, val ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::setFullOff( Channel channel, bool enable ) -> Result< void >
{
	const auto base = 0x06 + 4 * static_cast< int >( channel );
	const auto reg = static_cast< std::uint8_t >( base + 3 ); // LEDn_OFF_H

	std::uint8_t val{};
	if( !read( reg, val ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	val = enable ? ( val | 0x10 ) : ( val & ~0x10 );
	if( !write( reg, val ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::setOutputMode( bool totemPole ) -> Result< void >
{
	std::uint8_t mode2{};
	if( !read( 0x01, mode2 ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	mode2 = totemPole ? ( mode2 | 0x04 ) : ( mode2 & ~0x04 );
	if( !write( 0x01, mode2 ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::enableAllCallAddress( bool enable ) -> Result< void >
{
	std::uint8_t mode1{};
	if( !read( 0x00, mode1 ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	mode1 = enable ? ( mode1 | 0x01 ) : ( mode1 & ~0x01 );
	if( !write( 0x00, mode1 ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::PCA9685Controller::softwareReset() -> Result< void >
{
	constexpr std::array< std::uint8_t, 2 > kResetCmd{ 0x06, 0x00 };
	if( !write( 0x00, kResetCmd.data(), kResetCmd.size() ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	sleep( std::chrono::milliseconds( 10 ) );

	return utils::MakeSuccess();
}

} // namespace pbl::i2c
