/**
 * @author MrAviator93
 * @date 25 October 2025
 * @brief Declaration of SHT31Controller class for managing the SHT31 temperature and humidity sensor.
 *
 * For license details, see the LICENSE file in the project root.
 */
#ifndef PBL_I2C_SHT31_CONTROLLER_HPP__
#define PBL_I2C_SHT31_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

// C++
#include <string>
#include <expected>

namespace pbl::i2c
{
inline namespace v1
{

/**
 * @class SHT31Controller
 * @brief A controller interface to manage and communicate with the SHT31 sensor.
 *
 * The SHT31Controller class provides a structured interface for managing an SHT31 temperature and humidity sensor.
 * It enables communication via I2C, allowing for operations such as reading temperature and humidity,
 * triggering measurements, and monitoring sensor status.
 *
 * This controller supports:
 *  - Reading ambient temperature in Celsius or Fahrenheit.
 *  - Reading relative humidity in percentage.
 *  - Configuring repeatability and measurement modes.
 *  - Soft-reset and status register management.
 *
 * Designed for use in embedded and IoT applications, it's well-suited for environmental sensing and monitoring.
 * 
 * @note The SHT31 does not use traditional register addresses like the LM75. Instead,
 * it communicates via command codes, which are 16-bit values sent directly over I²C.
 *
 * Example usage:
 * @code
 * BusController busController;
 * SHT31Controller sht31(busController);
 *
 * auto temperature = sht31.getTemperatureC();
 * auto humidity = sht31.getHumidity();
 * @endcode
 */
class SHT31Controller final : public ICBase, public utils::Counter< SHT31Controller >
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	enum class Address : std::uint8_t
	{
		H44 = 0x44, // ADDR pin low
		H45 = 0x45 // ADDR pin high
	};

	enum class Repeatability : std::uint8_t
	{
		HIGH,
		MEDIUM,
		LOW
	};

	using enum Address;
	using enum Repeatability;

	explicit SHT31Controller( BusController& busController, Address address = H44 ) noexcept;

	/// Triggers a single-shot measurement and reads temperature and humidity
	[[nodiscard]] Result< void > triggerMeasurement( Repeatability rep = HIGH );

	/// Gets the last measured temperature in Celsius
	[[nodiscard]] Result< float > getTemperatureC();

	/// Gets the last measured temperature in Fahrenheit
	[[nodiscard]] Result< float > getTemperatureF();

	/// Gets the last measured relative humidity in %
	[[nodiscard]] Result< float > getHumidity();

	/// Sends a soft reset command to the sensor
	[[nodiscard]] Result< void > reset();

	/// Reads the sensor status register
	[[nodiscard]] Result< uint16_t > readStatus();

private:
	SHT31Controller( const SHT31Controller& ) = delete;
	SHT31Controller& operator=( const SHT31Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_SHT31_CONTROLLER_HPP__
