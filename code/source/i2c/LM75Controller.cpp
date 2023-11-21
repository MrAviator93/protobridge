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
#include "I2CBusController.hpp"

#include <utils/Math.hpp>

// C++
#include <array>
#include <bitset>

namespace I2C
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

LM75Controller::LM75Controller( I2CBusController& busController, Address address ) noexcept
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

std::optional< LM75Controller::PowerMode > LM75Controller::getPowerMode()
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

	return std::nullopt;
}

std::optional< LM75Controller::ThermostatMode > LM75Controller::getThermostatMode()
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

	return std::nullopt;
}

std::optional< bool > LM75Controller::getAlertStatus()
{
	std::uint8_t config{};
	if( read( kConfigurationRegister, config ) )
	{
		std::bitset< 8 > configBits( config );
		return configBits.test( kAlertStatusBit );
	}

	return std::nullopt;
}

std::optional< float > LM75Controller::getTemperatureC()
{
	std::array< std::uint8_t, 2 > data{ 0x00, 0x00 };
	if( auto size = read( kTempReadRegister, data.data(), data.size() ); size > 0 )
	{
		// Calculate temperature in Celsius
		std::int16_t iTemp = ( std::int16_t{ data[ 0 ] } << 8 ) | data[ 1 ];
		return iTemp / 256.0f;
	}

	return std::nullopt;
}

std::optional< float > LM75Controller::getTemperatureF()
{
	if( auto temp = getTemperatureC(); temp.has_value() )
	{
		return celsiusToFahrenheit( temp.value() );
	}

	return std::nullopt;
}

} // namespace I2C
