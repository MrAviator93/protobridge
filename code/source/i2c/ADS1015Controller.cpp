#include "ADS1015Controller.hpp"
#include "BusController.hpp"

namespace pbl::i2c
{

ADS1015Controller::ADS1015Controller( BusController& busController, Address address )
	: ICBase{ busController, address }
{ }

} // namespace pbl::i2c