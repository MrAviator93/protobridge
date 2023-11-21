
#ifndef I2C_DEVICES_MPU9250_CONTROLLER_HPP__
#define I2C_DEVICES_MPU9250_CONTROLLER_HPP__

#include "ICBase.hpp"

namespace I2C
{

/**
 * @brief TBW
 * 
 */
class MPU9250Controller final : public ICBase
{
public:
	enum class Address : std::uint8_t
	{

	};

	explicit MPU9250Controller(class I2CBusController& busController, Address address);
};

} // namespace I2C
#endif // I2C_DEVICES_MPU9250_CONTROLLER_HPP__
