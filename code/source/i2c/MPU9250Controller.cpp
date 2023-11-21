
#include "MPU9250Controller.hpp"
#include "I2CBusController.hpp"

namespace I2C
{

MPU9250Controller::MPU9250Controller( I2CBusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace I2C
