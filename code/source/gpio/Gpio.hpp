#ifndef PBL_GPIO_GPIO_HPP__
#define PBL_GPIO_GPIO_HPP__

// Third Party
#include <gpiod.hpp>

// C++
#include <string_view>

namespace pbl::gpio
{

struct GpioLineDeleter final
{
	void operator()( gpiod_line* pLine ) const noexcept
	{
		if( pLine )
		{
			::gpiod_line_release( pLine );
			pLine = nullptr;
		}
	}
};

using GpioLineUPtr = std::unique_ptr< gpiod_line, GpioLineDeleter >;

// [[nodiscard]] inline GpioLineUPtr MakeGpioLine( gpiod_chip* pChip, std::size_t lineNumber )
// {
// 	// TODO:
// }

struct GpiodChipDeleter final
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

using GpioChipPtr = std::unique_ptr< gpiod_chip, GpiodChipDeleter >;

[[nodiscard]] inline GpioChipPtr MakeGpioChip( std::string_view chipName )
{
	return GpioChipPtr( ::gpiod_chip_open_by_name( chipName.data() ) );
}

} // namespace pbl::gpio
#endif // PBL_GPIO_GPIO_HPP__
