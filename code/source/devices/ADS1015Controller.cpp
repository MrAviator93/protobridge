
#include "ADS1015Controller.hpp"
#include "I2CBusController.hpp"

namespace I2C
{

ADS1015Controller::ADS1015Controller( I2CBusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace I2C