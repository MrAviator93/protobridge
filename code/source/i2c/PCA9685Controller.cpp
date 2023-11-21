#include "PCA9685Controller.hpp"
#include "I2CBusController.hpp"

namespace PBL::I2C
{

PCA9685Controller::PCA9685Controller( I2CBusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace PBL::I2C
