
#ifndef I2C_DEVICES_ADS1015_CONTROLLER_HPP__
#define I2C_DEVICES_ADS1015_CONTROLLER_HPP__

#include "ICBase.hpp"

namespace PBL::I2C
{

/**
 * @brief Controller for the ADS1015 12-bit ADC with I2C interface.
 * 
 */
class ADS1015Controller final : public ICBase
{
public:
	enum class Address : std::uint8_t
	{
		H48 = 0x48, // ADDR connected to GND
		H49 = 0x49, // ADDR connected to VDD
		H4A = 0x4A, // ADDR connected to SDA
		H4B = 0x4B // ADDR connected to SCL
	};

	using enum Address;

	explicit ADS1015Controller( class I2CBusController& busController, Address address = H48 );
};

} // namespace PBL::I2C
#endif // I2C_DEVICES_ADS1015_CONTROLLER_HPP__
