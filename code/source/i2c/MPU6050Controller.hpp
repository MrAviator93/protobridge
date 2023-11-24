#ifndef PBL_I2C_MPU6050_CONTROLLER_HPP__
#define PBL_I2C_MPU6050_CONTROLLER_HPP__

// I2C
#include "ICBase.hpp"
#include <math/Linear.hpp>

namespace PBL::I2C
{

class BusController;

/**
 * @brief The MPU6050Controller class provides a high-level utility for interacting with an MPU6050 sensor module.
 * This class encapsulates the detailed communication with the MPU6050 over the I2C protocol, offering an abstracted
 * interface for configuration, data retrieval, and sensor calibration. The class leverages the capabilities of the
 * MPU6050 to retrieve motion-tracking and environmental data, essential for various real-time applications such
 * as fitness monitoring, gaming interaction, industrial control, and more. It is optimized for efficient data
 * retrieval and minimal hardware dependency, ensuring broad compatibility with various microcontroller platforms.
 * 
 * @author AK aka MrAviator93
 */
class MPU6050Controller final : public ICBase
{
	static constexpr std::size_t kAccelCalibReadIterations{ 1'000 };
	static constexpr std::size_t kGyroCalibReadIterations{ 1'000 };

public:
	enum class Address : std::uint8_t
	{
		DEFAULT = 0x68,
		ALTERNATIVE = 0x69 // AD0 pin is set to logic level high
	};

	using enum Address;

	/**
     * @brief Constructs a new MPU6050Controller object, initializing the I2C communication with the provided
     * bus controller and sensor address. It sets up the necessary configurations for the MPU6050 operation.
	 * 
     * @param busController Reference to the BusController, facilitating the I2C communication functions.
	 * @param address The device address for the MPU6050 sensor, defaulted to 0x68, the standard I2C address.
     */
	explicit MPU6050Controller( BusController& busController, Address address = DEFAULT ) noexcept;

	/**
     * @brief Retrieves the current angular orientation of the MPU6050, using a complementary filter for
     * increased accuracy and stability. This function returns the processed Euler angles for the X, Y, and Z axes.
     * 
     * @return std::tuple<float, float, float> The tuple contains the roll, pitch, and yaw angles in degrees.
     */
	[[nodiscard]] Utils::Vector3f angles();

	// [[nodiscard]] Vector3f getRawAccelValues();
	// [[nodiscard]] Vector3f getRawGyroValues();

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

} // namespace PBL::I2C
#endif //!PBL_I2C_MPU6050_CONTROLLER_HPP__