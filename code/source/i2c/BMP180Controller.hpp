/**
 * @author Artur K.
 * @date 4 November 2025
 * @brief Declaration of BMP180Controller class for managing the BMP180 sensor.
 *
 * For license details, see the LICENSE file in the project root.
 */

#ifndef I2C_DEVICES_BMP180_CONTROLLER_HPP__
#define I2C_DEVICES_BMP180_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>
#include <utils/FastPimpl.hpp>

// C++
#include <optional>
#include <expected>

namespace pbl::i2c
{

inline namespace v1
{

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
 * BusController busController;
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
class BMP180Controller final : public ICBase, public utils::Counter< BMP180Controller >
{
	struct CalibrationConstants;
	static constexpr std::size_t kCalibConstSize = 22;
	static constexpr std::size_t kCalibConstAlign = 2;
	static constexpr float kPressureAtSeaLevelPa = 101325.0f; // Pa

public:
	template < typename T >
	using Result = utils::Result< T >;

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

	explicit BMP180Controller( BusController& busController,
							   Address address = DEFAULT,
							   SamplingAccuracy sAccuracy = STANDARD );

	~BMP180Controller();

	/// Retrieves the temperature in degrees Celsius
	[[nodiscard]] Result< float > getTrueTemperatureC();

	/// Retrieves the temperature in Fahrenheit
	[[nodiscard]] Result< float > getTemperatureF();

	/// Retrieves the true pressure in pascals
	[[nodiscard]] Result< float > getTruePressurePa();

	/**
	 * @brief Calculates the absolute altitude using international barometric formula,
	 * you must provide local pressure in Pa, to get an accurate reading of the true
	 * altitude, above ground.
	 */
	[[nodiscard]] Result< float > getAbsoluteAltitude( float localPressure = kPressureAtSeaLevelPa );

private:
	BMP180Controller( const BMP180Controller& ) = delete;
	BMP180Controller& operator=( const BMP180Controller& ) = delete;

private:
	SamplingAccuracy m_samplingAccuracy;
	utils::FastPimpl< CalibrationConstants, kCalibConstSize, kCalibConstAlign > m_constants;
};

} // namespace v1
} // namespace pbl::i2c
#endif // I2C_DEVICES_BMP180_CONTROLLER_HPP__
