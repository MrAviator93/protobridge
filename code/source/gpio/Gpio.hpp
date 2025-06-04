#ifndef PBL_GPIO_GPIO_HPP__
#define PBL_GPIO_GPIO_HPP__

// Third Party
#include <gpiod.hpp>

// C++
#include <string_view>

namespace pbl::gpio
{

struct GpiodChipDeleter
{
	void operator()( gpiod_chip* pChip ) const noexcept
	{
		if( pChip )
		{
			::gpiod_chip_close( pChip );
            pChip = nullptr;
		}
	}
};

[[nodiscard]] std::unique_ptr< gpiod_chip, GpiodChipDeleter > MakeGpioChip( std::string_view chipName )
{
	return std::unique_ptr< gpiod_chip, GpiodChipDeleter >( ::gpiod_chip_open_by_name( chipName.data() ) );
}

} // namespace pbl::gpio
#endif // PBL_GPIO_GPIO_HPP__
