/**
 * @author MrAviator93
 * @date 4 November 2025
 * @brief Declaration of MPU9250Controller class for managing the MPU9250 sensor.
 *
 * For license details, see the LICENSE file in the project root.
 * 
 * Provides an interface to communicate with and control the MPU9250 sensor. This includes retrieving 
 * accelerometer, gyroscope, and magnetometer readings, setting up the power and operational modes, 
 * and configuring sensor settings.
 * 
 * For license details, see the LICENSE file in the project root.
 */

#ifndef PBL_I2C_MPU9250_CONTROLLER_HPP__
#define PBL_I2C_MPU9250_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <cmath>
#include <optional>
#include <expected>

namespace pbl::i2c
{

inline namespace v1
{

/**
 * @class MPU9250Controller
 * @brief Controller interface for the MPU9250 9-axis motion sensor.
 *
 * The MPU9250Controller class provides an abstraction layer for managing the MPU9250, a 9-axis sensor
 * that combines a 3-axis gyroscope, a 3-axis accelerometer, and a 3-axis magnetometer. The class allows 
 * for retrieving sensor readings, configuring power modes, and setting measurement scales and configurations.
 *
 * Key functionalities included in this class:
 *  - Retrieve acceleration, angular velocity, and magnetic field data.
 *  - Configure sensor settings, including full-scale ranges and sampling rates.
 *  - Set power modes for efficient energy management.
 *
 * Example usage:
 * @code
 * BusController busController;
 * MPU9250Controller mpu9250(busController, MPU9250Controller::DEFAULT);
 * 
 * auto accelData = mpu9250.getAcceleration();
 * auto gyroData = mpu9250.getGyroscope();
 * auto magData = mpu9250.getMagnetometer();
 * @endcode
 *
 * @note This class requires an initialized I2C bus and proper sensor setup for accurate readings.
 */
class MPU9250Controller final : public ICBase, public utils::Counter< MPU9250Controller >
{
	static constexpr std::size_t kAccelCalibReadIterations{ 1'000 };
	static constexpr std::size_t kGyroCalibReadIterations{ 1'000 };

public:
	template < typename T >
	using Result = utils::Result< T >;

	enum class Address : std::uint8_t
	{
		DEFAULT = 0x68,
		ALTERNATIVE = 0x69 // AD0 pin is set to logic level high
	};

	enum class PowerMode : std::uint8_t
	{
		NORMAL,
		SLEEP
	};

	enum class Scale : std::uint8_t
	{
		ACCEL_2G,
		ACCEL_4G,
		ACCEL_8G,
		ACCEL_16G,
		GYRO_250DPS,
		GYRO_500DPS,
		GYRO_1000DPS,
		GYRO_2000DPS,
		MAG_14BITS,
		MAG_16BITS
	};

	using enum Scale;
	using enum Address;
	using enum PowerMode;

	explicit MPU9250Controller( class BusController& busController, Address address = Address::DEFAULT );

	/// Sets the power mode of the MPU9250 (normal or sleep).
	// Result<void> setPowerMode( PowerMode mode );

	/// Configures the accelerometer, gyroscope, and magnetometer scales.
	// Result< void > configureScales( Scale accelScale, Scale gyroScale, Scale magScale );

	/// Retrieves the current power mode.
	// [[nodiscard]] Result< PowerMode > getPowerMode();

	/// Reads accelerometer values (X, Y, Z) in g.
	// [[nodiscard]] Result< std::tuple< float, float, float > > getAcceleration();

	/// Reads gyroscope values (X, Y, Z) in degrees per second.
	// [[nodiscard]] Result< std::tuple< float, float, float > > getGyroscope();

	/// Reads magnetometer values (X, Y, Z) in microteslas.
	// [[nodiscard]] Result< std::tuple< float, float, float > > getMagnetometer();

	/// Retrieves the sensor's temperature in degrees Celsius.
	// [[nodiscard]] Result< float > getTemperature();

private:
	MPU9250Controller( const MPU9250Controller& ) = delete;
	MPU9250Controller& operator=( const MPU9250Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_MPU9250_CONTROLLER_HPP__
