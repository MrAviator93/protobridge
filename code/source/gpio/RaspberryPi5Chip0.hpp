#ifndef PBL_GPIO_RASPBERRY_PI_5_CHIP_0_HPP__
#define PBL_GPIO_RASPBERRY_PI_5_CHIP_0_HPP__

#include "GpioLine.hpp"

// C++
#include <array>
#include <optional>
#include <expected>
#include <string_view>

namespace pbl::gpio
{

// TODO: Rename to just Rpi5Chip0?
class RaspberryPi5Chip0 final
{
	static constexpr std::string_view kChipName{ "gpiochip0" };

public:
	template < typename T >
	using Result = std::expected< T, utils::ErrorCode >;

	RaspberryPi5Chip0();
	~RaspberryPi5Chip0();

	[[nodiscard]] bool isOpen() const noexcept;

	// TODO?
	// static Result< RaspberryPi5Chip0 > make();

private:
	gpiod_chip* m_pChip{ nullptr };

	// Using optional here, allows us to lazy initialize the lines,
	// we don't need to use all the gpio lines always
	std::array< std::optional< GpioLine >, 22 > m_lines;
};

} // namespace pbl::gpio
#endif // PBL_GPIO_RASPBERRY_PI_5_CHIP_0_HPP__
