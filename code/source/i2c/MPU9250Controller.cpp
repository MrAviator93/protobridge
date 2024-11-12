#include "MPU9250Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

v1::MPU9250Controller::MPU9250Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace pbl::i2c
