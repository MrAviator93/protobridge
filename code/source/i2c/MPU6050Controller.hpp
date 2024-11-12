/**
 * @author MrAviator93
 * @date 4 November 2024
 * @brief Declaration of MPU6050Controller class for managing the MPU6050 sensor.
 *
 * Provides an interface to interact with the MPU6050, a 6-axis motion sensor that combines a 3-axis accelerometer 
 * and a 3-axis gyroscope. This class supports initializing the sensor, retrieving accelerometer and gyroscope data, 
 * and configuring various sensor settings for reliable motion tracking.
 */

#ifndef PBL_I2C_MPU6050_CONTROLLER_HPP__
#define PBL_I2C_MPU6050_CONTROLLER_HPP__

// I2C
#include "ICBase.hpp"
#include <math/Linear.hpp>
#include <utils/Counter.hpp>

// C++
#include <expected>

namespace pbl::i2c
{

inline namespace v1
{

/**
 * @class MPU6050Controller
 * @brief High-level controller class for the MPU6050 6-axis motion sensor.
 *
 * The MPU6050Controller class provides methods to retrieve accelerometer and gyroscope data,
 * configure sensor settings, and perform calibration. This controller abstracts I2C communication
 * and offers an easy-to-use API for integrating MPU6050 functionality in applications requiring
 * motion tracking and environmental sensing.
 *
 * Example usage:
 * @code
 * BusController busController;
 * MPU6050Controller mpu6050(busController);
 * 
 * auto accelData = mpu6050.getAcceleration();
 * auto gyroData = mpu6050.getGyroscope();
 * @endcode
 */
class MPU6050Controller final : public ICBase, public utils::Counter< MPU6050Controller >
{
	static constexpr std::size_t kAccelCalibReadIterations{ 1'000 };
	static constexpr std::size_t kGyroCalibReadIterations{ 1'000 };

public:
	template < typename T >
	using Result = std::expected< T, utils::ErrorCode >;

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
		GYRO_2000DPS
	};

	using enum Scale;
	using enum Address;
	using enum PowerMode;

	/**
     * @brief Constructs a new MPU6050Controller object, initializing the I2C communication with the provided
     * bus controller and sensor address. It sets up the necessary configurations for the MPU6050 operation.
	 * 
     * @param busController Reference to the BusController, facilitating the I2C communication functions.
	 * @param address The device address for the MPU6050 sensor, defaulted to 0x68, the standard I2C address.
     */
	explicit MPU6050Controller( BusController& busController, Address address = DEFAULT ) noexcept;

	/**
     * @brief Sets the power mode of the MPU6050 (normal or sleep).
     * @param mode The desired power mode.
     * @return True if the power mode was set successfully, otherwise false.
     */
	// Result<void> setPowerMode( PowerMode mode );

	/**
     * @brief Configures the accelerometer and gyroscope scales.
     * @param accelScale The accelerometer scale.
     * @param gyroScale The gyroscope scale.
     * @return True if the scale configuration was successful, otherwise false.
     */
	// Result<void> configureScales( Scale accelScale, Scale gyroScale );

	/**
     * @brief Retrieves the current angular orientation of the MPU6050, using a complementary filter for
     * increased accuracy and stability. This function returns the processed Euler angles for the X, Y, and Z axes.
     * 
     * @return std::tuple<float, float, float> The tuple contains the roll, pitch, and yaw angles in degrees.
     */
	[[nodiscard]] Result< math::Vector3f > angles();

private:
	/// TBW
	bool reset();

	/// TBW
	bool configure();

	// Note that this function must be called when the IMY sensor is in flat and still position
	// really this should be done only once for each sensor module, and stored individually for
	// the sensor (sort of calibration) as offsets.
	void calculateImuError();
};

} // namespace v1
} // namespace pbl::i2c
#endif //!PBL_I2C_MPU6050_CONTROLLER_HPP__