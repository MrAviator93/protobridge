/**
 *  @brief Implementation of SHT31Controller class for managing the SHT31 sensor.
 *  @author MrAviator93
 *  @date 25 October 2025
 *
 *  Copyright (c) 2025 MrAviator93
 *
 *  For license details, see the LICENSE file in the project root.
 */

#include "SHT31Controller.hpp"
#include "BusController.hpp"
#include <math/Math.hpp>

// C++
#include <array>
#include <bitset>
#include <cmath>

namespace pbl::i2c
{

namespace
{

// SHT31 Commands
constexpr std::uint16_t kSingleShotHighRepeatability = 0x2400;
constexpr std::uint16_t kSingleShotMediumRepeatability = 0x240B;
constexpr std::uint16_t kSingleShotLowRepeatability = 0x2416;
constexpr std::uint16_t kSoftReset = 0x30A2;
constexpr std::uint16_t kReadStatus = 0xF32D;

[[nodiscard]] inline constexpr std::uint8_t highByte( std::uint16_t value ) noexcept
{
	return static_cast< std::uint8_t >( value >> 8 );
}

[[nodiscard]] inline constexpr std::uint8_t lowByte( std::uint16_t value ) noexcept
{
	return static_cast< std::uint8_t >( value & 0xFF );
}

} // namespace

v1::SHT31Controller::SHT31Controller( BusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
{ }

auto v1::SHT31Controller::triggerMeasurement( Repeatability rep ) -> Result< void >
{
	std::uint16_t command{};
	switch( rep )
	{
		using enum Repeatability;
		case HIGH: command = kSingleShotHighRepeatability; break;
		case MEDIUM: command = kSingleShotMediumRepeatability; break;
		case LOW: command = kSingleShotLowRepeatability; break;
	}

	std::array< std::uint8_t, 2 > cmd{ highByte( command ), lowByte( command ) };

	if( !write( cmd ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::SHT31Controller::getTemperatureC() -> Result< float >
{
	std::array< std::uint8_t, 6 > data{};

	if( read( data ) < 6 ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	const std::uint16_t rawTemp = ( static_cast< std::uint16_t >( data[ 0 ] ) << 8 ) | data[ 1 ];
	const float tempC = -45.0f + 175.0f * static_cast< float >( rawTemp ) / 65535.0f;
	return tempC;
}

auto v1::SHT31Controller::getTemperatureF() -> Result< float >
{
	return getTemperatureC().transform( math::CelsiusToFarenheit< float >{} );
}

auto v1::SHT31Controller::getHumidity() -> Result< float >
{
	std::array< std::uint8_t, 6 > data{};

	if( read( data ) < 6 ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	const auto rawHumidity = ( static_cast< std::uint16_t >( data[ 3 ] ) << 8 ) | data[ 4 ];
	const auto humidity = 100.0f * static_cast< float >( rawHumidity ) / 65535.0f;
	return humidity;
}

auto v1::SHT31Controller::reset() -> Result< void >
{
	std::array< std::uint8_t, 2 > cmd{ highByte( kSoftReset ), lowByte( kSoftReset ) };

	if( !write( cmd ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::SHT31Controller::readStatus() -> Result< uint16_t >
{
	std::array< std::uint8_t, 2 > cmd{ highByte( kReadStatus ), lowByte( kReadStatus ) };

	if( !write( cmd ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	std::array< std::uint8_t, 3 > response{};

	if( read( response ) < 3 ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	return static_cast< uint16_t >( ( response[ 0 ] << 8 ) | response[ 1 ] );
}

} // namespace pbl::i2c