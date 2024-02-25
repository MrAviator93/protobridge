#include "MPU9250Controller.hpp"
#include "BusController.hpp"

namespace PBL::I2C
{

MPU9250Controller::MPU9250Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace PBL::I2C
