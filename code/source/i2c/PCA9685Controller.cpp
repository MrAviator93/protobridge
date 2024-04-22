#include "PCA9685Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

PCA9685Controller::PCA9685Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace pbl::i2c
