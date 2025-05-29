/**
 *  @brief Implementation of TMP102Controller class for managing the TMP102 sensor.
 *  @date 30 April 2025
 *  @author Artur K.
 *
 *  Copyright (c) 2025 Artur K.
 *
 *  For license details, see the LICENSE file in the project root.
 */

#include "TMP102Controller.hpp"
#include "BusController.hpp"
#include <math/Math.hpp>

// C++
#include <array>
#include <bitset>

namespace pbl::i2c
{

namespace
{
// TMP102 Register Addresses
constexpr std::uint8_t kTemperatureRegister{ 0x00 };
constexpr std::uint8_t kConfigurationRegister{ 0x01 };
constexpr std::uint8_t kTLowRegister{ 0x02 };
constexpr std::uint8_t kTHighRegister{ 0x03 };

// TMP102 Temperature Conversion Constants
constexpr std::uint16_t kExtendedModeNegativeMask = 0x1000;
constexpr std::uint16_t kExtendedModeSignExtendMask = 0xE000;
constexpr std::uint16_t kStandardModeNegativeMask = 0x0800;
constexpr std::uint16_t kStandardModeSignExtendMask = 0xF000;

// TMP102 Configuration Bits
constexpr std::size_t kShutdownBit = 0; // Bit 0: Shutdown Mode
constexpr std::size_t kExtendedModeBit = 4; // Bit 4: Extended Mode (12-bit vs 13-bit)
constexpr std::size_t kOneShotBit = 7; // Bit 7: One-Shot (when in shutdown mode)
constexpr std::size_t kAlertBit = 5; // Bit 5 (read only in some TMP102 variants) for alert

} // namespace

v1::TMP102Controller::TMP102Controller( BusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
{ }

auto v1::TMP102Controller::getTemperatureC() -> Result< float >
{
	std::array< std::uint8_t, 2 > data{};
	if( read( kTemperatureRegister, data.data(), data.size() ) < 2 ) [[unlikely]]
	{
		return std::unexpected( utils::ErrorCode::FAILED_TO_READ );
	}

	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) <= 0 ) [[unlikely]]
	{
		return std::unexpected( utils::ErrorCode::FAILED_TO_READ );
	}

	// TODO: Do we need to convert config's endieness here?
	const bool extendedMode = ( config[ 1 ] & ( 1 << kExtendedModeBit ) ) != 0;

	std::uint16_t raw = ( static_cast< std::uint16_t >( data[ 0 ] ) << 4 ) | ( data[ 1 ] >> 4 );

	if( extendedMode )
	{
		raw = ( static_cast< std::uint16_t >( data[ 0 ] ) << 5 ) | ( data[ 1 ] >> 3 );

		if( raw & kExtendedModeNegativeMask )
		{
			raw |= kExtendedModeSignExtendMask;
		}

		return static_cast< float >( static_cast< std::int16_t >( raw ) ) * 0.0625f;
	}
	else
	{
		if( raw & kStandardModeNegativeMask )
		{
			raw |= kStandardModeSignExtendMask;
		}

		return static_cast< float >( static_cast< std::int16_t >( raw ) ) * 0.0625f;
	}
}

auto v1::TMP102Controller::getTemperatureF() -> Result< float >
{
	return getTemperatureC().transform( math::CelsiusToFarenheit< float >{} );
}

bool v1::TMP102Controller::setShutdownMode( bool enable )
{
	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) < 2 ) [[unlikely]]
	{
		return false;
	}

	std::bitset< 8 > lsb( config[ 1 ] );
	lsb.set( kShutdownBit, enable );
	config[ 1 ] = static_cast< std::uint8_t >( lsb.to_ulong() );

	return write( kConfigurationRegister, config, 2 );
}

auto v1::TMP102Controller::getShutdownMode() -> Result< bool >
{
	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) < 2 ) [[unlikely]]
	{
		return std::unexpected( utils::ErrorCode::FAILED_TO_READ );
	}

	std::bitset< 8 > lsb( config[ 1 ] );
	return lsb.test( kShutdownBit );
}

bool v1::TMP102Controller::setExtendedMode( bool enable )
{
	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) < 2 ) [[unlikely]]
	{
		return false;
	}

	std::bitset< 8 > msb( config[ 0 ] );
	msb.set( kExtendedModeBit, enable );
	config[ 0 ] = static_cast< std::uint8_t >( msb.to_ulong() );

	return write( kConfigurationRegister, config, 2 );
}

auto v1::TMP102Controller::getExtendedMode() -> Result< bool >
{
	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) < 2 ) [[unlikely]]
	{
		return std::unexpected( utils::ErrorCode::FAILED_TO_READ );
	}

	std::bitset< 8 > msb( config[ 0 ] );
	return msb.test( kExtendedModeBit );
}

bool v1::TMP102Controller::triggerOneShot()
{
	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) < 2 ) [[unlikely]]
	{
		return false;
	}

	std::bitset< 8 > msb( config[ 0 ] );
	msb.set( kOneShotBit );
	config[ 0 ] = static_cast< std::uint8_t >( msb.to_ulong() );

	return write( kConfigurationRegister, config, 2 );
}

auto v1::TMP102Controller::getAlertStatus() -> Result< bool >
{
	// TMP102 alert status can often be read from ALERT pin;
	// here we simulate register-based
	std::uint8_t config[ 2 ]{};
	if( read( kConfigurationRegister, config, 2 ) < 2 ) [[unlikely]]
	{
		return std::unexpected( utils::ErrorCode::FAILED_TO_READ );
	}

	std::bitset< 8 > lsb( config[ 1 ] );
	return lsb.test( kAlertBit );
}

} // namespace pbl::i2c
