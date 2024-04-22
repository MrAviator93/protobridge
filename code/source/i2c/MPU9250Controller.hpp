#ifndef PBL_I2C_MPU9250_CONTROLLER_HPP__
#define PBL_I2C_MPU9250_CONTROLLER_HPP__

#include "ICBase.hpp"

namespace pbl::i2c
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

	explicit MPU9250Controller(class BusController& busController, Address address);
};

} // namespace pbl::i2c
#endif // PBL_I2C_MPU9250_CONTROLLER_HPP__
