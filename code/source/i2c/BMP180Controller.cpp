#include "BMP180Controller.hpp"
#include "BusController.hpp"
#include <math/Math.hpp>

// C++
#include <array>
#include <cmath>
#include <chrono>
#include <utility>

// Debug
#include <print>
#include <format>
#include <iostream>

// https://os.mbed.com/users/onehorse/code/BMP180/file/06dc60296e6e/BMP180.h/

namespace pbl::i2c
{

namespace
{

// BMP180 Registers
constexpr std::uint8_t kBmp180WhoIam{ 0xD0 }; //!< 'WHO_AM_I' ID register (expected 0x55).
constexpr std::uint8_t kBmp180Reset{ 0xE0 }; //!< Soft reset command register.
constexpr std::uint8_t kBmp180Control{ 0xF4 }; //!< Measurement control register.
constexpr std::uint8_t kBmp180OutMsb{ 0xF6 }; //!< Data output - Most Significant Byte.
constexpr std::uint8_t kBmp180OutLsb{ 0xF7 }; //!< Data output - Least Significant Byte.
constexpr std::uint8_t kBmp180OutXlsb{ 0xF8 }; //!< Data output - Extension Byte (highest resolution).

// Calibration registers
constexpr std::uint8_t kBmp180CalibAc1{ 0xAA }; //!< Calibration data (AC1 value).
constexpr std::uint8_t kBmp180CalibAc2{ 0xAC }; //!< Calibration data (AC2 value).
constexpr std::uint8_t kBmp180CalibAc3{ 0xAE }; //!< Calibration data (AC3 value).
constexpr std::uint8_t kBmp180CalibAc4{ 0xB0 }; //!< Calibration data (AC4 value).
constexpr std::uint8_t kBmp180CalibAc5{ 0xB2 }; //!< Calibration data (AC5 value).
constexpr std::uint8_t kBmp180CalibAc6{ 0xB4 }; //!< Calibration data (AC6 value).
constexpr std::uint8_t kBmp180CalibB1{ 0xB6 }; //!< Calibration data (B1 value).
constexpr std::uint8_t kBmp180CalibB2{ 0xB8 }; //!< Calibration data (B2 value).
constexpr std::uint8_t kBmp180CalibMb{ 0xBA }; //!< Calibration data (MB value).
constexpr std::uint8_t kBmp180CalibMc{ 0xBC }; //!< Calibration data (MC value).
constexpr std::uint8_t kBmp180CalibMd{ 0xBE }; //!< Calibration data (MD value).

// BMP180 Commands
constexpr std::uint8_t kBmp180CmdTemp{ 0x2E }; //!< Start temperature measurement.
constexpr std::uint8_t kBmp180CmdPressUltraLow{ 0x34 }; //!< Pressure in ultra low power mode.
constexpr std::uint8_t kBmp180CmdPressStandard{ 0x74 }; //!< Pressure in standard mode.
constexpr std::uint8_t kBmp180CmdPressHighRes{ 0xB4 }; //!< Pressure in high-res mode.
constexpr std::uint8_t kBmp180CmdPressUltraHighRes{ 0xF4 }; //!< Pressure in ultra high-res mode.

// You need to use this structure in order to be able to figure out which command to use at what sampling parameter
constexpr std::array< std::pair< BMP180Controller::SamplingAccuracy, std::uint8_t >, 4 > kCmdLookupTable = {
	std::pair{ BMP180Controller::SamplingAccuracy::ULTRA_LOW_POWER, kBmp180CmdPressUltraLow },
	std::pair{ BMP180Controller::SamplingAccuracy::STANDARD, kBmp180CmdPressStandard },
	std::pair{ BMP180Controller::SamplingAccuracy::HIGH_RESOLUTION, kBmp180CmdPressHighRes },
	std::pair{ BMP180Controller::SamplingAccuracy::ULTRA_HIGH_RESOLUTION, kBmp180CmdPressUltraHighRes } };

constexpr std::uint8_t getBmp180CmdForMode( BMP180Controller::SamplingAccuracy mode )
{
	for( const auto& [ accuracy, cmd ] : kCmdLookupTable )
	{
		if( accuracy == mode )
		{
			return cmd;
		}
	}

	return {};
}

template < typename T >
T readCalibConst( BusController& busController, const std::uint8_t bmp180Addr, const std::uint8_t regAddr )
{
	std::uint8_t highByte{};
	std::uint8_t lowByte{};

	busController.read( bmp180Addr, regAddr, highByte );
	busController.read( bmp180Addr, regAddr + 1, lowByte );

	T value{};
	value = highByte << 8;
	value |= lowByte;

	return value;
}

} // namespace

struct BMP180Controller::CalibrationConstants
{
	/// Read calibration constants from BME180 device
	void read( BusController& busController, std::uint8_t address );

	std::int16_t ac1{};
	std::int16_t ac2{};
	std::int16_t ac3{};
	std::uint16_t ac4{};
	std::uint16_t ac5{};
	std::uint16_t ac6{};
	std::int16_t b1{};
	std::int16_t b2{};
	std::int16_t mb{};
	std::int16_t mc{};
	std::int16_t md{};
};

void BMP180Controller::CalibrationConstants::read( BusController& busController, std::uint8_t address )
{
	ac1 = readCalibConst< std::int16_t >( busController, address, kBmp180CalibAc1 );
	ac2 = readCalibConst< std::int16_t >( busController, address, kBmp180CalibAc2 );
	ac3 = readCalibConst< std::int16_t >( busController, address, kBmp180CalibAc3 );
	ac4 = readCalibConst< std::uint16_t >( busController, address, kBmp180CalibAc4 );
	ac5 = readCalibConst< std::uint16_t >( busController, address, kBmp180CalibAc5 );
	ac6 = readCalibConst< std::uint16_t >( busController, address, kBmp180CalibAc6 );
	b1 = readCalibConst< std::int16_t >( busController, address, kBmp180CalibB1 );
	b2 = readCalibConst< std::int16_t >( busController, address, kBmp180CalibB2 );
	mb = readCalibConst< std::int16_t >( busController, address, kBmp180CalibMb );
	mc = readCalibConst< std::int16_t >( busController, address, kBmp180CalibMc );
	md = readCalibConst< std::int16_t >( busController, address, kBmp180CalibMd );
}

BMP180Controller::BMP180Controller( BusController& busController, Address address, SamplingAccuracy sAccuracy )
	: ICBase{ busController, address }
	, m_samplingAccuracy{ sAccuracy }
	, m_constants{}
{
	m_constants->read( busController, static_cast< std::uint8_t >( address ) );
}

BMP180Controller::~BMP180Controller() = default;

auto BMP180Controller::getTrueTemperatureC() -> Result< float >
{
	using namespace std::chrono_literals;

	// Read uncompensated temperature (UT) value
	write( kBmp180Control, kBmp180CmdTemp ); // Start temperature measurement
	sleep( 4500us );

	// Read raw temperature measurement
	std::array< std::uint8_t, 2 > rawUT{};
	read( kBmp180OutMsb, rawUT.data(), rawUT.size() );

	std::int16_t UT = ( ( int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Calculate true temperature
	std::int64_t X1 = ( std::int64_t( UT - m_constants->ac6 ) * m_constants->ac5 ) / std::pow( 2.0, 15 );
	std::int64_t X2 = m_constants->mc * std::pow( 2.0, 11 ) / ( X1 + m_constants->md );
	std::int64_t B5 = X1 + X2;

	// Temperature calculated here will be in units of 0.1 deg C
	// TODO: There seems to be some type conversion issues here
	std::int64_t T = ( B5 + 8 ) / std::pow( 2.0, 4 );

	return static_cast< float >( T ) * 0.1f;
}

auto BMP180Controller::getTemperatureF() -> Result< float >
{
	const auto temp = getTrueTemperatureC();
	if( temp.has_value() )
	{
		return utils::celsiusToFahrenheit( temp.value() );
	}

	return std::unexpected( temp.error() );
}

auto BMP180Controller::getTruePressurePa() -> Result< float >
{
	using namespace std::chrono_literals;

	// Pressure resolution (is this correct!?)
	const std::uint8_t OSS = static_cast< std::uint8_t >( m_samplingAccuracy );

	// Start temperature measurement
	write( kBmp180Control, kBmp180CmdTemp );
	sleep( 4500us );

	// Read raw temperature measurement
	std::array< std::uint8_t, 2 > rawUT{};
	read( kBmp180OutMsb, rawUT.data(), rawUT.size() );

	std::int16_t UT = ( ( std::int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Start pressure measurement
	write( kBmp180Control, 0x34 | OSS << 6 ); // Configure pressure measurement for highest resolution

	sleep( std::chrono::milliseconds( static_cast< int >( ( 5.0f + 8.0f * OSS ) ) ) );

	// UP = ( MSB << 16 + LSB << 8 + XLSB ) >> ( 8 - oss );
	std::array< std::uint8_t, 3 > upRawData{};
	read( kBmp180OutMsb, upRawData.data(), upRawData.size() ); // Read raw ...

	std::int64_t UP = ( ( static_cast< std::int64_t >( upRawData[ 0 ] ) << 16 ) |
						( static_cast< std::int64_t >( upRawData[ 1 ] ) << 8 ) | upRawData[ 2 ] ) >>
					  ( 8 - OSS );

	// Temp related calculations
	std::int64_t X1 = ( std::int64_t( UT - m_constants->ac6 ) * m_constants->ac5 ) / std::pow( 2.0, 15 );
	std::int64_t X2 = m_constants->mc * std::pow( 2.0, 11 ) / ( X1 + m_constants->md );
	std::int64_t B5 = X1 + X2;

	// Calculate true pressure
	std::int64_t B6 = B5 - 4000;
	X1 = ( m_constants->b2 * ( B6 * B6 / std::pow( 2, 12 ) ) / std::pow( 2, 11 ) );
	X2 = m_constants->ac2 * B6 / std::pow( 2, 11 );
	std::int64_t X3 = X1 + X2;
	std::int64_t B3 = ( ( ( m_constants->ac1 * 4 + X3 ) << OSS ) + 2 ) / 4;

	std::println( "B6 {}", B6 );
	std::println( "X1 {}", X1 );
	std::println( "X2 {}", X2 );
	std::println( "X3 {}", X3 );
	std::println( "B3 {}", B3 );

	X1 = m_constants->ac3 * B6 / std::pow( 2, 13 );
	X2 = ( m_constants->b1 * ( B6 * B6 / std::pow( 2, 12 ) ) ) / std::pow( 2, 16 );
	X3 = ( ( X1 + X2 ) + 2 ) / std::pow( 2, 2 );
	std::int64_t B4 = m_constants->ac4 * static_cast< std::int64_t >( X3 + 32768 ) / std::pow( 2, 15 );
	std::int64_t B7 = ( static_cast< std::int64_t >( UP ) - B3 ) * ( 5000 >> OSS );

	std::println( "X1 {}", X1 );
	std::println( "X2 {}", X2 );
	std::println( "X3 {}", X3 );
	std::println( "B4 {}", B4 );
	std::println( "B7 {}", B7 );

	std::int32_t p{};
	if( B7 < 0x80000000 )
	{
		p = ( B7 * 2 ) / B4;
	}
	else
	{
		p = ( B7 / B4 ) * 2;
	}

	// TODO (AK): Debug this part, there is an error!
	X1 = ( p / std::pow( 2, 8 ) * ( p / std::pow( 2, 8 ) ) );
	std::println( "X1 {}", X1 );

	X1 = ( X1 * 3038 ) / std::pow( 2, 16 );
	std::println( "X1 {}", X1 );

	X2 = ( -7357 * p ) / std::pow( 2, 16 );
	std::println( "X2 {}", X2 );

	p = p + ( X1 + X2 + 3791 ) / std::pow( 2, 4 ); // Pressure in units of Pa.

	std::println( "P {}", p );

	// It could be that the pressure and accuracy combination configuration is wrong!
#warning "This needs investigation the measured pressure appears to be 10 times lower than expected"

	return static_cast< float >( p ) * 10.0f;
}

// TODO: Consider moving this function to utils/Math.hpp
auto BMP180Controller::getAbsoluteAltitude() -> Result< float >
{
	// H = 44330 * [1 - (P/p0)^(1/5.255) ]
	constexpr float exponent = 1.0f / 5.255f;
	constexpr float pressureAtSeaLevel = 101325.0f; // Pa

	if( auto truePressure = getTruePressurePa(); truePressure.has_value() )
	{
		float pd = truePressure.value() / pressureAtSeaLevel;
		float r = 1.0f - std::pow( pd, exponent );
		return 44330.0f * r;
	}

	return std::unexpected( utils::ErrorCode::FAILED_TO_READ );
}

} // namespace pbl::i2c
