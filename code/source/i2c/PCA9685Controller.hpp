#ifndef PBL_I2C_PCA9658_CONTROLLER_HPP__
#define PBL_I2C_PCA9658_CONTROLLER_HPP__

#include "ICBase.hpp"

namespace pbl::i2c
{

/**
 * @brief Controller for the PCA9685 16-channel, 12-bit PWM Fm+ I2C-bus LED controller.
 * 
 */
class PCA9685Controller final : public ICBase
{
public:
	enum class Address : std::uint8_t
	{
		H40 = 0x40, // A0=0, A1=0, A2=0
		H41 = 0x41, // A0=1, A1=0, A2=0
		H42 = 0x42, // A0=0, A1=1, A2=0
		H43 = 0x43, // A0=1, A1=1, A2=0
		H44 = 0x44, // A0=0, A1=0, A2=1
		H45 = 0x45, // A0=1, A1=0, A2=1
		H46 = 0x46, // A0=0, A1=1, A2=1
		H47 = 0x47 // A0=1, A1=1, A2=1
	};

	using enum Address;

	explicit PCA9685Controller( class BusController& busController, Address address = H40 );
};

} // namespace pbl::i2c
#endif // PBL_I2C_PCA9658_CONTROLLER_HPP__
