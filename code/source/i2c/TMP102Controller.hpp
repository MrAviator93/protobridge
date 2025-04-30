/**
 * @author MrAviator93
 * @date 30 April 2025
 * @brief Declaration of TMP102Controller class for managing the TMP102 sensor.
 *
 * For license details, see the LICENSE file in the project root.
 */

#ifndef PBL_I2C_TMP102_CONTROLLER_HPP__
#define PBL_I2C_TMP102_CONTROLLER_HPP__

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
  * @class TMP102Controller
  * @brief A controller interface to manage and communicate with the TMP102 temperature sensor.
  *
  * The TMP102Controller class provides an interface for using the TMP102 temperature sensor over I2C,
  * supporting ambient temperature reading, alert handling, extended mode, and low-power configurations.
  *
  * Example usage:
  * @code
  * BusController busController;
  * TMP102Controller tmp102(busController);
  * 
  * float tempC = tmp102.getTemperatureC().value_or(0.0f);
  * tmp102.setExtendedMode(true);
  * tmp102.setShutdownMode(true);
  * @endcode
  */
class TMP102Controller final : public ICBase, public utils::Counter< TMP102Controller >
{
public:
	template < typename T >
	using Result = std::expected< T, utils::ErrorCode >;

	enum class Address : std::uint8_t
	{
		H48 = 0x48, // ADD0 = GND
		H49 = 0x49, // ADD0 = V+
		H4A = 0x4A, // ADD0 = SDA
		H4B = 0x4B // ADD0 = SCL
	};

	using enum Address;

	/**
      * @brief Default Ctor
      * @param busController 
      * @param address the address of a TMP102 IC on the I2C bus.
      */
	explicit TMP102Controller( BusController& busController, Address address = H48 ) noexcept;

	/// Retrieves the temperature in degrees Celsius.
	[[nodiscard]] Result< float > getTemperatureC();

	/// Retrieves the temperature in degrees Fahrenheit.
	[[nodiscard]] Result< float > getTemperatureF();

	/// Sets shutdown mode for power saving (true = shutdown, false = continuous conversion).
	bool setShutdownMode( bool enable );

	/// Gets the current shutdown mode state.
	[[nodiscard]] Result< bool > getShutdownMode();

	/// Enables or disables extended temperature mode.
	bool setExtendedMode( bool enable );

	/// Gets the current extended mode setting.
	[[nodiscard]] Result< bool > getExtendedMode();

	/// Triggers a one-shot temperature measurement (only works if in shutdown mode).
	bool triggerOneShot();

	/// Reads the alert status from the Alert pin or register.
	[[nodiscard]] Result< bool > getAlertStatus();

private:
	TMP102Controller( const TMP102Controller& ) = delete;
	TMP102Controller& operator=( const TMP102Controller& ) = delete;
};

} // namespace v1
} // namespace pbl::i2c
#endif // PBL_I2C_TMP102_CONTROLLER_HPP__
