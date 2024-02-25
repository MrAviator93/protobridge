#include "PCA9685Controller.hpp"
#include "BusController.hpp"

namespace PBL::I2C
{

PCA9685Controller::PCA9685Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace PBL::I2C
