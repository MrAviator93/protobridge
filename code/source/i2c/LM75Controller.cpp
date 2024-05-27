/**
 *  @brief Implementation of LM75Controller class for managing the LM75 sensor.
 *  @author Artur K.
 *  @date 25 October 2023
 *
 *  Copyright (c) 2023 Artur K.
 *
 *  For license details, see the LICENSE file in the project root.
 */

#include "LM75Controller.hpp"
#include "BusController.hpp"
#include <math/Math.hpp>

// C++
#include <array>
#include <bitset>

namespace pbl::i2c
{

namespace
{

// LM75 Register Addresses
constexpr std::uint8_t kTempReadRegister{ 0x00 }; //!< Temperature read-out, 0.125°C resolution.
constexpr std::uint8_t kConfigurationRegister{ 0x01 }; //!< Config register, sets sensor operating conditions.
constexpr std::uint8_t kTempSwitchOffRegister{ 0x02 }; //!< Hysteresis register, defines lower limit for interrupt.
constexpr std::uint8_t kTempSwitchOnRegister{ 0x03 }; //!< Over-temp shutdown, defines high limit for interrupt.

// LM75 Configuration Bits
constexpr std::size_t kOSFaultQueueBit = 4; // Bit 4: OS fault queue (prevents false tripping).
constexpr std::size_t kAlertStatusBit = 5; // Bit 5 is used for the alert status.
constexpr std::size_t kThermostatModeBit = 6; // Bit 6 is used for the thermostat mode.
constexpr std::size_t kShutdownModeBit = 7; // Bit 7 is used for the shutdown mode.

} // namespace

LM75Controller::LM75Controller( BusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
{ }

bool LM75Controller::setPowerMode( PowerMode mode )
{
	std::uint8_t config{};
	if( read( kConfigurationRegister, config ) )
	{
		std::bitset< 8 > configBits{ config };
		configBits.set( kShutdownModeBit, static_cast< std::uint8_t >( mode ) );
		return write( kConfigurationRegister, static_cast< std::uint8_t >( configBits.to_ulong() ) );
	}

	return false;
}

bool LM75Controller::setThermostatMode( ThermostatMode mode )
{
	std::uint8_t config{};
	if( read( kConfigurationRegister, config ) )
	{
		std::bitset< 8 > configBits{ config };
		configBits.set( kThermostatModeBit, static_cast< std::uint8_t >( mode ) );
		return write( kConfigurationRegister, static_cast< std::uint8_t >( configBits.to_ulong() ) );
	}

	return false;
}

auto LM75Controller::getPowerMode() -> Result< PowerMode >
{
	std::uint8_t config{};
	if( read( kConfigurationRegister, config ) )
	{
		std::bitset< 8 > configBits{ config };
		if( configBits.test( kShutdownModeBit ) )
		{
			return PowerMode::LOW_POWER;
		}

		return PowerMode::NORMAL;
	}

	return std::unexpected( ICError::FAILED_TO_READ );
}

auto LM75Controller::getThermostatMode() -> Result< ThermostatMode >
{
	std::uint8_t config{};
	if( read( kConfigurationRegister, config ) )
	{
		std::bitset< 8 > configBits{ config };
		if( configBits.test( kThermostatModeBit ) )
		{
			return ThermostatMode::INTERRUPT;
		}

		return ThermostatMode::COMPARATOR;
	}

	return std::unexpected( ICError::FAILED_TO_READ );
}

auto LM75Controller::getAlertStatus() -> Result< bool >
{
	std::uint8_t config{};
	if( read( kConfigurationRegister, config ) )
	{
		std::bitset< 8 > configBits( config );
		return configBits.test( kAlertStatusBit );
	}

	return std::unexpected( ICError::FAILED_TO_READ );
}

auto LM75Controller::getTemperatureC() -> Result< float >
{
	std::array< std::uint8_t, 2 > data{ 0x00, 0x00 };
	if( const auto size = read( kTempReadRegister, data.data(), data.size() ); size > 0 )
	{
		// Calculate temperature in Celsius
		std::int16_t iTemp = ( std::int16_t{ data[ 0 ] } << 8 ) | data[ 1 ];
		return static_cast< float >( iTemp ) / 256.0f;
	}

	return std::unexpected( ICError::FAILED_TO_READ );
}

auto LM75Controller::getTemperatureF() -> Result< float >
{
	auto temp = getTemperatureC();
	if( temp.has_value() )
	{
		return utils::celsiusToFahrenheit( temp.value() );
	}

	return temp;
}

} // namespace pbl::i2c
