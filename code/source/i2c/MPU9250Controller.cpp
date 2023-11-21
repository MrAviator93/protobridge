
#include "MPU9250Controller.hpp"
#include "I2CBusController.hpp"

namespace PBL::I2C
{

MPU9250Controller::MPU9250Controller( I2CBusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace PBL::I2C
