#include "MPU6050Controller.hpp"
#include "MPU6050Definitions.hpp"
#include "BusController.hpp"

// C++
#include <bitset>

// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/

namespace pbl::i2c
{

v1::MPU6050Controller::MPU6050Controller( BusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
{ }

auto v1::MPU6050Controller::setPowerMode( PowerMode mode ) -> Result< void >
{
	std::uint8_t config{};
	if( !read( MPU6050::kPowerManagement1Register, config ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	std::bitset< 8 > bits{ config };
	bits.set( 6, mode == PowerMode::SLEEP ); // Bit 6 controls SLEEP

	if( !write( MPU6050::kPowerManagement1Register, static_cast< std::uint8_t >( bits.to_ulong() ) ) )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::MPU6050Controller::configureScales( Scale accelScale, Scale gyroScale ) -> Result< void >
{
	using namespace MPU6050;

	std::uint8_t accelConfig{};
	switch( accelScale )
	{
		case Scale::ACCEL_2G: accelConfig = static_cast< uint8_t >( AccelSensitivity::G_2 ); break;
		case Scale::ACCEL_4G: accelConfig = static_cast< uint8_t >( AccelSensitivity::G_4 ); break;
		case Scale::ACCEL_8G: accelConfig = static_cast< uint8_t >( AccelSensitivity::G_8 ); break;
		case Scale::ACCEL_16G: accelConfig = static_cast< uint8_t >( AccelSensitivity::G_16 ); break;
		default: return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
	}

	uint8_t gyroConfig{};
	switch( gyroScale )
	{
		case Scale::GYRO_250DPS: gyroConfig = static_cast< uint8_t >( GyroSensitivity::DPS_250 ); break;
		case Scale::GYRO_500DPS: gyroConfig = static_cast< uint8_t >( GyroSensitivity::DPS_500 ); break;
		case Scale::GYRO_1000DPS: gyroConfig = static_cast< uint8_t >( GyroSensitivity::DPS_1000 ); break;
		case Scale::GYRO_2000DPS: gyroConfig = static_cast< uint8_t >( GyroSensitivity::DPS_2000 ); break;
		default: return utils::MakeError( utils::ErrorCode::INVALID_ARGUMENT );
	}

	const bool success = write( kAccelConfigRegister, accelConfig ) && write( kGyroConfigRegister, gyroConfig );
	if( !success )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_WRITE );
	}

	return utils::MakeSuccess();
}

auto v1::MPU6050Controller::reset() -> Result< void >
{
	// bool rslt{ true };
	// rslt = write( MPU6050::kPowerManagement1Register, 0x00 );
	// m_busController.write( m_icAddress, MPU6050::kPowerManagement2Register, 0x00 );

	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

auto v1::MPU6050Controller::configure() -> Result< void >
{
	// bool rslt{ true };

	// reset();

	// // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
	// // 0x10 sets the register bits as 00010000 (+/- 8g full scale range)
	// m_busController.write(
	// 	m_icAddress, MPU6050::kAccelConfigRegister, static_cast< uint8_t >( MPU6050::AccelSensitivity::G_8 ) );

	// // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
	// // 0x10 sets the register bits as 00010000 (1000deg/s full scale)
	// m_busController.write(
	// 	m_icAddress, MPU6050::kGyroConfigRegister, static_cast< uint8_t >( MPU6050::GyroSensitivity::DPS_1000 ) );

	// m_busController.sleep( std::chrono::milliseconds( 20 ) );

	// // Call this function if you need to get the IMU error values for your module
	// calculateImuError();

	// m_busController.sleep( std::chrono::milliseconds( 20 ) );

	return utils::MakeError( utils::Error::NOT_IMPLEMENTED );
}

auto v1::MPU6050Controller::calculateImuError() -> Result< void >
{
	// We can call this function in the setup section to calculate
	// the accelerometer and gyro data error. From here we will get
	// the error values used in the above equations printed on the
	// Serial Monitor.

	// Note that we should place the IMU flat in order to get the
	// proper values, so that we then can estimate correct values.

	// TODO: Think about flashing/embedding the calibration constants
	// maybe we could provide them as arguments?

	using namespace MPU6050;

	std::size_t count{};

	double accErrorX{ 0.0 };
	double accErrorY{ 0.0 };

	// Read accelerometer data
	for( count = 0; count < kAccelCalibReadIterations; ++count )
	{
		std::array< uint8_t, 6 > raw{};
		if( read( kAccelXOutHRegister, raw.data(), raw.size() ) < 6 )
		{
			return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
		}

		const int16_t rawX = static_cast< int16_t >( raw[ 0 ] << 8 | raw[ 1 ] );
		const int16_t rawY = static_cast< int16_t >( raw[ 2 ] << 8 | raw[ 3 ] );
		const int16_t rawZ = static_cast< int16_t >( raw[ 4 ] << 8 | raw[ 5 ] );

		const double accX = static_cast< double >( rawX ) / 16384.0;
		const double accY = static_cast< double >( rawY ) / 16384.0;
		const double accZ = static_cast< double >( rawZ ) / 16384.0;

		// Calculate angle errors
		accErrorX += std::atan2( accY, std::sqrt( accX * accX + accZ * accZ ) ) * 180.0 / M_PI;
		accErrorY += std::atan2( -accX, std::sqrt( accY * accY + accZ * accZ ) ) * 180.0 / M_PI;
	}

	accErrorX /= static_cast< double >( kAccelCalibReadIterations );
	accErrorY /= static_cast< double >( kAccelCalibReadIterations );

	// --- Gyroscope Calibration ---
	count = 0;

	double gyroErrorX{ 0.0 };
	double gyroErrorY{ 0.0 };
	double gyroErrorZ{ 0.0 };

	for( count = 0; count < kGyroCalibReadIterations; ++count )
	{
		std::array< uint8_t, 6 > raw{};
		if( read( kGyroXOutHRegister, raw.data(), raw.size() ) < 6 )
			return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );

		const int16_t rawX = static_cast< int16_t >( raw[ 0 ] << 8 | raw[ 1 ] );
		const int16_t rawY = static_cast< int16_t >( raw[ 2 ] << 8 | raw[ 3 ] );
		const int16_t rawZ = static_cast< int16_t >( raw[ 4 ] << 8 | raw[ 5 ] );

		gyroErrorX += static_cast< double >( rawX ) / 131.0;
		gyroErrorY += static_cast< double >( rawY ) / 131.0;
		gyroErrorZ += static_cast< double >( rawZ ) / 131.0;
	}

	gyroErrorX /= static_cast< double >( kGyroCalibReadIterations );
	gyroErrorY /= static_cast< double >( kGyroCalibReadIterations );
	gyroErrorZ /= static_cast< double >( kGyroCalibReadIterations );

	// You could store these for compensation later, e.g.:
	// this->m_calibration = { accErrorX, accErrorY, gyroErrorX, gyroErrorY, gyroErrorZ };

	return utils::MakeSuccess();
}

auto v1::MPU6050Controller::angles() -> Result< math::Vector3f >
{
	// 1. Read raw accelerometer values
	std::array< uint8_t, 6 > accData{};
	if( read( MPU6050::kAccelXOutHRegister, accData.data(), accData.size() ) != 6 )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	auto readWord = []( uint8_t high, uint8_t low ) { return static_cast< int16_t >( ( high << 8 ) | low ); };

	const float accX = static_cast< float >( readWord( accData[ 0 ], accData[ 1 ] ) ) / 16384.0f;
	const float accY = static_cast< float >( readWord( accData[ 2 ], accData[ 3 ] ) ) / 16384.0f;
	const float accZ = static_cast< float >( readWord( accData[ 4 ], accData[ 5 ] ) ) / 16384.0f;

	const float roll =
		std::atan2( accY, std::sqrt( accX * accX + accZ * accZ ) ) * 180.0f / static_cast< float >( M_PI );
	const float pitch =
		std::atan2( -accX, std::sqrt( accY * accY + accZ * accZ ) ) * 180.0f / static_cast< float >( M_PI );

	// 2. Read raw gyroscope values
	std::array< uint8_t, 6 > gyroData{};
	if( read( MPU6050::kGyroXOutHRegister, gyroData.data(), gyroData.size() ) != 6 )
	{
		return utils::MakeError( utils::ErrorCode::FAILED_TO_READ );
	}

	const float gyroZ = static_cast< float >( readWord( gyroData[ 4 ], gyroData[ 5 ] ) ) / 131.0f;

	// TODO: Add elapsed time delta for true yaw integration
	// For now, just return raw gyro Z
	const float yaw = gyroZ;

	return utils::MakeSuccess( math::Vector3f{ roll, pitch, yaw } );
}

// A way to extract the angles
// void loop() {
//
//   // === Read acceleromter data === //
//   Wire.beginTransmission(MPU);
//   Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
//
//   // For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
//   AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
//   AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
//   AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
//
//   // Calculating Roll and Pitch from the accelerometer data
//   accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
//   accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
//
//   // === Read gyroscope data === //
//   previousTime = currentTime;        // Previous time is stored before the actual time read
//   currentTime = millis();            // Current time actual time read
//   elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
//   Wire.beginTransmission(MPU);
//   Wire.write(0x43); // Gyro data first register address 0x43
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
//   GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
//   GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
//   GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
//
//   // Correct the outputs with the calculated error values
//   GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
//   GyroY = GyroY - 2; // GyroErrorY ~(2)
//   GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
//
//   // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
//   gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
//   gyroAngleY = gyroAngleY + GyroY * elapsedTime;
//   yaw =  yaw + GyroZ * elapsedTime;
//
//   // Complementary filter - combine accelometer and gyro angle values
//   roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
//   pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

//   // Print the values on the serial monitor
//   Serial.print(roll);
//   Serial.print("/");
//   Serial.print(pitch);
//   Serial.print("/");
//   Serial.println(yaw);
// }

// Idea applies for x, y and z axis ...
// float MPU6050Controller::x() const
// {
// 	// Here the logic must be a bit more complicated,
// 	// we need to use the timestamp to determine the last read
// 	// time, if last read time exceeds some threshold,
// 	// then we need to repeat read.
// 	// Or maybe have a single function to read all three
// 	// values at once ?
// 	return m_x;
// }

} // namespace pbl::i2c
