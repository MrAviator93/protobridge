/**
 * @author MrAviator93
 * @date 25 October 2025
 * @brief Declaration of LM75Controller class for managing the LM75 sensor.
 *
 * For license details, see the LICENSE file in the project root.
 */

#ifndef PBL_I2C_LM75_CONTROLLER_HPP__
#define PBL_I2C_LM75_CONTROLLER_HPP__

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
 * @class LM75Controller
 * @brief A controller interface to manage and communicate with the LM75 temperature sensor.
 *
 * The LM75Controller class provides a comprehensive interface for managing an LM75 temperature sensor.
 * It enables the host system to communicate via I2C, allowing for operations such as reading ambient temperature,
 * setting up sensor modes, and monitoring sensor health status.
 *
 * This controller encapsulates essential functionalities of the LM75, making it possible to perform:
 *  - Read operations for ambient temperature in Celsius or Fahrenheit.
 *  - Control of the sensor's operational state, i.e., active mode or low-power shutdown mode.
 *  - Configuration of the sensor’s thermostat settings for different thermal management strategies.
 *  - Monitoring the alert system that indicates when certain temperature thresholds are breached.
 *
 * The class uses an instance of BusController to handle the actual I2C communications, ensuring compatibility 
 * with various I2C bus management strategies. It's designed with robustness in mind, allowing seamless integration 
 * into larger systems, and is particularly well-suited for applications involving environmental monitoring, 
 * thermal protection, and industrial process control.
 *
 * Example usage:
 * @code
 * BusController busController;
 * LM75Controller lm75(busController);
 * 
 * float currentTemp = lm75.getTemperatureC(); // Gets the current temperature in Celsius
 * lm75.setPowerMode(LM75Controller::LOW_POWER); // Puts the device in low-power mode
 * bool isOverheating = lm75.getAlertStatus(); // Checks if the temperature exceeded a critical threshold
 * @endcode
 *
 * @note This class requires a functioning I2C environment and proper sensor configuration to operate correctly.
 * Ensure the I2C bus is initialized and the LM75 sensor is properly set up before using this controller.
 * 
 * @author AK aka MrAviator93
 */
class LM75Controller final : public ICBase, public utils::Counter< LM75Controller >
{
public:
	template < typename T >
	using Result = utils::Result< T >;

	enum class Address : std::uint8_t
	{
		H48 = 0x48, // A2=0, A1=0, A0=0
		H49 = 0x49, // A2=0, A1=0, A0=1
		H4A = 0x4A, // A2=0, A1=1, A0=0
		H4B = 0x4B, // A2=0, A1=1, A0=1
		H4C = 0x4C, // A2=1, A1=0, A0=0
		H4D = 0x4D, // A2=1, A1=0, A0=1
		H4E = 0x4E, // A2=1, A1=1, A0=0
		H4F = 0x4F // A2=1, A1=1, A0=1
	};

	enum class PowerMode : std::uint8_t
	{
		NORMAL = 0, // Normal operation, the sensor is actively measuring
		LOW_POWER = 1 // Low power mode, measurements are suspended to save power
	};

	enum class ThermostatMode : std::uint8_t
	{
		COMPARATOR = 0, // OS output behaves like a thermostat controlled switch
		INTERRUPT = 1 // OS output asserts until reset, acting as an interrupt
	};

	using enum Address;
	using enum PowerMode;
	using enum ThermostatMode;

	/**
     * @brief Default Ctor
     * @param busController 
     * @param address the address of an LM75 IC on the I2C bus.
     */
	explicit LM75Controller( BusController& busController, Address address = H48 ) noexcept;

	/// Sets the LM75's shutdown mode to reduce power consumption, returns true on success.
	[[nodiscard]] Result< void > setPowerMode( PowerMode mode );

	/// Sets the thermostat mode of the LM75 (comparator or interrupt), returns true on success.
	[[nodiscard]] Result< void > setThermostatMode( ThermostatMode mode );

	/// Retrieves the current state of the LM75's shutdown mode.
	[[nodiscard]] Result< PowerMode > getPowerMode();

	/// Retrieves the current thermostat mode of the LM75.
	[[nodiscard]] Result< ThermostatMode > getThermostatMode();

	/// Checks the status of the LM75's alert flag.
	[[nodiscard]] Result< bool > getAlertStatus();

	/// Retrieves the temperature in degrees Celsius
	[[nodiscard]] Result< float > getTemperatureC();

	/// Retrieves the temperature in Fahrenheit
	[[nodiscard]] Result< float > getTemperatureF();

private:
	LM75Controller( const LM75Controller& ) = delete;
	LM75Controller& operator=( const LM75Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_LM75_CONTROLLER_HPP__
