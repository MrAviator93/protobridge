#include "BMP180Controller.hpp"
#include "BusController.hpp"

#include <math/Math.hpp>
#include <math/Dynamics.hpp>

// C++
#include <array>
#include <cmath>
#include <chrono>
#include <utility>
#include <algorithm>

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
constexpr std::uint8_t kBmp180CalibAc1Lsb{ 0xAB }; //!< Calibration data (AC1 value).
constexpr std::uint8_t kBmp180CalibAc2{ 0xAC }; //!< Calibration data (AC2 value).
constexpr std::uint8_t kBmp180CalibAc2Lsb{ 0xAD }; //!< Calibration data (AC2 value).
constexpr std::uint8_t kBmp180CalibAc3{ 0xAE }; //!< Calibration data (AC3 value).
constexpr std::uint8_t kBmp180CalibAc3Lsb{ 0xAF }; //!< Calibration data (AC3 value).
constexpr std::uint8_t kBmp180CalibAc4{ 0xB0 }; //!< Calibration data (AC4 value).
constexpr std::uint8_t kBmp180CalibAc4Lsb{ 0xB1 }; //!< Calibration data (AC4 value).
constexpr std::uint8_t kBmp180CalibAc5{ 0xB2 }; //!< Calibration data (AC5 value).
constexpr std::uint8_t kBmp180CalibAc5Lsb{ 0xB3 }; //!< Calibration data (AC5 value).
constexpr std::uint8_t kBmp180CalibAc6{ 0xB4 }; //!< Calibration data (AC6 value).
constexpr std::uint8_t kBmp180CalibAc6Lsb{ 0xB5 }; //!< Calibration data (AC6 value).
constexpr std::uint8_t kBmp180CalibB1{ 0xB6 }; //!< Calibration data (B1 value).
constexpr std::uint8_t kBmp180CalibB1Lsb{ 0xB7 }; //!< Calibration data (B1 value).
constexpr std::uint8_t kBmp180CalibB2{ 0xB8 }; //!< Calibration data (B2 value).
constexpr std::uint8_t kBmp180CalibB2Lsb{ 0xB9 }; //!< Calibration data (B2 value).
constexpr std::uint8_t kBmp180CalibMb{ 0xBA }; //!< Calibration data (MB value).
constexpr std::uint8_t kBmp180CalibMbLsb{ 0xBB }; //!< Calibration data (MB value).
constexpr std::uint8_t kBmp180CalibMc{ 0xBC }; //!< Calibration data (MC value).
constexpr std::uint8_t kBmp180CalibMcLsb{ 0xBD }; //!< Calibration data (MC value).
constexpr std::uint8_t kBmp180CalibMd{ 0xBE }; //!< Calibration data (MD value).
constexpr std::uint8_t kBmp180CalibMdLsb{ 0xBF }; //!< Calibration data (MD value).

// BMP180 Commands
constexpr std::uint8_t kBmp180CmdTemp{ 0x2E }; //!< Start temperature measurement.
constexpr std::uint8_t kBmp180CmdPressUltraLow{ 0x34 }; //!< Pressure in ultra low power mode.
constexpr std::uint8_t kBmp180CmdPressStandard{ 0x74 }; //!< Pressure in standard mode.
constexpr std::uint8_t kBmp180CmdPressHighRes{ 0xB4 }; //!< Pressure in high-res mode.
constexpr std::uint8_t kBmp180CmdPressUltraHighRes{ 0xF4 }; //!< Pressure in ultra high-res mode.

// You need to use this structure in order to be able to figure out which command to use at what sampling parameter
constexpr std::array kCmdLookupTable = {
	std::pair{ BMP180Controller::SamplingAccuracy::ULTRA_LOW_POWER, kBmp180CmdPressUltraLow },
	std::pair{ BMP180Controller::SamplingAccuracy::STANDARD, kBmp180CmdPressStandard },
	std::pair{ BMP180Controller::SamplingAccuracy::HIGH_RESOLUTION, kBmp180CmdPressHighRes },
	std::pair{ BMP180Controller::SamplingAccuracy::ULTRA_HIGH_RESOLUTION, kBmp180CmdPressUltraHighRes } };

constexpr std::uint8_t commandForMode( BMP180Controller::SamplingAccuracy mode )
{
	const auto it = std::ranges::find_if( kCmdLookupTable, [ mode ]( const auto& v ) { return v.first == mode; } );
	if( it != kCmdLookupTable.end() )
	{
		return it->second;
	}

	// Just return the standard pressure setting
	return kBmp180CmdPressStandard;
}

template < typename T >
bool readCalibConst( T& value,
					 BusController& busController,
					 const std::uint8_t bmp180Addr,
					 const std::uint8_t msb,
					 const std::uint8_t lsb )
{
	std::uint8_t highByte{};
	std::uint8_t lowByte{};

	if( !busController.read( bmp180Addr, msb, highByte ) ) [[unlikely]]
	{
		return false;
	}

	if( !busController.read( bmp180Addr, lsb, lowByte ) ) [[unlikely]]
	{
		return false;
	}

	value = highByte << 8 | lowByte;

	return true;
}

} // namespace

struct v1::BMP180Controller::CalibrationConstants
{
	/// Read calibration constants from BME180 device
	bool read( BusController& busController, std::uint8_t address );

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

bool v1::BMP180Controller::CalibrationConstants::read( BusController& busController, std::uint8_t address )
{
	bool rslt{ true };
	rslt |= readCalibConst( ac1, busController, address, kBmp180CalibAc1, kBmp180CalibAc1Lsb );
	rslt |= readCalibConst( ac2, busController, address, kBmp180CalibAc2, kBmp180CalibAc2Lsb );
	rslt |= readCalibConst( ac3, busController, address, kBmp180CalibAc3, kBmp180CalibAc3Lsb );
	rslt |= readCalibConst( ac4, busController, address, kBmp180CalibAc4, kBmp180CalibAc4Lsb );
	rslt |= readCalibConst( ac5, busController, address, kBmp180CalibAc5, kBmp180CalibAc5Lsb );
	rslt |= readCalibConst( ac6, busController, address, kBmp180CalibAc6, kBmp180CalibAc6Lsb );
	rslt |= readCalibConst( b1, busController, address, kBmp180CalibB1, kBmp180CalibB1Lsb );
	rslt |= readCalibConst( b2, busController, address, kBmp180CalibB2, kBmp180CalibB2Lsb );
	rslt |= readCalibConst( mb, busController, address, kBmp180CalibMb, kBmp180CalibMbLsb );
	rslt |= readCalibConst( mc, busController, address, kBmp180CalibMc, kBmp180CalibMcLsb );
	rslt |= readCalibConst( md, busController, address, kBmp180CalibMd, kBmp180CalibMdLsb );
	return rslt;
}

v1::BMP180Controller::BMP180Controller( BusController& busController, Address address, SamplingAccuracy sAccuracy )
	: ICBase{ busController, address }
	, m_samplingAccuracy{ sAccuracy }
	, m_constants{}
{
	if( !m_constants->read( busController, static_cast< std::uint8_t >( address ) ) ) [[unlikely]]
	{
		// TODO: We need to do something about this ...
		// return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}
}

v1::BMP180Controller::~BMP180Controller() = default;

auto v1::BMP180Controller::getTrueTemperatureC() -> Result< float >
{
	using namespace std::chrono_literals;

	// Read uncompensated temperature (UT) value
	if( !write( kBmp180Control, kBmp180CmdTemp ) ) [[unlikely]] // Start temperature measurement
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	sleep( 4500us );

	// Read raw temperature measurement
	std::array< std::uint8_t, 2 > rawUT{};
	if( !read( kBmp180OutMsb, rawUT.data(), rawUT.size() ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	const std::int16_t UT = ( ( int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Calculate true temperature
	const std::int64_t X1 = ( std::int64_t( UT - m_constants->ac6 ) * m_constants->ac5 ) / 32768;
	const std::int64_t X2 = m_constants->mc * 2048 / ( X1 + m_constants->md );
	const std::int64_t B5 = X1 + X2;

	// Temperature calculated here will be in units of 0.1 deg C
	const std::int64_t T = ( B5 + 8 ) / 16;

	return static_cast< float >( T ) * 0.1f;
}

auto v1::BMP180Controller::getTemperatureF() -> Result< float >
{
	return getTrueTemperatureC().transform( math::CelsiusToFarenheit< float >{} );
}

auto v1::BMP180Controller::getTruePressurePa() -> Result< float >
{
	using namespace std::chrono_literals;

	// Pressure resolution (is this correct!?)
	const std::uint8_t OSS = static_cast< std::uint8_t >( m_samplingAccuracy );

	// Start temperature measurement
	if( !write( kBmp180Control, kBmp180CmdTemp ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	sleep( 4500us );

	// Read uncompensated temperature value
	std::array< std::uint8_t, 2 > rawUT{};
	if( !read( kBmp180OutMsb, rawUT.data(), rawUT.size() ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	const std::int16_t UT = ( ( std::int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Start pressure measurement
	const std::uint8_t cmdOss = commandForMode( m_samplingAccuracy );
	if( !write( kBmp180Control, cmdOss ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	sleep( std::chrono::milliseconds( static_cast< int >( ( 5.0f + 8.0f * OSS ) ) ) );

	// Read uncompensated pressure value
	// UP = ( MSB << 16 + LSB << 8 + XLSB ) >> ( 8 - oss );
	std::array< std::uint8_t, 3 > upRawData{};
	if( !read( kBmp180OutMsb, upRawData.data(), upRawData.size() ) ) [[unlikely]]
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	// + or | here?
	const std::int64_t UP = ( ( static_cast< std::int64_t >( upRawData[ 0 ] ) << 16 ) +
							  ( static_cast< std::int64_t >( upRawData[ 1 ] ) << 8 ) + upRawData[ 2 ] ) >>
							( 8 - OSS );

	// const std::int64_t A1 = static_cast< std::int64_t >( upRawData[ 0 ] );
	// const std::int64_t A2 = static_cast< std::int64_t >( upRawData[ 1 ] );
	// const std::int64_t A3 = static_cast< std::int64_t >( upRawData[ 2 ] );
	// const std::int64_t UP = (A1 << 16 | A2 << 8 | A3) >> (8 - OSS);

	// Calculate true temperature (correct - validated against real temperature calculation)
	std::int64_t X1 = ( std::int64_t( UT - m_constants->ac6 ) * m_constants->ac5 ) / 32768;
	std::int64_t X2 = m_constants->mc * 2048 / ( X1 + m_constants->md );
	std::int64_t B5 = X1 + X2;

	// Calculate true pressure
	std::int64_t B6 = B5 - 4000;
	X1 = ( m_constants->b2 * ( B6 * B6 / 4096 ) / 2048 );
	X2 = m_constants->ac2 * B6 / 2048;

	std::int64_t X3 = X1 + X2;
	std::int64_t B3 = ( ( ( m_constants->ac1 * 4 + X3 ) << OSS ) + 2 ) / 4;

	X1 = m_constants->ac3 * B6 / 8192;
	X2 = ( m_constants->b1 * ( B6 * B6 / 4096 ) ) / 65536;
	X3 = ( ( X1 + X2 ) + 2 ) / 4;
	const std::uint64_t B4 = ( m_constants->ac4 * std::uint64_t( X3 + 32768 ) / 32768 );
	const std::uint64_t B7 = ( std::uint64_t( UP ) - B3 ) * ( 50000 >> OSS );

	std::int32_t p{};
	if( B7 < 0x80000000 )
	{
		p = ( ( B7 * 2 ) / B4 );
	}
	else
	{
		p = ( B7 / B4 ) * 2;
	}

	X1 = ( p / 256 * ( p / 256 ) );
	X1 = ( X1 * 3038 ) / 65536;
	X2 = ( -7357 * p ) / 65536;
	p = p + ( X1 + X2 + 3791 ) / 16; // Pressure in units of Pa.

	return static_cast< float >( p );
}

auto v1::BMP180Controller::getAbsoluteAltitude( float localPressure ) -> Result< float >
{
	const auto transform = [ = ]( float pressurePa ) { return math::pressureToAltitude( pressurePa, localPressure ); };
	return getTruePressurePa().transform( transform );
}

} // namespace pbl::i2c
