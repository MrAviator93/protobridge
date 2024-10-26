#ifndef PBL_GPIO_UTILS_HPP__
#define PBL_GPIO_UTILS_HPP__

// C++
#include <vector>
#include <string>
#include <cstdint>

namespace pbl::gpio
{

struct GpioInfo
{
	std::string chip{}; //!< GPIO chip name as represented in the kernel.
	std::string label{}; //!< GPIO chip label as represented in the kernel.
	std::uint32_t numLines{}; //!< The number of GPIO lines exposed by this chip.
	// struct gpiod_chip* chip { nullptr }; //!< A pointer, that points to libgpiod chip object.
};

/// Returns a vector of all available gpio chips
[[nodiscard]] std::vector< GpioInfo > listAvailableGpioChips();

} // namespace pbl::gpio
#endif // !PBL_GPIO_UTILS_HPP__
