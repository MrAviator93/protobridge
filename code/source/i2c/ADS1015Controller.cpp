#include "ADS1015Controller.hpp"
#include "BusController.hpp"

namespace PBL::I2C
{

ADS1015Controller::ADS1015Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace PBL::I2C