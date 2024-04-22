#include "MPU6050Controller.hpp"
#include "MPU6050Definitions.hpp"
#include "BusController.hpp"

// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/

namespace pbl::i2c
{

MPU6050Controller::MPU6050Controller( BusController& busController, Address address ) noexcept
	: ICBase{ busController, address }
{ }

bool MPU6050Controller::reset()
{
	bool rslt{ true };
	rslt = write( MPU6050::kPowerManagement1Register, 0x00 );
	// m_busController.write( m_icAddress, MPU6050::kPowerManagement2Register, 0x00 );

	return rslt;
}

bool MPU6050Controller::configure()
{
	bool rslt{ true };

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

	return rslt;
}

struct CalibrationConstants
{
	// Acceleration
	double AccErrorX{};
	double AccErrorY{};

	// Gyroscope
	double GyroErrorX{};
	double GyroErrorY{};
	double GyroErrorZ{};
};

void MPU6050Controller::calculateImuError()
{
	// We can call this function in the setup section to calculate
	// the accelerometer and gyro data error. From here we will get
	// the error values used in the above equations printed on the
	// Serial Monitor.

	// Note that we should place the IMU flat in order to get the
	// proper values, so that we then can estimate correct values.

	// TODO: Think about flashing/embedding the calibration constants
	// maybe we could provide them as arguments?

	std::size_t count{};

	double AccErrorX{};
	double AccErrorY{};

	double AccX{};
	double AccY{};
	double AccZ{};

	// Read accelerometer values for kAccelCalibReadIterations times
	while( count < kAccelCalibReadIterations )
	{
		// Use MPU6050::kAccelXOutHRegister
		// Wire.beginTransmission( MPU );
		// Wire.write( 0x3B );
		// Wire.endTransmission( false );
		// Wire.requestFrom( MPU, 6, true );

		// AccX = ( Wire.read() << 8 | Wire.read() ) / 16384.0;
		// AccY = ( Wire.read() << 8 | Wire.read() ) / 16384.0;
		// AccZ = ( Wire.read() << 8 | Wire.read() ) / 16384.0;

		// // Sum all readings
		// AccErrorX =
		// 	AccErrorX +
		// 	( ( std::atan( ( AccY ) / std::sqrt( std::pow( ( AccX ), 2 ) + std::pow( ( AccZ ), 2 ) ) ) * 180 / PI ) );
		// AccErrorY = AccErrorY +
		// 			( ( std::atan( -1 * ( AccX ) / std::sqrt( std::pow( ( AccY ), 2 ) + std::pow( ( AccZ ), 2 ) ) ) *
		// 				180 / PI ) );

		++count;
	}

	AccErrorX = AccErrorX / static_cast< double >( kAccelCalibReadIterations );
	AccErrorY = AccErrorY / static_cast< double >( kAccelCalibReadIterations );

	count = {};

	double GyroErrorX{};
	double GyroErrorY{};
	double GyroErrorZ{};

	double GyroX{};
	double GyroY{};
	double GyroZ{};

	// Read gyro values kGyroCalibReadIterations times
	while( count < kGyroCalibReadIterations )
	{
		// Use MPU6050::kGyroXOutHRegister
		// Wire.beginTransmission( MPU );
		// Wire.write( 0x43 );
		// Wire.endTransmission( false );
		// Wire.requestFrom( MPU, 6, true );

		// GyroX = Wire.read() << 8 | Wire.read();
		// GyroY = Wire.read() << 8 | Wire.read();
		// GyroZ = Wire.read() << 8 | Wire.read();

		// Sum all readings
		// GyroErrorX = GyroErrorX + ( GyroX / 131.0 );
		// GyroErrorY = GyroErrorY + ( GyroY / 131.0 );
		// GyroErrorZ = GyroErrorZ + ( GyroZ / 131.0 );

		++count;
	}

	// Divide the sum by 200 to get the error value
	GyroErrorX = GyroErrorX / static_cast< double >( kGyroCalibReadIterations );
	GyroErrorY = GyroErrorY / static_cast< double >( kGyroCalibReadIterations );
	GyroErrorZ = GyroErrorZ / static_cast< double >( kGyroCalibReadIterations );
}

math::Vector3f MPU6050Controller::angles()
{
	// TODO (AK)
	return {};
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
