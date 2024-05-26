#ifndef PBL_I2C_ADS1015_CONTROLLER_HPP__
#define PBL_I2C_ADS1015_CONTROLLER_HPP__

#include "ICBase.hpp"
#include <utils/Counter.hpp>

namespace pbl::i2c
{

/**
 * @brief Controller for the ADS1015 12-bit ADC with I2C interface.
 * 
 */
class ADS1015Controller final : public ICBase, public utils::Counter< ADS1015Controller >
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

	explicit ADS1015Controller( class BusController& busController, Address address = H48 );
};

} // namespace pbl::i2c
#endif // PBL_I2C_ADS1015_CONTROLLER_HPP__
