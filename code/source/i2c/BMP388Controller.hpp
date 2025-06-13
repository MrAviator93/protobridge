#ifndef PBL_I2C_BMP388_CONTROLLER_HPP__
#define PBL_I2C_BMP388_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <optional>
#include <expected>

namespace pbl::i2c
{

inline namespace v1
{

/**
 * @class BMP388Controller
 * @brief Controller interface for the BMP388 barometric pressure and temperature sensor.
 *
 * The BMP388Controller class manages communication with the Bosch BMP388 sensor over I2C.
 * The BMP388 is a highly accurate, low-power digital barometer that provides both
 * atmospheric pressure and temperature measurements. It's commonly used in drones,
 * mobile devices, and GPS-enhanced altitude tracking systems.
 *
 * This class abstracts the low-level I2C communication and register manipulation,
 * allowing simple interaction through high-level methods to configure the device,
 * retrieve compensated sensor data, and optionally calculate altitude.
 *
 * Key functionalities supported:
 *  - Temperature and pressure measurement with compensation using factory calibration data.
 *  - Altitude estimation based on current pressure and reference sea-level pressure.
 *  - Configuration of power modes (sleep, forced, normal) to balance power vs. responsiveness.
 *  - Flexible oversampling options for temperature and pressure (x1 to x32).
 *  - Configurable IIR filtering for noise reduction in dynamic environments.
 *
 * Example usage:
 * @code
 * BusController bus;
 * BMP388Controller bmp(bus, BMP388Controller::Address::DEFAULT);
 * bmp.configure(BMP388Controller::PowerMode::NORMAL,
 *               BMP388Controller::Oversampling::X4,
 *               BMP388Controller::Oversampling::X8,
 *               BMP388Controller::IIRFilter::S4);
 *
 * auto data = bmp.read(); // std::expected<std::pair<float, float>, ErrorCode>
 * if (data) {
 *     float temperature = data->first;
 *     float pressure = data->second;
 *     float altitude = pressureToAltitude(pressure);
 * }
 * @endcode
 *
 * @note Proper sensor initialization and calibration data usage are required for
 * accurate readings. The sensor must be configured before retrieving measurements.
 * Reading raw or partially initialized data may result in unreliable values.
 *
 * @author AK
 */
class BMP388Controller final : public ICBase, public utils::Counter< BMP388Controller >
{
	static constexpr float kPressureAtSeaLevelPa = 10'1325.0f; // Pa
public:
	template < typename T >
	using Result = utils::Result< T >;

	enum class Address : std::uint8_t
	{
		DEFAULT = 0x76, ///< SDO to GND
		ALTERNATIVE = 0x77 ///< SDO to VCC
	};

	enum class PowerMode : std::uint8_t
	{
		SLEEP = 0x00,
		FORCED = 0x01,
		NORMAL = 0x03
	};

	enum class Oversampling : std::uint8_t
	{
		SKIPPED = 0x00,
		X2 = 0x01,
		X4 = 0x02,
		X8 = 0x03,
		X16 = 0x04,
		X32 = 0x05
	};

	enum class IIRFilter : std::uint8_t
	{
		OFF = 0x00,
		S2 = 0x01,
		S4 = 0x02,
		S8 = 0x03,
		S16 = 0x04,
		S32 = 0x05,
		S64 = 0x06,
		S128 = 0x07
	};

	using enum Address;

	/// Retrieves the temperature in degrees Celsius.
	// [[nodiscard]] Result< float > getTemperatureC();

	/// Retrieves the temperature in Fahrenheit
	// [[nodiscard]] Result< float > getTemperatureF();

	/// Retrieves the true pressure in pascals.
	// [[nodiscard]] Result< float > getPressurePa();

	/**
	 * @brief Calculates the absolute altitude using international barometric formula,
	 * you must provide local pressure in Pa, to get an accurate reading of the true
	 * altitude, above ground.
	 */
	// [[nodiscard]] Result< float > getAbsoluteAltitude( float seaLevelPressure = 101325.0f );

	/**
	 * @brief Enables the BMP388 FIFO buffer for storing multiple measurements.
	 *
	 * Once enabled, the FIFO will store pressure and temperature samples
	 * for later batch reading, reducing I2C traffic and enabling burst reads.
	 *
	 * @return Result<void> indicating success or error.
	 */
	// [[nodiscard]] Result< void > enableFifo();

	/**
	 * @brief Reads and parses all available data from the BMP388 FIFO buffer.
	 *
	 * This function retrieves all samples currently stored in the FIFO, returning
	 * them as a vector of (temperature, pressure) pairs. Data is automatically
	 * compensated using calibration parameters.
	 *
	 * @return Result containing a vector of temperature and pressure readings, or error.
	 */
	// [[nodiscard]] Result< std::vector< std::pair< float, float > > > readFifoData();

	/**
	 * @brief Enables interrupt generation from the BMP388 sensor.
	 *
	 * Configures the sensor to trigger an interrupt on configurable events
	 * such as new data availability or FIFO thresholds. Requires additional
	 * external hardware setup to handle interrupts.
	 *
	 * @return Result<void> indicating success or error.
	 */
	// [[nodiscard]] Result< void > enableInterrupts();

private:
	BMP388Controller( const BMP388Controller& ) = delete;
	BMP388Controller& operator=( const BMP388Controller& ) = delete;

	/**
	 * @brief Configures the BMP388 sensor with desired power mode, oversampling, and filtering settings.
	 *
	 * This method applies user-specified sensor configuration, which affects the accuracy,
	 * response time, and power consumption. Must be called before data acquisition.
	 *
	 * @param mode Power mode of the sensor (SLEEP, FORCED, or NORMAL).
	 * @param tempOsr Temperature oversampling setting (SKIPPED to X32).
	 * @param pressOsr Pressure oversampling setting (SKIPPED to X32).
	 * @param filter IIR filter coefficient to reduce output noise (OFF to S128).
	 * @return Result<void> indicating success or error.
	 */
	// [[nodiscard]] Result< void >
	// configure( PowerMode mode, Oversampling tempOsr, Oversampling pressOsr, IIRFilter filter );
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_BMP388_CONTROLLER_HPP__
