/***********************************************************************
FILE BMP180CONTROLLER.HPP CONTAINS BMP180 CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (24.10.2023) BY ARTUR K. 
***********************************************************************/

#ifndef I2C_DEVICES_BMP180_CONTROLLER_HPP__
#define I2C_DEVICES_BMP180_CONTROLLER_HPP__

// I2C
#include "ICBase.hpp"
#include <utils/FastPimpl.hpp>

// C++
#include <optional>

namespace PBL::I2C
{

class I2CBusController;

/**
 * @class BMP180Controller
 * @brief Controller interface for the BMP180 barometric pressure sensor.
 *
 * The BMP180Controller class facilitates the communication with and control of a BMP180 sensor, 
 * a precision sensor for measuring barometric pressure and temperature. Given its accuracy, 
 * the BMP180 is commonly used in mobile phones, tablets, and sports devices. This class abstracts 
 * the complexities of interfacing with the device over I2C, providing simple methods to retrieve 
 * temperature, pressure, and calculate absolute altitude.
 *
 * Key functionalities included in this class:
 *  - Read ambient temperature in both Celsius and Fahrenheit.
 *  - Read atmospheric pressure in pascals.
 *  - Calculate the absolute altitude based on the current pressure reading.
 *  - Selection of data sampling accuracy, allowing for a trade-off between power consumption 
 *    and measurement precision.
 *
 * The BMP180Controller requires calibration data that the sensor typically stores in its EEPROM. 
 * These calibration coefficients help in converting the raw sensor readings to accurate real-world 
 * values. The class uses these constants to provide accurate measurements to the calling program.
 *
 * Example usage:
 * @code
 * I2CBusController busController;
 * BMP180Controller bmp180(busController, BMP180Controller::HIGH_RESOLUTION);
 * 
 * float temperature = bmp180.getTrueTemperatureC(); // Read temperature in Celsius
 * float pressure = bmp180.getTruePressurePa(); // Read pressure in pascals
 * float altitude = bmp180.getAbsoluteAltitude(); // Calculate the absolute altitude
 * @endcode
 *
 * @note The class necessitates a proper I2C bus configuration and specific sensor initialization 
 * before retrieving any measurements. The calling environment must ensure that the I2C bus is 
 * available and operational. Furthermore, correct calibration coefficients must be accessible for 
 * accurate readings.
 * 
 * @author AK aka MrAviator93
 */
class BMP180Controller final : public ICBase
{
	struct CalibrationConstants;
	static constexpr std::size_t kCalibConstSize = 22;
	static constexpr std::size_t kCalibConstAlign = 2;

public:
	enum class Address : std::uint8_t
	{
		DEFAULT = 0x77,
		ALTERNATIVE = 0x76 // SD0 pin set to high
	};

	// Can be selected by the variable oversampling_setting
	enum class SamplingAccuracy : std::uint8_t
	{
		ULTRA_LOW_POWER = 0,
		STANDARD = 1,
		HIGH_RESOLUTION = 2,
		ULTRA_HIGH_RESOLUTION = 3
	};

	using enum Address;
	using enum SamplingAccuracy;

	explicit BMP180Controller( I2CBusController& busController,
							   SamplingAccuracy sAccuracy = STANDARD,
							   Address address = DEFAULT );

	~BMP180Controller();

	/// Retrieves the temperature in degrees Celsius
	[[nodiscard]] std::optional<float> getTrueTemperatureC();

	/// Retrieves the temperature in Fahrenheit
	[[nodiscard]] std::optional<float> getTemperatureF();

	/// Retrieves the true pressure in pascals
	[[nodiscard]] std::optional<float> getTruePressurePa();

	/// Calculates the absolute altitude using international barometric formula
	[[nodiscard]] std::optional<float> getAbsoluteAltitude();

private:
	SamplingAccuracy m_samplingAccuracy;
	Utils::FastPimpl< CalibrationConstants, kCalibConstSize, kCalibConstAlign > m_constants;
};

} // namespace PBL::I2C
#endif // I2C_DEVICES_BMP180_CONTROLLER_HPP__
