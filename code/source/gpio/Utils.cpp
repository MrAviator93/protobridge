
#include "Utils.hpp"

// Thid Party
#include <gpiod.hpp>

namespace pbl::gpio
{

std::vector< GpioInfo > listAvailableGpioChips()
{
	std::vector< GpioInfo > iffos;

	auto it = ::gpiod_chip_iter_new();
	if( !it ) [[unlikely]]
	{
		// Not a single gpio chip is available
		return iffos;
	}

	::gpiod_chip* chip{ nullptr };
	gpiod_foreach_chip( it, chip )
	{
		GpioInfo ci{ ::gpiod_chip_name( chip ), ::gpiod_chip_label( chip ), ::gpiod_chip_num_lines( chip ) };
		iffos.push_back( ci );
	}
	::gpiod_chip_iter_free( it );

	return iffos;
}

} // namespace pbl::gpio
