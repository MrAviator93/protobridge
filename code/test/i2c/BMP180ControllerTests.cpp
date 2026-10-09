// PBL
#include <i2c/BMP180Controller.hpp>

// Third Party
#include <gtest/gtest.h>

// C++
#include <array>
#include <vector>

namespace pbl::i2c
{
namespace
{

class BMP180Transport final : public Transport
{
public:
	bool failCalibration{};
	bool failWrite{};
	bool failMeasurement{};
	std::vector< std::vector< std::uint8_t > > writes;

	utils::Result< void > write( Address address, std::span< const std::uint8_t > data ) override
	{
		EXPECT_EQ( address, 0x77 );
		writes.emplace_back( data.begin(), data.end() );
		if( failWrite ) return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
		return utils::MakeSuccess();
	}

	utils::Result< void > read( Address, std::span< std::uint8_t > ) override
	{
		ADD_FAILURE() << "Unexpected raw read";
		return utils::MakeError( utils::ErrorCode::UNSUPPORTED_OPERATION );
	}

	utils::Result< void >
	writeRead( Address address, std::span< const std::uint8_t > request, std::span< std::uint8_t > response ) override
	{
		EXPECT_EQ( address, 0x77 );
		EXPECT_EQ( request.size(), 1 );
		if( request.size() != 1 ) return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
		// Fixed calibration and raw values exercise compensation independently of hardware.
		const std::array< std::int32_t, 11 > calibration{
			408, -72, -14383, 32741, 32757, 23153, 6190, 4, -32768, -8711, 2868 };
		const auto reg = request[ 0 ];
		if( reg >= 0xAA && reg <= 0xBF )
		{
			if( failCalibration ) return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
			EXPECT_EQ( response.size(), 1 );
			if( response.size() != 1 ) return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
			const auto offset = reg - 0xAA;
			const auto word = static_cast< std::uint16_t >( calibration[ offset / 2 ] );
			response[ 0 ] = static_cast< std::uint8_t >( offset % 2 ? word : word >> 8 );
		}
		else if( reg == 0xF6 )
		{
			if( failMeasurement ) return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
			if( response.size() == 2 )
			{
				response[ 0 ] = 0x6C;
				response[ 1 ] = 0xFA;
			}
			else if( response.size() == 3 )
			{
				response[ 0 ] = 0x5D;
				response[ 1 ] = 0x23;
				response[ 2 ] = 0x00;
			}
			else
				ADD_FAILURE() << "Unexpected response size";
		}
		else
			ADD_FAILURE() << "Unexpected register";
		return utils::MakeSuccess();
	}
};

TEST( BMP180ControllerTest, CompensatesTemperatureAndSendsConversionCommand )
{
	// Arrange
	BMP180Transport transport;
	BMP180Controller sensor{ transport };

	// Act
	const auto result = sensor.getTrueTemperatureC();
	const auto transportWritesSizeResult = transport.writes.size();

	// Assert
	ASSERT_TRUE( result );
	EXPECT_FLOAT_EQ( *result, 15.0f );
	ASSERT_EQ( transportWritesSizeResult, 1 );
	EXPECT_EQ( transport.writes.front(), ( std::vector< std::uint8_t >{ 0xF4, 0x2E } ) );
}

TEST( BMP180ControllerTest, CompensatesPressureAtLowestOversampling )
{
	// Arrange
	BMP180Transport transport;
	BMP180Controller sensor{ transport, BMP180Controller::DEFAULT, BMP180Controller::ULTRA_LOW_POWER };

	// Act
	const auto result = sensor.getTruePressurePa();
	const auto transportWritesSizeResult = transport.writes.size();

	// Assert
	ASSERT_TRUE( result );
	EXPECT_NEAR( *result, 69964.0f, 2.0f );
	ASSERT_EQ( transportWritesSizeResult, 2 );
	EXPECT_EQ( transport.writes.back(), ( std::vector< std::uint8_t >{ 0xF4, 0x34 } ) );
}

TEST( BMP180ControllerTest, RejectsUnavailableCalibration )
{
	// Arrange
	BMP180Transport transport;
	transport.failCalibration = true;
	BMP180Controller sensor{ transport };

	// Act
	const auto sensorGetTrueTemperatureCResult = sensor.getTrueTemperatureC();
	const auto sensorGetTruePressurePaResult = sensor.getTruePressurePa();
	const auto transportWritesEmptyResult = transport.writes.empty();

	// Assert
	EXPECT_FALSE( sensorGetTrueTemperatureCResult );
	EXPECT_FALSE( sensorGetTruePressurePaResult );
	EXPECT_TRUE( transportWritesEmptyResult );
}

TEST( BMP180ControllerTest, ReportsFailedConversionWrite )
{
	// Arrange
	BMP180Transport transport;
	BMP180Controller sensor{ transport };
	transport.failWrite = true;

	// Act
	const auto result = sensor.getTrueTemperatureC();

	// Assert
	ASSERT_FALSE( result );
	EXPECT_EQ( static_cast< utils::ErrorCode >( result.error() ), utils::ErrorCode::FAILED_TO_WRITE );
}

TEST( BMP180ControllerTest, ReportsFailedMeasurementRead )
{
	// Arrange
	BMP180Transport transport;
	BMP180Controller sensor{ transport };
	transport.failMeasurement = true;

	// Act
	const auto result = sensor.getTrueTemperatureC();
	const auto sensorGetTruePressurePaResult = sensor.getTruePressurePa();

	// Assert
	ASSERT_FALSE( result );
	EXPECT_EQ( static_cast< utils::ErrorCode >( result.error() ), utils::ErrorCode::FAILED_TO_READ );
	EXPECT_FALSE( sensorGetTruePressurePaResult );
}

} // namespace
} // namespace pbl::i2c
